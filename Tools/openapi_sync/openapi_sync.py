# OpenAI, Copyright LifeEXE. All Rights Reserved.
"""
Compares the plugin's C++ API surface with the official OpenAI OpenAPI spec
(https://github.com/openai/openai-openapi) and writes a Markdown drift report.

What is checked:
  * endpoints used by UOpenAIProvider exist in the spec / are not deprecated
  * request and response USTRUCT fields vs spec schemas (missing, unknown, type mismatch, deprecated)
  * request query-parameter structs vs spec query parameters
  * plugin enums (string conversions in OpenAIFuncLib.cpp, UENUMs used in responses) vs spec enums
  * spec operations in the same API groups that the plugin doesn't implement
  * what changed in the spec since the version recorded in spec.lock.json

Only the Python standard library is used.

Usage (from the plugin root):
  python Tools/openapi_sync/openapi_sync.py                         # compare with latest spec, report to Saved/OpenAPISync/report.md
  python Tools/openapi_sync/openapi_sync.py --spec path/openapi.json
  python Tools/openapi_sync/openapi_sync.py --update-lock           # mark the checked spec version as handled
  python Tools/openapi_sync/openapi_sync.py --export-docs OpenAIDocs/Generated
"""

from __future__ import annotations

import argparse
import dataclasses
import datetime as dt
import json
import os
import pathlib
import re
import sys
import urllib.request
from collections import defaultdict

TOOL_DIR = pathlib.Path(__file__).resolve().parent
PLUGIN_ROOT = TOOL_DIR.parent.parent
CONFIG_PATH = TOOL_DIR / "config.json"
LOCK_PATH = TOOL_DIR / "spec.lock.json"

SOURCE_DIR = PLUGIN_ROOT / "Source" / "OpenAI"
TYPES_DIR = SOURCE_DIR / "Public" / "Provider"
PROVIDER_CPP = SOURCE_DIR / "Private" / "Provider" / "OpenAIProvider.cpp"
API_H = SOURCE_DIR / "Public" / "API" / "API.h"
FUNCLIB_CPP = SOURCE_DIR / "Private" / "FuncLib" / "OpenAIFuncLib.cpp"

HTTP_VERBS = ("get", "post", "put", "patch", "delete")


# ---------------------------------------------------------------------------------------------------------------------
# Spec download
# ---------------------------------------------------------------------------------------------------------------------


def http_get(url: str) -> bytes:
    headers = {"User-Agent": "openapi-sync", "Accept": "application/vnd.github+json"}
    token = os.environ.get("GITHUB_TOKEN") or os.environ.get("GH_TOKEN")
    if token and "api.github.com" in url:
        headers["Authorization"] = f"Bearer {token}"
    with urllib.request.urlopen(urllib.request.Request(url, headers=headers), timeout=120) as response:
        return response.read()


def resolve_ref(repo: str, ref: str) -> dict:
    data = json.loads(http_get(f"https://api.github.com/repos/{repo}/commits/{ref}"))
    return {"sha": data["sha"], "date": data["commit"]["committer"]["date"], "message": data["commit"]["message"].split("\n")[0]}


def download_spec(repo: str, sha: str) -> dict:
    return json.loads(http_get(f"https://raw.githubusercontent.com/{repo}/{sha}/openapi.json"))


def compare_commits(repo: str, old_sha: str, new_sha: str) -> list[str]:
    try:
        data = json.loads(http_get(f"https://api.github.com/repos/{repo}/compare/{old_sha}...{new_sha}"))
    except Exception as error:  # noqa: BLE001 - the commit list is informational only
        return [f"(could not load commit list: {error})"]
    return [f"{c['commit']['committer']['date'][:10]} {c['commit']['message'].splitlines()[0]}" for c in data.get("commits", [])]


# ---------------------------------------------------------------------------------------------------------------------
# C++ parsing
# ---------------------------------------------------------------------------------------------------------------------


@dataclasses.dataclass
class CppField:
    name: str
    cpp_type: str


@dataclasses.dataclass
class CppStruct:
    name: str
    base: str | None
    fields: list[CppField]
    file: str


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    return re.sub(r"//[^\n]*", "", text)


def matching_brace(text: str, open_index: int) -> int:
    depth = 0
    for i in range(open_index, len(text)):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return i
    return len(text) - 1


def matching_paren(text: str, open_index: int) -> int:
    depth = 0
    for i in range(open_index, len(text)):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                return i
    return len(text) - 1


def parse_structs(root: pathlib.Path) -> dict[str, CppStruct]:
    structs: dict[str, CppStruct] = {}
    for path in sorted(root.rglob("*.h")):
        text = strip_comments(path.read_text(encoding="utf-8-sig", errors="ignore"))
        for match in re.finditer(r"USTRUCT\s*\([^)]*\)\s*struct\s+(?:\w+_API\s+)?(F\w+)\s*(?::\s*public\s+(F\w+))?\s*\{", text):
            body_start = match.end() - 1
            body = text[body_start + 1 : matching_brace(text, body_start)]
            fields = []
            for prop in re.finditer(r"UPROPERTY\s*\(", body):
                close = matching_paren(body, prop.end() - 1)
                declaration = body[close + 1 : body.find(";", close)].strip()
                declaration = re.sub(r"\s*(\{.*\}|=.*)$", "", declaration, flags=re.S).strip()
                parts = re.match(r"(.+?)\s*\b(\w+)$", declaration, flags=re.S)
                if parts:
                    fields.append(CppField(parts.group(2), " ".join(parts.group(1).split())))
            structs[match.group(1)] = CppStruct(match.group(1), match.group(2), fields, path.relative_to(PLUGIN_ROOT).as_posix())
    return structs


def parse_uenums(root: pathlib.Path) -> dict[str, list[str]]:
    enums = {}
    for path in sorted(root.rglob("*.h")):
        text = strip_comments(path.read_text(encoding="utf-8-sig", errors="ignore"))
        for match in re.finditer(r"UENUM\s*\([^)]*\)\s*enum\s+class\s+(E\w+)\s*(?::\s*\w+)?\s*\{(.*?)\}", text, flags=re.S):
            values = []
            for item in match.group(2).split(","):
                item = re.sub(r"UMETA\s*\(.*?\)", "", item, flags=re.S)
                item = item.split("=")[0].strip()
                if item:
                    values.append(item)
            enums[match.group(1)] = values
    return enums


def parse_enum_strings(funclib_text: str) -> dict[str, dict[str, str]]:
    """EEnum -> {EnumValue: "api-string"} from `case EEnum::Value: return "api-string";` switches."""
    result: dict[str, dict[str, str]] = defaultdict(dict)
    for match in re.finditer(r'case\s+(E\w+)::(\w+)\s*:\s*return\s+(?:TEXT\()?"([^"]*)"', funclib_text):
        result[match.group(1)][match.group(2)] = match.group(3)
    return result


@dataclasses.dataclass
class PluginEndpoint:
    method_name: str
    verb: str
    multipart: bool
    path: str  # with {} placeholders, without /v1
    body_struct: str | None
    query_structs: list[str]
    response_structs: list[str]
    streaming_structs: list[str]


def function_bodies(text: str, prefix: str) -> dict[str, tuple[str, str]]:
    """name -> (parameter list, body) for every `void <prefix>Name(...) {`."""
    bodies = {}
    for match in re.finditer(rf"void\s+{re.escape(prefix)}(\w+)\s*\(", text):
        params_end = matching_paren(text, match.end() - 1)
        brace = text.find("{", params_end)
        if brace == -1 or text[params_end + 1 : brace].strip() not in ("", "const"):
            continue
        bodies[match.group(1)] = (text[match.end() : params_end], text[brace + 1 : matching_brace(text, brace)])
    return bodies


def multipart_structs(cpp: str) -> set[str]:
    """Request structs serialized by a multipart MakeRequest overload (e.g. FCreateSkill)."""
    result = set()
    for match in re.finditer(r"FHttpRequestRef\s+UOpenAIProvider::MakeRequest\(\s*const\s+(F\w+)\s*&", cpp):
        brace = cpp.find("{", match.end())
        if "HttpHelper::" in cpp[brace : matching_brace(cpp, brace)]:
            result.add(match.group(1))
    return result


def parse_endpoints() -> list[PluginEndpoint]:
    api_paths = dict(re.findall(r'virtual\s+FString\s+(\w+)\(\)\s*const\s+override\s*\{\s*return\s+API_URL\s*\+\s*"([^"]+)"', API_H.read_text(encoding="utf-8-sig")))
    cpp = strip_comments(PROVIDER_CPP.read_text(encoding="utf-8-sig"))
    bodies = function_bodies(cpp, "UOpenAIProvider::")
    multipart_types = multipart_structs(cpp)

    # callback -> parsed response type
    callback_types: dict[str, tuple[list[str], bool]] = {}
    for name in re.findall(r"DEFINE_HTTP_CALLBACK\((\w+)\)", cpp):
        callback_types[f"On{name}Completed"] = ([f"F{name}Response"], False)
    for name, (_, body) in bodies.items():
        if not name.startswith("On"):
            continue
        stream = re.findall(r"OnStream(?:Completed|Progress)<(F\w+)>", body)
        handled = re.findall(r"HandleResponse<(F\w+)>", body)
        local = re.findall(r"\b(F\w+(?:Response|Object))\s+\w+\s*;", body)
        if stream:
            callback_types[name] = (stream, True)
        elif handled:
            callback_types[name] = (handled, False)
        elif local:
            callback_types[name] = (local[:1], False)

    endpoints = []
    for name, (params, body) in bodies.items():
        if name.startswith("On") or name in ("SetAPI", "ProcessRequest", "Log", "LogResponse", "LogError", "BroadcastRequestError"):
            continue
        api_call = re.search(r"API->(\w+)\(\)", body)
        if not api_call or api_call.group(1) not in api_paths:
            continue
        statement = body[api_call.end() : body.find(";", api_call.end())]
        path = api_paths[api_call.group(1)]
        for literal, variable, to_query in re.findall(r'\.Append\(\s*(?:"([^"]*)"|(\w+)(\.ToQuery\(\))?)', statement):
            if literal:
                path += literal
            elif variable and not to_query:
                path += "{}"
        verb_match = re.search(r'"(GET|POST|PUT|PATCH|DELETE)"', body)
        verb = verb_match.group(1).lower() if verb_match else "post"

        body_struct, query_structs = None, []
        for type_name, variable in re.findall(r"const\s+(F\w+)\s*&\s*(\w+)", params):
            if type_name in ("FOpenAIAuth", "FString", "FText", "FName"):
                continue
            if verb in ("get", "delete") or re.search(rf"\b{variable}\.ToQuery\(\)", body) or re.search(r"(Params|Parameters|Query)", type_name):
                query_structs.append(type_name)
            else:
                body_struct = type_name

        responses, streaming = [], []
        for callback in re.findall(r"&ThisClass::(On\w+)\)", body):
            if callback not in callback_types:
                continue
            type_names, is_stream = callback_types[callback]
            target = streaming if is_stream else responses
            target.extend(t for t in type_names if t not in target)
        multipart = "HttpHelper::" in body or (body_struct is not None and body_struct in multipart_types)
        endpoints.append(PluginEndpoint(name, verb, multipart, re.sub(r"^/v1", "", path), body_struct, query_structs, responses, streaming))
    return endpoints


# ---------------------------------------------------------------------------------------------------------------------
# Spec helpers
# ---------------------------------------------------------------------------------------------------------------------


class Spec:
    def __init__(self, data: dict):
        self.data = data
        self.schemas = data.get("components", {}).get("schemas", {})
        self.parameters = data.get("components", {}).get("parameters", {})
        self.used_refs: set[str] = set()
        self.operations: dict[tuple[str, str], tuple[str, dict]] = {}
        for raw_path, item in data.get("paths", {}).items():
            for verb, operation in item.items():
                if verb in HTTP_VERBS and isinstance(operation, dict):
                    self.operations[(verb, normalize_path(raw_path))] = (raw_path, operation)

    def resolve(self, schema: dict | None) -> dict:
        seen = 0
        while isinstance(schema, dict) and "$ref" in schema and seen < 50:
            name = schema["$ref"].split("/")[-1]
            self.used_refs.add(name)
            schema = self.schemas.get(name, {})
            seen += 1
        return schema or {}

    def variants(self, schema: dict) -> list[dict]:
        """Flattens anyOf/oneOf into concrete (resolved) alternatives."""
        schema = self.resolve(schema)
        alternatives = schema.get("anyOf") or schema.get("oneOf")
        if not alternatives:
            return [schema]
        result = []
        for alternative in alternatives:
            result.extend(self.variants(alternative))
        return result

    def kinds(self, schema: dict) -> set[str]:
        kinds: set[str] = set()
        for variant in self.variants(schema):
            if variant.get("allOf"):
                for part in variant["allOf"]:
                    kinds |= self.kinds(part)
                continue
            declared = variant.get("type")
            if isinstance(declared, list):
                kinds |= set(declared)
            elif declared:
                kinds.add(declared)
            elif "enum" in variant or "const" in variant:
                values = variant.get("enum") or [variant.get("const")]
                kinds |= {json_kind(value) for value in values}
            elif "properties" in variant:
                kinds.add("object")
        kinds.discard("null")
        return kinds

    def properties(self, schema: dict, depth: int = 0) -> tuple[dict, set[str]]:
        """Merged properties and required names of an object schema (all variants, nested allOf parts)."""
        properties: dict = {}
        required: set[str] | None = None
        if depth > 20:
            return properties, set()
        for variant in self.variants(schema):
            variant_required = set(variant.get("required", []))
            properties.update(variant.get("properties", {}))
            for part in variant.get("allOf", []):
                part_properties, part_required = self.properties(part, depth + 1)
                properties.update(part_properties)
                variant_required |= part_required
            # with oneOf/anyOf a field is only required if every alternative requires it
            required = variant_required if required is None else required & variant_required
        required = required or set()
        # JSON keys like "self-harm" or "harassment/threatening" map to Self_Harm / Harassment_Threatening
        return {json_key_to_field(k): v for k, v in properties.items()}, {json_key_to_field(k) for k in required}

    def array_items(self, schema: dict) -> dict | None:
        for variant in self.variants(schema):
            if "items" in variant:
                return variant["items"]
        return None

    def object_variants(self, schema: dict) -> dict | None:
        props, _ = self.properties(schema)
        return schema if props else None

    def enum_values(self, schema: dict) -> tuple[list, dict]:
        """All enum/const values reachable through refs and unions, plus deprecation notes."""
        values, deprecations = [], {}
        for variant in self.variants(schema):
            for part in [variant] + [self.resolve(p) for p in variant.get("allOf", [])]:
                values.extend(part.get("enum", []))
                if "const" in part:
                    values.append(part["const"])
                for value, info in (part.get("x-openai-enum-deprecations") or {}).items():
                    deprecations[value] = (info or {}).get("message", "deprecated")
        return values, deprecations

    def schema_at(self, location: str) -> dict | None:
        """'Schema.prop.prop[]' -> schema; [] steps into array items."""
        head, *rest = location.split(".")
        schema = self.schemas.get(head)
        if schema is None:
            return None
        self.used_refs.add(head)
        for step in rest:
            to_items = step.endswith("[]")
            step = step.removesuffix("[]")
            properties, _ = self.properties(schema)
            if step not in properties:
                return None
            schema = properties[step]
            if to_items:
                schema = self.array_items(schema) or {}
        return schema


def json_key_to_field(key: str) -> str:
    return re.sub(r"[/\-]", "_", key)


def normalize_path(path: str) -> str:
    return re.sub(r"\{[^}]+\}", "{}", path.rstrip("/"))


def json_kind(value) -> str:
    if isinstance(value, bool):
        return "boolean"
    if isinstance(value, int):
        return "integer"
    if isinstance(value, float):
        return "number"
    if isinstance(value, str):
        return "string"
    return "object"


def first_line(text: str | None, limit: int = 140) -> str:
    text = " ".join((text or "").split())
    return text if len(text) <= limit else text[: limit - 1] + "…"


# ---------------------------------------------------------------------------------------------------------------------
# Comparison
# ---------------------------------------------------------------------------------------------------------------------

OPTIONAL_TYPES = {"FOptionalString": "string", "FOptionalInt": "integer", "FOptionalFloat": "number", "FOptionalBool": "boolean"}
SCALAR_TYPES = {
    "FString": "string", "FText": "string", "FName": "string", "bool": "boolean",
    "int32": "integer", "int64": "integer", "uint8": "integer", "uint32": "integer", "uint64": "integer", "int": "integer",
    "float": "number", "double": "number",
}  # fmt: skip


@dataclasses.dataclass
class Finding:
    severity: str  # error | warning | info
    category: str
    location: str
    message: str


class Checker:
    def __init__(self, spec: Spec, structs: dict[str, CppStruct], uenums: dict[str, list[str]], config: dict):
        self.spec = spec
        self.structs = structs
        self.uenums = uenums
        self.config = config
        self.findings: list[Finding] = []
        self.visited: set[tuple[str, int]] = set()
        self.ignored_fields = {k: {name.lower() for name in v} for k, v in config.get("ignore_fields", {}).items()}
        self.global_ignored_fields = {name.lower() for name in config.get("ignore_fields_everywhere", [])}

    def add(self, severity: str, category: str, location: str, message: str):
        self.findings.append(Finding(severity, category, location, message))

    def struct_fields(self, name: str) -> list[CppField]:
        struct = self.structs.get(name)
        if not struct:
            return []
        inherited = self.struct_fields(struct.base) if struct.base else []
        own = {f.name.lower() for f in struct.fields}
        return [f for f in inherited if f.name.lower() not in own] + struct.fields

    def plugin_kind(self, cpp_type: str) -> tuple[str, str | None, bool]:
        """-> (json kind, nested type name, optional)."""
        cpp_type = cpp_type.replace("TObjectPtr", "").strip()
        if cpp_type in OPTIONAL_TYPES:
            return OPTIONAL_TYPES[cpp_type], None, True
        if cpp_type in SCALAR_TYPES:
            return SCALAR_TYPES[cpp_type], None, False
        array = re.match(r"T(?:Array|Set)\s*<\s*(.+)\s*>$", cpp_type)
        if array:
            return "array", array.group(1).strip(), False
        if cpp_type.startswith("TMap"):
            return "map", None, False
        if re.match(r"E\w+$", cpp_type):
            return "enum", cpp_type, False
        if cpp_type in self.structs:
            return "object", cpp_type, False
        return "unknown", None, False

    def compare_struct(self, struct_name: str, schema: dict, location: str, is_request: bool):
        schema = self.spec.resolve(schema)
        key = (struct_name, id(schema))
        if key in self.visited or struct_name not in self.structs:
            return
        self.visited.add(key)

        properties, required = self.spec.properties(schema)
        if not properties:
            return
        ignored = self.ignored_fields.get(struct_name, set()) | self.global_ignored_fields
        fields = {f.name.lower(): f for f in self.struct_fields(struct_name) if f.name.lower() not in ignored}

        missing = sorted(set(properties) - set(fields) - ignored)
        for name in missing:
            spec_property = self.spec.resolve(properties[name])
            if spec_property.get("deprecated"):
                continue
            severity = "warning" if name in required else "info"
            note = " (required)" if name in required else ""
            self.add(severity, "missing field", f"{location} → `{struct_name}`", f"`{name}`{note}: {first_line(spec_property.get('description'))}")

        for name, field in fields.items():
            if name not in properties:
                self.add("warning", "unknown field", f"{location} → `{struct_name}`", f"`{field.name}` ({field.cpp_type}) is not in the spec")
                continue
            spec_property = properties[name]
            resolved = self.spec.resolve(spec_property)
            if resolved.get("deprecated"):
                self.add("warning", "deprecated field", f"{location} → `{struct_name}`", f"`{name}`: {first_line(resolved.get('description'))}")
            self.compare_field(struct_name, field, spec_property, f"{location} → `{struct_name}.{name}`", is_request, name in required)

    def compare_field(self, struct_name: str, field: CppField, spec_property: dict, location: str, is_request: bool, required: bool):
        kind, nested, optional = self.plugin_kind(field.cpp_type)
        spec_kinds = self.spec.kinds(spec_property)
        if is_request and not optional and not required and kind in ("string", "integer", "number", "boolean", "enum"):
            self.add("info", "always sent", location, f"`{field.cpp_type}` is always serialized, but the field is optional in the spec")

        if kind == "unknown" or not spec_kinds:
            return
        if kind == "map":
            if "object" not in spec_kinds:
                self.mismatch(location, field, spec_kinds)
            return
        if kind == "enum":
            if "string" not in spec_kinds:
                self.mismatch(location, field, spec_kinds)
            elif not is_request:
                self.compare_uenum(nested, spec_property, location)
            return
        if kind == "array":
            if "array" not in spec_kinds:
                self.mismatch(location, field, spec_kinds)
                return
            items = self.spec.array_items(spec_property)
            if items is None:
                return
            inner_kind, inner_nested, _ = self.plugin_kind(nested)
            if inner_kind == "object" and inner_nested:
                if "object" in self.spec.kinds(items):
                    self.compare_struct(inner_nested, items, location, is_request)
                else:
                    self.add("error", "type mismatch", location, f"array items: plugin `{nested}` is an object, spec items are {sorted(self.spec.kinds(items))}")
            elif inner_kind not in ("unknown",) and self.spec.kinds(items) and not compatible(inner_kind, self.spec.kinds(items)):
                self.add("error", "type mismatch", location, f"array items: plugin `{nested}` ({inner_kind}), spec items {sorted(self.spec.kinds(items))}")
            return
        if kind == "object":
            if "object" not in spec_kinds:
                self.mismatch(location, field, spec_kinds)
                return
            self.compare_struct(nested, spec_property, location, is_request)
            return
        if not compatible(kind, spec_kinds):
            self.mismatch(location, field, spec_kinds)

    def mismatch(self, location: str, field: CppField, spec_kinds: set[str]):
        self.add("error", "type mismatch", location, f"plugin `{field.cpp_type}`, spec {'/'.join(sorted(spec_kinds))}")

    def compare_uenum(self, enum_name: str, spec_property: dict, location: str):
        if enum_name not in self.uenums:
            return
        values, _ = self.spec.enum_values(spec_property)
        spec_values = {str(v).lower() for v in values if isinstance(v, str)}
        if not spec_values:
            return
        plugin_values = {v.lower() for v in self.uenums[enum_name]}
        for value in sorted(plugin_values - spec_values):
            self.add("warning", "enum value", f"{location} (`{enum_name}`)", f"`{value}` is not a spec value")
        for value in sorted(spec_values - plugin_values):
            self.add("info", "enum value", f"{location} (`{enum_name}`)", f"spec value `{value}` is missing in the plugin")

    def compare_query(self, struct_name: str, operation: dict, location: str):
        names = set()
        for parameter in operation.get("parameters", []):
            if "$ref" in parameter:
                parameter = self.spec.parameters.get(parameter["$ref"].split("/")[-1], {})
            if parameter.get("in") == "query":
                names.add(parameter["name"])
        if struct_name not in self.structs:
            return
        fields = {f.name.lower(): f for f in self.struct_fields(struct_name)}
        for name in sorted(set(fields) - names):
            self.add("warning", "unknown query parameter", f"{location} → `{struct_name}`", f"`{fields[name].name}` is not a query parameter in the spec")
        for name in sorted(names - set(fields)):
            self.add("info", "missing query parameter", f"{location} → `{struct_name}`", f"`{name}`")


def compatible(kind: str, spec_kinds: set[str]) -> bool:
    if kind in spec_kinds:
        return True
    if kind == "number" and "integer" in spec_kinds:
        return True
    if kind == "integer" and "number" in spec_kinds:
        return True  # reported separately would be noisy; int for number only loses fractions
    return False


def request_schema(operation: dict, multipart: bool = False) -> tuple[dict | None, str | None]:
    content = operation.get("requestBody", {}).get("content", {})
    order = ("multipart/form-data", "application/json") if multipart else ("application/json", "multipart/form-data")
    for media in (*order, "application/x-www-form-urlencoded"):
        if media in content:
            return content[media].get("schema"), media
    return None, None


def response_schema(operation: dict, streaming: bool) -> dict | None:
    for code, response in operation.get("responses", {}).items():
        if not str(code).startswith("2"):
            continue
        content = response.get("content", {})
        media = "text/event-stream" if streaming else "application/json"
        if media in content:
            return content[media].get("schema")
    return None


def check(spec: Spec, config: dict) -> tuple[Checker, list[PluginEndpoint], dict]:
    structs = parse_structs(TYPES_DIR)
    uenums = parse_uenums(SOURCE_DIR / "Public")
    checker = Checker(spec, structs, uenums, config)
    endpoints = parse_endpoints()
    ignored_endpoints = set(config.get("ignore_endpoints", []))
    used_tags: set[str] = set()
    matched_ops: set[tuple[str, str]] = set()

    for endpoint in endpoints:
        if endpoint.method_name in ignored_endpoints:
            continue
        label = f"`{endpoint.verb.upper()} {endpoint.path}` ({endpoint.method_name})"
        found = spec.operations.get((endpoint.verb, normalize_path(endpoint.path)))
        if not found:
            checker.add("error", "endpoint", label, "not found in the spec")
            continue
        raw_path, operation = found
        matched_ops.add((endpoint.verb, normalize_path(endpoint.path)))
        used_tags |= set(operation.get("tags", []))
        if operation.get("deprecated"):
            checker.add("warning", "endpoint", label, f"deprecated in the spec: {first_line(operation.get('description'))}")

        if endpoint.body_struct:
            schema, _ = request_schema(operation, endpoint.multipart)
            if schema is not None:
                checker.compare_struct(endpoint.body_struct, schema, label, is_request=True)
            else:
                checker.add("warning", "request body", label, f"`{endpoint.body_struct}` is sent but the spec has no request body")
        for struct_name in endpoint.query_structs:
            checker.compare_query(struct_name, operation, label)
        for struct_name in endpoint.response_structs:
            schema = response_schema(operation, streaming=False)
            if schema is not None:
                checker.compare_struct(struct_name, schema, label, is_request=False)
        for struct_name in endpoint.streaming_structs:
            schema = response_schema(operation, streaming=True)
            if schema is not None:
                checker.compare_struct(struct_name, schema, label + " [stream]", is_request=False)

    # schemas actually compared with plugin structs; the spec diff is limited to these
    struct_refs = set(spec.used_refs)

    # plugin enums that are converted to API strings
    enum_strings = parse_enum_strings(strip_comments(FUNCLIB_CPP.read_text(encoding="utf-8-sig")))
    for enum_name, enum_config in config.get("enums", {}).items():
        plugin_values = {v for v in enum_strings.get(enum_name, {}).values() if v}
        if not plugin_values:
            checker.add("warning", "enum config", f"`{enum_name}`", "no string conversions found in OpenAIFuncLib.cpp")
            continue
        spec_values: set[str] = set()
        deprecations: dict[str, str] = {}
        locations = enum_config.get("spec", [])
        if locations == "all_models":
            locations = all_model_locations(spec)
        for location in locations:
            schema = spec.schema_at(location)
            if schema is None:
                checker.add("warning", "enum config", f"`{enum_name}`", f"spec location `{location}` not found")
                continue
            values, notes = spec.enum_values(schema)
            spec_values |= {v for v in values if isinstance(v, str)}
            deprecations.update(notes)
        checks = set(enum_config.get("checks", ["unknown", "new", "deprecated"]))
        label = f"`{enum_name}`"
        for value in sorted(plugin_values & set(deprecations)):
            checker.add("warning", "deprecated enum value", label, f"`{value}`: {first_line(deprecations[value])}")
        if "unknown" in checks:
            for value in sorted(plugin_values - spec_values):
                checker.add("warning", "enum value", label, f"`{value}` is not accepted by the spec")
        if "new" in checks:
            for value in sorted(spec_values - plugin_values - set(deprecations)):
                checker.add("info", "new enum value", label, f"`{value}`")

    # operations in the same API groups that the plugin doesn't call
    coverage = defaultdict(list)
    for (verb, path), (raw_path, operation) in sorted(spec.operations.items(), key=lambda item: item[0][1]):
        if (verb, path) in matched_ops or operation.get("deprecated"):
            continue
        tags = set(operation.get("tags", []))
        if tags & used_tags:
            coverage[", ".join(sorted(tags))].append(f"`{verb.upper()} {raw_path}` — {first_line(operation.get('summary'), 80)}")
    return checker, endpoints, coverage, struct_refs


def all_model_locations(spec: Spec) -> list[str]:
    locations = [name for name in spec.schemas if name.startswith("ModelIds")]
    for name, schema in spec.schemas.items():
        properties, _ = spec.properties(schema)
        if "model" in properties:
            locations.append(f"{name}.model")
    return locations


# ---------------------------------------------------------------------------------------------------------------------
# Spec diff (lock vs current)
# ---------------------------------------------------------------------------------------------------------------------

NOISE_KEYS = {"description", "example", "examples", "x-oaiMeta", "summary", "title"}


def canonical(value):
    if isinstance(value, dict):
        return {k: canonical(v) for k, v in sorted(value.items()) if k not in NOISE_KEYS}
    if isinstance(value, list):
        return [canonical(v) for v in value]
    return value


def spec_diff(old: Spec, new: Spec, names: set[str], plugin_ops: set[tuple[str, str]]) -> dict:
    changed = sorted(n for n in names if canonical(old.schemas.get(n)) != canonical(new.schemas.get(n)))
    added_ops = sorted(f"{v.upper()} {new.operations[(v, p)][0]}" for v, p in set(new.operations) - set(old.operations))
    removed_ops = sorted(f"{v.upper()} {old.operations[(v, p)][0]}" for v, p in set(old.operations) - set(new.operations))
    changed_ops = sorted(
        f"{v.upper()} {new.operations[(v, p)][0]}"
        for v, p in plugin_ops
        if (v, p) in old.operations and (v, p) in new.operations and canonical(old.operations[(v, p)][1]) != canonical(new.operations[(v, p)][1])
    )
    return {"changed_schemas": changed, "added_operations": added_ops, "removed_operations": removed_ops, "changed_plugin_operations": changed_ops}


# ---------------------------------------------------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------------------------------------------------

SEVERITY_ORDER = {"error": 0, "warning": 1, "info": 2}
SEVERITY_TITLE = {"error": "Errors", "warning": "Warnings", "info": "Info"}


def write_report(path: pathlib.Path, spec_info: dict, lock: dict | None, checker: Checker, endpoints, coverage, diff, commits) -> str:
    findings = sorted(checker.findings, key=lambda f: (SEVERITY_ORDER[f.severity], f.category, f.location, f.message))
    counts = {s: sum(1 for f in findings if f.severity == s) for s in SEVERITY_ORDER}
    lines = [
        "# OpenAPI sync report",
        "",
        f"- Spec: [`{spec_info.get('sha', 'local')[:10]}`](https://github.com/openai/openai-openapi/commit/{spec_info.get('sha', '')}) "
        f"{spec_info.get('date', '')} — {spec_info.get('message', '')}",
        f"- Locked (last handled) spec: `{(lock or {}).get('sha', 'none')[:10]}` {(lock or {}).get('date', '')}",
        f"- Generated: {dt.datetime.now(dt.timezone.utc).strftime('%Y-%m-%d %H:%M UTC')}",
        f"- Plugin endpoints checked: {len(endpoints)}",
        f"- Findings: **{counts['error']}** errors, **{counts['warning']}** warnings, {counts['info']} info",
        "",
    ]
    if diff is not None:
        lines += ["## Spec changes since the locked version", ""]
        if commits:
            lines += ["<details><summary>Commits</summary>", "", *[f"- {c}" for c in commits], "", "</details>", ""]
        for title, key in (
            ("Changed plugin operations", "changed_plugin_operations"),
            ("Changed schemas mapped to plugin structs", "changed_schemas"),
            ("New operations", "added_operations"),
            ("Removed operations", "removed_operations"),
        ):
            items = diff[key]
            lines.append(f"**{title}** ({len(items)})" + (":" if items else ": none"))
            lines += [f"- `{item}`" for item in items]
            lines.append("")

    for severity in SEVERITY_ORDER:
        group = [f for f in findings if f.severity == severity]
        if not group:
            continue
        lines += [f"## {SEVERITY_TITLE[severity]} ({len(group)})", ""]
        by_category = defaultdict(list)
        for finding in group:
            by_category[finding.category].append(finding)
        for category, items in by_category.items():
            collapsed = severity == "info"
            lines += [f"<details><summary><b>{category}</b> ({len(items)})</summary>", ""] if collapsed else [f"### {category} ({len(items)})", ""]
            lines += [f"- {f.location}: {f.message}" for f in items]
            lines += ["", "</details>", ""] if collapsed else [""]

    if coverage:
        lines += ["## Not implemented operations in API groups the plugin uses", ""]
        for tags, operations in coverage.items():
            lines += [f"<details><summary><b>{tags}</b> ({len(operations)})</summary>", "", *[f"- {o}" for o in operations], "", "</details>", ""]

    text = "\n".join(lines)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")
    return text


def schema_table(spec: Spec, schema: dict | None) -> list[str]:
    if schema is None:
        return ["_No schema._", ""]
    properties, required = spec.properties(schema)
    if not properties:
        kinds = "/".join(sorted(spec.kinds(schema))) or "any"
        return [f"_Type: {kinds}_", ""]
    rows = ["| Field | Type | Required | Description |", "|---|---|---|---|"]
    for name, value in properties.items():
        resolved = spec.resolve(value)
        kinds = "/".join(sorted(spec.kinds(value))) or "any"
        enum, _ = spec.enum_values(value)
        if enum and len(enum) <= 12:
            kinds += ": " + ", ".join(f"`{e}`" for e in enum)
        deprecated = " **(deprecated)**" if resolved.get("deprecated") else ""
        description = first_line(resolved.get("description") or value.get("description"), 300).replace("|", "\\|")
        rows.append(f"| `{name}` | {kinds} | {'yes' if name in required else ''} | {description}{deprecated} |")
    return rows + [""]


def export_docs(spec: Spec, endpoints: list[PluginEndpoint], out_dir: pathlib.Path) -> int:
    count = 0
    for endpoint in endpoints:
        found = spec.operations.get((endpoint.verb, normalize_path(endpoint.path)))
        if not found:
            continue
        raw_path, operation = found
        tag = (operation.get("tags") or ["Other"])[0]
        file = out_dir / re.sub(r"[^\w\- ]", "_", tag) / f"{operation.get('operationId', endpoint.method_name)}.md"
        request, media = request_schema(operation)
        lines = [
            f"# {operation.get('summary', endpoint.method_name)}",
            "",
            f"`{endpoint.verb.upper()} https://api.openai.com/v1{raw_path}`" + (" — **deprecated**" if operation.get("deprecated") else ""),
            "",
            f"Plugin: `UOpenAIProvider::{endpoint.method_name}`",
            "",
            operation.get("description", "").strip(),
            "",
        ]
        parameters = []
        for parameter in operation.get("parameters", []):
            if "$ref" in parameter:
                parameter = spec.parameters.get(parameter["$ref"].split("/")[-1], {})
            parameters.append(f"| `{parameter.get('name')}` | {parameter.get('in')} | {'yes' if parameter.get('required') else ''} | {first_line(parameter.get('description'), 300)} |")
        if parameters:
            lines += ["## Parameters", "", "| Name | In | Required | Description |", "|---|---|---|---|", *parameters, ""]
        if request is not None:
            lines += [f"## Request body ({media})", "", *schema_table(spec, request)]
        lines += ["## Response", "", *schema_table(spec, response_schema(operation, streaming=False))]
        file.parent.mkdir(parents=True, exist_ok=True)
        file.write_text("\n".join(lines), encoding="utf-8")
        count += 1
    return count


# ---------------------------------------------------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--spec", help="local openapi.json instead of downloading")
    parser.add_argument("--ref", default="main", help="branch, tag or commit of openai/openai-openapi (default: main)")
    parser.add_argument("--report", default=str(PLUGIN_ROOT / "Saved" / "OpenAPISync" / "report.md"))
    parser.add_argument("--update-lock", action="store_true", help="record the checked spec version in spec.lock.json")
    parser.add_argument("--no-diff", action="store_true", help="skip comparing with the locked spec version")
    parser.add_argument("--export-docs", metavar="DIR", help="write Markdown docs for every endpoint the plugin uses")
    parser.add_argument("--fail-on", choices=["error", "warning", "never"], default="never", help="exit with code 1 on findings of this severity")
    parser.add_argument("--github-output", help="append key=value results for GitHub Actions")
    args = parser.parse_args()

    config = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
    repo = config.get("spec_repo", "openai/openai-openapi")
    lock = json.loads(LOCK_PATH.read_text(encoding="utf-8")) if LOCK_PATH.exists() else None

    if args.spec:
        spec_info = {"sha": "local", "date": "", "message": args.spec}
        spec = Spec(json.loads(pathlib.Path(args.spec).read_text(encoding="utf-8")))
    else:
        spec_info = resolve_ref(repo, args.ref)
        print(f"Spec {spec_info['sha'][:10]} ({spec_info['date']}): {spec_info['message']}")
        spec = Spec(download_spec(repo, spec_info["sha"]))

    checker, endpoints, coverage, struct_refs = check(spec, config)

    diff, commits = None, []
    if lock and not args.no_diff and spec_info["sha"] not in ("local", lock["sha"]):
        old = Spec(download_spec(repo, lock["sha"]))
        plugin_ops = {(e.verb, normalize_path(e.path)) for e in endpoints}
        diff = spec_diff(old, spec, struct_refs, plugin_ops)
        commits = compare_commits(repo, lock["sha"], spec_info["sha"])

    report_path = pathlib.Path(args.report)
    write_report(report_path, spec_info, lock, checker, endpoints, coverage, diff, commits)
    counts = {s: sum(1 for f in checker.findings if f.severity == s) for s in SEVERITY_ORDER}
    print(f"Endpoints: {len(endpoints)}; errors: {counts['error']}, warnings: {counts['warning']}, info: {counts['info']}")
    print(f"Report: {report_path}")

    if args.export_docs:
        print(f"Docs written: {export_docs(spec, endpoints, pathlib.Path(args.export_docs))} files -> {args.export_docs}")

    if args.update_lock:
        if spec_info["sha"] == "local":
            print("--update-lock needs a downloaded spec (not --spec)", file=sys.stderr)
            return 2
        LOCK_PATH.write_text(json.dumps(spec_info, indent=2) + "\n", encoding="utf-8")
        print(f"Lock updated: {spec_info['sha'][:10]}")

    if args.github_output:
        spec_changed = bool(lock is None or spec_info["sha"] != lock.get("sha"))
        with open(args.github_output, "a", encoding="utf-8") as output:
            output.write(f"sha={spec_info['sha']}\nspec_changed={str(spec_changed).lower()}\n")
            output.write(f"errors={counts['error']}\nwarnings={counts['warning']}\n")

    if args.fail_on != "never" and any(SEVERITY_ORDER[f.severity] <= SEVERITY_ORDER[args.fail_on] for f in checker.findings):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())

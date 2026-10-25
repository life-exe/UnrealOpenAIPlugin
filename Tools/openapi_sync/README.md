# OpenAPI Sync

Compares the plugin with the official OpenAI spec ([openai/openai-openapi](https://github.com/openai/openai-openapi), `openapi.json`) and writes a Markdown drift report. Only the Python 3.10+ standard library is used.

## Usage

Run from the plugin root:

```powershell
# compare with the latest spec -> Saved/OpenAPISync/report.md
python Tools/openapi_sync/openapi_sync.py

# compare with a specific spec commit or a local file
python Tools/openapi_sync/openapi_sync.py --ref <commit-sha>
python Tools/openapi_sync/openapi_sync.py --spec path/to/openapi.json

# generate Markdown docs from the spec for every endpoint the plugin uses
python Tools/openapi_sync/openapi_sync.py --export-docs OpenAIDocs/Generated

# after the report has been handled: mark this spec version as the new baseline
python Tools/openapi_sync/openapi_sync.py --update-lock
```

Set `GITHUB_TOKEN` (or `GH_TOKEN`) to avoid GitHub API rate limits, e.g. `$env:GITHUB_TOKEN = gh auth token`.

## How it works

Nothing is mapped by hand except enums. The tool reads the plugin sources directly:

| Source | What is extracted |
|---|---|
| `Source/OpenAI/Public/API/API.h` | base path of every endpoint (`/v1/vector_stores`, ...) |
| `Source/OpenAI/Private/Provider/OpenAIProvider.cpp` | for every public provider method: HTTP verb, full path (`.Append(...)` chain), request struct, query structs, multipart or JSON, and the response struct of the bound callback |
| `Source/OpenAI/Public/Provider/**/*.h` | `USTRUCT` fields (with inheritance) and `UENUM` values |
| `Source/OpenAI/Private/FuncLib/OpenAIFuncLib.cpp` | API strings of plugin enums (`case EEnum::X: return "..."`) |

Each provider method is matched to a spec operation by verb and path. Its request and response structs are then compared recursively with the operation's schemas. Field names are matched case-insensitively (`Expires_At` == `expires_at`), and `-` or `/` in JSON keys equals `_`.

## Report sections

- **Spec changes since the locked version**: commits, changed operations used by the plugin, changed schemas mapped to plugin structs, new and removed operations. The baseline is `spec.lock.json`.
- **Errors**:
  - `endpoint`: the endpoint is missing from the spec.
  - `type mismatch`: the plugin field type doesn't match the spec, e.g. `FString` where the spec has an object.
- **Warnings**:
  - deprecated endpoints, fields and enum values (with the shutdown date when the spec has it);
  - `unknown field`: the plugin sends or expects a field the spec doesn't have;
  - `missing field` for fields the spec requires;
  - enum values the spec doesn't accept.
- **Info**:
  - optional missing fields;
  - new enum values;
  - `always sent`: plain request fields that are always serialized although the spec marks them optional. Candidates for `FOptional*`.
- **Not implemented operations** in API groups (tags) the plugin already uses.

## Configuration (`config.json`)

- `enums`: plugin enum → spec locations (`Schema.property`, `[]` steps into array items, `all_models` = every `model` enum in the spec). `checks` selects which of these to report:
  - `unknown`: a plugin value the spec doesn't accept;
  - `new`: a spec value the plugin doesn't have;
  - `deprecated`: a plugin value the spec marks deprecated.
- `ignore_fields_everywhere`, `ignore_fields`: plugin-only helper fields such as `IsSet` (stripped before sending) or `FMessage.ContentArray` (renamed by `ChatParser`).
- `ignore_endpoints`: provider method names to skip.

Some findings are intentional: for example, `FFunctionRequest.Parameters` is a JSON string that `ChatParser` turns into an object. Add those to `ignore_fields` rather than changing the code.

## CI

`.github/workflows/openapi_sync.yml` runs every Monday and can be started manually. If the spec moved past `spec.lock.json`, it creates or updates an issue labeled `openapi-sync` with the report. The full report is also uploaded as an artifact.

Workflow:
1. The issue appears.
2. Implement the changes using the report and `AIInstuctions/API_INTEGRATION_GUIDE.md`.
3. Run the live API tests.
4. Run `--update-lock` and commit the lock.
5. Close the issue.

The spec is not always accurate. For example, `/fine_tuning/jobs/{id}/pause` is still in the spec while the server answers `Invalid url`. Confirm changes with the live API tests (`OpenAI.Provider.ActualAPI`).

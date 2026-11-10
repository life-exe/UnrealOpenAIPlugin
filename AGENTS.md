# AI Agent Guidelines - OpenAI Unreal Engine Plugin

Entry point for AI agents (Antigravity, Claude, Codex, Cursor) working on the OpenAI Unreal Engine plugin.

## Documentation & Runbooks
- [**AI Development Guide**](./AIInstuctions/AI_GUIDE.md) — Master guide for build, test, and architecture standards.
- [**API Integration Guide**](./AIInstuctions/API_INTEGRATION_GUIDE.md) — Step-by-step workflow for adding and updating endpoints.
- [**Update Models Guide**](./AIInstuctions/UPDATE_MODELS.md) — Workflow for syncing OpenAI model enums and string conversions.
- [**OpenAPI Sync Tool**](./Tools/openapi_sync/README.md) — Specification sync and validation documentation.

## Available Agent Skills
- **`openai-api-sync`** (`.agents/skills/openai-api-sync/SKILL.md`): Use when syncing with the OpenAI OpenAPI spec, checking diffs, updating models, or resolving `openapi_sync.py` errors/warnings.
- **`openai-endpoint-integration`** (`.agents/skills/openai-endpoint-integration/SKILL.md`): Use when adding new API endpoints, creating Blueprint Async Actions, implementing provider callbacks, or writing offline fake/stress tests.

## Essential Commands (CLI)
- **OpenAPI Spec Sync Check**:
  ```powershell
  python Tools/openapi_sync/openapi_sync.py
  ```
- **Update Spec Lock**:
  ```powershell
  python Tools/openapi_sync/openapi_sync.py --update-lock
  ```
- **Build Plugin**:
  ```powershell
  cmd /c "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" OpenAIPluginTestEditor Win64 Development -Project="C:\_Projects\UnrealEngine\OpenAIPluginTest\OpenAIPluginTest.uproject" -waitmutex
  ```
- **Run Unit Tests (Offline)**:
  ```powershell
  & "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "C:\_Projects\UnrealEngine\OpenAIPluginTest\OpenAIPluginTest.uproject" -ExecCmds="Automation SetFilter All;RunTests OpenAI.OpenAIFuncLib;RunTests OpenAI.Parser;RunTests OpenAI.Provider.Fake;Quit" -log -nosplash -nullrhi -unattended
  ```
- **Code Formatting**:
  ```powershell
  .\format_all_files.bat
  ```

## Invariants
1. **Never skip Blueprint Async Actions**: Every endpoint must be accessible in Blueprints via `UOpenAIActionBase` action node.
2. **Never skip Offline Unit Tests**: Every endpoint response parsing must be covered in `OpenAIProviderFake.spec.cpp`.
3. **Keep 0 errors in OpenAPI Sync**: No unresolved unknown/missing fields or enum values against `openai/openai-openapi`.

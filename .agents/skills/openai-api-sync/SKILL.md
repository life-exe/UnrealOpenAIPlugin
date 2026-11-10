---
name: openai-api-sync
description: >-
  Synchronize the Unreal Engine OpenAI plugin with the official OpenAI OpenAPI specification.
  Use when asked to sync OpenAI API, update models, check spec diffs, or resolve openapi_sync report errors/warnings.
---

# OpenAI OpenAPI Sync Skill

Runbook for synchronizing the Unreal Engine OpenAI Plugin with the official [openai/openai-openapi](https://github.com/openai/openai-openapi) specification.

## Core Principle
- **0 errors, 0 unknown fields, 0 missing fields, 0 invalid enum values** in `Tools/openapi_sync/openapi_sync.py`.
- Warnings are permitted only for officially deprecated OpenAI endpoints/fields/models (`deprecated: true`).
- All code changes must compile cleanly in Unreal Engine and pass all unit automation tests (`OpenAI.Parser`, `OpenAI.OpenAIFuncLib`, `OpenAI.Provider.Fake`).

## Workflow Steps

### 1. Run OpenAPI Sync Tool
From the plugin root (`Plugins/OpenAI`):
```powershell
python Tools/openapi_sync/openapi_sync.py
```
This generates the diff report at `Saved/OpenAPISync/report.md` comparing against `Tools/openapi_sync/spec.lock.json`.

### 2. Inspect Findings
Check `Saved/OpenAPISync/report.md`:
- **New / Missing Endpoints**: Follow the `openai-endpoint-integration` skill to add missing provider methods, delegates, and Blueprint async actions.
- **New Models & Enums**:
  - Add enum variants to `Source/OpenAI/Public/Provider/Types/ModelTypes.h` (`EAllModelEnum`, `EMainModelEnum`), `AudioTypes.h`, `ImageTypes.h`, etc.
  - Implement string conversions in `Source/OpenAI/Private/FuncLib/OpenAIFuncLib.cpp`.
  - Add test assertions in `Source/OpenAITestRunner/Private/OpenAIFuncLib.spec.cpp`.
- **Field Mismatches / New Properties**:
  - Update `USTRUCT`s in `Source/OpenAI/Public/Provider/Types/`.
  - Ensure all fields have `UPROPERTY(BlueprintReadWrite/BlueprintReadOnly)` and correct default initializers.
- **Intentional Differences**:
  - If a difference is intentional (e.g. plugin uses a richer internal structure or custom mapper), register it in `Tools/openapi_sync/config.json` under `ignore_fields` or `ignore_enum_values`.

### 3. Verify Code Compilation & Automation Tests
```powershell
# 1. Compile plugin in Unreal Engine
& "<EnginePath>/Build/BatchFiles/Build.bat" <ProjectName>Editor Win64 Development -Project="<ProjectPath>.uproject" -WaitMutex

# 2. Run offline unit tests
& "<EnginePath>/Binaries/Win64/UnrealEditor-Cmd.exe" "<ProjectPath>.uproject" -ExecCmds="Automation SetFilter All;RunTests OpenAI.OpenAIFuncLib;RunTests OpenAI.Parser;RunTests OpenAI.Provider.Fake;Quit" -log -nosplash -nullrhi -unattended
```

### 4. Format Code & Update Lock
Once all errors are resolved and tests pass:
```powershell
# Format code with clang-format
.\format_all_files.bat

# Update the spec lock
python Tools/openapi_sync/openapi_sync.py --update-lock
```
Ensure `git status` shows clean formatting and updated `spec.lock.json`.

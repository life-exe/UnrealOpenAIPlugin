# AI Development Guide - OpenAI Plugin

This document provides technical instructions for AI agents and developers working on the OpenAI Unreal Engine plugin.

## Core Responsibilities
- **Maintain Plugin Architecture**: Follow the established provider/parser pattern for all OpenAI API integrations.
- **Ensure Code Quality**: Adhere to Unreal Engine C++ standards and the plugin's specific conventions (e.g., `TEXT()` macros).
- **Test-Driven Development**: Always include or update automation tests (Parser Specs) and stress tests (Actual API Specs) when modifying functionality.
## Documentation
Keep the following documents updated as the project evolves:
- [**API Integration Guide**](./API_INTEGRATION_GUIDE.md) - Workflow for adding new endpoints.
- [**Update Models Guide**](./UPDATE_MODELS.md) - Workflow for syncing model enums.

## Project Structure
*Note: All paths are relative to the Plugin Root.*

- `Source/OpenAI`: Core module containing API provider and data types.
- `Source/OpenAIEditor`: Unreal Editor extensions (ChatGPT widget).
- `Source/OpenAITestRunner`: Automation testing module.
- `Source/OpenAITestRunner/Data`: Test assets and authentication files.

## Build and Environment
### Prerequisites
- **Engine**: Unreal Engine 5.7+
- **Toolchain**: Visual Studio 2022

### Environment Variables
Replace the following placeholders in the commands below:
- `<EnginePath>`: Path to your UE 5.7 engine folder (Default: `C:/Program Files/Epic Games/UE_5.7/Engine`)
- `<ProjectPath>`: Absolute path to your `.uproject` file.
- `<ProjectName>`: The name of your project.

### Compilation
Build the development editor via CLI:
```powershell
cmd /c "<EnginePath>/Build/BatchFiles/Build.bat" <ProjectName>Editor Win64 Development -Project="<ProjectPath>.uproject" -waitmutex
```

## Testing Workflow
### 1. Automation Tests (Internal)
Run all parser tests to verify JSON serialization:
```powershell
& "<EnginePath>/Binaries/Win64/UnrealEditor.exe" "<ProjectPath>.uproject" -ExecCmds="Automation SetFilter All;RunTests OpenAI.Parser;Quit" -log -nosplash -nullrhi -unattended
```

### 2. Stress Tests (Live API)
Run tests against real OpenAI endpoints:
```powershell
& "<EnginePath>/Binaries/Win64/UnrealEditor.exe" "<ProjectPath>.uproject" -ExecCmds="Automation SetFilter All;RunTests OpenAI.Provider.ActualAPI;Quit" -log -nosplash -nullrhi -unattended
```

## Adding New APIs
Follow the architectural steps when implementing new features. A detailed walkthrough is available here: [**API Integration Guide**](./API_INTEGRATION_GUIDE.md).

1.  **Analyze Documentation**: Identify methods, schemas, and headers.
2.  **Define Types**: Create `USTRUCT`s in `Source/OpenAI/Public/Provider/Types/`. Use `FOptional` types for optional fields.
3.  **Register Delegates**: Add multi-cast delegates in `Delegates.h`.
4.  **Map Endpoints**: Update `IAPI` and its implementations in `API.h`.
5.  **Local API Update**: Update `AAPIOverview::SetYourOwnAPI::MyAPI` in `APIOverview.cpp`.
6.  **JSON Serialization**: Implement custom logic in `Source/OpenAI/Private/Provider/JsonParsers/`. **Must include `OPENAI_API` macro.**
7.  **Provider Integration**: Implement public methods and HTTP callbacks in `OpenAIProvider.cpp`.
8.  **Add Sample Examples**: Update `AAPIOverview` actor with manual testing options.
9.  **Write Tests**:
    - Parser Specs for JSON verification.
    - API Stress Specs for live validation.

## Coding Standards
- **Naming**: Use `F` prefix for structs, `E` for enums, `U` for UObjects.
- **Strings**: Wrap all literals in `TEXT()` macro.
- **Logging**: Use `UE_LOGFMT` for structured logging.
- **Cleanup**: Always run `format_all_files.bat` (clang-format) before finalizing changes.

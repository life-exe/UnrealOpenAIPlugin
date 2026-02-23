# Update OpenAI Models Guide

Follow these steps to synchronize the plugin's internal model enums with the latest available models from the OpenAI API.

*Note: All paths are relative to the Plugin Root.*

## Step 1: Run Model List Test
Execute the automated test that fetches the current model list from OpenAI. 

### Environment Variables
Replace the following placeholders in the commands below:
- `<EnginePath>`: Path to your UE 5.7 engine folder (Default: `C:/Program Files/Epic Games/UE_5.7/Engine`)
- `<ProjectPath>`: Absolute path to your `.uproject` file.
- `<ProjectName>`: The name of your project.

```powershell
# 1. Build the project
cmd /c "<EnginePath>/Build/BatchFiles/Build.bat" <ProjectName>Editor Win64 Development -Project="<ProjectPath>.uproject" -waitmutex

# 2. Run the specific test
& "<EnginePath>/Binaries/Win64/UnrealEditor.exe" "<ProjectPath>.uproject" -ExecCmds="Automation SetFilter All;RunTests OpenAI.Provider.ActualAPI.Models.ListModelsRequestShouldResponseCorrectly;Quit" -log -abslog="<ProjectPath>/Build/Tests/ModelsSync.log" -nosplash -nullrhi -unattended
```

> **Note**: `SetFilter All` is mandatory because the live API tests use the `StressFilter`.
> Test Location: `Source/OpenAITestRunner/Private/Stress/OpenAIModelAPI.spec.cpp`

## Step 2: Analyze Results
Review the log file (e.g., `Build/Tests/ModelsSync.log`).
- Look for warnings about models that don't exist in `EAllModelEnum`.
- Identify newly added models.
- Identify deprecated/removed models.

## Step 3: Update Model Enum
Update the `EAllModelEnum` definition:
- **File**: `Source/OpenAI/Public/Provider/Types/ModelTypes.h`
- Add new model constants following the established naming convention (e.g., `GPT_4O_Mini`).

## Step 4: Update Function Library
Update the string conversion logic:
- **File**: `Source/OpenAI/Private/FuncLib/OpenAIFuncLib.cpp`
- Update `OpenAIAllModelToString` and related mapping functions to support the new enum members.

## Step 5: Update Related Tests and Utils
Ensure the test utilities can handle the new models:
- **File**: `Source/OpenAITestRunner/Private/OpenAIFuncLib.spec.cpp`
- **File**: `Source/OpenAITestRunner/Private/TestUtils.cpp`

## Step 6: Verify Updates
Re-run the commands from **Step 1**. 
- Verify the output log contains no warnings regarding missing models.
- Ensure all model-related tests pass.

## Step 7: Finalize
Run the formatting script before committing:
```powershell
.\format_all_files.bat
```
Commit message: `Update OpenAI models`

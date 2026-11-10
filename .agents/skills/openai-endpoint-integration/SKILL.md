---
name: openai-endpoint-integration
description: >-
  Add or update an OpenAI API endpoint in the Unreal Engine plugin.
  Use when implementing new OpenAI endpoints, creating Blueprint async actions, writing provider methods, or adding offline fake/stress tests.
---

# OpenAI Endpoint Integration Skill

Standard runbook for adding or updating OpenAI API endpoints in the Unreal Engine plugin.

## Architecture Layers
For every OpenAI API endpoint, the following layers must be implemented:
1. **Types** (`Source/OpenAI/Public/Provider/Types/`)
2. **Delegates** (`Source/OpenAI/Public/Provider/Delegates.h`)
3. **Endpoints Mapping** (`Source/OpenAI/Public/API/API.h` and `Source/OpenAI/Private/Sample/APIOverview.cpp`)
4. **Provider API & Callbacks** (`Source/OpenAI/Public/Provider/OpenAIProvider.h` & `Private/.../OpenAIProvider.cpp`)
5. **Blueprint Async Action** (`Source/OpenAI/Public/BlueprintAsyncActions/` & `Private/...`)
6. **Offline Tests** (`Source/OpenAITestRunner/Private/OpenAIProviderFake.spec.cpp`)
7. **Stress Tests** (`Source/OpenAITestRunner/Private/Stress/OpenAI*API.spec.cpp`)

---

## Step-by-Step Procedure

### 1. Types
- Define request and response structs in `Source/OpenAI/Public/Provider/Types/<Category>Types.h`.
- Use `USTRUCT(BlueprintType)` and `UPROPERTY(BlueprintReadWrite/BlueprintReadOnly)`.
- Use `FOptionalString`, `FOptionalInt`, etc. for nullable/optional fields.

### 2. Delegates
In `Source/OpenAI/Public/Provider/Delegates.h`:
```cpp
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOn<Action>Completed, const F<ResponseStruct>&, const FOpenAIResponseMetadata&);
```

### 3. API Mapping
In `Source/OpenAI/Public/API/API.h`:
- Add pure virtual method to `IAPI`.
- Implement in `OpenAIAPI` (e.g. `return API_URL + "/v1/..."`).
- Add field to `FOpenAIEndpoints`.
- Implement in `GenericAPI`.
- **Crucial**: Implement method in `AAPIOverview::SetYourOwnAPI::MyAPI` in `Source/OpenAI/Private/Sample/APIOverview.cpp` to prevent abstract class compilation errors.

### 4. Provider Interface & Implementation
In `Source/OpenAI/Public/Provider/OpenAIProvider.h`:
- Declare public method: `void <Action>(const F<RequestStruct>& Params, const FOpenAIAuth& Auth);`
- Add event getter: `DEFINE_EVENT_GETTER(<Action>Completed)`
- Declare callback: `DECLARE_HTTP_CALLBACK(On<Action>Completed)`

In `Source/OpenAI/Private/Provider/OpenAIProvider.cpp`:
- Implement the request dispatcher: URL, HTTP verb, auth headers, JSON / multipart content, bind callback.
- Implement callback handler: check HTTP status, deserialize JSON via `HandleResponse<F<ResponseStruct>>`, broadcast delegate or error.

### 5. Blueprint Async Action
Create:
- `Source/OpenAI/Public/BlueprintAsyncActions/<Category>/<Name>Action.h`
- `Source/OpenAI/Private/BlueprintAsyncActions/<Category>/<Name>Action.cpp`

Pattern:
- Inherit from `UOpenAIActionBase`.
- Declare dynamic multicast delegate:
  `DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOn<Name>, const F<ResponseStruct>&, Response, const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);`
- Expose factory method:
  `UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | <Category>")`
- Implement `Activate()`:
  - Call `CreateProvider()`.
  - Bind `Provider->On<Action>Completed().AddUObject(this, &ThisClass::On<Action>Completed)`.
  - Call `Provider-><Action>(Params, Auth)`.
- Implement `On<Action>Completed`:
  - `OnCompleted.Broadcast(Response, ResponseMetadata, {});`
- Implement `OnRequestError`:
  - `OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});`
- Implement `SetEndpoint`:
  - Override `Endpoints.<CategoryEndpoint> = URL;`

### 6. Verification & Automated Tests
1. **Offline Unit Tests** (`Source/OpenAITestRunner/Private/OpenAIProviderFake.spec.cpp`):
   - Add test using `UOpenAIProviderFake::SetResponse(MockJsonString)`.
   - Verify parsing into USTRUCT fields without making network calls.
2. **Automation Runner**:
   - Run `OpenAI.Provider.Fake` via `UnrealEditor-Cmd.exe` and confirm exit code 0.
3. **Format**:
   - Run `format_all_files.bat`.

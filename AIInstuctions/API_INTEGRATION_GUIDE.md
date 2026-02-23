# OpenAI API Integration Guide

This guide outlines the standard workflow for adding new OpenAI API endpoints to the Unreal Engine plugin. Adhere to these steps to ensure consistency, maintainability, and proper test coverage.

---

## Step 1: Analyze Documentation
Review the API reference in `Plugins/OpenAI/OpenAIDocs/` or the official OpenAI documentation. Identify:
- **HTTP Methods** (GET, POST, DELETE, etc.)
- **Request Body Schema** (Required and optional fields)
- **Response Object Schema**
- **Query Parameters** for list/search endpoints
- **Beta Headers** (e.g., `OpenAI-Beta: assistants=v2`)

## Step 2: Define Data Structures (`Source/OpenAI/Public/Provider/Types/`)
*Note: All source paths are relative to the Plugin Root.*

Create a new header file (e.g., `NewFeatureTypes.h`) or update an existing one.
- Use `USTRUCT(BlueprintType)` for all data objects.
- Follow naming conventions: `FCreate[Feature]`, `FUpdate[Feature]`, `F[Feature]Response`.
- Use `FOptionalString`, `FOptionalInt`, etc. (from `OpenAIOptional.h`) for optional fields.
- **Critical:** Ensure all properties use `UPROPERTY()` to support JSON serialization via `FJsonObjectConverter`.

```cpp
// Example: NewFeatureTypes.h
USTRUCT(BlueprintType)
struct FNewFeatureResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;
    // ... other fields
};
```

## Step 3: Register Types
1.  **AllTypesHeader.h**: Include your new header file here.
2.  **Delegates.h**: Declare multicast delegates for your new callbacks.
    ```cpp
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateFeatureCompleted, const FCreateFeatureResponse&, const FOpenAIResponseMetadata&);
    ```

## Step 4: Map Endpoints (`Source/OpenAI/Public/API/API.h`)
1.  Add a virtual method to `IAPI`.
2.  Implement it in `OpenAIAPI` (V1) using the base URL.
3.  Add the field to `FOpenAIEndpoints` struct.
4.  Implement it in `GenericAPI`.
5.  **Important:** Update the local `MyAPI` class in `APIOverview.cpp` to prevent "abstract class" compilation errors.

## Step 5: Declare Provider Interface (`OpenAIProvider.h`)
1.  Include your new `JsonParsers/NewFeatureParser.h` (created in the next step).
2.  Add public methods for the API calls.
3.  Add `DEFINE_EVENT_GETTER` for your delegates.
4.  Add `DECLARE_HTTP_CALLBACK` for internal response handling.
5.  Add `MakeRequest` template specializations if the feature requires custom JSON cleanup.

## Step 6: Implement JSON Serialization (`Provider/JsonParsers/`)
Create `NewFeatureParser.h` and `.cpp`.
- Use the `OPENAI_API` macro on the class to export it for tests.
- Implement logic to handle OpenAI-specific requirements (e.g., renaming fields, converting enums to lowercase, removing nulls).

```cpp
// NewFeatureParser.h
namespace OpenAI {
    class OPENAI_API NewFeatureParser {
    public:
        static FString FeatureToJson(const FFeatureRequest& Request);
    };
}
```

## Step 7: Provider Implementation (`OpenAIProvider.cpp`)
1.  **Public Methods**: Construct the URL, create the request using `MakeRequest`, set any required Beta headers, and bind the callback.
2.  **Callbacks**: Use `HandleResponse<T>` to deserialize the JSON and broadcast the delegate.
3.  **Specializations**: Implement any `MakeRequest` specializations declared in the header.

## Step 8: Verification (Tests)
Always add tests to verify the new integration:
1.  **Parser Tests** (`Source/OpenAITestRunner/Private/[Feature]Parser.spec.cpp`): Verify that C++ structs convert to the exact JSON expected by OpenAI.
2.  **Stress Tests** (`Source/OpenAITestRunner/Private/Stress/OpenAI[Feature]API.spec.cpp`): Perform actual CRUD operations against the live API. 
    - Use `ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted))` for async handling.
    - Follow the pattern: Create -> Retrieve -> Update -> List -> Delete.

## Step 9: Add Sample Examples (`Source/OpenAI/Public/Sample/APIOverview.h` & `.cpp`)
To make the new API accessible for manual testing via the `AAPIOverview` actor:
1.  **APIOverview.h**:
    - Add new entries to the `EAPIOverviewAction` enum.
    - Declare new methods for each action.
2.  **APIOverview.cpp**:
    - Register the new enum entries in the `AAPIOverview` constructor's `ActionMap`.
    - Implement the methods, typically using `AddLambda` to handle the response and `UE_LOGFMT` to output the results.
    - Use `UEnum::GetValueAsString` for readable status/enum logging.

---

## Common Pitfalls
- **Case Sensitivity**: Response IDs are often mapped as `.ID` in some legacy structs and `.Id` in others. Check the base struct definition.
- **Beta Headers**: Forgetting `OpenAI-Beta` headers will cause 400 Bad Request or 404 Not Found errors on newer endpoints.
- **Linker Errors**: Forgetting `OPENAI_API` on a Parser class will cause unresolved external symbol errors in the `OpenAITestRunner` module.
- **Abstract Class Errors**: Adding a method to `IAPI` without updating `Sample/APIOverview.cpp` will break the build.

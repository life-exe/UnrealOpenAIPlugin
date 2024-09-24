#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Provider/Types/ModelTypes.h"
#include "Logging/StructuredLog.h"

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForRequestCompleted, bool&, RequestCompleted);
DEFINE_LOG_CATEGORY_STATIC(LogTestUtils, All, All);

namespace OpenAI
{
namespace Tests
{
class TestUtils
{
public:
    static FString RemovePunctuation(const FString& Input);
    static FString PluginEnumToOpenAIModelName(EAllModelEnum PluginEnum);
    static FString OpenAIModelNameToPluginEnum(const FString& ModelName);
    static FString FileFullPath(const FString& FileName);
    static bool IsValidURL(const FString& URL);

    static bool TestFinishReason(const FString& Reason);

    template <typename ResponseType>
    static void TestStreamResponse(FAutomationTestBase* Test, const ResponseType& Response, const FString& ModelName, const FString& Oject)
    {
        if (!Test)
        {
            UE_LOGFMT(LogTestUtils, Error, "Automation test object is invalid");
            return;
        }

        // Test->TestTrue("Name should be valid", Response.Model.Equals(ModelName));
        Test->TestTrue("Created should be valid", Response.Created > 0);
        Test->TestTrue("ID should be valid", !Response.ID.IsEmpty());
        Test->TestTrue("Object should be valid", Response.Object.Equals(Oject));
        for (const auto& Choice : Response.Choices)
        {
            Test->TestTrue("Choice index should be valid", Choice.Index == 0);
            Test->TestTrue("Choice finish_reason should be valid", TestFinishReason(Choice.Finish_Reason));
        }
    }
};
}  // namespace Tests
}  // namespace OpenAI

#endif
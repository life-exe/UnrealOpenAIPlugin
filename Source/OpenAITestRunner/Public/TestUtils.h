#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Provider/Types/ModelTypes.h"

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForRequestCompleted, bool&, RequestCompleted);
bool FWaitForRequestCompleted::Update()
{
    return RequestCompleted;
}

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
};
}  // namespace Tests
}  // namespace OpenAI

#endif
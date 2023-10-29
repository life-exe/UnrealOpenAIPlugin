#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"

namespace OpenAI
{
namespace Tests
{
class TestUtils
{
public:
    static FString RemovePunctuation(const FString& Input);
    static FString PluginEnumToOpenAIModelName(EAllModelEnum ModelEnum);
    static FString FileFullPath(const FString& FileName);
    static bool IsValidURL(const FString& URL);
};
}  // namespace Tests
}  // namespace OpenAI

#endif
// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#if WITH_AUTOMATION_TESTS

#include "TestUtils.h"
#include "FuncLib\ModelTypes.h"
#include "Internationalization/Regex.h"

using namespace OpenAI::Tests;

FString TestUtils::RemovePunctuation(const FString& Input)
{
    FRegexPattern Pattern(TEXT("[\\p{P}\\p{S}]"));
    FRegexMatcher Matcher(Pattern, Input);

    FString Result;
    int32 LastPosition = 0;

    while (Matcher.FindNext())
    {
        Result.Append(Input.Mid(LastPosition, Matcher.GetMatchBeginning() - LastPosition));
        LastPosition = Matcher.GetMatchEnding();
    }

    Result.Append(Input.Mid(LastPosition));

    return Result;
}

FString TestUtils::PluginEnumToOpenAIModelName(EAllModelEnum ModelEnum)
{
    const FString EnumAsString = UEnum::GetValueAsString(ModelEnum);

    // EAllModelEnum::GPT_3_5_Turbo_0301 -> EAllModelEnum, GPT_3_5_Turbo_0301
    FString EnumName, EnumElementName;
    EnumAsString.Split("::", &EnumName, &EnumElementName);

    // GPT_3_5_Turbo_0301 -> gpt-3-5-turbo-0301
    EnumElementName = EnumElementName.ToLower().Replace(TEXT("_"), TEXT("-"));

    // special case: gpt-3-5-turbo-0301 -> gpt-3.5-turbo-0301
    EnumElementName = EnumElementName.Replace(TEXT("3-5"), TEXT("3.5"));

    return EnumElementName;
}

FString TestUtils::FileFullPath(const FString& FileName)
{
    return FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir().Append("OpenAI/Source/OpenAITestRunner/Data/").Append(FileName));
}

bool TestUtils::IsValidURL(const FString& URL)
{
    const FString URLPattern = "^((http|https)://)[-a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)$";
    const FRegexPattern Pattern(URLPattern);
    FRegexMatcher Matcher(Pattern, URL);

    return Matcher.FindNext();
}

#endif
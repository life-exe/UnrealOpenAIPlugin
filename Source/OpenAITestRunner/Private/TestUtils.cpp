// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#if WITH_AUTOMATION_TESTS

#include "TestUtils.h"
#include "Provider/Types/ModelTypes.h"
#include "Internationalization/Regex.h"
#include "FuncLib/OpenAIFuncLib.h"

using namespace OpenAI::Tests;

bool FWaitForRequestCompleted::Update()
{
    return RequestCompleted;
}

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

FString TestUtils::PluginEnumToOpenAIModelName(EAllModelEnum PluginEnum)
{
    const FString EnumAsString = UEnum::GetValueAsString(PluginEnum);

    // EAllModelEnum::GPT_3_5_Turbo_0301 -> EAllModelEnum, GPT_3_5_Turbo_0301
    FString EnumName, EnumElementName;
    EnumAsString.Split("::", &EnumName, &EnumElementName);

    // GPT_3_5_Turbo_0301 -> gpt-3-5-turbo-0301
    EnumElementName = EnumElementName.ToLower().Replace(TEXT("_"), TEXT("-"));

    // special case: gpt-3-5-turbo-0301 -> gpt-3.5-turbo-0301
    EnumElementName = EnumElementName.Replace(TEXT("3-5"), TEXT("3.5"));

    return EnumElementName;
}

FString TestUtils::OpenAIModelNameToPluginEnum(const FString& ModelName)
{
    // gpt-3.5-turbo-0301 -> gpt_3.5_turbo_0301
    FString EnumName = ModelName.ToLower().Replace(TEXT("-"), TEXT("_"));

    // special case: gpt-3.5-turbo-0301 -> gpt_3_5_turbo_0301
    EnumName = EnumName.Replace(TEXT("3.5"), TEXT("3_5"));

    // gpt-3.5-turbo-0301 -> GPT_3_5_turbo_0301
    EnumName = EnumName.Replace(TEXT("gpt"), TEXT("GPT"));

    // GPT_3_5_turbo_super_turbo_0301 -> GPT_3_5_Turbo_Super_Turbo_0301
    FString Result;
    bool bMakeNextUpperCase{true};
    for (TCHAR Char : EnumName)
    {
        if (bMakeNextUpperCase && FChar::IsAlpha(Char))
        {
            Char = FChar::ToUpper(Char);
            bMakeNextUpperCase = false;
        }
        else if (Char == TEXT('_'))
        {
            bMakeNextUpperCase = true;
        }
        Result += Char;
    }

    return FString::Format(TEXT("EAllModelEnum::{0}"), {Result});
}

FString TestUtils::FileFullPath(const FString& FileName)
{
    const FString RelativeFilePath = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), FileName);
    return FPaths::ConvertRelativePathToFull(RelativeFilePath);
}

bool TestUtils::IsValidURL(const FString& URL)
{
    const FString URLPattern = "^((http|https)://)[-a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)$";
    const FRegexPattern Pattern(URLPattern);
    FRegexMatcher Matcher(Pattern, URL);

    return Matcher.FindNext();
}

bool TestUtils::TestFinishReason(const FString& Reason)
{
    const TSet<FString> FinishReson{UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Stop),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Length),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Tool_Calls),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Content_Filter),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Null)};
    return FinishReson.Contains(Reason);
}

#endif
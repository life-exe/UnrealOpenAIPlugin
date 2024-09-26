// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/Chat/ChatCommonTypes.h"

namespace OpenAI
{
class OPENAI_API ChatParser
{
public:
    static FString ChatCompletionToJsonRepresentation(const FChatCompletion& ChatCompletion);
    static bool CleanChunkResponseString(FString& IncomeString, bool& LastString);
};
}  // namespace OpenAI
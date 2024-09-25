// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Provider/Types/Chat/ChatCommonTypes.h"

class OPENAI_API ChatStructTransform
{
public:
    static FString StructToJsonString(const FChatCompletion& ChatCompletion);
};
// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/ModerationTypes.h"

namespace OpenAI
{
class OPENAI_API ModerationParser
{
public:
    static bool DeserializeResponse(const FString& ResponseString, FModerationsResponse& ModerationResponse);
};
}  // namespace OpenAI
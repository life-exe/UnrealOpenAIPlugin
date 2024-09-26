// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/ImageTypes.h"

namespace OpenAI
{
class OPENAI_API ImageParser
{
public:
    static bool DeserializeResponse(const FString& ResponseString, FImageResponse& ImageResponse);
};
}  // namespace OpenAI
// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace OpenAI
{
class OPENAI_API AudioParser
{
public:
    static bool IsVerboseResponse(const FString& ResponseString);
};
}  // namespace OpenAI
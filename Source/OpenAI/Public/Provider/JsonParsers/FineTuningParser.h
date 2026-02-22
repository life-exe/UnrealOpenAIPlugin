// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/FineTuneTypes.h"

namespace OpenAI
{
class OPENAI_API FineTuningParser
{
public:
    static FString FineTuningJobToJsonRepresentation(const FFineTuningJob& FineTuningJob);
};
}  // namespace OpenAI

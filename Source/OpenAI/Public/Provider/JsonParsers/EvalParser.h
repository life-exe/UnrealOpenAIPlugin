// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/EvalTypes.h"

namespace OpenAI
{
class OPENAI_API EvalParser
{
public:
    static FString CreateEvalToJsonRepresentation(const FCreateEval& CreateEval);
    static FString UpdateEvalToJsonRepresentation(const FUpdateEval& UpdateEval);
};
}  // namespace OpenAI

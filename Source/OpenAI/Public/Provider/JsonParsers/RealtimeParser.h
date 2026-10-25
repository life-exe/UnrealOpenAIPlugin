// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/RealtimeTypes.h"

namespace OpenAI
{
class OPENAI_API RealtimeParser
{
public:
    static FString CreateClientSecretToJsonRepresentation(const FCreateRealtimeClientSecret& CreateSecret);
};
}  // namespace OpenAI

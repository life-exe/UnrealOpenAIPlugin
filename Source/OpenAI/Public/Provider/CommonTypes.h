// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.generated.h"

USTRUCT(BlueprintType)
struct FMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Role;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Content;
};

USTRUCT(BlueprintType)
struct FOpenAIAuth
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString APIKey{};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString OrganizationID{};

    bool IsEmpty() const { return APIKey.IsEmpty() || OrganizationID.IsEmpty(); }
};

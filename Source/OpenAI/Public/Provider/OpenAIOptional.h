// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OpenAIOptional.generated.h"

USTRUCT(BlueprintType)
struct FOptionalString
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Value{};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};

    void Set(const FString& InValue)
    {
        Value = InValue;
        IsSet = true;
    }

    void UnSet()
    {
        Value = {};
        IsSet = false;
    }
};

USTRUCT(BlueprintType)
struct FOptionalInt
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    int32 Value{};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};

    void Set(int32 InValue)
    {
        Value = InValue;
        IsSet = true;
    }

    void UnSet()
    {
        Value = {};
        IsSet = false;
    }
};

USTRUCT(BlueprintType)
struct FOptionalBool
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool Value{};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};

    void Set(bool InValue)
    {
        Value = InValue;
        IsSet = true;
    }

    void UnSet()
    {
        Value = {};
        IsSet = false;
    }
};
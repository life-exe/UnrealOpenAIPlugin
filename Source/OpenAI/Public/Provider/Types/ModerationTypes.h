// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModerationTypes.generated.h"

UENUM(BlueprintType)
enum class EModerationsModelEnum : uint8
{
    Text_Moderation_Stable,
    Text_Moderation_Latest
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FModerations
{
    GENERATED_BODY()

    /**
      The input text to classify
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    TArray<FString> Input;

    /**
      Two content moderations models are available: text-moderation-stable and text-moderation-latest.

      The default is text-moderation-latest which will be automatically upgraded over time.
      This ensures you are always using our most accurate model.
      If you use text-moderation-stable, we will provide advanced notice before updating the model.
      Accuracy of text-moderation-stable may be slightly lower than for text-moderation-latest.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model{"text-moderation-stable"};
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FModerationCategories
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Hate{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Hate_Threatening{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Self_Harm{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Sexual{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Sexual_Minors{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Violence{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Violence_Graphic{};
};

USTRUCT(BlueprintType)
struct FModerationScores
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Hate{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Hate_Threatening{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Self_Harm{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Sexual{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Sexual_Minors{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Violence{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Violence_Graphic{};
};

USTRUCT(BlueprintType)
struct FModerationResults
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FModerationCategories Categories;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FModerationScores Category_Scores;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Flagged{};
};

USTRUCT(BlueprintType)
struct FModerationsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FModerationResults> Results;
};

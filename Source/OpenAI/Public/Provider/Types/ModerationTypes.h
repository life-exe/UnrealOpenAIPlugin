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
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
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
    bool Harassment{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Harassment_Threatening{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Self_Harm{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Self_Harm_Intent{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Self_Harm_Instructions{};

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
struct FModerationCategoryScores
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Hate{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Hate_Threatening{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Harassment{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Harassment_Threatening{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Self_Harm{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Self_Harm_Intent{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Self_Harm_Instructions{};

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

    /**
      Whether any of the below categories are flagged.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Flagged{};

    /**
      A list of the categories, and whether they are flagged or not.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FModerationCategories Categories;

    /**
      A list of the categories along with their scores as predicted by model.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FModerationCategoryScores Category_Scores;
};

USTRUCT(BlueprintType)
struct FModerationsResponse
{
    GENERATED_BODY()

    /**
      The unique identifier for the moderation request.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID;

    /**
      The model used to generate the moderation results.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model;

    /**
      A list of moderation objects.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FModerationResults> Results;
};

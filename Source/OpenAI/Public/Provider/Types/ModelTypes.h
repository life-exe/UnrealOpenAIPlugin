// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModelTypes.generated.h"

UENUM(BlueprintType)
enum class EAllModelEnum : uint8
{
    Whisper_1,
    GPT_3_5_Turbo,
    GPT_3_5_Turbo_16k,
    GPT_3_5_Turbo_Instruct_0914,
    GPT_3_5_Turbo_Instruct,
    Text_Embedding_Ada_002,
    Babbage_002,
    Davinci_002,
    GPT_4,
    GPT_4_0613,
    GPT_4_0314,
    DALL_E_2,
    DALL_E_3,
    GPT_4_1106_Preview,
    GPT_4_Vision_Preview,
    GPT_3_5_Turbo_1106,
    TTS_1,
    TTS_1_HD,
    TTS_1_1106,
    TTS_1_HD_1106,
    Text_Embedding_3_Large,
    GPT_4_32K_0314,
    GPT_3_5_Turbo_0125,
    Text_Embedding_3_Small,
    GPT_4_0125_Preview,
    GPT_4_Turbo_Preview,
    GPT_4O_2024_05_13,
    GPT_4O,
    GPT_4_Turbo_2024_04_09,
    GPT_4_Turbo,
    GPT_4_1106_Vision_Preview,
    GPT_4O_Mini,
    GPT_4O_Mini_2024_07_18,
    ChatGPT_4O_Latest,
    GPT_4O_2024_08_06
};

UENUM(BlueprintType)
enum class EMainModelEnum : uint8
{
    GPT_4O,
    GPT_4,
    GPT_4_1106_Preview,
    GPT_4_Vision_Preview,
    GPT_4_0613,
    GPT_4_0314,
    GPT_4O_Mini,
    GPT_3_5_Turbo,
    GPT_3_5_Turbo_Instruct
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

// ... nothing

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FOpenAIModel
{
    GENERATED_BODY()
    /**
      The model identifier, which can be referenced in the API endpoints.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID;

    /**
      The Unix timestamp (in seconds) when the model was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created{};

    /**
      The object type, which is always "model".
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The organization that owns the model.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Owned_By;
};

USTRUCT(BlueprintType)
struct FListModelsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FOpenAIModel> Data;
};

USTRUCT(BlueprintType)
struct FRetrieveModelResponse : public FOpenAIModel
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FDeleteFineTunedModelResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Deleted;
};

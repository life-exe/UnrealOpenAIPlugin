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
    DALL_E_2,
    DALL_E_3,
    GPT_4_1106_Preview,
    GPT_3_5_Turbo_1106,
    TTS_1,
    TTS_1_HD,
    TTS_1_1106,
    TTS_1_HD_1106,
    Text_Embedding_3_Large,
    GPT_3_5_Turbo_0125,
    Text_Embedding_3_Small,
    GPT_4_0125_Preview,
    GPT_4_Turbo_Preview,
    GPT_4O_2024_05_13,
    GPT_4O,
    GPT_4_Turbo_2024_04_09,
    GPT_4_Turbo,
    GPT_4O_Mini,
    GPT_4O_Mini_2024_07_18,
    ChatGPT_4O_Latest,
    GPT_4O_2024_08_06,
    O1_Preview,
    O1_Preview_2024_09_12,
    O1_Mini,
    O1_Mini_2024_09_12,
    GPT_4O_Audio_Preview,
    GPT_4O_Audio_Preview_2024_10_01,
    GPT_4O_Realtime_Preview,
    GPT_4O_Realtime_Preview_2024_10_01,
    GPT_4O_Mini_Realtime_Preview_2024_12_17,
    GPT_4O_Mini_Realtime_Preview,
    GPT_4O_Mini_Audio_Preview_2024_12_17,
    GPT_4O_Mini_Audio_Preview,
    Omni_Moderation_Latest,
    Omni_Moderation_2024_09_26,
    GPT_4O_Audio_Preview_2024_12_17,
    O1,
    O1_2024_12_17,
    O3_Mini_2025_01_31,
    O3_Mini,
    GPT_4O_2024_11_20,
    GPT_4O_Realtime_Preview_2024_12_17,
    GPT_4_5_Preview,
    GPT_4_5_Preview_2025_02_27,
    GPT_4O_Mini_Transcribe,
    GPT_4O_Mini_TTS,
    GPT_4O_Mini_Search_Preview,
    GPT_4O_Search_Preview,
    GPT_4O_Mini_Search_Preview_2025_03_11,
    GPT_4O_Search_Preview_2025_03_11,
    O1_Pro_2025_03_19,
    O1_Pro,
    GPT_4O_Transcribe,
    GPT_4_1_Nano_2025_04_14,
    GPT_4_1_2025_04_14,
    GPT_4_1,
    GPT_4_1_Mini,
    GPT_4_1_Mini_2025_04_14,
    GPT_4_1_Nano,
    O4_Mini,
    O4_Mini_2025_04_16,
    GPT_Image_1,
    O3,
    O3_2025_04_16
};

UENUM(BlueprintType)
enum class EMainModelEnum : uint8
{
    GPT_4O,
    GPT_4,
    GPT_4_1106_Preview,
    GPT_4_0613,
    GPT_4O_Mini,
    GPT_3_5_Turbo,
    GPT_3_5_Turbo_Instruct,
    O1_Mini,
    O1,
    O3,
    O3_Mini,
    GPT_4_5_Preview,
    GPT_4_1,
    GPT_4_1_Mini,
    GPT_4_1_Nano,
    O4_Mini
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

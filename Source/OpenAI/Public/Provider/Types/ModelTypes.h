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
    GPT_4O_2024_08_06,
    GPT_4O_Audio_Preview,
    GPT_4O_Realtime_Preview,
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
    O4_Mini_Deep_Research,
    GPT_4O_Audio_Preview_2025_06_03,
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
    Computer_Use_Preview,
    Computer_Use_Preview_2025_03_11,
    GPT_4O_Realtime_Preview_2025_06_03,
    O4_Mini_Deep_Research_2025_06_26,
    GPT_4_0314,
    GPT_5_Nano_2025_08_07,
    Sora_2,
    Sora_2_Pro,
    GPT_Realtime_Mini,
    O3_2025_04_16,
    O3,
    GPT_5_Nano,
    GPT_5_Mini,
    GPT_5_Mini_2025_08_07,
    GPT_5,
    GPT_5_1_2025_11_13,
    GPT_5_Codex,
    GPT_5_2025_08_07,
    GPT_5_1_Codex_Mini,
    GPT_5_Search_Api,
    GPT_5_1_Chat_Latest,
    GPT_Realtime_Mini_2025_10_06,
    GPT_4O_Transcribe_Diarize,
    GPT_5_Search_Api_2025_10_14,
    GPT_5_1_Codex,
    GPT_5_Pro,
    GPT_5_Pro_2025_10_06,
    GPT_5_Chat_Latest,
    GPT_Audio_Mini,
    GPT_Audio_Mini_2025_10_06,
    GPT_5_1,
    GPT_Realtime_2025_08_28,
    GPT_Realtime,
    GPT_Audio,
    GPT_Audio_2025_08_28,
    GPT_Image_1_Mini,
    GPT_5_2_Codex,
    GPT_4O_Mini_TTS_2025_12_15,
    GPT_Realtime_Mini_2025_12_15,
    GPT_Audio_Mini_2025_12_15,
    ChatGPT_Image_Latest,
    GPT_5_1_Codex_Max,
    GPT_Image_1_5,
    GPT_5_2_2025_12_11,
    GPT_5_2,
    GPT_5_2_Pro_2025_12_11,
    GPT_5_2_Pro,
    GPT_5_2_Chat_Latest,
    GPT_4O_Mini_Transcribe_2025_12_15,
    GPT_4O_Mini_Transcribe_2025_03_20,
    GPT_4O_Mini_TTS_2025_03_20
};

UENUM(BlueprintType)
enum class EMainModelEnum : uint8
{
    GPT_4O,
    GPT_4,
    GPT_4O_Mini,
    O1,
    O3,
    O3_Mini,
    GPT_4_1,
    GPT_4_1_Mini,
    GPT_4_1_Nano,
    O4_Mini,
    GPT_5_Nano,
    GPT_5_Mini,
    GPT_5,
    GPT_5_1,
    GPT_5_Pro,
    GPT_5_Codex
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
    bool Deleted{};
};

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
    GPT_3_5_Turbo_1106,
    TTS_1,
    TTS_1_HD,
    TTS_1_1106,
    TTS_1_HD_1106,
    Text_Embedding_3_Large,
    GPT_3_5_Turbo_0125,
    Text_Embedding_3_Small,
    GPT_4O_2024_05_13,
    GPT_4O,
    GPT_4_Turbo_2024_04_09,
    GPT_4_Turbo,
    GPT_4O_Mini,
    GPT_4O_Mini_2024_07_18,
    GPT_4O_2024_08_06,
    Omni_Moderation_Latest,
    Omni_Moderation_2024_09_26,
    O1,
    O1_2024_12_17,
    O3_Mini_2025_01_31,
    O3_Mini,
    GPT_4O_2024_11_20,
    O4_Mini_Deep_Research,
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
    O4_Mini_Deep_Research_2025_06_26,
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
    GPT_4O_Mini_TTS_2025_03_20,
    GPT_5_3_Codex,
    GPT_Realtime_1_5,
    GPT_Audio_1_5,
    GPT_5_3_Chat_Latest,
    GPT_5_4_2026_03_05,
    GPT_5_4_Pro,
    GPT_5_4_Pro_2026_03_05,
    GPT_5_4,
    GPT_5_4_Nano_2026_03_17,
    GPT_5_4_Nano,
    GPT_5_4_Mini_2026_03_17,
    GPT_5_4_Mini,
    GPT_Image_2,
    GPT_Image_2_2026_04_21,
    GPT_5_5,
    GPT_5_5_2026_04_23,
    GPT_5_5_Pro,
    GPT_5_5_Pro_2026_04_23,
    Chat_Latest,
    GPT_Realtime_Translate,
    GPT_Realtime_2,
    GPT_Realtime_Whisper,
    GPT_5_6_Sol,
    GPT_5_6_Terra,
    GPT_5_6_Luna,
    GPT_Realtime_2_1,
    GPT_Realtime_2_1_Mini,
    GPT_Transcribe,
    GPT_Live_Transcribe
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

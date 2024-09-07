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
    GPT_3_5_Turbo_0301,
    GPT_3_5_Turbo,
    GPT_3_5_Turbo_Instruct
};

UENUM(BlueprintType)
enum class EModerationsModelEnum : uint8
{
    Text_Moderation_Stable,
    Text_Moderation_Latest
};

UENUM(BlueprintType)
enum class EImageModelEnum : uint8
{
    DALL_E_2,
    DALL_E_3
};

UENUM(BlueprintType)
enum class EImageSizeDalle2 : uint8
{
    Size_256x256,
    Size_512x512,
    Size_1024x1024
};

UENUM(BlueprintType)
enum EImageSizeDalle3 : uint8
{
    Size_1024x1024,
    Size_1792x1024,
    Size_1024x1792
};

UENUM(BlueprintType)
enum EOpenAIImageQuality
{
    Standard,
    HD
};

UENUM(BlueprintType)
enum EOpenAIImageStyle
{
    // Vivid causes the model to lean towards generating hyper - real and dramatic images.
    Vivid,
    // Natural causes the model to produce more natural, less hyper - real looking images.
    Natural
};

UENUM(BlueprintType)
enum class EOpenAIImageFormat : uint8
{
    URL,
    B64_JSON
};

UENUM(BlueprintType)
enum class ERole : uint8
{
    System,
    User,
    Assistant,
    Function,
    Tool
};

UENUM(BlueprintType)
enum class ETranscriptFormat : uint8
{
    JSON,
    Text,
    Str,
    Verbose_JSON,
    Vtt
};

UENUM(BlueprintType)
enum class EEmbeddingsEncodingFormat : uint8
{
    Float,
    Base64
};

UENUM(BlueprintType)
enum class EChatResponseFormat : uint8
{
    Text,
    JSON_Object
};

/*
    @todo: add more errors https://platform.openai.com/docs/guides/error-codes
*/
UENUM(BlueprintType)
enum class EOpenAIResponseError : uint8
{
    InvalidAPIKey = 0,
    NetworkError,
    ModelNotFound,
    Unknown
};

UENUM(BlueprintType)
enum class EOpenAIFinishReason : uint8
{
    Stop = 0,
    Length,
    Content_Filter,
    Tool_Calls,
    Null
};

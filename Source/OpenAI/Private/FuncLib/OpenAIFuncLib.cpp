// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/OpenAIFuncLib.h"
#include "Internationalization/Regex.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Logging/StructuredLog.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIFuncLib, All, All);

FString UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum Model)
{
    switch (Model)
    {
        case EAllModelEnum::Whisper_1: return "whisper-1";
        case EAllModelEnum::GPT_3_5_Turbo: return "gpt-3.5-turbo";
        case EAllModelEnum::GPT_3_5_Turbo_16k: return "gpt-3.5-turbo-16k";
        case EAllModelEnum::GPT_3_5_Turbo_Instruct_0914: return "gpt-3.5-turbo-instruct-0914";
        case EAllModelEnum::GPT_3_5_Turbo_Instruct: return "gpt-3.5-turbo-instruct";
        case EAllModelEnum::Text_Embedding_Ada_002: return "text-embedding-ada-002";
        case EAllModelEnum::GPT_4: return "gpt-4";
        case EAllModelEnum::GPT_4_0613: return "gpt-4-0613";
        case EAllModelEnum::DALL_E_2: return "dall-e-2";
        case EAllModelEnum::DALL_E_3: return "dall-e-3";
        case EAllModelEnum::GPT_4_1106_Preview: return "gpt-4-1106-preview";
        case EAllModelEnum::GPT_3_5_Turbo_1106: return "gpt-3.5-turbo-1106";
        case EAllModelEnum::TTS_1: return "tts-1";
        case EAllModelEnum::TTS_1_HD: return "tts-1-hd";
        case EAllModelEnum::TTS_1_1106: return "tts-1-1106";
        case EAllModelEnum::TTS_1_HD_1106: return "tts-1-hd-1106";
        case EAllModelEnum::Text_Embedding_3_Large: return "text-embedding-3-large";
        case EAllModelEnum::GPT_3_5_Turbo_0125: return "gpt-3.5-turbo-0125";
        case EAllModelEnum::Text_Embedding_3_Small: return "text-embedding-3-small";
        case EAllModelEnum::GPT_4_0125_Preview: return "gpt-4-0125-preview";
        case EAllModelEnum::GPT_4_Turbo_Preview: return "gpt-4-turbo-preview";
        case EAllModelEnum::GPT_4O_2024_05_13: return "gpt-4o-2024-05-13";
        case EAllModelEnum::GPT_4O: return "gpt-4o";
        case EAllModelEnum::GPT_4_Turbo_2024_04_09: return "gpt-4-turbo-2024-04-09";
        case EAllModelEnum::GPT_4_Turbo: return "gpt-4-turbo";
        case EAllModelEnum::GPT_4O_Mini: return "gpt-4o-mini";
        case EAllModelEnum::GPT_4O_Mini_2024_07_18: return "gpt-4o-mini-2024-07-18";
        case EAllModelEnum::ChatGPT_4O_Latest: return "chatgpt-4o-latest";
        case EAllModelEnum::GPT_4O_2024_08_06: return "gpt-4o-2024-08-06";
        case EAllModelEnum::O1_Preview: return "o1-preview";
        case EAllModelEnum::O1_Preview_2024_09_12: return "o1-preview-2024-09-12";
        case EAllModelEnum::O1_Mini: return "o1-mini";
        case EAllModelEnum::O1_Mini_2024_09_12: return "o1-mini-2024-09-12";
        case EAllModelEnum::GPT_4O_Audio_Preview: return "gpt-4o-audio-preview";
        case EAllModelEnum::GPT_4O_Audio_Preview_2024_10_01: return "gpt-4o-audio-preview-2024-10-01";
        case EAllModelEnum::GPT_4O_Realtime_Preview: return "gpt-4o-realtime-preview";
        case EAllModelEnum::GPT_4O_Realtime_Preview_2024_10_01: return "gpt-4o-realtime-preview-2024-10-01";
        case EAllModelEnum::GPT_4O_Mini_Realtime_Preview_2024_12_17: return "gpt-4o-mini-realtime-preview-2024-12-17";
        case EAllModelEnum::GPT_4O_Mini_Realtime_Preview: return "gpt-4o-mini-realtime-preview";
        case EAllModelEnum::GPT_4O_Mini_Audio_Preview_2024_12_17: return "gpt-4o-mini-audio-preview-2024-12-17";
        case EAllModelEnum::GPT_4O_Mini_Audio_Preview: return "gpt-4o-mini-audio-preview";
        case EAllModelEnum::Omni_Moderation_Latest: return "omni-moderation-latest";
        case EAllModelEnum::Omni_Moderation_2024_09_26: return "omni-moderation-2024-09-26";
        case EAllModelEnum::GPT_4O_Audio_Preview_2024_12_17: return "gpt-4o-audio-preview-2024-12-17";
        case EAllModelEnum::O1: return "o1";
        case EAllModelEnum::O1_2024_12_17: return "o1-2024-12-17";
        case EAllModelEnum::O3_Mini_2025_01_31: return "o3-mini-2025-01-31";
        case EAllModelEnum::O3_Mini: return "o3-mini";
        case EAllModelEnum::GPT_4O_2024_11_20: return "gpt-4o-2024-11-20";
        case EAllModelEnum::GPT_4O_Realtime_Preview_2024_12_17: return "gpt-4o-realtim-preview-2024-12-17";
        case EAllModelEnum::GPT_4_5_Preview: return "gpt-4.5-preview";
        case EAllModelEnum::GPT_4_5_Preview_2025_02_27: return "gpt-4.5-preview-2025-02-27";
        case EAllModelEnum::GPT_4O_Mini_Transcribe: return "gpt-4o-mini-transcribe";
        case EAllModelEnum::GPT_4O_Mini_TTS: return "gpt-4o-mini-tts";
        case EAllModelEnum::GPT_4O_Mini_Search_Preview: return "gpt-4o-mini-search-preview";
        case EAllModelEnum::GPT_4O_Search_Preview: return "gpt-4o-search-preview";
        case EAllModelEnum::GPT_4O_Mini_Search_Preview_2025_03_11: return "gpt-4o-mini-search-preview-2025-03-11";
        case EAllModelEnum::GPT_4O_Search_Preview_2025_03_11: return "gpt-4o-search-preview-2025-03-11";
        case EAllModelEnum::O1_Pro_2025_03_19: return "o1-pro-2025-03-19";
        case EAllModelEnum::O1_Pro: return "o1-pro";
        case EAllModelEnum::GPT_4O_Transcribe: return "gpt-4o-transcribe";
        case EAllModelEnum::GPT_4_1_Nano_2025_04_14: return "gpt-4.1-nano-2025-04-14";
        case EAllModelEnum::GPT_4_1_2025_04_14: return "gpt-4.1-2025-04-14";
        case EAllModelEnum::GPT_4_1: return "gpt-4.1";
        case EAllModelEnum::GPT_4_1_Mini: return "gpt-4.1-mini";
        case EAllModelEnum::GPT_4_1_Mini_2025_04_14: return "gpt-4.1-mini-2025-04-14";
        case EAllModelEnum::GPT_4_1_Nano: return "gpt-4.1-nano";
        case EAllModelEnum::O4_Mini: return "o4-mini";
        case EAllModelEnum::O4_Mini_2025_04_16: return "o4-mini-2025-04-16";
        case EAllModelEnum::GPT_Image_1: return "gpt-image-1";
        case EAllModelEnum::O3: return "o3";
        case EAllModelEnum::O3_2025_04_16: return "o3-2025-04-16";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum Model)
{
    switch (Model)
    {
        case EMainModelEnum::GPT_4O: return "gpt-4o";
        case EMainModelEnum::GPT_4: return "gpt-4";
        case EMainModelEnum::GPT_4_1106_Preview: return "gpt-4-1106-preview";
        case EMainModelEnum::GPT_4_0613: return "gpt-4-0613";
        case EMainModelEnum::GPT_4O_Mini: return "gpt-4o-mini";
        case EMainModelEnum::GPT_3_5_Turbo: return "gpt-3.5-turbo";
        case EMainModelEnum::GPT_3_5_Turbo_Instruct: return "gpt-3.5-turbo-instruct";
        case EMainModelEnum::O1_Mini: return "o1-mini";
        case EMainModelEnum::O1: return "o1";
        case EMainModelEnum::O3_Mini: return "o3-mini";
        case EMainModelEnum::GPT_4_5_Preview: return "gpt-4.5-preview";
        case EMainModelEnum::GPT_4_1: return "gpt-4.1";
        case EMainModelEnum::GPT_4_1_Mini: return "gpt-4.1-mini";
        case EMainModelEnum::GPT_4_1_Nano: return "gpt-4.1-nano";
        case EMainModelEnum::O4_Mini: return "o4-mini";
        case EMainModelEnum::O3: return "o3";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIModerationModelToString(EModerationsModelEnum Model)
{
    switch (Model)
    {
        case EModerationsModelEnum::Text_Moderation_Latest: return "text-moderation-latest";
        case EModerationsModelEnum::Text_Moderation_Stable: return "text-moderation-stable";
    }
    checkNoEntry();
    return {};
}

bool UOpenAIFuncLib::ModelSupportsVision(const FString& Model)
{
    return OpenAIAllModelToString(EAllModelEnum::O1).Equals(Model) ||  //
           OpenAIAllModelToString(EAllModelEnum::GPT_4O).Equals(Model) ||
           OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini).Equals(Model) ||
           OpenAIAllModelToString(EAllModelEnum::GPT_4_Turbo).Equals(Model);
}

FString UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModel Model)
{
    switch (Model)
    {
        case EAudioModel::Whisper_1: return "whisper-1";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAITTSModelToString(ETTSModel Model)
{
    switch (Model)
    {
        case ETTSModel::TTS_1: return "tts-1";
        case ETTSModel::TTS_1_HD: return "tts-1-hd";
        case ETTSModel::GPT_4O_MINI_TTS: return "gpt-4o-mini-tts";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIVoiceToString(EVoice Voice)
{
    switch (Voice)
    {
        case EVoice::Alloy: return "alloy";
        case EVoice::Ash: return "ash";
        case EVoice::Ballad: return "ballad";
        case EVoice::Coral: return "coral";
        case EVoice::Echo: return "echo";
        case EVoice::Fable: return "fable";
        case EVoice::Onyx: return "onyx";
        case EVoice::Nova: return "nova";
        case EVoice::Sage: return "sage";
        case EVoice::Shimmer: return "shimmer";
        case EVoice::Verse: return "verse";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAITTSAudioFormatToString(ETTSAudioFormat Format)
{
    switch (Format)
    {
        case ETTSAudioFormat::AAC: return "aac";
        case ETTSAudioFormat::FLAC: return "flac";
        case ETTSAudioFormat::MP3: return "mp3";
        case ETTSAudioFormat::OPUS: return "opus";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum Model)
{
    switch (Model)
    {
        case EImageModelEnum::DALL_E_2: return "dall-e-2";
        case EImageModelEnum::DALL_E_3: return "dall-e-3";
        case EImageModelEnum::GPT_Image_1: return "gpt-image-1";
    }
    checkNoEntry();
    return {};
}

EImageModelEnum UOpenAIFuncLib::StringToOpenAIImageModel(const FString& Model)
{
    if (Model.Equals("dall-e-2")) return EImageModelEnum::DALL_E_2;
    if (Model.Equals("dall-e-3")) return EImageModelEnum::DALL_E_3;
    if (Model.Equals("gpt-image-1")) return EImageModelEnum::GPT_Image_1;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown EImageModelEnum: {0}", Model);
    checkNoEntry();

    return {};
}

FString UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2 ImageSize)
{
    switch (ImageSize)
    {
        case EImageSizeDalle2::Size_256x256: return "256x256";
        case EImageSizeDalle2::Size_512x512: return "512x512";
        case EImageSizeDalle2::Size_1024x1024: return "1024x1024";
    }
    checkNoEntry();
    return {};
}

EImageSizeDalle2 UOpenAIFuncLib::StringToOpenAIImageSizeDalle2(const FString& ImageSize)
{
    if (ImageSize.Equals("256x256")) return EImageSizeDalle2::Size_256x256;
    if (ImageSize.Equals("512x512")) return EImageSizeDalle2::Size_512x512;
    if (ImageSize.Equals("1024x1024")) return EImageSizeDalle2::Size_1024x1024;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown EImageSizeDalle2: {0}", ImageSize);
    checkNoEntry();

    return {};
}

FString UOpenAIFuncLib::OpenAIImageSizeDalle3ToString(EImageSizeDalle3 ImageSize)
{
    switch (ImageSize)
    {
        case EImageSizeDalle3::Size_1024x1024: return "1024x1024";
        case EImageSizeDalle3::Size_1024x1792: return "1024x1792";
        case EImageSizeDalle3::Size_1792x1024: return "1792x1024";
    }
    checkNoEntry();
    return {};
}

EImageSizeDalle3 UOpenAIFuncLib::StringToOpenAIImageSizeDalle3(const FString& ImageSize)
{
    if (ImageSize.Equals("1024x1024")) return EImageSizeDalle3::Size_1024x1024;
    if (ImageSize.Equals("1024x1792")) return EImageSizeDalle3::Size_1024x1792;
    if (ImageSize.Equals("1792x1024")) return EImageSizeDalle3::Size_1792x1024;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown EImageSizeDalle3: {0}", ImageSize);
    checkNoEntry();

    return {};
}

FString UOpenAIFuncLib::OpenAIImageSizeGptImage1ToString(EImageSizeGptImage1 ImageSize)
{
    switch (ImageSize)
    {
        case EImageSizeGptImage1::Auto: return "auto";
        case EImageSizeGptImage1::Size_1024x1024: return "1024x1024";
        case EImageSizeGptImage1::Size_1024x1536: return "1024x1536";
        case EImageSizeGptImage1::Size_1536x1024: return "1536x1024";
    }
    checkNoEntry();
    return {};
}

EImageSizeGptImage1 UOpenAIFuncLib::StringToOpenAIImageSizeGptImage1(const FString& ImageSize)
{
    if (ImageSize.Equals("auto")) return EImageSizeGptImage1::Auto;
    if (ImageSize.Equals("1024x1024")) return EImageSizeGptImage1::Size_1024x1024;
    if (ImageSize.Equals("1024x1536")) return EImageSizeGptImage1::Size_1024x1536;
    if (ImageSize.Equals("1536x1024")) return EImageSizeGptImage1::Size_1536x1024;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown EImageSizeGptImage1: {0}", ImageSize);
    checkNoEntry();

    return {};
}

FString UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat ImageFormat)
{
    switch (ImageFormat)
    {
        case EOpenAIImageFormat::URL: return "url";
        case EOpenAIImageFormat::B64_JSON: return "b64_json";
    }
    checkNoEntry();
    return {};
}

EOpenAIImageFormat UOpenAIFuncLib::StringToOpenAIImageFormat(const FString& ImageFormat)
{
    if (ImageFormat.Equals("url")) return EOpenAIImageFormat::URL;
    if (ImageFormat.Equals("b64_json")) return EOpenAIImageFormat::B64_JSON;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown EOpenAIImageFormat: {0}", ImageFormat);
    checkNoEntry();

    return {};
}

FString UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality ImageQuality)
{
    switch (ImageQuality)
    {
        case EOpenAIImageQuality::HD: return "hd";
        case EOpenAIImageQuality::Standard: return "standard";
        case EOpenAIImageQuality::High: return "high";
        case EOpenAIImageQuality::Medium: return "medium";
        case EOpenAIImageQuality::Low: return "low";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIImageOutputFormatToString(EOpenAIImageOutputFormat ImageOutputFormat)
{
    switch (ImageOutputFormat)
    {
        case EOpenAIImageOutputFormat::Jpeg: return "jpeg";
        case EOpenAIImageOutputFormat::Png: return "png";
        case EOpenAIImageOutputFormat::Webp: return "webp";
    }
    checkNoEntry();
    return {};
}

EOpenAIImageQuality UOpenAIFuncLib::StringToOpenAIImageQuality(const FString& ImageQuality)
{
    if (ImageQuality.Equals("hd")) return EOpenAIImageQuality::HD;
    if (ImageQuality.Equals("standard")) return EOpenAIImageQuality::Standard;
    if (ImageQuality.Equals("high")) return EOpenAIImageQuality::High;
    if (ImageQuality.Equals("medium")) return EOpenAIImageQuality::Medium;
    if (ImageQuality.Equals("low")) return EOpenAIImageQuality::Low;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown EOpenAIImageQuality: {0}", ImageQuality);
    checkNoEntry();

    return {};
}

FString UOpenAIFuncLib::OpenAIImageStyleToString(EOpenAIImageStyle ImageStyle)
{
    switch (ImageStyle)
    {
        case EOpenAIImageStyle::Natural: return "natural";
        case EOpenAIImageStyle::Vivid: return "vivid";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIImageBackgroundToString(EOpenAIImageBackground ImageBackground)
{
    switch (ImageBackground)
    {
        case EOpenAIImageBackground::Auto: return "auto";
        case EOpenAIImageBackground::Transparent: return "transparent";
        case EOpenAIImageBackground::Opaque: return "opaque";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIImageModerationToString(EOpenAIImageModeration ImageModeration)
{
    switch (ImageModeration)
    {
        case EOpenAIImageModeration::Auto: return "auto";
        case EOpenAIImageModeration::Low: return "low";
    }
    checkNoEntry();
    return {};
}

EOpenAIImageStyle UOpenAIFuncLib::StringToOpenAIImageStyle(const FString& ImageStyle)
{
    if (ImageStyle.Equals("natural")) return EOpenAIImageStyle::Natural;
    if (ImageStyle.Equals("vivid")) return EOpenAIImageStyle::Vivid;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown EOpenAIImageStyle: {0}", ImageStyle);
    checkNoEntry();

    return {};
}

FString UOpenAIFuncLib::OpenAIRoleToString(ERole Role)
{
    switch (Role)
    {
        case ERole::System: return "system";
        case ERole::User: return "user";
        case ERole::Assistant: return "assistant";
        case ERole::Function: return "function";
        case ERole::Tool: return "tool";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason FinishReason)
{
    switch (FinishReason)
    {
        case EOpenAIFinishReason::Stop: return "stop";
        case EOpenAIFinishReason::Length: return "length";
        case EOpenAIFinishReason::Content_Filter: return "content_filter";
        case EOpenAIFinishReason::Tool_Calls: return "tool_calls";
        case EOpenAIFinishReason::Null: return "";
    }
    checkNoEntry();
    return {};
}

EOpenAIFinishReason UOpenAIFuncLib::StringToOpenAIFinishReason(const FString& FinishReason)
{
    if (FinishReason.Equals("stop")) return EOpenAIFinishReason::Stop;
    if (FinishReason.Equals("length")) return EOpenAIFinishReason::Length;
    if (FinishReason.Equals("content_filter")) return EOpenAIFinishReason::Content_Filter;
    if (FinishReason.Equals("tool_calls")) return EOpenAIFinishReason::Tool_Calls;
    if (FinishReason.IsEmpty()) return EOpenAIFinishReason::Null;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown OpenAIFinishReason: {0}", FinishReason);
    checkNoEntry();
    return {};
}

ERole UOpenAIFuncLib::StringToOpenAIRole(const FString& Role)
{
    if (Role.ToLower().Equals("system")) return ERole::System;
    if (Role.ToLower().Equals("user")) return ERole::User;
    if (Role.ToLower().Equals("assistant")) return ERole::Assistant;
    if (Role.ToLower().Equals("function")) return ERole::Function;
    if (Role.ToLower().Equals("tool")) return ERole::Tool;

    UE_LOGFMT(LogOpenAIFuncLib, Error, "Unknown OpenAIRole: {0}", Role);
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIHeaderTypeToString(EOpenAIHttpHeaderType Type)
{
    switch (Type)
    {
        case EOpenAIHttpHeaderType::XRequestId: return "x-request-id";
        case EOpenAIHttpHeaderType::OpenAIProcessingMS: return "openai-processing-ms";
        case EOpenAIHttpHeaderType::OpenAIOrganization: return "openai-organization";
        case EOpenAIHttpHeaderType::OpenAIVersion: return "openai-version";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::FindOpenAIHttpHeaderByType(const FOpenAIResponseMetadata& Headers, EOpenAIHttpHeaderType Type)
{
    const FString HeaderName = OpenAIHeaderTypeToString(Type);
    for (const auto& Header : Headers.HttpHeaders)
    {
        FString Name, Value;
        if (Header.Split(TEXT(": "), &Name, &Value))
        {
            if (HeaderName.Equals(Name)) return Value;
        }
    }
    return {};
}

FString UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat TranscriptFormat)
{
    switch (TranscriptFormat)
    {
        case ETranscriptFormat::JSON: return "json";
        case ETranscriptFormat::Text: return "text";
        case ETranscriptFormat::Str: return "str";
        case ETranscriptFormat::Verbose_JSON: return "verbose_json";
        case ETranscriptFormat::Vtt: return "vtt";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat EmbeddingsEncodingFormat)
{
    switch (EmbeddingsEncodingFormat)
    {
        case EEmbeddingsEncodingFormat::Float: return "float";
        case EEmbeddingsEncodingFormat::Base64: return "base64";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIChatResponseFormatToString(EChatResponseFormat ChatResponseFormat)
{
    switch (ChatResponseFormat)
    {
        case EChatResponseFormat::Text: return "text";
        case EChatResponseFormat::JSON_Object: return "json_object";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIModelToString(const FOpenAIModel& OpenAIModel)
{
    FString Out = FString::Printf(TEXT("id: %s\n"), *OpenAIModel.ID);
    Out.Append(FString::Printf(TEXT("object: %s\n"), *OpenAIModel.Object));
    Out.Append(FString::Printf(TEXT("created: %i\n"), OpenAIModel.Created));
    Out.Append(FString::Printf(TEXT("owned_by: %s\n"), *OpenAIModel.Owned_By));
    return Out;
}

FString UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType MessageContentType)
{
    switch (MessageContentType)
    {
        case EMessageContentType::Text: return "text";
        case EMessageContentType::Image_URL: return "image_url";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::BoolToString(bool Value)
{
    return Value ? TEXT("true") : TEXT("false");
}

FString UOpenAIFuncLib::RemoveWhiteSpaces(const FString& Input)
{
    FString Result;
    const TSet<TCHAR> Whitespaces{'\t', '\n', '\r'};

    for (const TCHAR& Char : Input)
    {
        if (!Whitespaces.Contains(Char))
        {
            Result += Char;
        }
    }
    return Result;
}

FString UOpenAIFuncLib::OpenAIModerationsToString(const FModerationResults& ModerationResults)
{
    FString Out;
    Out.Append(FString::Printf(TEXT("hate: %s\n"), *BoolToString(ModerationResults.Categories.Hate)));
    Out.Append(FString::Printf(TEXT("hate/threatening: %s\n"), *BoolToString(ModerationResults.Categories.Hate_Threatening)));
    Out.Append(FString::Printf(TEXT("harassment: %s\n"), *BoolToString(ModerationResults.Categories.Harassment)));
    Out.Append(FString::Printf(TEXT("harassment/threatening: %s\n"), *BoolToString(ModerationResults.Categories.Harassment_Threatening)));
    Out.Append(FString::Printf(TEXT("self-harm: %s\n"), *BoolToString(ModerationResults.Categories.Self_Harm)));
    Out.Append(FString::Printf(TEXT("self-harm/intent: %s\n"), *BoolToString(ModerationResults.Categories.Self_Harm_Intent)));
    Out.Append(FString::Printf(TEXT("self-harm/instructions: %s\n"), *BoolToString(ModerationResults.Categories.Self_Harm_Instructions)));
    Out.Append(FString::Printf(TEXT("sexual: %s\n"), *BoolToString(ModerationResults.Categories.Sexual)));
    Out.Append(FString::Printf(TEXT("sexual/minors: %s\n"), *BoolToString(ModerationResults.Categories.Sexual_Minors)));
    Out.Append(FString::Printf(TEXT("violence: %s\n"), *BoolToString(ModerationResults.Categories.Violence)));
    Out.Append(FString::Printf(TEXT("violence/graphic: %s\n\n"), *BoolToString(ModerationResults.Categories.Violence_Graphic)));

    Out.Append(FString::Printf(TEXT("hate: %f\n"), ModerationResults.Category_Scores.Hate));
    Out.Append(FString::Printf(TEXT("hate/threatening: %f\n"), ModerationResults.Category_Scores.Hate_Threatening));
    Out.Append(FString::Printf(TEXT("harassment: %f\n"), ModerationResults.Category_Scores.Harassment));
    Out.Append(FString::Printf(TEXT("harassment/threatening: %f\n"), ModerationResults.Category_Scores.Harassment_Threatening));
    Out.Append(FString::Printf(TEXT("self-harm: %f\n"), ModerationResults.Category_Scores.Self_Harm));
    Out.Append(FString::Printf(TEXT("self-harm/intent: %f\n"), ModerationResults.Category_Scores.Self_Harm_Intent));
    Out.Append(FString::Printf(TEXT("self-harm/instructions: %f\n"), ModerationResults.Category_Scores.Self_Harm_Instructions));
    Out.Append(FString::Printf(TEXT("sexual: %f\n"), ModerationResults.Category_Scores.Sexual));
    Out.Append(FString::Printf(TEXT("sexual/minors: %f\n"), ModerationResults.Category_Scores.Sexual_Minors));
    Out.Append(FString::Printf(TEXT("violence: %f\n"), ModerationResults.Category_Scores.Violence));
    Out.Append(FString::Printf(TEXT("violence/graphic: %f\n\n"), ModerationResults.Category_Scores.Violence_Graphic));

    Out.Append(FString::Printf(TEXT("flagged: %s"), *BoolToString(ModerationResults.Flagged)));

    return Out;
}

FString UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose UploadFilePurpose)
{
    switch (UploadFilePurpose)
    {
        case EUploadFilePurpose::Assistants: return "assistants";
        case EUploadFilePurpose::Vision: return "vision";
        case EUploadFilePurpose::Batch: return "batch";
        case EUploadFilePurpose::FineTune: return "fine-tune";
    }

    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIBatchEndpointToString(EBatchEndpoint BatchEndpoint)
{
    switch (BatchEndpoint)
    {
        case EBatchEndpoint::ChatCompletions: return "/v1/chat/completions";
        case EBatchEndpoint::Completions: return "/v1/completions";
        case EBatchEndpoint::Embeddings: return "/v1/embeddings";
    }

    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIBatchCompletionWindowToString(EBatchCompletionWindow BatchCompletionWindow)
{
    switch (BatchCompletionWindow)
    {
        case EBatchCompletionWindow::Window_24h: return "24h";
    }

    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus UploadStatus)
{
    switch (UploadStatus)
    {
        case EUploadStatus::Pending: return "pending";
        case EUploadStatus::Completed: return "completed";
        case EUploadStatus::Cancelled: return "cancelled";
    }

    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIAssistantToolTypeToString(EAssistantToolType AssistantToolType)
{
    switch (AssistantToolType)
    {
        case EAssistantToolType::CodeInterpreter: return "code_interpreter";
        case EAssistantToolType::FileSearch: return "file_search";
        case EAssistantToolType::Function: return "function";
    }

    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIServiceTierToString(EServiceTier ServiceTier)
{
    switch (ServiceTier)
    {
        case EServiceTier::Auto: return "auto";
        case EServiceTier::Default: return "default";
    }

    checkNoEntry();
    return {};
}

EOpenAIResponseError UOpenAIFuncLib::GetErrorCode(const FString& RawError)
{
    if (RawError.Contains("ConnectionError")) return EOpenAIResponseError::NetworkError;

    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawError);
    TSharedPtr<FJsonObject> JsonObject;

    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        return EOpenAIResponseError::Unknown;
    }

    if (JsonObject.IsValid() && JsonObject->HasField(TEXT("error")))
    {
        const auto Error = JsonObject->GetObjectField(TEXT("error"));
        if (Error->HasField(TEXT("code")))
        {
            const auto Code = Error->GetStringField(TEXT("code"));
            if (Code.Contains("invalid_api_key"))
            {
                return EOpenAIResponseError::InvalidAPIKey;
            }
            else if (Code.Contains("model_not_found"))
            {
                return EOpenAIResponseError::ModelNotFound;
            }
            else if (Code.Contains("insufficient_quota"))
            {
                return EOpenAIResponseError::InsufficientQuota;
            }
            else if (Code.Contains("invalid_language_format"))
            {
                return EOpenAIResponseError::InvalidLanguageFormat;
            }
        }
    }

    return EOpenAIResponseError::Unknown;
}

FString UOpenAIFuncLib::GetErrorMessage(const FString& RawError)
{
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawError);
    TSharedPtr<FJsonObject> JsonObject;

    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject)) return {};

    if (JsonObject.IsValid() && JsonObject->HasField(TEXT("error")))
    {
        const auto Error = JsonObject->GetObjectField(TEXT("error"));
        if (Error->HasField(TEXT("message")))
        {
            return Error->GetStringField(TEXT("message"));
        }
    }

    return {};
}

FString UOpenAIFuncLib::ResponseErrorToString(EOpenAIResponseError Code)
{
    switch (Code)
    {
        case EOpenAIResponseError::InvalidAPIKey: return "Invalid API key";
        case EOpenAIResponseError::NetworkError: return "Network error";
        case EOpenAIResponseError::ModelNotFound: return "Model not found";
        case EOpenAIResponseError::InsufficientQuota: return "Insufficient quota";
        case EOpenAIResponseError::InvalidLanguageFormat: return "Invalid language format";
        case EOpenAIResponseError::Unknown: return "Unknown error";
    }

    return "Unknown error code";
}

// misc

FString UOpenAIFuncLib::MakeURLWithQuery(const FString& URL, const OpenAI::QueryPairs& Args)
{
    FString URLWithQuery = URL + "?";
    for (const auto& [Name, Param] : Args)
    {
        URLWithQuery.Append(Name).Append("=").Append(Param).Append("&");
    }
    URLWithQuery = URLWithQuery.LeftChop(1);
    return URLWithQuery;
}

FString UOpenAIFuncLib::WrapBase64(const FString& Base64String)
{
    return FString::Format(TEXT("data:image/png;base64,{0}"), {Base64String});
}

FString UOpenAIFuncLib::UnWrapBase64(const FString& Base64String)
{
    const FString ToRemove = TEXT("data:image/png;base64,");
    return Base64String.Replace(*ToRemove, TEXT(""));
}

FString UOpenAIFuncLib::FilePathToBase64(const FString& FilePath)
{
    TArray<uint8> ImageData;
    if (!FFileHelper::LoadFileToArray(ImageData, *FilePath))
    {
        return {};
    }
    const FString ImageInBase64 = FBase64::Encode(ImageData);
    return UOpenAIFuncLib::WrapBase64(ImageInBase64);
}

FOpenAIAuth UOpenAIFuncLib::LoadAPITokensFromFile(const FString& FilePath)
{
    TArray<FString> FileLines;
    if (!FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
    {
        UE_LOGFMT(LogOpenAIFuncLib, Error, "Failed loading file: {0}", FilePath);
        return {};
    }
    else if (FileLines.Num() < 2)
    {
        UE_LOGFMT(LogOpenAIFuncLib, Error, "Auth file might have 2 or 3 lines only");
        return {};
    }
    FOpenAIAuth Auth;

    FString ParamName, ParamValue;
    FileLines[0].Split("=", &ParamName, &ParamValue);
    Auth.APIKey = ParamValue;

    FileLines[1].Split("=", &ParamName, &ParamValue);
    Auth.OrganizationID = ParamValue;

    if (FileLines.Num() > 2)
    {
        FileLines[2].Split("=", &ParamName, &ParamValue);
        Auth.ProjectID = ParamValue;
    }

    return Auth;
}

FOpenAIAuth UOpenAIFuncLib::LoadAPITokensFromFileOnce(const FString& FilePath)
{
    static FOpenAIAuth Auth;
    if (Auth.IsEmpty())
    {
        Auth = LoadAPITokensFromFile(FilePath);
    }
    return Auth;
}

OpenAI::ServiceSecrets UOpenAIFuncLib::LoadServiceSecretsFromFile(const FString& FilePath)
{
    TArray<FString> FileLines;
    if (!FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
    {
        UE_LOGFMT(LogOpenAIFuncLib, Error, "Failed loading file: {0}", FilePath);
        return {};
    }

    OpenAI::ServiceSecrets Secrets;
    for (const auto& Line : FileLines)
    {
        FString SecretName, SecretValue;
        Line.Split("=", &SecretName, &SecretValue);
        Secrets.Add(MakeTuple(SecretName, SecretValue));
    }

    return Secrets;
}

bool UOpenAIFuncLib::LoadSecretByName(const OpenAI::ServiceSecrets& Secrets, const FString& SecretName, FString& SecretValue)
{
    const auto* Found =
        Secrets.FindByPredicate([&](const TTuple<FString, FString>& SecretData) { return SecretData.Key.Equals(SecretName); });

    if (Found)
    {
        SecretValue = *Found->Value;
        return true;
    }

    SecretValue = {};
    return false;
}

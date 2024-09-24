// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/OpenAIFuncLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Internationalization/Regex.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Logging/StructuredLog.h"

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
        case EAllModelEnum::GPT_4_0314: return "gpt-4-0314";
        case EAllModelEnum::GPT_4_0613: return "gpt-4-0613";
        case EAllModelEnum::DALL_E_2: return "dall-e-2";
        case EAllModelEnum::DALL_E_3: return "dall-e-3";
        case EAllModelEnum::GPT_4_1106_Preview: return "gpt-4-1106-preview";
        case EAllModelEnum::GPT_4_Vision_Preview: return "gpt-4-vision-preview";
        case EAllModelEnum::GPT_3_5_Turbo_1106: return "gpt-3.5-turbo-1106";
        case EAllModelEnum::TTS_1: return "tts-1";
        case EAllModelEnum::TTS_1_HD: return "tts-1-hd";
        case EAllModelEnum::TTS_1_1106: return "tts-1-1106";
        case EAllModelEnum::TTS_1_HD_1106: return "tts-1-hd-1106";
        case EAllModelEnum::Text_Embedding_3_Large: return "text-embedding-3-large";
        case EAllModelEnum::GPT_4_32K_0314: return "gpt-4-32k-0314";
        case EAllModelEnum::GPT_3_5_Turbo_0125: return "gpt-3.5-turbo-0125";
        case EAllModelEnum::Text_Embedding_3_Small: return "text-embedding-3-small";
        case EAllModelEnum::GPT_4_0125_Preview: return "gpt-4-0125-preview";
        case EAllModelEnum::GPT_4_Turbo_Preview: return "gpt-4-turbo-preview";
        case EAllModelEnum::GPT_4O_2024_05_13: return "gpt-4o-2024-05-13";
        case EAllModelEnum::GPT_4O: return "gpt-4o";
        case EAllModelEnum::GPT_4_Turbo_2024_04_09: return "gpt-4-turbo-2024-04-09";
        case EAllModelEnum::GPT_4_Turbo: return "gpt-4-turbo";
        case EAllModelEnum::GPT_4_1106_Vision_Preview: return "gpt-4-1106-vision-preview";
        case EAllModelEnum::GPT_4O_Mini: return "gpt-4o-mini";
        case EAllModelEnum::GPT_4O_Mini_2024_07_18: return "gpt-4o-mini-2024-07-18";
        case EAllModelEnum::ChatGPT_4O_Latest: return "chatgpt-4o-latest";
        case EAllModelEnum::GPT_4O_2024_08_06: return "gpt-4o-2024-08-06";
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
        case EMainModelEnum::GPT_4_0314: return "gpt-4-0314";
        case EMainModelEnum::GPT_4_0613: return "gpt-4-0613";
        case EMainModelEnum::GPT_4_1106_Preview: return "gpt-4-1106-preview";
        case EMainModelEnum::GPT_4_Vision_Preview: return "gpt-4-vision-preview";
        case EMainModelEnum::GPT_3_5_Turbo: return "gpt-3.5-turbo";
        case EMainModelEnum::GPT_3_5_Turbo_Instruct: return "gpt-3.5-turbo-instruct";
        case EMainModelEnum::GPT_4O_Mini: return "gpt-4o-mini";
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
    return OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview).Equals(Model) ||
           OpenAIAllModelToString(EAllModelEnum::GPT_4_1106_Vision_Preview).Equals(Model) ||
           OpenAIAllModelToString(EAllModelEnum::GPT_4O).Equals(Model);
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
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIVoiceToString(EVoice Voice)
{
    switch (Voice)
    {
        case EVoice::Alloy: return "alloy";
        case EVoice::Echo: return "echo";
        case EVoice::Fable: return "fable";
        case EVoice::Nova: return "nova";
        case EVoice::Onyx: return "onyx";
        case EVoice::Shimmer: return "shimmer";
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
    }
    checkNoEntry();
    return {};
}

EImageModelEnum UOpenAIFuncLib::StringToOpenAIImageModel(const FString& Model)
{
    if (Model.Equals("dall-e-2")) return EImageModelEnum::DALL_E_2;
    if (Model.Equals("dall-e-3")) return EImageModelEnum::DALL_E_3;

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
    }
    checkNoEntry();
    return {};
}

EOpenAIImageQuality UOpenAIFuncLib::StringToOpenAIImageQuality(const FString& ImageQuality)
{
    if (ImageQuality.Equals("hd")) return EOpenAIImageQuality::HD;
    if (ImageQuality.Equals("standard")) return EOpenAIImageQuality::Standard;

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
    Out.Append(FString::Printf(TEXT("root: %s\n"), *OpenAIModel.Root));
    Out.Append(FString::Printf(TEXT("parent: %s\n"), *OpenAIModel.Parent));
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

bool UOpenAIFuncLib::StringToJson(const FString& JsonString, TSharedPtr<FJsonObject>& JsonObject)
{
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
    return FJsonSerializer::Deserialize(JsonReader, JsonObject);
}

namespace
{
void ConvertKeysToLowercaseRecursive(TSharedPtr<FJsonValue> Value);

void ConvertObjectKeysToLowercase(TSharedPtr<FJsonObject> JsonObject)
{
    TSharedPtr<FJsonObject> NewJsonObject = MakeShareable(new FJsonObject);

    for (const auto& Elem : JsonObject->Values)
    {
        const FString LowerKey = Elem.Key.ToLower();
        ConvertKeysToLowercaseRecursive(Elem.Value);
        NewJsonObject->SetField(LowerKey, Elem.Value);
    }

    *JsonObject = *NewJsonObject;
}

void ConvertKeysToLowercaseRecursive(TSharedPtr<FJsonValue> Value)
{
    if (Value->Type == EJson::Object)
    {
        ConvertObjectKeysToLowercase(Value->AsObject());
    }
    else if (Value->Type == EJson::Array)
    {
        const TArray<TSharedPtr<FJsonValue>>& Array = Value->AsArray();
        for (const auto& Item : Array)
        {
            ConvertKeysToLowercaseRecursive(Item);
        }
    }
}

}  // namespace

bool UOpenAIFuncLib::JsonToString(const TSharedPtr<FJsonObject>& JsonObject, FString& JsonString)
{
    TSharedPtr<FJsonObject> NewJsonObject = JsonObject;
    ConvertObjectKeysToLowercase(NewJsonObject);
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
}

FString UOpenAIFuncLib::OpenAIModerationsToString(const FModerationResults& ModerationResults)
{
    FString Out;
    Out.Append(FString::Printf(TEXT("hate: %s\n"), *BoolToString(ModerationResults.Categories.Hate)));
    Out.Append(FString::Printf(TEXT("hate/threatening: %s\n"), *BoolToString(ModerationResults.Categories.Hate_Threatening)));
    Out.Append(FString::Printf(TEXT("self-harm: %s\n"), *BoolToString(ModerationResults.Categories.Self_Harm)));
    Out.Append(FString::Printf(TEXT("sexual: %s\n"), *BoolToString(ModerationResults.Categories.Sexual)));
    Out.Append(FString::Printf(TEXT("sexual/minors: %s\n"), *BoolToString(ModerationResults.Categories.Sexual_Minors)));
    Out.Append(FString::Printf(TEXT("violence: %s\n"), *BoolToString(ModerationResults.Categories.Violence)));
    Out.Append(FString::Printf(TEXT("violence/graphic: %s\n\n"), *BoolToString(ModerationResults.Categories.Violence_Graphic)));

    Out.Append(FString::Printf(TEXT("hate: %f\n"), ModerationResults.Category_Scores.Hate));
    Out.Append(FString::Printf(TEXT("hate/threatening: %f\n"), ModerationResults.Category_Scores.Hate_Threatening));
    Out.Append(FString::Printf(TEXT("self-harm: %f\n"), ModerationResults.Category_Scores.Self_Harm));
    Out.Append(FString::Printf(TEXT("sexual: %f\n"), ModerationResults.Category_Scores.Sexual));
    Out.Append(FString::Printf(TEXT("sexual/minors: %f\n"), ModerationResults.Category_Scores.Sexual_Minors));
    Out.Append(FString::Printf(TEXT("violence: %f\n"), ModerationResults.Category_Scores.Violence));
    Out.Append(FString::Printf(TEXT("violence/graphic: %f\n\n"), ModerationResults.Category_Scores.Violence_Graphic));

    Out.Append(FString::Printf(TEXT("flagged: %s"), *BoolToString(ModerationResults.Flagged)));

    return Out;
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

// we need two markes to make clean JSON object during request serialization
// basically to remove quotes

const FString UOpenAIFuncLib::START_FUNCTION_OBJECT_MARKER = "START_FUNCTION_OBJECT_MARKER";
const FString UOpenAIFuncLib::END_FUNCTION_OBJECT_MARKER = "END_FUNCTION_OBJECT_MARKER";

FString UOpenAIFuncLib::MakeFunctionsString(const TSharedPtr<FJsonObject>& Json)
{
    FString Functions;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Functions);
    FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

    Functions.Append(END_FUNCTION_OBJECT_MARKER);
    Functions = START_FUNCTION_OBJECT_MARKER + Functions;

    return RemoveWhiteSpaces(Functions);
}

FString UOpenAIFuncLib::CleanUpFunctionsObject(const FString& Input)
{
    FString Output{Input};
    const FString StartMarker = FString::Format(TEXT("\"{0}"), {START_FUNCTION_OBJECT_MARKER});
    const FString EndMarker = FString::Format(TEXT("{0}\""), {END_FUNCTION_OBJECT_MARKER});

    const auto Find = [&](const FString& Str, int32 StartIndex)
    { return Output.Find(Str, ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex); };

    int32 StartIndex = Find(StartMarker, 0);
    int32 EndIndex = Find(EndMarker, StartIndex);

    while (StartIndex != INDEX_NONE && EndIndex != INDEX_NONE)
    {
        int32 ContentStart = StartIndex + StartMarker.Len();
        int32 ContentEnd = EndIndex;

        // Extract the substring that needs to have backslashes removed
        FString ContentToClean = Output.Mid(ContentStart, ContentEnd - ContentStart);
        // Replace backslashes within the extracted content
        FString CleanedContent = ContentToClean.Replace(TEXT("\\"), TEXT(""));
        // Replace old content with cleaned content
        Output = Output.Left(ContentStart) + CleanedContent + Output.Mid(EndIndex);

        StartIndex = Find(StartMarker, EndIndex);
        EndIndex = Find(EndMarker, StartIndex);
    }

    Output = Output.Replace(*StartMarker, TEXT(""));
    Output = Output.Replace(*EndMarker, TEXT(""));

    return Output;
}

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

// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/OpenAIFuncLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIFuncLib, All, All);

FString UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum Model)
{
    switch (Model)
    {
        case EAllModelEnum::Babbage: return "babbage";
        case EAllModelEnum::Davinci: return "davinci";
        case EAllModelEnum::Text_Davinci_Edit_001: return "text-davinci-edit-001";
        case EAllModelEnum::Babbage_Code_Search_Code: return "babbage-code-search-code";
        case EAllModelEnum::Text_Similarity_Babbage_001: return "text-similarity-babbage-001";
        case EAllModelEnum::Code_Davinci_Edit_001: return "code-davinci-edit-001";
        case EAllModelEnum::Text_Davinci_001: return "text-davinci-001";
        case EAllModelEnum::Text_Davinci_003: return "text-davinci-003";
        case EAllModelEnum::Ada: return "ada";
        case EAllModelEnum::Curie_Instruct_Beta: return "curie-instruct-beta";
        case EAllModelEnum::Babbage_Code_Search_Text: return "babbage-code-search-text";
        case EAllModelEnum::Babbage_Similarity: return "babbage-similarity";
        case EAllModelEnum::Whisper_1: return "whisper-1";
        case EAllModelEnum::Code_Search_Babbage_Text_001: return "code-search-babbage-text-001";
        case EAllModelEnum::GPT_3_5_Turbo_0301: return "gpt-3.5-turbo-0301";
        case EAllModelEnum::GPT_3_5_Turbo: return "gpt-3.5-turbo";
        case EAllModelEnum::GPT_3_5_Turbo_16k_0613: return "gpt-3.5-turbo-16k-0613";
        case EAllModelEnum::GPT_3_5_Turbo_16k: return "gpt-3.5-turbo-16k";
        case EAllModelEnum::GPT_3_5_Turbo_0613: return "gpt-3.5-turbo-0613";
        case EAllModelEnum::GPT_3_5_Turbo_Instruct_0914: return "gpt-3.5-turbo-instruct-0914";
        case EAllModelEnum::GPT_3_5_Turbo_Instruct: return "gpt-3.5-turbo-instruct";
        case EAllModelEnum::Code_Search_Babbage_Code_001: return "code-search-babbage-code-001";
        case EAllModelEnum::Text_Ada_001: return "text-ada-001";
        case EAllModelEnum::Text_Embedding_Ada_002: return "text-embedding-ada-002";
        case EAllModelEnum::Text_Similarity_Ada_001: return "text-similarity-ada-001";
        case EAllModelEnum::Ada_Code_Search_Code: return "ada-code-search-code";
        case EAllModelEnum::Ada_Similarity: return "ada-similarity";
        case EAllModelEnum::Code_Search_Ada_Text_001: return "code-search-ada-text-001";
        case EAllModelEnum::Text_Search_Ada_Query_001: return "text-search-ada-query-001";
        case EAllModelEnum::Davinci_Search_Document: return "davinci-search-document";
        case EAllModelEnum::Ada_Code_Search_Text: return "ada-code-search-text";
        case EAllModelEnum::Text_Search_Ada_Doc_001: return "text-search-ada-doc-001";
        case EAllModelEnum::Davinci_Instruct_Beta: return "davinci-instruct-beta";
        case EAllModelEnum::Text_Similarity_Curie_001: return "text-similarity-curie-001";
        case EAllModelEnum::Code_Search_Ada_Code_001: return "code-search-ada-code-001";
        case EAllModelEnum::Ada_Search_Query: return "ada-search-query";
        case EAllModelEnum::Text_Search_Davinci_Query_001: return "text-search-davinci-query-001";
        case EAllModelEnum::Curie_Search_Query: return "curie-search-query";
        case EAllModelEnum::Davinci_Search_Query: return "davinci-search-query";
        case EAllModelEnum::Babbage_Search_Document: return "babbage-search-document";
        case EAllModelEnum::Ada_Search_Document: return "ada-search-document";
        case EAllModelEnum::Text_Search_Curie_Query_001: return "text-search-curie-query-001";
        case EAllModelEnum::Text_Search_Babbage_Doc_001: return "text-search-babbage-doc-001";
        case EAllModelEnum::Curie_Search_Document: return "curie-search-document";
        case EAllModelEnum::Text_Search_Curie_Doc_001: return "text-search-curie-doc-001";
        case EAllModelEnum::Babbage_Search_Query: return "babbage-search-query";
        case EAllModelEnum::Text_Babbage_001: return "text-babbage-001";
        case EAllModelEnum::Text_Search_Davinci_Doc_001: return "text-search-davinci-doc-001";
        case EAllModelEnum::Text_Search_Babbage_Query_001: return "text-search-babbage-query-001";
        case EAllModelEnum::Curie_Similarity: return "curie-similarity";
        case EAllModelEnum::Curie: return "curie";
        case EAllModelEnum::Text_Similarity_Davinci_001: return "text-similarity-davinci-001";
        case EAllModelEnum::Text_Davinci_002: return "text-davinci-002";
        case EAllModelEnum::Davinci_Similarity: return "davinci-similarity";
        case EAllModelEnum::Text_Curie_001: return "text-curie:001";
        case EAllModelEnum::GPT_4: return "gpt-4";
        case EAllModelEnum::GPT_4_0314: return "gpt-4-0314";
        case EAllModelEnum::GPT_4_0613: return "gpt-4-0613";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum Model)
{
    switch (Model)
    {
        case EMainModelEnum::GPT_4: return "gpt-4";
        case EMainModelEnum::GPT_4_0314: return "gpt-4-0314";
        case EMainModelEnum::GPT_4_0613: return "gpt-4-0613";
        case EMainModelEnum::GPT_3_5_Turbo_0301: return "gpt-3.5-turbo-0301";
        case EMainModelEnum::GPT_3_5_Turbo: return "gpt-3.5-turbo";
        case EMainModelEnum::GPT_3_5_Turbo_Instruct: return "gpt-3.5-turbo-instruct";
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

FString UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModelEnum Model)
{
    switch (Model)
    {
        case EAudioModelEnum::Whisper_1: return "whisper-1";
    }
    checkNoEntry();
    return {};
}

FString UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize ImageSize)
{
    switch (ImageSize)
    {
        case EImageSize::Size_256x256: return "256x256";
        case EImageSize::Size_512x512: return "512x512";
        case EImageSize::Size_1024x1024: return "1024x1024";
    }
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

FString UOpenAIFuncLib::OpenAIRoleToString(ERole Role)
{
    switch (Role)
    {
        case ERole::System: return "system";
        case ERole::User: return "user";
        case ERole::Assistant: return "assistant";
        case ERole::Function: return "function";
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
        case EOpenAIFinishReason::Function_Call: return "function_call";
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
    if (FinishReason.Equals("function_call")) return EOpenAIFinishReason::Function_Call;
    if (FinishReason.IsEmpty()) return EOpenAIFinishReason::Null;

    UE_LOG(LogOpenAIFuncLib, Error, TEXT("Unknown OpenAIFinishReason: %s"), *FinishReason);
    checkNoEntry();
    return {};
}

ERole UOpenAIFuncLib::StringToOpenAIRole(const FString& Role)
{
    if (Role.ToLower().Equals("system")) return ERole::System;
    if (Role.ToLower().Equals("user")) return ERole::User;
    if (Role.ToLower().Equals("assistant")) return ERole::Assistant;
    if (Role.ToLower().Equals("function")) return ERole::Function;

    UE_LOG(LogOpenAIFuncLib, Error, TEXT("Unknown OpenAIRole: %s"), *Role);
    checkNoEntry();
    return {};
}

FOpenAIAuth UOpenAIFuncLib::LoadAPITokensFromFile(const FString& FilePath)
{
    TArray<FString> FileLines;
    if (!FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
    {
        UE_LOG(LogOpenAIFuncLib, Error, TEXT("Failed loading file: %s"), *FilePath);
        return {};
    }
    else if (FileLines.Num() != 2)
    {
        UE_LOG(LogOpenAIFuncLib, Error, TEXT("Auth file might have 2 lines only"));
        return {};
    }
    FOpenAIAuth Auth;

    FString ParamName, ParamValue;
    FileLines[0].Split("=", &ParamName, &ParamValue);
    Auth.APIKey = ParamValue;

    FileLines[1].Split("=", &ParamName, &ParamValue);
    Auth.OrganizationID = ParamValue;

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
        UE_LOG(LogOpenAIFuncLib, Error, TEXT("Failed loading file: %s"), *FilePath);
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
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        return false;
    }
    return true;
}

bool UOpenAIFuncLib::JsonToString(const TSharedPtr<FJsonObject>& JsonObject, FString& JsonString)
{
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

    if (JsonObject.IsValid() && JsonObject->HasField("error"))
    {
        const auto Error = JsonObject->GetObjectField("error");
        if (Error->HasField("code"))
        {
            const auto Code = Error->GetStringField("code");
            if (Code.Contains("invalid_api_key"))
            {
                return EOpenAIResponseError::InvalidAPIKey;
            }
            else if (Code.Contains("model_not_found"))
            {
                return EOpenAIResponseError::ModelNotFound;
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

    if (JsonObject.IsValid() && JsonObject->HasField("error"))
    {
        const auto Error = JsonObject->GetObjectField("error");
        if (Error->HasField("message"))
        {
            return Error->GetStringField("message");
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

    return Output.ToLower();
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
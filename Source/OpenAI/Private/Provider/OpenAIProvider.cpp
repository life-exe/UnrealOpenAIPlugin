// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Provider/OpenAIProvider.h"
#include "Provider/JsonParsers/ModerationParser.h"
#include "Provider/JsonParsers/ImageParser.h"
#include "Provider/JsonParsers/AudioParser.h"
#include "Provider/JsonParsers/EvalParser.h"
#include "API/API.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Http/HttpHelper.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/JsonFuncLib.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIProvider, All, All);

using namespace OpenAI;

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UOpenAIProvider::UOpenAIProvider() : API(MakeShared<OpenAI::V1::OpenAIAPI>())  //
{
}

void UOpenAIProvider::SetAPI(const TSharedPtr<IAPI>& _API)
{
    API = _API;
}

void UOpenAIProvider::ListModels(const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(API->Models(), "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListModelsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveModel(const FString& ModelName, const FOpenAIAuth& Auth)
{
    check(!ModelName.IsEmpty());

    const FString URL = FString(API->Models()).Append("/").Append(ModelName);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveModelCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DeleteFineTunedModel(const FString& ModelID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Models()).Append("/").Append(ModelID);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteFineTunedModelCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateCompletion(const FCompletion& Completion, const FOpenAIAuth& Auth)
{
    check(!Completion.Model.IsEmpty());
    check(!Completion.Prompt.IsEmpty());

    auto HttpRequest = MakeRequest(Completion, API->Completion(), "POST", Auth);
    if (Completion.Stream)
    {
        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateCompletionStreamCompleted);
        HttpRequest->OnRequestProgress64().BindUObject(this, &ThisClass::OnCreateCompletionStreamProgress);
    }
    else
    {
        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateCompletionCompleted);
    }

    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateChatCompletion(const FChatCompletion& ChatCompletion, const FOpenAIAuth& Auth)
{
    check(!ChatCompletion.Model.IsEmpty());
    check(ChatCompletion.Messages.Num() > 0);

    auto HttpRequest = MakeRequest(ChatCompletion, API->ChatCompletion(), "POST", Auth);
    if (ChatCompletion.Stream)
    {
        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateChatCompletionStreamCompleted);
        HttpRequest->OnRequestProgress64().BindUObject(this, &ThisClass::OnCreateChatCompletionStreamProgress);
    }
    else
    {
        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateChatCompletionCompleted);
    }
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateImage(const FOpenAIImage& Image, const FOpenAIAuth& Auth)
{
    check(!Image.Prompt.IsEmpty());

    auto HttpRequest = MakeRequest(Image, API->ImageGenerations(), "POST", Auth);
    // @todo: make this an API parameter
    const float timeoutSeconds = 60.0f * 5.0f;  // 5 mins
    HttpRequest->SetActivityTimeout(timeoutSeconds);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateImageCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->ImageEdits());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    for (const auto& Image : ImageEdit.Image)
    {
        RequestContent.Append(HttpHelper::AddMIMEFile(Image, "image", BeginBoundary));
    }

    if (!ImageEdit.Mask.IsEmpty())
    {
        RequestContent.Append(HttpHelper::AddMIMEFile(ImageEdit.Mask, "mask", BeginBoundary));
    }
    RequestContent.Append(HttpHelper::AddMIME("model", ImageEdit.Model, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("prompt", ImageEdit.Prompt, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("n", FString::FromInt(ImageEdit.N), BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("size", ImageEdit.Size, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", ImageEdit.Response_Format, BeginBoundary));
    if (ImageEdit.User.IsSet)
    {
        RequestContent.Append(HttpHelper::AddMIME("user", ImageEdit.User.Value, BeginBoundary));
    }
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateImageEditCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateImageVariation(const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->ImageVariations());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(ImageVariation.Image, "image", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("n", FString::FromInt(ImageVariation.N), BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("size", ImageVariation.Size, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", ImageVariation.Response_Format, BeginBoundary));
    if (ImageVariation.User.IsSet)
    {
        RequestContent.Append(HttpHelper::AddMIME("user", ImageVariation.User.Value, BeginBoundary));
    }
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateImageVariationCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateEmbeddings(const FEmbeddings& Embeddings, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(Embeddings, API->Embeddings(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateEmbeddingsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateSpeech(const FSpeech& Speech, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(Speech, API->Speech(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateSpeechCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateAudioTranscription(const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->AudioTranscriptions());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(AudioTranscription.File, "file", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("model", AudioTranscription.Model, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("prompt", AudioTranscription.Prompt, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", AudioTranscription.Response_Format, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("temperature", FString::SanitizeFloat(AudioTranscription.Temperature), BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("language", AudioTranscription.Language, BeginBoundary));
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateAudioTranscriptionCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateAudioTranslation(const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->AudioTranslations());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(AudioTranslation.File, "file", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("model", AudioTranslation.Model, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("prompt", AudioTranslation.Prompt, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", AudioTranslation.Response_Format, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("temperature", FString::SanitizeFloat(AudioTranslation.Temperature), BeginBoundary));
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateAudioTranslationCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateVoice(const FCreateVoice& CreateVoice, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->AudioVoices());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(CreateVoice.Audio_Sample, "audio_sample", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("consent", CreateVoice.Consent, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("name", CreateVoice.Name, BeginBoundary));
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateVoiceCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFiles(const FListFilesParams& ListFilesParams, const FOpenAIAuth& Auth)
{
    FString URL = API->Files();
    TArray<FString> QueryParams;
    if (ListFilesParams.After.IsSet) QueryParams.Add(FString("after=").Append(ListFilesParams.After.Value));
    if (ListFilesParams.Limit.IsSet) QueryParams.Add(FString("limit=").Append(FString::FromInt(ListFilesParams.Limit.Value)));
    if (ListFilesParams.Order.IsSet) QueryParams.Add(FString("order=").Append(ListFilesParams.Order.Value));
    if (ListFilesParams.Purpose.IsSet) QueryParams.Add(FString("purpose=").Append(ListFilesParams.Purpose.Value));
    if (QueryParams.Num() > 0) URL.Append("?").Append(FString::Join(QueryParams, TEXT("&")));

    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFilesCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::UploadFile(const FUploadFile& UploadFile, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();
    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->Files());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(UploadFile.File, "file", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("purpose", UploadFile.Purpose, BeginBoundary));
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnUploadFileCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DeleteFile(const FString& FileID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Files()).Append("/").Append(FileID);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteFileCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveFile(const FString& FileID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Files()).Append("/").Append(FileID);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveFileCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveFileContent(const FString& FileID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Files()).Append("/").Append(FileID).Append("/content");
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveFileContentCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(Moderations, API->Moderations(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateModerationsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateFineTuningJob(const FFineTuningJob& FineTuningJob, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(FineTuningJob, API->FineTuningJobs(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateFineTuningJobCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFineTuningJobs(const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters)
{
    const auto URL = FString(API->FineTuningJobs()).Append(FineTuningQueryParameters.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuningJobsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFineTuningEvents(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters)
{
    const auto URL =
        FString(API->FineTuningJobs()).Append("/").Append(FineTuningJobID).Append("/events").Append(FineTuningQueryParameters.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuningEventsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFineTuningCheckpoints(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters)
{
    const auto URL = FString(API->FineTuningJobs())
                         .Append("/")
                         .Append(FineTuningJobID)
                         .Append("/checkpoints")
                         .Append(FineTuningQueryParameters.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuningCheckpointsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->FineTuningJobs()).Append("/").Append(FineTuningJobID);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveFineTuningJobCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CancelFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->FineTuningJobs()).Append("/").Append(FineTuningJobID).Append("/cancel");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCancelFineTuningJobCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::PauseFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->FineTuningJobs()).Append("/").Append(FineTuningJobID).Append("/pause");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnPauseFineTuningJobCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ResumeFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->FineTuningJobs()).Append("/").Append(FineTuningJobID).Append("/resume");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnResumeFineTuningJobCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateBatch(const FCreateBatch& CreateBatch, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(CreateBatch, API->Batches(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateBatchCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveBatch(const FString& BatchId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Batches()).Append("/").Append(BatchId);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveBatchCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CancelBatch(const FString& BatchId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Batches()).Append("/").Append(BatchId).Append("/cancel");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCancelBatchCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListBatch(const FListBatch& ListBatch, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Batches()).Append(ListBatch.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListBatchCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateUpload(const FCreateUpload& CreateUpload, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(CreateUpload, API->Uploads(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateUploadCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::AddUploadPart(const FString& UploadId, const FAddUploadPart& AddUploadPart, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();
    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    const FString URL = API->Uploads().Append("/").Append(UploadId).Append("/parts");
    HttpRequest->SetURL(URL);
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(AddUploadPart.Data, "data", BeginBoundary));
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnAddUploadPartCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CompleteUpload(const FString& UploadId, const FCompleteUpload& CompleteUpload, const FOpenAIAuth& Auth)
{
    const FString URL = API->Uploads().Append("/").Append(UploadId).Append("/complete");
    auto HttpRequest = MakeRequest(CompleteUpload, URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCompleteUploadCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CancelUpload(const FString& UploadId, const FOpenAIAuth& Auth)
{
    const FString URL = API->Uploads().Append("/").Append(UploadId).Append("/cancel");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCancelUploadCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateAssistant(const FCreateAssistant& CreateAssistant, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(CreateAssistant, API->Assistants(), "POST", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateAssistantCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListAssistants(const FListAssistants& ListAssistants, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Assistants()).Append(ListAssistants.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListAssistantsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveAssistant(const FString& AssistantId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Assistants()).Append("/").Append(AssistantId);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveAssistantCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ModifyAssistant(const FString& AssistantId, const FModifyAssistant& ModifyAssistant, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Assistants()).Append("/").Append(AssistantId);
    auto HttpRequest = MakeRequest(ModifyAssistant, URL, "POST", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnModifyAssistantCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DeleteAssistant(const FString& AssistantId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Assistants()).Append("/").Append(AssistantId);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteAssistantCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateVideo(const FCreateVideo& CreateVideo, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(CreateVideo, API->Videos(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateVideoCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveVideo(const FString& VideoId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Videos()).Append("/").Append(VideoId);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveVideoCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListVideos(const FListVideos& ListVideos, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Videos()).Append(ListVideos.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListVideosCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DeleteVideo(const FString& VideoId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Videos()).Append("/").Append(VideoId);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteVideoCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RemixVideo(const FString& VideoId, const FRemixVideo& RemixVideo, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Videos()).Append("/").Append(VideoId).Append("/remix");
    auto HttpRequest = MakeRequest(RemixVideo, URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRemixVideoCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DownloadVideoContent(
    const FString& VideoId, const FDownloadVideoContent& DownloadVideoContent, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Videos()).Append("/").Append(VideoId).Append("/content").Append(DownloadVideoContent.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDownloadVideoContentCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateEval(const FCreateEval& CreateEval, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(CreateEval, API->Evals(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateEvalCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListEvals(const FEvalQueryParams& QueryParams, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Evals()).Append(QueryParams.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListEvalsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveEval(const FString& EvalId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Evals()).Append("/").Append(EvalId);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveEvalCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::UpdateEval(const FString& EvalId, const FUpdateEval& UpdateEval, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Evals()).Append("/").Append(EvalId);
    auto HttpRequest = MakeRequest(UpdateEval, URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnUpdateEvalCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DeleteEval(const FString& EvalId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Evals()).Append("/").Append(EvalId);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteEvalCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateVectorStore(const FCreateVectorStore& CreateVectorStore, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(CreateVectorStore, API->VectorStores(), "POST", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateVectorStoreCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListVectorStores(const FVectorStoreQueryParams& QueryParams, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append(QueryParams.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListVectorStoresCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveVectorStore(const FString& VectorStoreId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveVectorStoreCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::UpdateVectorStore(const FString& VectorStoreId, const FUpdateVectorStore& UpdateVectorStore, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId);
    auto HttpRequest = MakeRequest(UpdateVectorStore, URL, "POST", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnUpdateVectorStoreCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DeleteVectorStore(const FString& VectorStoreId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteVectorStoreCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateVectorStoreFile(const FString& VectorStoreId, const FString& FileId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/files");
    TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject());
    Json->SetStringField("file_id", FileId);
    FString RequestBodyStr;
    UJsonFuncLib::JsonToString(Json, RequestBodyStr);

    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb("POST");
    HttpRequest->SetContentAsString(RequestBodyStr);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateVectorStoreFileCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListVectorStoreFiles(
    const FString& VectorStoreId, const FVectorStoreFileQueryParams& QueryParams, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/files").Append(QueryParams.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListVectorStoreFilesCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveVectorStoreFile(const FString& VectorStoreId, const FString& FileId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/files/").Append(FileId);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveVectorStoreFileCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::DeleteVectorStoreFile(const FString& VectorStoreId, const FString& FileId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/files/").Append(FileId);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteVectorStoreFileCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateVectorStoreFileBatch(
    const FString& VectorStoreId, const FCreateVectorStoreFileBatch& CreateBatch, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/file_batches");
    auto HttpRequest = MakeRequest(CreateBatch, URL, "POST", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateVectorStoreFileBatchCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveVectorStoreFileBatch(const FString& VectorStoreId, const FString& BatchId, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/file_batches/").Append(BatchId);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveVectorStoreFileBatchCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CancelVectorStoreFileBatch(const FString& VectorStoreId, const FString& BatchId, const FOpenAIAuth& Auth)
{
    const auto URL =
        FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/file_batches/").Append(BatchId).Append("/cancel");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCancelVectorStoreFileBatchCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::SearchVectorStore(const FString& VectorStoreId, const FVectorStoreSearch& Search, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->VectorStores()).Append("/").Append(VectorStoreId).Append("/search");
    auto HttpRequest = MakeRequest(Search, URL, "POST", Auth);
    HttpRequest->SetHeader("OpenAI-Beta", "assistants=v2");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnSearchVectorStoreCompleted);
    ProcessRequest(HttpRequest);
}

///////////////////////////// CALLBACKS /////////////////////////////

#define DEFINE_HTTP_CALLBACK(Name)                                                                                    \
    void UOpenAIProvider::On##Name##Completed(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful) \
    {                                                                                                                 \
        HandleResponse<F##Name##Response>(Response, WasSuccessful, Name##Completed);                                  \
    }
DEFINE_HTTP_CALLBACK(ListModels)
DEFINE_HTTP_CALLBACK(RetrieveModel)
DEFINE_HTTP_CALLBACK(DeleteFineTunedModel)

void UOpenAIProvider::OnCreateCompletionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FCompletionResponse>(Response, WasSuccessful, CreateCompletionCompleted);
}

void UOpenAIProvider::OnCreateCompletionStreamCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    OnStreamCompleted<FCompletionStreamResponse>(Request, Response, WasSuccessful, CreateCompletionStreamCompleted);
}

void UOpenAIProvider::OnCreateCompletionStreamProgress(FHttpRequestPtr Request, uint64 BytesSent, uint64 BytesReceived)
{
    OnStreamProgress<FCompletionStreamResponse>(Request, BytesSent, BytesReceived, CreateCompletionStreamProgresses);
}

void UOpenAIProvider::OnCreateChatCompletionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FChatCompletionResponse>(Response, WasSuccessful, CreateChatCompletionCompleted);
}

void UOpenAIProvider::OnCreateChatCompletionStreamCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    OnStreamCompleted<FChatCompletionStreamResponse>(Request, Response, WasSuccessful, CreateChatCompletionStreamCompleted);
}

void UOpenAIProvider::OnCreateChatCompletionStreamProgress(FHttpRequestPtr Request, uint64 BytesSent, uint64 BytesReceived)
{
    OnStreamProgress<FChatCompletionStreamResponse>(Request, BytesSent, BytesReceived, CreateChatCompletionStreamProgresses);
}

void UOpenAIProvider::OnCreateImageCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Request.IsValid())
    {
        LogError("OnCreateImageCompleted: Request object is invalid!");
        Log(FString::Format(TEXT("OnCreateImageCompleted: Request URL: {0}"), {Request->GetURL()}));
        Log(FString::Format(TEXT("OnCreateImageCompleted: Request Verb: {0}"), {Request->GetVerb()}));
        Log(FString::Format(TEXT("OnCreateImageCompleted: Request Status: {0}"), {EHttpRequestStatus::ToString(Request->GetStatus())}));
        Log(FString::Format(TEXT("OnCreateImageCompleted: Request Elapsed Time: {0} seconds"), {Request->GetElapsedTime()}));
        return;
    }

    if (!Success(Response, WasSuccessful)) return;

    FImageResponse ImageResponse;
    const FString Content = Response.IsValid() ? Response->GetContentAsString() : FString{};
    const FString ResponseURL = Response.IsValid() ? Response->GetURL() : FString{};
    const bool Status = ImageParser::DeserializeResponse(Content, ImageResponse);

    if (!Status || ImageResponse.Data.Num() == 0)
    {
        LogError("Failed to parse image response");
        LogError(Content);
        RequestError.Broadcast(ResponseURL, Content);
        return;
    }

    CreateImageCompleted.Broadcast(ImageResponse, GetResponseHeaders(Response));
}

void UOpenAIProvider::OnCreateImageEditCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Success(Response, WasSuccessful)) return;

    const FString Content = Response ? Response->GetContentAsString() : FString{};
    const FString ResponseURL = Response ? Response->GetURL() : FString{};

    FImageEditResponse ImageEditResponse;
    const bool Status = ImageParser::DeserializeResponse(Content, ImageEditResponse);

    if (!Status || ImageEditResponse.Data.Num() == 0)
    {
        LogError("Failed to parse image edit response");
        LogError(Content);
        RequestError.Broadcast(ResponseURL, Content);
        return;
    }
    CreateImageEditCompleted.Broadcast(ImageEditResponse, GetResponseHeaders(Response));
}

void UOpenAIProvider::OnCreateImageVariationCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Success(Response, WasSuccessful)) return;

    const FString Content = Response.IsValid() ? Response->GetContentAsString() : FString{};
    const FString ResponseURL = Response.IsValid() ? Response->GetURL() : FString{};

    FImageVariationResponse ImageVariationResponse;
    const bool Status = ImageParser::DeserializeResponse(Content, ImageVariationResponse);

    if (!Status || ImageVariationResponse.Data.Num() == 0)
    {
        LogError("Failed to parse image variation response");
        LogError(Content);
        RequestError.Broadcast(ResponseURL, Content);
        return;
    }
    CreateImageVariationCompleted.Broadcast(ImageVariationResponse, GetResponseHeaders(Response));
}

void UOpenAIProvider::OnCreateEmbeddingsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FEmbeddingsResponse>(Response, WasSuccessful, CreateEmbeddingsCompleted);
}

void UOpenAIProvider::OnCreateSpeechCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (WasSuccessful && Response.IsValid())
    {
        FSpeechResponse SpeechResponse;
        SpeechResponse.Bytes = Response->GetContent();  //@todo move semantics needed
        CreateSpeechCompleted.Broadcast(SpeechResponse, GetResponseHeaders(Response));
    }
    else
    {
        LogError("On create speech error");
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
    }
}

void UOpenAIProvider::OnCreateAudioTranscriptionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (Response.IsValid() && AudioParser::IsVerboseResponse(Response->GetContentAsString()))
    {
        HandleResponse<FAudioTranscriptionVerboseResponse>(Response, WasSuccessful, CreateAudioTranscriptionVerboseCompleted);
    }
    else
    {
        HandleResponse<FAudioTranscriptionResponse>(Response, WasSuccessful, CreateAudioTranscriptionCompleted);
    }
}

void UOpenAIProvider::OnCreateAudioTranslationCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FAudioTranslationResponse>(Response, WasSuccessful, CreateAudioTranslationCompleted);
}

void UOpenAIProvider::OnCreateVoiceCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FCreateVoiceResponse>(Response, WasSuccessful, CreateVoiceCompleted);
}

DEFINE_HTTP_CALLBACK(ListFiles)
DEFINE_HTTP_CALLBACK(UploadFile)
DEFINE_HTTP_CALLBACK(DeleteFile)
DEFINE_HTTP_CALLBACK(RetrieveFile)

void UOpenAIProvider::OnRetrieveFileContentCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    const FString Content = Response.IsValid() ? Response->GetContentAsString() : FString{};
    const FString ResponseURL = Response.IsValid() ? Response->GetURL() : FString{};

    if (!WasSuccessful)
    {
        RequestError.Broadcast(ResponseURL, Content);
        return;
    }

    FRetrieveFileContentResponse ParsedResponse;
    ParsedResponse.Content = Content;
    RetrieveFileContentCompleted.Broadcast(ParsedResponse, GetResponseHeaders(Response));
}

void UOpenAIProvider::OnCreateModerationsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Success(Response, WasSuccessful)) return;

    const FString Content = Response.IsValid() ? Response->GetContentAsString() : FString{};
    const FString ResponseURL = Response.IsValid() ? Response->GetURL() : FString{};

    FModerationsResponse ModerationResponse;
    const bool Status = ModerationParser::DeserializeResponse(Content, ModerationResponse);
    if (!Status)
    {
        LogError("Failed to parse moderations response");
        RequestError.Broadcast(ResponseURL, Content);
        return;
    }

    CreateModerationsCompleted.Broadcast(ModerationResponse, GetResponseHeaders(Response));
}

void UOpenAIProvider::OnCreateFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, CreateFineTuningJobCompleted);
}

DEFINE_HTTP_CALLBACK(ListFineTuningJobs)
DEFINE_HTTP_CALLBACK(ListFineTuningEvents)
DEFINE_HTTP_CALLBACK(ListFineTuningCheckpoints)

void UOpenAIProvider::OnRetrieveFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, RetrieveFineTuningJobCompleted);
}

void UOpenAIProvider::OnCancelFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, CancelFineTuningJobCompleted);
}

void UOpenAIProvider::OnPauseFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, PauseFineTuningJobCompleted);
}

void UOpenAIProvider::OnResumeFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, ResumeFineTuningJobCompleted);
}

DEFINE_HTTP_CALLBACK(CreateBatch)
DEFINE_HTTP_CALLBACK(RetrieveBatch)
DEFINE_HTTP_CALLBACK(CancelBatch)
DEFINE_HTTP_CALLBACK(ListBatch)

void UOpenAIProvider::OnCreateUploadCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FUploadObjectResponse>(Response, WasSuccessful, CreateUploadCompleted);
}

void UOpenAIProvider::OnAddUploadPartCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FUploadPartObjectResponse>(Response, WasSuccessful, AddUploadPartCompleted);
}

void UOpenAIProvider::OnCompleteUploadCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FUploadObjectResponse>(Response, WasSuccessful, CompleteUploadCompleted);
}

void UOpenAIProvider::OnCancelUploadCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FUploadObjectResponse>(Response, WasSuccessful, CancelUploadCompleted);
}

void UOpenAIProvider::OnCreateAssistantCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FAssistantObjectResponse>(Response, WasSuccessful, CreateAssistantCompleted);
}

DEFINE_HTTP_CALLBACK(ListAssistants)

void UOpenAIProvider::OnRetrieveAssistantCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FAssistantObjectResponse>(Response, WasSuccessful, RetrieveAssistantCompleted);
}

void UOpenAIProvider::OnModifyAssistantCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FAssistantObjectResponse>(Response, WasSuccessful, ModifyAssistantCompleted);
}

DEFINE_HTTP_CALLBACK(DeleteAssistant)

void UOpenAIProvider::OnCreateVideoCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVideoObject>(Response, WasSuccessful, CreateVideoCompleted);
}

void UOpenAIProvider::OnRetrieveVideoCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVideoObject>(Response, WasSuccessful, RetrieveVideoCompleted);
}

DEFINE_HTTP_CALLBACK(ListVideos)
DEFINE_HTTP_CALLBACK(DeleteVideo)
DEFINE_HTTP_CALLBACK(CreateEval)
DEFINE_HTTP_CALLBACK(ListEvals)
DEFINE_HTTP_CALLBACK(RetrieveEval)
DEFINE_HTTP_CALLBACK(UpdateEval)
DEFINE_HTTP_CALLBACK(DeleteEval)

void UOpenAIProvider::OnRemixVideoCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVideoObject>(Response, WasSuccessful, RemixVideoCompleted);
}

void UOpenAIProvider::OnCreateVectorStoreCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStore>(Response, WasSuccessful, CreateVectorStoreCompleted);
}

void UOpenAIProvider::OnListVectorStoresCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FListVectorStoresResponse>(Response, WasSuccessful, ListVectorStoresCompleted);
}

void UOpenAIProvider::OnRetrieveVectorStoreCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStore>(Response, WasSuccessful, RetrieveVectorStoreCompleted);
}

void UOpenAIProvider::OnUpdateVectorStoreCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStore>(Response, WasSuccessful, UpdateVectorStoreCompleted);
}

void UOpenAIProvider::OnDeleteVectorStoreCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreDeletedResponse>(Response, WasSuccessful, DeleteVectorStoreCompleted);
}

void UOpenAIProvider::OnCreateVectorStoreFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreFile>(Response, WasSuccessful, CreateVectorStoreFileCompleted);
}

void UOpenAIProvider::OnListVectorStoreFilesCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FListVectorStoreFilesResponse>(Response, WasSuccessful, ListVectorStoreFilesCompleted);
}

void UOpenAIProvider::OnRetrieveVectorStoreFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreFile>(Response, WasSuccessful, RetrieveVectorStoreFileCompleted);
}

void UOpenAIProvider::OnDeleteVectorStoreFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreDeletedResponse>(Response, WasSuccessful, DeleteVectorStoreFileCompleted);
}

void UOpenAIProvider::OnCreateVectorStoreFileBatchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreFileBatch>(Response, WasSuccessful, CreateVectorStoreFileBatchCompleted);
}

void UOpenAIProvider::OnRetrieveVectorStoreFileBatchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreFileBatch>(Response, WasSuccessful, RetrieveVectorStoreFileBatchCompleted);
}

void UOpenAIProvider::OnCancelVectorStoreFileBatchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreFileBatch>(Response, WasSuccessful, CancelVectorStoreFileBatchCompleted);
}

void UOpenAIProvider::OnSearchVectorStoreCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FVectorStoreSearchResponse>(Response, WasSuccessful, SearchVectorStoreCompleted);
}

void UOpenAIProvider::OnDownloadVideoContentCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    const FString ResponseURL = Response.IsValid() ? Response->GetURL() : FString{};
    const FString Content = Response.IsValid() ? Response->GetContentAsString() : FString{};

    if (!WasSuccessful)
    {
        RequestError.Broadcast(ResponseURL, Content);
        return;
    }

    FDownloadVideoContentResponse ParsedResponse;
    ParsedResponse.Content = Response->GetContent();
    DownloadVideoContentCompleted.Broadcast(ParsedResponse, GetResponseHeaders(Response));
}

///////////////////////////// HELPER FUNCTIONS /////////////////////////////

void UOpenAIProvider::ProcessRequest(FHttpRequestRef HttpRequest)
{
    if (bLogEnabled)
    {
        UE_LOGFMT(LogOpenAIProvider, Display, "Request processing started: {0} {1}", HttpRequest->GetVerb(), HttpRequest->GetURL());
    }

    if (!HttpRequest->ProcessRequest())
    {
        LogError(FString::Format(TEXT("Can't process: {0}"), {HttpRequest->GetURL()}));
        RequestError.Broadcast(HttpRequest->GetURL(), {});
    }
}

bool UOpenAIProvider::Success(FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Response.IsValid())
    {
        LogError(FString::Format(TEXT("Response is nullptr, WasSuccessful: {0}"), {WasSuccessful ? TEXT("true") : TEXT("false")}));
        RequestError.Broadcast("null", "null");
        return false;
    }

    const FString Content = Response->GetContentAsString();
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Content);
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        LogError(FString::Format(TEXT("JSON deserialization error: {0}"), {Content}));
        RequestError.Broadcast(Response->GetURL(), Content);
        return false;
    }

    if (!WasSuccessful || !JsonObject.IsValid() || UJsonFuncLib::OpenAIResponseContainsError(JsonObject))
    {
        LogError(Content);
        RequestError.Broadcast(Response->GetURL(), Content);
        return false;
    }

    LogResponse(Response);
    return true;
}

void UOpenAIProvider::Log(const FString& Info) const
{
    if (bLogEnabled)
    {
        UE_LOGFMT(LogOpenAIProvider, Display, "{0}", Info);
    }
}

void UOpenAIProvider::LogResponse(FHttpResponsePtr Response) const
{
    if (bLogEnabled)
    {
        UE_LOGFMT(LogOpenAIProvider, Display, "Response. Request URL: {0}", Response.IsValid() ? Response->GetURL() : FString{});
        UE_LOGFMT(LogOpenAIProvider, Display, "Response. Content: {0}", Response.IsValid() ? Response->GetContentAsString() : FString{});
    }
}

void UOpenAIProvider::LogError(const FString& ErrorText) const
{
    UE_LOGFMT(LogOpenAIProvider, Error, "{0}", ErrorText);
}

void UOpenAIProvider::SetOptional(TSharedPtr<FJsonObject> RequestBody, const TOptional<FString>& Param, const FString& ParamName)
{
    if (!Param.IsSet()) return;
    RequestBody->SetStringField(ParamName, Param.GetValue());
}

void UOpenAIProvider::SetOptional(TSharedPtr<FJsonObject> RequestBody, const TOptional<bool>& Param, const FString& ParamName)
{
    if (!Param.IsSet()) return;
    RequestBody->SetBoolField(ParamName, Param.GetValue());
}

void UOpenAIProvider::SetOptional(TSharedPtr<FJsonObject> RequestBody, const TOptional<TArray<float>>& Param, const FString& ParamName)
{
    if (!Param.IsSet()) return;

    TArray<TSharedPtr<FJsonValue>> JsonValues;
    for (float Value : Param.GetValue())
    {
        JsonValues.Add(MakeShareable(new FJsonValueNumber(Value)));
    }
    RequestBody->SetArrayField(ParamName, JsonValues);
}

TTuple<FString, FString> UOpenAIProvider::GetErrorData(FHttpRequestPtr Request, FHttpResponsePtr Response) const
{
    const auto ResponseContent = Response ? Response->GetContentAsString() : FString{};
    const auto ResponseURL = Response ? Response->GetURL() : FString{};

    if (!ResponseContent.IsEmpty())
    {
        return MakeTuple(ResponseURL, ResponseContent);
    }

    const auto Status = Request ? EHttpRequestStatus::ToString(Request->GetStatus()) : FString{};
    return MakeTuple(ResponseURL, Status);
}

FHttpRequestRef UOpenAIProvider::MakeRequest(const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);
    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FChatCompletion& ChatCompletion, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = ChatParser::ChatCompletionToJsonRepresentation(ChatCompletion);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FFineTuningJob& FineTuningJob, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = FineTuningParser::FineTuningJobToJsonRepresentation(FineTuningJob);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FCreateEval& CreateEval, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = EvalParser::CreateEvalToJsonRepresentation(CreateEval);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FUpdateEval& UpdateEval, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = EvalParser::UpdateEvalToJsonRepresentation(UpdateEval);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FCreateVectorStore& CreateVectorStore, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = VectorStoreParser::CreateVectorStoreToJsonRepresentation(CreateVectorStore);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FUpdateVectorStore& UpdateVectorStore, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = VectorStoreParser::UpdateVectorStoreToJsonRepresentation(UpdateVectorStore);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FCreateVectorStoreFileBatch& CreateBatch, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = VectorStoreParser::CreateVectorStoreFileBatchToJsonRepresentation(CreateBatch);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FVectorStoreSearch& Search, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = MakeRequestHeaders(Auth);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    const FString RequestBodyStr = VectorStoreParser::SearchVectorStoreToJsonRepresentation(Search);
    Log(FString("Postprocessed content was set as: ").Append(RequestBodyStr));
    HttpRequest->SetContentAsString(RequestBodyStr);

    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequestHeaders(const FOpenAIAuth& Auth) const
{
    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", FString("Bearer ").Append(Auth.APIKey));
    HttpRequest->SetHeader("OpenAI-Organization", Auth.OrganizationID);
    HttpRequest->SetHeader("OpenAI-Project", Auth.ProjectID);
    return HttpRequest;
}

FOpenAIResponseMetadata UOpenAIProvider::GetResponseHeaders(FHttpResponsePtr Response) const
{
    FOpenAIResponseMetadata Metadata;
    if (Response.IsValid() && Response->GetStatus() != EHttpRequestStatus::Processing)
    {
        Metadata.HttpHeaders = Response->GetAllHeaders();
    }
    return {};
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS

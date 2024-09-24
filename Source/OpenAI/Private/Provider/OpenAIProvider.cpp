// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Provider/OpenAIProvider.h"
#include "API/API.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Http/HttpHelper.h"
#include "FuncLib/OpenAIFuncLib.h"
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

void UOpenAIProvider::CreateCompletion(const FCompletion& Completion, const FOpenAIAuth& Auth)
{
    check(!Completion.Model.IsEmpty());
    check(!Completion.Prompt.IsEmpty());

    auto HttpRequest = MakeRequest(Completion, API->Completion(), "POST", Auth);
    if (Completion.Stream)
    {
        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateCompletionStreamCompleted);
        HttpRequest->OnRequestProgress().BindUObject(this, &ThisClass::OnCreateCompletionStreamProgress);
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
        HttpRequest->OnRequestProgress().BindUObject(this, &ThisClass::OnCreateChatCompletionStreamProgress);
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
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateImageCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->ImageEdits());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary =" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(ImageEdit.Image, "image", BeginBoundary));
    if (!ImageEdit.Mask.IsEmpty())
    {
        RequestContent.Append(HttpHelper::AddMIMEFile(ImageEdit.Mask, "mask", BeginBoundary));
    }
    RequestContent.Append(HttpHelper::AddMIME("prompt", ImageEdit.Prompt, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("n", FString::FromInt(ImageEdit.N), BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("size", ImageEdit.Size, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", ImageEdit.Response_Format, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("user", ImageEdit.User, BeginBoundary));
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
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary =" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(ImageVariation.Image, "image", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("n", FString::FromInt(ImageVariation.N), BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("size", ImageVariation.Size, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", ImageVariation.Response_Format, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("user", ImageVariation.User, BeginBoundary));
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
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary =" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(AudioTranscription.File, "file", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("model", AudioTranscription.Model, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("prompt", AudioTranscription.Prompt, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", AudioTranscription.Response_Format, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("temperature", FString::FromInt(AudioTranscription.Temperature), BeginBoundary));
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
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary =" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(AudioTranslation.File, "file", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("model", AudioTranslation.Model, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("prompt", AudioTranslation.Prompt, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("response_format", AudioTranslation.Response_Format, BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("temperature", FString::FromInt(AudioTranslation.Temperature), BeginBoundary));
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateAudioTranslationCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFiles(const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(API->Files(), "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFilesCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::UploadFile(const FUploadFile& UploadFile, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();
    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(API->Files());
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary =" + Boundary);
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

void UOpenAIProvider::DeleteFineTunedModel(const FString& ModelID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->Models()).Append("/").Append(ModelID);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteFineTunedModelCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(Moderations, API->Moderations(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateModerationsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFineTuningJobs(const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters)
{
    const auto URL = FString(API->FineTuningJobs()).Append(FineTuningQueryParameters.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuningJobsCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CreateFineTuningJob(const FFineTuningJob& FineTuningJob, const FOpenAIAuth& Auth)
{
    // The current request is case sensitive for file ids and optional params.
    // That's why special serialisation is needed.

    auto HttpRequest = MakeRequest(API->FineTuningJobs(), "POST", Auth);

    TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject());
    RequestBody->SetStringField("training_file", FineTuningJob.Training_File);
    RequestBody->SetStringField("model", FineTuningJob.Model);
    if (FineTuningJob.Hyperparameters.IsSet())
    {
        TSharedPtr<FJsonObject> HyperparametersObj = MakeShareable(new FJsonObject());
        HyperparametersObj->SetStringField("n_epochs", FineTuningJob.Hyperparameters.GetValue().N_Epochs);
        RequestBody->SetObjectField("hyperparameters", HyperparametersObj);
    }
    SetOptional(RequestBody, FineTuningJob.Suffix, "suffix");
    SetOptional(RequestBody, FineTuningJob.Validation_File, "validation_file");

    FString RequestBodyStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBodyStr);
    FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(RequestBodyStr);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateFineTuningJobCompleted);
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

void UOpenAIProvider::ListFineTuningEvents(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters)
{
    const auto URL =
        FString(API->FineTuningJobs()).Append("/").Append(FineTuningJobID).Append("/events").Append(FineTuningQueryParameters.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuningEventsCompleted);
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

///////////////////////////// CALLBACKS /////////////////////////////

void UOpenAIProvider::OnListModelsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FListModelsResponse>(Response, WasSuccessful, ListModelsCompleted);
}

void UOpenAIProvider::OnRetrieveModelCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FRetrieveModelResponse>(Response, WasSuccessful, RetrieveModelCompleted);
}

void UOpenAIProvider::OnCreateCompletionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FCompletionResponse>(Response, WasSuccessful, CreateCompletionCompleted);
}

void UOpenAIProvider::OnCreateCompletionStreamCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    OnStreamCompleted<FCompletionStreamResponse>(Request, Response, WasSuccessful, CreateCompletionStreamCompleted);
}

void UOpenAIProvider::OnCreateCompletionStreamProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
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

void UOpenAIProvider::OnCreateChatCompletionStreamProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
    OnStreamProgress<FChatCompletionStreamResponse>(Request, BytesSent, BytesReceived, CreateChatCompletionStreamProgresses);
}

void UOpenAIProvider::OnCreateImageCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Success(Response, WasSuccessful)) return;

    FImageResponse ImageResponse;
    if (ParseImageRequest(Response, ImageResponse))
    {
        CreateImageCompleted.Broadcast(ImageResponse);
    }
}

void UOpenAIProvider::OnCreateImageEditCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Success(Response, WasSuccessful)) return;

    FImageEditResponse ImageEditResponse;
    if (ParseImageRequest(Response, ImageEditResponse))
    {
        CreateImageEditCompleted.Broadcast(ImageEditResponse);
    }
}

void UOpenAIProvider::OnCreateImageVariationCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Success(Response, WasSuccessful)) return;

    FImageVariationResponse ImageVariationResponse;
    if (ParseImageRequest(Response, ImageVariationResponse))
    {
        CreateImageVariationCompleted.Broadcast(ImageVariationResponse);
    }
}

bool UOpenAIProvider::ParseImageRequest(FHttpResponsePtr Response, FImageResponse& ImageResponse)
{
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    TSharedPtr<FJsonObject> JsonObject;
    FJsonSerializer::Deserialize(JsonReader, JsonObject);

    ImageResponse.Created = JsonObject->GetNumberField("created");

    const auto& DataArray = JsonObject->GetArrayField("data");
    for (const auto& DataElem : DataArray)
    {
        FImageObject ImageObject;
        if (FJsonObjectConverter::JsonObjectToUStruct(DataElem->AsObject().ToSharedRef(), &ImageObject, 0, 0))
        {
            ImageResponse.Data.Push(ImageObject);
        }
    }

    if (ImageResponse.Data.Num() == 0)
    {
        LogError("JSON deserialization error");
        LogError(Response->GetContentAsString());
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        return false;
    }

    return true;
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
        CreateSpeechCompleted.Broadcast(SpeechResponse);
    }
    else
    {
        LogError("On create speech error");
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
    }
}

void UOpenAIProvider::OnCreateAudioTranscriptionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FAudioTranscriptionResponse>(Response, WasSuccessful, CreateAudioTranscriptionCompleted);
}

void UOpenAIProvider::OnCreateAudioTranslationCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FAudioTranslationResponse>(Response, WasSuccessful, CreateAudioTranslationCompleted);
}

void UOpenAIProvider::OnListFilesCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FListFilesResponse>(Response, WasSuccessful, ListFilesCompleted);
}

void UOpenAIProvider::OnUploadFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FUploadFileResponse>(Response, WasSuccessful, UploadFileCompleted);
}

void UOpenAIProvider::OnDeleteFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FDeleteFileResponse>(Response, WasSuccessful, DeleteFileCompleted);
}

void UOpenAIProvider::OnRetrieveFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FRetrieveFileResponse>(Response, WasSuccessful, RetrieveFileCompleted);
}

void UOpenAIProvider::OnRetrieveFileContentCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!WasSuccessful)
    {
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        return;
    }

    FRetrieveFileContentResponse ParsedResponse;
    ParsedResponse.Content = Response->GetContentAsString();
    RetrieveFileContentCompleted.Broadcast(ParsedResponse);
}

void UOpenAIProvider::OnCreateModerationsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Success(Response, WasSuccessful)) return;

    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject)) return;

    FModerationsResponse ParsedResponse;
    ParsedResponse.ID = JsonObject->GetStringField("id");
    ParsedResponse.Model = JsonObject->GetStringField("Model");

    const auto& ResultsObject = JsonObject->GetArrayField("results");
    for (const auto& ResultObject : ResultsObject)
    {
        const auto& CategoriesObject = ResultObject->AsObject()->GetObjectField("categories");
        FModerationCategories Categories;
        Categories.Hate = CategoriesObject->GetBoolField("hate");
        Categories.Hate_Threatening = CategoriesObject->GetBoolField("hate/threatening");
        Categories.Self_Harm = CategoriesObject->GetBoolField("self-harm");
        Categories.Sexual = CategoriesObject->GetBoolField("sexual");
        Categories.Sexual_Minors = CategoriesObject->GetBoolField("sexual/minors");
        Categories.Violence = CategoriesObject->GetBoolField("violence");
        Categories.Violence_Graphic = CategoriesObject->GetBoolField("violence/graphic");

        const auto& CategoryScoreObject = ResultObject->AsObject()->GetObjectField("category_scores");
        FModerationScores CategoryScores;
        CategoryScores.Hate = CategoryScoreObject->GetNumberField("hate");
        CategoryScores.Hate_Threatening = CategoryScoreObject->GetNumberField("hate/threatening");
        CategoryScores.Self_Harm = CategoryScoreObject->GetNumberField("self-harm");
        CategoryScores.Sexual = CategoryScoreObject->GetNumberField("sexual");
        CategoryScores.Sexual_Minors = CategoryScoreObject->GetNumberField("sexual/minors");
        CategoryScores.Violence = CategoryScoreObject->GetNumberField("violence");
        CategoryScores.Violence_Graphic = CategoryScoreObject->GetNumberField("violence/graphic");

        FModerationResults ModerationResults;
        ModerationResults.Categories = Categories;
        ModerationResults.Category_Scores = CategoryScores;
        ModerationResults.Flagged = ResultObject->AsObject()->GetBoolField("flagged");
        ParsedResponse.Results.Add(ModerationResults);
    }

    CreateModerationsCompleted.Broadcast(ParsedResponse);
}

void UOpenAIProvider::OnListFineTuneEventsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuneEventsResponse>(Response, WasSuccessful, ListFineTuneEventsCompleted);
}
void UOpenAIProvider::OnDeleteFineTunedModelCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FDeleteFineTuneResponse>(Response, WasSuccessful, DeleteFineTunedModelCompleted);
}

void UOpenAIProvider::OnCreateFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, CreateFineTuningJobCompleted);
}

void UOpenAIProvider::OnListFineTuningJobsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FListFineTuningJobsResponse>(Response, WasSuccessful, ListFineTuningJobsCompleted);
}

void UOpenAIProvider::OnRetrieveFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, RetrieveFineTuningJobCompleted);
}

void UOpenAIProvider::OnCancelFineTuningJobCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobObjectResponse>(Response, WasSuccessful, CancelFineTuningJobCompleted);
}

void UOpenAIProvider::OnListFineTuningEventsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuningJobEventResponse>(Response, WasSuccessful, ListFineTuningEventsCompleted);
}

void UOpenAIProvider::OnCreateBatchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FCreateBatchResponse>(Response, WasSuccessful, CreateBatchCompleted);
}

void UOpenAIProvider::OnRetrieveBatchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FRetrieveBatchResponse>(Response, WasSuccessful, RetrieveBatchCompleted);
}

void UOpenAIProvider::OnCancelBatchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FCancelBatchResponse>(Response, WasSuccessful, CancelBatchCompleted);
}

void UOpenAIProvider::OnListBatchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FListBatchResponse>(Response, WasSuccessful, ListBatchCompleted);
}

///////////////////////////// HELPER FUNCTIONS /////////////////////////////

void UOpenAIProvider::ProcessRequest(FHttpRequestRef HttpRequest)
{
    if (bLogEnabled)
    {
        UE_LOGFMT(LogOpenAIProvider, Display, "Request processing started: {0}", HttpRequest->GetURL());
    }

    if (!HttpRequest->ProcessRequest())
    {
        LogError(FString::Printf(TEXT("Can't process %s"), *HttpRequest->GetURL()));
        RequestError.Broadcast(HttpRequest->GetURL(), {});
    }
}

bool UOpenAIProvider::Success(FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Response)
    {
        LogError("Response is nullptr");
        RequestError.Broadcast("null", "null");
        return false;
    }

    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        LogError("JSON deserialization error");
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        return false;
    }

    if (!WasSuccessful || !JsonObject.IsValid())
    {
        LogError(Response->GetContentAsString());
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        return false;
    }

    if (JsonObject->HasField("error"))
    {
        const auto ErrorObject = JsonObject->GetObjectField("error");
        if (ErrorObject->HasField("type") && ErrorObject->HasField("message") && ErrorObject->HasField("code"))
        {
            LogError(Response->GetContentAsString());
            RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
            return false;
        }
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
        UE_LOGFMT(LogOpenAIProvider, Display, "Request URL: {0}", Response->GetURL());
        UE_LOGFMT(LogOpenAIProvider, Display, "{0}", Response->GetContentAsString());
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
    const auto Content = Response->GetContentAsString();
    if (!Content.IsEmpty())
    {
        return MakeTuple(Response->GetURL(), Response->GetContentAsString());
    }

    const auto Status = EHttpRequestStatus::ToString(Request->GetStatus());
    return MakeTuple(Response->GetURL(), Status);
}

bool UOpenAIProvider::HandleString(FString& IncomeString, bool& LastString) const
{
    if (IncomeString.StartsWith("data: "))
    {
        IncomeString.RemoveFromStart("data: ");
    }

    // igone role chunck // @todo handle this case in another struct
    // if (IncomeString.Find("role") != INDEX_NONE) return false;
    if (IncomeString.Equals("[DONE]"))
    {
        LastString = true;
    }

    return true;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", FString("Bearer ").Append(Auth.APIKey));
    HttpRequest->SetHeader("OpenAI-Organization", Auth.OrganizationID);
    HttpRequest->SetHeader("OpenAI-Project", Auth.ProjectID);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);
    return HttpRequest;
}

FHttpRequestRef UOpenAIProvider::MakeRequest(
    const FChatCompletion& ChatCompletion, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
{
    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", FString("Bearer ").Append(Auth.APIKey));
    HttpRequest->SetHeader("OpenAI-Organization", Auth.OrganizationID);
    HttpRequest->SetHeader("OpenAI-Project", Auth.ProjectID);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ChatCompletion);
    CleanChatCompletionFieldsThatCantBeEmpty(ChatCompletion, Json);

    FString RequestBodyStr;
    UOpenAIFuncLib::JsonToString(Json, RequestBodyStr);
    RequestBodyStr = UOpenAIFuncLib::CleanUpFunctionsObject(RequestBodyStr);

    HttpRequest->SetContentAsString(RequestBodyStr);
    return HttpRequest;
}

void UOpenAIProvider::CleanChatCompletionFieldsThatCantBeEmpty(const FChatCompletion& ChatCompletion, TSharedPtr<FJsonObject>& Json) const
{
    if (ChatCompletion.Tools.IsEmpty())
    {
        Json->RemoveField("Tools");
    }

    if (ChatCompletion.Tool_Choice.Function.Name.IsEmpty())
    {
        Json->RemoveField("Tool_Choice");
    }

    if (ChatCompletion.Stop.IsEmpty())
    {
        Json->RemoveField("Stop");
    }

    if (ChatCompletion.Logit_Bias.IsEmpty())
    {
        Json->RemoveField("Logit_Bias");
    }

    if (ChatCompletion.Model == UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview))
    {
        Json->RemoveField("Response_Format");
    }

    for (int32 i = 0; i < ChatCompletion.Messages.Num(); ++i)
    {
        const auto& Message = ChatCompletion.Messages[i];
        auto& MessageObj = Json->GetArrayField("Messages")[i]->AsObject();

        if (Message.Tool_Calls.IsEmpty())
        {
            MessageObj->RemoveField("Tool_Calls");
        }
        if (Message.Tool_Call_ID.IsEmpty())
        {
            MessageObj->RemoveField("Tool_Call_ID");
        }
        if (Message.Name.IsEmpty())
        {
            MessageObj->RemoveField("Name");
        }
        if (Message.ContentArray.IsEmpty())
        {
            MessageObj->RemoveField("ContentArray");
        }
        else
        {
            auto Content = MessageObj->GetArrayField("ContentArray");

            for (int32 j = 0; j < Content.Num(); ++j)
            {
                TSharedPtr<FJsonObject> MessageContent = Content[j]->AsObject();
                if (MessageContent->GetStringField("Type").Equals("text"))
                {
                    MessageContent->RemoveField("Image_URL");
                }
                else
                {
                    MessageContent->RemoveField("Text");
                }
            }
            MessageObj->SetArrayField("Content", Content);
            MessageObj->RemoveField("ContentArray");
        }
    }
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS

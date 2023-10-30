// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Provider/OpenAIProvider.h"
#include "API/API.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Http/HttpHelper.h"
#include "FuncLib/OpenAIFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIProvider, All, All);

using namespace OpenAI;

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

void UOpenAIProvider::CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth)
{
    check(!Edit.Model.IsEmpty());
    check(!Edit.Input.IsEmpty());
    check(!Edit.Instruction.IsEmpty());

    auto HttpRequest = MakeRequest(Edit, API->Edits(), "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateEditCompleted);
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

void UOpenAIProvider::CreateFineTune(const FFineTune& FineTune, const FOpenAIAuth& Auth)
{
    // The current request is case sensitive for file ids and optional params.
    // That's why special serialisation is needed.

    auto HttpRequest = MakeRequest(API->FineTunes(), "POST", Auth);

    TSharedPtr<FJsonObject> RequestBody = MakeShareable(new FJsonObject());
    RequestBody->SetStringField("training_file", FineTune.Training_File);
    RequestBody->SetStringField("model", FineTune.Model);

    SetOptional(RequestBody, FineTune.Validation_File, "validation_file");
    SetOptional(RequestBody, FineTune.N_Epochs, "n_epochs");
    SetOptional(RequestBody, FineTune.Batch_Size, "batch_size");
    SetOptional(RequestBody, FineTune.Learning_Rate_Multiplier, "learning_rate_multiplier");
    SetOptional(RequestBody, FineTune.Prompt_Loss_Weight, "prompt_loss_weight");
    SetOptional(RequestBody, FineTune.Compute_Classification_Metrics, "compute_classification_metrics");
    SetOptional(RequestBody, FineTune.Classification_N_Classes, "classification_n_classes");
    SetOptional(RequestBody, FineTune.Classification_Positive_Class, "classification_positive_class");
    SetOptional(RequestBody, FineTune.Suffix, "suffix");
    SetOptional(RequestBody, FineTune.Classification_Betas, "classification_betas");

    FString RequestBodyStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBodyStr);
    FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(RequestBodyStr);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateFineTuneCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFineTunes(const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(API->FineTunes(), "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTunesCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::RetrieveFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->FineTunes()).Append("/").Append(FineTuneID);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveFineTuneCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::CancelFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->FineTunes()).Append("/").Append(FineTuneID).Append("/cancel");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCancelFineTuneCompleted);
    ProcessRequest(HttpRequest);
}

void UOpenAIProvider::ListFineTuneEvents(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(API->FineTunes()).Append("/").Append(FineTuneID).Append("/events");
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuneEventsCompleted);
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
        FString(API->FineTunes()).Append("/").Append(FineTuningJobID).Append("/events").Append(FineTuningQueryParameters.ToQuery());
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuningEventsCompleted);
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

void UOpenAIProvider::OnCreateEditCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FEditResponse>(Response, WasSuccessful, CreateEditCompleted);
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
        const auto DataObject = DataElem->AsObject();
        if (DataObject->HasField("b64_json"))
        {
            ImageResponse.Data.Push(DataObject->GetStringField("b64_json"));
        }
        else if (DataObject->HasField("url"))
        {
            ImageResponse.Data.Push(DataObject->GetStringField("url"));
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

void UOpenAIProvider::OnCreateFineTuneCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuneResponse>(Response, WasSuccessful, CreateFineTuneCompleted);
}

void UOpenAIProvider::OnListFineTunesCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FListFineTuneResponse>(Response, WasSuccessful, ListFineTunesCompleted);
}

void UOpenAIProvider::OnRetrieveFineTuneCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuneResponse>(Response, WasSuccessful, RetrieveFineTuneCompleted);
}

void UOpenAIProvider::OnCancelFineTuneCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FFineTuneResponse>(Response, WasSuccessful, CancelFineTuneCompleted);
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

///////////////////////////// HELPER FUNCTIONS /////////////////////////////

void UOpenAIProvider::ProcessRequest(FHttpRequestRef HttpRequest)
{
    if (bLogEnabled)
    {
        UE_LOG(LogOpenAIProvider, Display, TEXT("Request processing started: %s"), *HttpRequest->GetURL());
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

    if (!WasSuccessful || !JsonObject.IsValid() /*|| JsonObject->HasField("error")*/)
    {
        LogError(Response->GetContentAsString());
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        return false;
    }

    LogResponse(Response);
    return true;
}

void UOpenAIProvider::LogResponse(FHttpResponsePtr Response)
{
    if (bLogEnabled)
    {
        UE_LOG(LogOpenAIProvider, Display, TEXT("Request URL: %s"), *Response->GetURL());
        UE_LOG(LogOpenAIProvider, Display, TEXT("%s"), *Response->GetContentAsString());
    }
}

void UOpenAIProvider::LogError(const FString& ErrorText)
{
    UE_LOG(LogOpenAIProvider, Error, TEXT("%s"), *ErrorText);
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
    if (IncomeString.Find("role") != INDEX_NONE) return false;
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
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Method);

    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ChatCompletion);
    CleanChatCompletionFieldsThatCantBeEmpty(ChatCompletion, Json);

    FString RequestBodyStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBodyStr);
    FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

    RequestBodyStr = UOpenAIFuncLib::CleanUpFunctionsObject(RequestBodyStr);

    HttpRequest->SetContentAsString(RequestBodyStr);
    return HttpRequest;
}

void UOpenAIProvider::CleanChatCompletionFieldsThatCantBeEmpty(const FChatCompletion& ChatCompletion, TSharedPtr<FJsonObject>& Json) const
{
    if (ChatCompletion.Functions.IsEmpty())
    {
        Json->RemoveField("Functions");
    }

    if (ChatCompletion.Function_Call.Arguments.IsEmpty() || ChatCompletion.Function_Call.Name.IsEmpty())
    {
        Json->RemoveField("Function_Call");
    }

    for (int32 i = 0; i < ChatCompletion.Messages.Num(); ++i)
    {
        const auto& Message = ChatCompletion.Messages[i];
        if (Message.Function_Call.Arguments.IsEmpty() || Message.Function_Call.Name.IsEmpty())
        {
            Json->GetArrayField("Messages")[i]->AsObject()->RemoveField("Function_Call");
        }
        if (Message.Name.IsEmpty())
        {
            Json->GetArrayField("Messages")[i]->AsObject()->RemoveField("Name");
        }
    }
}

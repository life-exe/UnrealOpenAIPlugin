// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Provider/OpenAIProvider.h"
#include "API/API.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Http/HttpHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIProvider, All, All);

using namespace OpenAI;
using namespace OpenAI::V1;

void UOpenAIProvider::ListModels(const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(ModelsURL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListModelsCompleted);
    ProcessRequest(HttpRequest, ModelsURL);
}

void UOpenAIProvider::RetrieveModel(const FString& ModelName, const FOpenAIAuth& Auth)
{
    check(!ModelName.IsEmpty());

    const FString URL = FString(ModelsURL).Append("/").Append(ModelName);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveModelCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::CreateCompletion(const FCompletion& Completion, const FOpenAIAuth& Auth)
{
    check(!Completion.Model.IsEmpty());
    check(!Completion.Prompt.IsEmpty());

    auto HttpRequest = MakeRequest(Completion, CompletionURL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateCompletionCompleted);
    ProcessRequest(HttpRequest, CompletionURL);
}

void UOpenAIProvider::CreateChatCompletion(const FChatCompletion& ChatCompletion, const FOpenAIAuth& Auth)
{
    check(!ChatCompletion.Model.IsEmpty());
    check(ChatCompletion.Messages.Num() > 0);

    auto HttpRequest = MakeRequest(ChatCompletion, ChatCompletionURL, "POST", Auth);
    if (ChatCompletion.Stream)
    {
        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateChatCompletionStreamCompleted);
        HttpRequest->OnRequestProgress().BindUObject(this, &ThisClass::OnCreateChatCompletionStreamProgress);
    }
    else
    {
        HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateChatCompletionCompleted);
    }
    ProcessRequest(HttpRequest, ChatCompletionURL);
}

void UOpenAIProvider::CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth)
{
    check(!Edit.Model.IsEmpty());
    check(!Edit.Input.IsEmpty());
    check(!Edit.Instruction.IsEmpty());

    auto HttpRequest = MakeRequest(Edit, EditsURL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateEditCompleted);
    ProcessRequest(HttpRequest, EditsURL);
}

void UOpenAIProvider::CreateImage(const FOpenAIImage& Image, const FOpenAIAuth& Auth)
{
    check(!Image.Prompt.IsEmpty());

    auto HttpRequest = MakeRequest(Image, ImageGenerationsURL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateImageCompleted);
    ProcessRequest(HttpRequest, ImageGenerationsURL);
}

void UOpenAIProvider::CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(ImageEditsURL);
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
    ProcessRequest(HttpRequest, ImageEditsURL);
}

void UOpenAIProvider::CreateImageVariation(const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(ImageVariationsURL);
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
    ProcessRequest(HttpRequest, ImageVariationsURL);
}

void UOpenAIProvider::CreateEmbeddings(const FEmbeddings& Embeddings, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(Embeddings, EmbeddingsURL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateEmbeddingsCompleted);
    ProcessRequest(HttpRequest, EmbeddingsURL);
}

void UOpenAIProvider::CreateAudioTranscription(const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(AudioTranscriptionsURL);
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
    ProcessRequest(HttpRequest, AudioTranscriptionsURL);
}

void UOpenAIProvider::CreateAudioTranslation(const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();

    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(AudioTranslationsURL);
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
    ProcessRequest(HttpRequest, AudioTranslationsURL);
}

void UOpenAIProvider::ListFiles(const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(FilesURL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFilesCompleted);
    ProcessRequest(HttpRequest, FilesURL);
}

void UOpenAIProvider::UploadFile(const FUploadFile& UploadFile, const FOpenAIAuth& Auth)
{
    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary();
    auto HttpRequest = CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer " + Auth.APIKey);
    HttpRequest->SetURL(FilesURL);
    HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary =" + Boundary);
    HttpRequest->SetVerb("POST");

    TArray<uint8> RequestContent;
    RequestContent.Append(HttpHelper::AddMIMEFile(UploadFile.File, "file", BeginBoundary));
    RequestContent.Append(HttpHelper::AddMIME("purpose", UploadFile.Purpose, BeginBoundary));
    RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

    HttpRequest->SetContent(RequestContent);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnUploadFileCompleted);
    ProcessRequest(HttpRequest, FilesURL);
}

void UOpenAIProvider::DeleteFile(const FString& FileID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(FilesURL).Append("/").Append(FileID);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteFileCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::RetrieveFile(const FString& FileID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(FilesURL).Append("/").Append(FileID);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveFileCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::RetrieveFileContent(const FString& FileID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(FilesURL).Append("/").Append(FileID).Append("/content");
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveFileContentCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::CreateFineTune(const FFineTune& FineTune, const FOpenAIAuth& Auth)
{
    // The current request is case sensitive for file ids and optional params.
    // That's why special serialisation is needed.

    auto HttpRequest = MakeRequest(FineTunesURL, "POST", Auth);

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
    ProcessRequest(HttpRequest, FineTunesURL);
}

void UOpenAIProvider::ListFineTunes(const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(FineTunesURL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTunesCompleted);
    ProcessRequest(HttpRequest, FineTunesURL);
}

void UOpenAIProvider::RetrieveFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(FineTunesURL).Append("/").Append(FineTuneID);
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRetrieveFineTuneCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::CancelFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(FineTunesURL).Append("/").Append(FineTuneID).Append("/cancel");
    auto HttpRequest = MakeRequest(URL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCancelFineTuneCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::ListFineTuneEvents(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(FineTunesURL).Append("/").Append(FineTuneID).Append("/events");
    auto HttpRequest = MakeRequest(URL, "GET", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnListFineTuneEventsCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::DeleteFineTunedModel(const FString& ModelID, const FOpenAIAuth& Auth)
{
    const auto URL = FString(ModelsURL).Append("/").Append(ModelID);
    auto HttpRequest = MakeRequest(URL, "DELETE", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnDeleteFineTunedModelCompleted);
    ProcessRequest(HttpRequest, URL);
}

void UOpenAIProvider::CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth)
{
    auto HttpRequest = MakeRequest(Moderations, ModerationsURL, "POST", Auth);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnCreateModerationsCompleted);
    ProcessRequest(HttpRequest, ModerationsURL);
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

void UOpenAIProvider::OnCreateChatCompletionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    HandleResponse<FChatCompletionResponse>(Response, WasSuccessful, CreateChatCompletionCompleted);
}

void UOpenAIProvider::OnCreateChatCompletionStreamCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!WasSuccessful)
    {
        const auto Content = Response->GetContentAsString();
        if (!Content.IsEmpty())
        {
            LogError(Content);
            RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        }
        else
        {
            const auto Status = EHttpRequestStatus::ToString(Request->GetStatus());
            LogError(Status);
            RequestError.Broadcast(Response->GetURL(), Status);
        }
        return;
    }

    TArray<FChatCompletionStreamResponse> ParsedResponses;
    if (ParseChatCompletionStreamRequest(Response, ParsedResponses))
    {
        LogResponse(Response);
        CreateChatCompletionStreamCompleted.Broadcast(ParsedResponses);
    }
    else
    {
        LogError("JSON deserialization error");
        LogError(Response->GetContentAsString());
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
    }
}

void UOpenAIProvider::OnCreateChatCompletionStreamProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
    const auto& Response = Request->GetResponse();
    TArray<FChatCompletionStreamResponse> ParsedResponses;

    if (ParseChatCompletionStreamRequest(Response, ParsedResponses))
    {
        LogResponse(Response);
        CreateChatCompletionStreamProgresses.Broadcast(ParsedResponses);
    }
    else if (Response)
    {
        LogError(Response->GetContentAsString());
        // RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
    }
    else if (BytesReceived == 0)
    {
        // UE_5.3: on some reason OnRequestProgress() sends first time with Request = nullptr
        // BytesReceived = 0 in this case, empty initial call ?
        // Remove error message from log because nothing bad happened
    }
    else
    {
        LogError(FString::Format(TEXT("JSON deserialization error BytesSent:{0} BytesReceived:{1}"), {BytesSent, BytesReceived}));
    }
}

bool UOpenAIProvider::ParseChatCompletionStreamRequest(FHttpResponsePtr Response, TArray<FChatCompletionStreamResponse>& Responses)
{
    if (!Response) return false;

    TArray<FString> StringArray;
    Response->GetContentAsString().ParseIntoArrayLines(StringArray);

    for (auto& Item : StringArray)
    {
        if (Item.StartsWith("data: "))
        {
            Item.RemoveFromStart("data: ");
        }

        // igone role chunck // @todo handle this case in another struct
        if (Item.Find("role") != INDEX_NONE) continue;
        if (Item.Equals("[DONE]")) break;

        FChatCompletionStreamResponse ParsedResponse;
        if (!ParseJSONToStruct(Item, &ParsedResponse)) return false;
        Responses.Add(ParsedResponse);
    }
    return true;
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

///////////////////////////// HELPER FUNCTIONS /////////////////////////////

void UOpenAIProvider::ProcessRequest(FHttpRequestRef HttpRequest, const FString& URL)
{
    if (bLogEnabled)
    {
        UE_LOG(LogOpenAIProvider, Display, TEXT("Request processing started: %s"), *URL);
    }

    if (!HttpRequest->ProcessRequest())
    {
        LogError(FString::Printf(TEXT("Can't process %s"), *URL));
        RequestError.Broadcast(URL, {});
    }
}

bool UOpenAIProvider::Success(FHttpResponsePtr Response, bool WasSuccessful)
{
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        LogError("JSON deserialization error");
        RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        return false;
    }

    if (!WasSuccessful || !JsonObject.IsValid() || JsonObject->HasField("error"))
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

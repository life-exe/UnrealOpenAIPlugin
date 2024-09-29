// OpenAI, Copyright LifeEXE. All Rights Reserved.

/*
 ___       ___  ________ _______
|\  \     |\  \|\  _____\\  ___ \
\ \  \    \ \  \ \  \__/\ \   __/|
 \ \  \    \ \  \ \   __\\ \  \_|/__
  \ \  \____\ \  \ \  \_| \ \  \_|\ \
   \ \_______\ \__\ \__\   \ \_______\
    \|_______|\|__|\|__|    \|_______|



 _______      ___    ___ _______
|\  ___ \    |\  \  /  /|\  ___ \
\ \   __/|   \ \  \/  / | \   __/|
 \ \  \_|/__  \ \    / / \ \  \_|/__
  \ \  \_|\ \  /     \/   \ \  \_|\ \
   \ \_______\/  /\   \    \ \_______\
    \|_______/__/ /\ __\    \|_______|
             |__|/ \|__|
*/

#pragma once

#include "CoreMinimal.h"
#include "HTTP.h"
#include "Delegates.h"
#include "Types/AllTypesHeader.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/JsonFuncLib.h"
#include "Provider/JsonParsers/ChatParser.h"
#include "JsonObjectConverter.h"
#include "OpenAIProvider.generated.h"

class FJsonObject;

namespace OpenAI
{
class IAPI;
}

UCLASS()
class OPENAI_API UOpenAIProvider : public UObject
{
    GENERATED_BODY()

public:
    UOpenAIProvider();

    void SetAPI(const TSharedPtr<OpenAI::IAPI>& API);

public:
    /**
      Lists the currently available models, and provides basic information about each one such as the owner and availability.
      https://platform.openai.com/docs/api-reference/models/list
    */
    void ListModels(const FOpenAIAuth& Auth);

    /**
      Retrieves a model instance, providing basic information about the model such as the owner and permissioning.
      https://platform.openai.com/docs/api-reference/models/retrieve
    */
    void RetrieveModel(const FString& ModelName, const FOpenAIAuth& Auth);

    /**
      Delete a fine-tuned model. You must have the Owner role in your organization.
      https://platform.openai.com/docs/api-reference/models/delete
    */
    void DeleteFineTunedModel(const FString& ModelID, const FOpenAIAuth& Auth);

    /**
      Creates a completion for the provided prompt and parameters.
      https://platform.openai.com/docs/api-reference/completions/create
    */
    void CreateCompletion(const FCompletion& Completion, const FOpenAIAuth& Auth);

    /**
      Creates a completion for the chat message.
      https://platform.openai.com/docs/api-reference/chat/create
    */
    void CreateChatCompletion(const FChatCompletion& Completion, const FOpenAIAuth& Auth);

    /**
      Creates an image given a prompt.
      https://platform.openai.com/docs/api-reference/images/create
    */
    void CreateImage(const FOpenAIImage& Image, const FOpenAIAuth& Auth);

    /**
      Creates an edited or extended image given an original image and a prompt.
      https://platform.openai.com/docs/api-reference/images/create-edit
    */
    void CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth);

    /**
      Creates a variation of a given image.
      https://platform.openai.com/docs/api-reference/images/create-variation
    */
    void CreateImageVariation(const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth);

    /**
      Creates an embedding vector representing the input text.
      https://platform.openai.com/docs/api-reference/embeddings/create
    */
    void CreateEmbeddings(const FEmbeddings& Embeddings, const FOpenAIAuth& Auth);

    /**
      Generates audio from the input text.
      https://platform.openai.com/docs/api-reference/audio/createSpeech
    */
    void CreateSpeech(const FSpeech& Speech, const FOpenAIAuth& Auth);

    /**
      Transcribes audio into the input language.
      https://platform.openai.com/docs/api-reference/audio/create
    */
    void CreateAudioTranscription(const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth);

    /**
      Translates audio into into English.
      https://platform.openai.com/docs/api-reference/audio/create
    */
    void CreateAudioTranslation(const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth);

    /**
      Returns a list of files that belong to the user's organization.
      https://platform.openai.com/docs/api-reference/files/list
    */
    void ListFiles(const FOpenAIAuth& Auth);

    /**
      Upload a file that contains document(s) to be used across various endpoints/features.
      Currently, the size of all the files uploaded by one organization can be up to 1 GB.
      Please contact us if you need to increase the storage limit.
      https://platform.openai.com/docs/api-reference/files/upload
    */
    void UploadFile(const FUploadFile& UploadFile, const FOpenAIAuth& Auth);

    /**
      Delete a file.
      https://platform.openai.com/docs/api-reference/files/delete
    */
    void DeleteFile(const FString& FileID, const FOpenAIAuth& Auth);

    /**
      Returns information about a specific file.
      https://platform.openai.com/docs/api-reference/files/retrieve
    */
    void RetrieveFile(const FString& FileID, const FOpenAIAuth& Auth);

    /**
      Returns the contents of the specified file.
      https://platform.openai.com/docs/api-reference/files/retrieve-content
    */
    void RetrieveFileContent(const FString& FileID, const FOpenAIAuth& Auth);

    /**
      Classifies if text violates OpenAI's Content Policy
      https://platform.openai.com/docs/api-reference/moderations/create
    */
    void CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth);

    /**
      Creates a job that fine-tunes a specified model from a given dataset.
      Response includes details of the enqueued job including job status and the name of the fine-tuned models once complete.
      https://platform.openai.com/docs/api-reference/fine-tuning/create
    */
    void CreateFineTuningJob(const FFineTuningJob& FineTuningJob, const FOpenAIAuth& Auth);

    /**
      List your organization's fine-tuning jobs.
      https://platform.openai.com/docs/api-reference/fine-tuning/list
    */
    void ListFineTuningJobs(const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters = {});

    /**
      Get status updates for a fine-tuning job.
      https://platform.openai.com/docs/api-reference/fine-tuning/list-events
    */
    void ListFineTuningEvents(
        const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters = {});

    /**
      Get status updates for a fine-tuning job.
      https://platform.openai.com/docs/api-reference/fine-tuning/list-events
    */
    void ListFineTuningCheckpoints(
        const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters = {});

    /**
      Get info about a fine-tuning job.
      https://platform.openai.com/docs/api-reference/fine-tuning/retrieve
    */
    void RetrieveFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth);

    /**
      Immediately cancel a fine-tune job.
      https://platform.openai.com/docs/api-reference/fine-tuning/cancel
    */
    void CancelFineTuningJob(const FString& FineTuneID, const FOpenAIAuth& Auth);

    /**
      Create large batches of API requests for asynchronous processing.
      The Batch API returns completions within 24 hours for a 50% discount.
      https://platform.openai.com/docs/api-reference/batch/create
    */
    void CreateBatch(const FCreateBatch& CreateBatch, const FOpenAIAuth& Auth);

    /**
      Retrieves a batch.
      https://platform.openai.com/docs/api-reference/batch/retrieve
    */
    void RetrieveBatch(const FString& BatchId, const FOpenAIAuth& Auth);

    /**
      Cancels an in-progress batch.
      The batch will be in status cancelling for up to 10 minutes,
      before changing to cancelled, where it will have partial
      results (if any) available in the output file.
      https://platform.openai.com/docs/api-reference/batch/cancel
    */
    void CancelBatch(const FString& BatchId, const FOpenAIAuth& Auth);

    /**
      List your organization's batches.
      https://platform.openai.com/docs/api-reference/batch/list
    */
    void ListBatch(const FListBatch& ListBatch, const FOpenAIAuth& Auth);

    /**
      Creates an intermediate Upload object that you can add Parts to.
      Currently, an Upload can accept at most 8 GB in total and expires after an hour after you create it.
      https://platform.openai.com/docs/api-reference/uploads/create
    */
    void CreateUpload(const FCreateUpload& CreateUpload, const FOpenAIAuth& Auth);

    /**
      Adds a Part to an Upload object.
      A Part represents a chunk of bytes from the file you are trying to upload.
      https://platform.openai.com/docs/api-reference/uploads/add-part
    */
    void AddUploadPart(const FString& UploadId, const FAddUploadPart& AddUploadPart, const FOpenAIAuth& Auth);

    /**
      Completes the Upload.
      Within the returned Upload object, there is a nested File object
      that is ready to use in the rest of the platform.
      https://platform.openai.com/docs/api-reference/uploads/complete
    */
    void CompleteUpload(const FString& UploadId, const FCompleteUpload& CompleteUpload, const FOpenAIAuth& Auth);

    /**
      Cancels the Upload. No Parts may be added after an Upload is cancelled.
      https://platform.openai.com/docs/api-reference/uploads/cancel
    */
    void CancelUpload(const FString& UploadId, const FOpenAIAuth& Auth);

    /**
      Print response to console
    */
    void SetLogEnabled(bool LogEnabled) { bLogEnabled = LogEnabled; }

#define DEFINE_EVENT_GETTER(Name)          \
public:                                    \
    FOn##Name& On##Name() { return Name; } \
                                           \
private:                                   \
    FOn##Name Name;

public:
    FOnRequestError& OnRequestError() { return RequestError; };
    DEFINE_EVENT_GETTER(ListModelsCompleted)
    DEFINE_EVENT_GETTER(RetrieveModelCompleted)
    DEFINE_EVENT_GETTER(CreateCompletionCompleted)
    DEFINE_EVENT_GETTER(CreateCompletionStreamCompleted)
    DEFINE_EVENT_GETTER(CreateCompletionStreamProgresses)
    DEFINE_EVENT_GETTER(CreateChatCompletionCompleted)
    DEFINE_EVENT_GETTER(CreateChatCompletionStreamCompleted)
    DEFINE_EVENT_GETTER(CreateChatCompletionStreamProgresses)
    DEFINE_EVENT_GETTER(CreateImageCompleted)
    DEFINE_EVENT_GETTER(CreateImageEditCompleted)
    DEFINE_EVENT_GETTER(CreateImageVariationCompleted)
    DEFINE_EVENT_GETTER(CreateEmbeddingsCompleted)
    DEFINE_EVENT_GETTER(CreateSpeechCompleted)
    DEFINE_EVENT_GETTER(CreateAudioTranscriptionCompleted)
    DEFINE_EVENT_GETTER(CreateAudioTranscriptionVerboseCompleted)
    DEFINE_EVENT_GETTER(CreateAudioTranslationCompleted)
    DEFINE_EVENT_GETTER(ListFilesCompleted)
    DEFINE_EVENT_GETTER(UploadFileCompleted)
    DEFINE_EVENT_GETTER(DeleteFileCompleted)
    DEFINE_EVENT_GETTER(RetrieveFileCompleted)
    DEFINE_EVENT_GETTER(RetrieveFileContentCompleted)
    DEFINE_EVENT_GETTER(CreateFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(ListFineTuningJobsCompleted)
    DEFINE_EVENT_GETTER(ListFineTuningEventsCompleted)
    DEFINE_EVENT_GETTER(ListFineTuningCheckpointsCompleted)
    DEFINE_EVENT_GETTER(DeleteFineTunedModelCompleted)
    DEFINE_EVENT_GETTER(CreateModerationsCompleted)
    DEFINE_EVENT_GETTER(RetrieveFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(CancelFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(ListBatchCompleted)
    DEFINE_EVENT_GETTER(CreateBatchCompleted)
    DEFINE_EVENT_GETTER(RetrieveBatchCompleted)
    DEFINE_EVENT_GETTER(CancelBatchCompleted)
    DEFINE_EVENT_GETTER(CreateUploadCompleted)
    DEFINE_EVENT_GETTER(AddUploadPartCompleted)
    DEFINE_EVENT_GETTER(CompleteUploadCompleted)
    DEFINE_EVENT_GETTER(CancelUploadCompleted)

private:
    TSharedPtr<OpenAI::IAPI> API;
    bool bLogEnabled{true};
    FOnRequestError RequestError;

#define DECLARE_HTTP_CALLBACK(Callback) virtual void Callback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
#define DECLARE_HTTP_CALLBACK_PROGRESS(Callback) virtual void Callback(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);

    DECLARE_HTTP_CALLBACK(OnListModelsCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveModelCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateCompletionCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateCompletionStreamCompleted)
    DECLARE_HTTP_CALLBACK_PROGRESS(OnCreateCompletionStreamProgress)
    DECLARE_HTTP_CALLBACK(OnCreateChatCompletionCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateChatCompletionStreamCompleted)
    DECLARE_HTTP_CALLBACK_PROGRESS(OnCreateChatCompletionStreamProgress)
    DECLARE_HTTP_CALLBACK(OnCreateImageCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateImageEditCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateImageVariationCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateEmbeddingsCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateSpeechCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateAudioTranscriptionCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateAudioTranslationCompleted)
    DECLARE_HTTP_CALLBACK(OnListFilesCompleted)
    DECLARE_HTTP_CALLBACK(OnUploadFileCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteFileCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveFileCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveFileContentCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteFineTunedModelCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateModerationsCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnListFineTuningJobsCompleted)
    DECLARE_HTTP_CALLBACK(OnListFineTuningEventsCompleted)
    DECLARE_HTTP_CALLBACK(OnListFineTuningCheckpointsCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnListBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateUploadCompleted)
    DECLARE_HTTP_CALLBACK(OnAddUploadPartCompleted)
    DECLARE_HTTP_CALLBACK(OnCompleteUploadCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelUploadCompleted)

    void ProcessRequest(FHttpRequestRef HttpRequest);

    bool Success(FHttpResponsePtr Response, bool WasSuccessful);
    void Log(const FString& Info) const;
    void LogResponse(FHttpResponsePtr Response) const;
    void LogError(const FString& ErrorText) const;

    template <typename OutStructType>
    FString SerializeRequest(const OutStructType& OutStruct) const
    {
        TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(OutStruct);
        UJsonFuncLib::RemoveEmptyArrays(Json);
        FString RequestBodyStr;
        UJsonFuncLib::JsonToString(Json, RequestBodyStr);
        return RequestBodyStr;
    }

    FHttpRequestRef MakeRequestHeaders(const FOpenAIAuth& Auth) const;
    FOpenAIResponseMetadata GetResponseHeaders(FHttpResponsePtr Response) const;

    template <typename OutStructType>
    FHttpRequestRef MakeRequest(const OutStructType& OutStruct, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
    {
        auto HttpRequest = MakeRequestHeaders(Auth);
        HttpRequest->SetURL(URL);
        HttpRequest->SetVerb(Method);

        const FString Content = SerializeRequest(OutStruct);
        Log(FString("Content: ").Append(Content));

        const FString PostprocessedContent = UJsonFuncLib::RemoveOptionalValuesThatNotSet(Content);
        Log(FString("Postprocessed content was set as: ").Append(PostprocessedContent));

        HttpRequest->SetContentAsString(PostprocessedContent);
        return HttpRequest;
    }
    // specializations
    FHttpRequestRef MakeRequest(const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;
    FHttpRequestRef MakeRequest(
        const FChatCompletion& ChatCompletion, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;

    template <typename ParsedResponseType, typename DelegateType>
    void HandleResponse(FHttpResponsePtr Response, bool WasSuccessful, DelegateType& Delegate)
    {
        if (!Success(Response, WasSuccessful)) return;

        const FString Content = Response.IsValid() ? Response->GetContentAsString() : FString{};
        const FString ResponseURL = Response.IsValid() ? Response->GetURL() : FString{};

        ParsedResponseType ParsedResponse;
        if (UJsonFuncLib::ParseJSONToStruct(Content, &ParsedResponse))
        {
            Delegate.Broadcast(ParsedResponse, GetResponseHeaders(Response));
        }
        else
        {
            LogError("JSON deserialization error");
            RequestError.Broadcast(ResponseURL, Content);
        }
    }

    virtual TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateRequest() const { return FHttpModule::Get().CreateRequest(); }

private:
    void SetOptional(TSharedPtr<FJsonObject> RequestBody, const TOptional<FString>& Param, const FString& ParamName);
    void SetOptional(TSharedPtr<FJsonObject> RequestBody, const TOptional<bool>& Param, const FString& ParamName);
    void SetOptional(TSharedPtr<FJsonObject> RequestBody, const TOptional<TArray<float>>& Param, const FString& ParamName);

    template <typename NumberType>
    void SetOptional(TSharedPtr<FJsonObject> RequestBody, const TOptional<NumberType>& Param, const FString& ParamName)
    {
        if (!Param.IsSet()) return;
        RequestBody->SetNumberField(ParamName, Param.GetValue());
    }

private:
    TTuple<FString, FString> GetErrorData(FHttpRequestPtr Request, FHttpResponsePtr Response) const;

    template <typename ResponseType>
    bool ParseStreamRequest(FHttpResponsePtr Response, TArray<ResponseType>& Responses)
    {
        if (!Response.IsValid()) return false;

        TArray<FString> StringArray;
        Response->GetContentAsString().ParseIntoArrayLines(StringArray);

        for (auto& String : StringArray)
        {
            bool LastString{false};
            if (OpenAI::ChatParser::CleanChunkResponseString(String, LastString))
            {
                if (LastString)
                {
                    break;
                }
                ResponseType ParsedResponse;
                if (!UJsonFuncLib::ParseJSONToStruct(String, &ParsedResponse)) continue;

                Responses.Add(ParsedResponse);
            }
        }
        return true;
    }

    template <typename ResponseType, typename DelegateType>
    void OnStreamProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived, DelegateType& Delegate)
    {
        const auto& Response = Request->GetResponse();
        TArray<ResponseType> ParsedResponses;

        if (ParseStreamRequest(Response, ParsedResponses))
        {
            LogResponse(Response);
            Delegate.Broadcast(ParsedResponses, GetResponseHeaders(Response));
        }
        else if (Response.IsValid())
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

    template <typename ResponseType, typename DelegateType>
    void OnStreamCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful, DelegateType& Delegate)
    {
        if (!WasSuccessful)
        {
            const auto& [URL, Content] = GetErrorData(Request, Response);
            LogError(Content);
            RequestError.Broadcast(URL, Content);
            return;
        }

        TArray<ResponseType> ParsedResponses;
        if (ParseStreamRequest(Response, ParsedResponses))
        {
            LogResponse(Response);
            Delegate.Broadcast(ParsedResponses, GetResponseHeaders(Response));
        }
        else
        {
            const FString Content = Response.IsValid() ? Response->GetContentAsString() : FString{};
            const FString ResponseURL = Response.IsValid() ? Response->GetURL() : FString{};

            LogError("JSON deserialization error");
            LogError(Content);
            RequestError.Broadcast(ResponseURL, Content);
        }
    }
};

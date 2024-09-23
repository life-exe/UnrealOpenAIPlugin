// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HTTP.h"
#include "Types/BatchTypes.h"
#include "Types/AudioTypes.h"
#include "Delegates.h"
#include "FuncLib/OpenAIFuncLib.h"
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
      Delete a fine-tuned model. You must have the Owner role in your organization.
      https://platform.openai.com/docs/api-reference/fine-tunes/delete-model
    */
    void DeleteFineTunedModel(const FString& ModelID, const FOpenAIAuth& Auth);

    /**
      Classifies if text violates OpenAI's Content Policy
      https://platform.openai.com/docs/api-reference/moderations/create
    */
    void CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth);

    /**
      List your organization's fine-tuning jobs.
      https://platform.openai.com/docs/api-reference/fine-tuning/list
    */
    void ListFineTuningJobs(const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters = {});

    /**
      Creates a job that fine-tunes a specified model from a given dataset.
      Response includes details of the enqueued job including job status and the name of the fine-tuned models once complete.
      https://platform.openai.com/docs/api-reference/fine-tuning/create
    */
    void CreateFineTuningJob(const FFineTuningJob& FineTuningJob, const FOpenAIAuth& Auth);

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
      Get status updates for a fine-tuning job.
      https://platform.openai.com/docs/api-reference/fine-tuning/list-events
    */
    void ListFineTuningEvents(
        const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FFineTuningQueryParameters& FineTuningQueryParameters = {});

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
    DEFINE_EVENT_GETTER(CreateAudioTranslationCompleted)
    DEFINE_EVENT_GETTER(ListFilesCompleted)
    DEFINE_EVENT_GETTER(UploadFileCompleted)
    DEFINE_EVENT_GETTER(DeleteFileCompleted)
    DEFINE_EVENT_GETTER(RetrieveFileCompleted)
    DEFINE_EVENT_GETTER(RetrieveFileContentCompleted)
    DEFINE_EVENT_GETTER(ListFineTuneEventsCompleted)
    DEFINE_EVENT_GETTER(DeleteFineTunedModelCompleted)
    DEFINE_EVENT_GETTER(CreateModerationsCompleted)
    DEFINE_EVENT_GETTER(ListFineTuningJobsCompleted)
    DEFINE_EVENT_GETTER(CreateFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(RetrieveFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(CancelFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(ListFineTuningEventsCompleted)
    DEFINE_EVENT_GETTER(ListBatchCompleted)
    DEFINE_EVENT_GETTER(CreateBatchCompleted)
    DEFINE_EVENT_GETTER(RetrieveBatchCompleted)
    DEFINE_EVENT_GETTER(CancelBatchCompleted)

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
    DECLARE_HTTP_CALLBACK(OnListFineTuneEventsCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteFineTunedModelCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateModerationsCompleted)
    DECLARE_HTTP_CALLBACK(OnListFineTuningJobsCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnListFineTuningEventsCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnListBatchCompleted)

    void ProcessRequest(FHttpRequestRef HttpRequest);
    bool ParseImageRequest(FHttpResponsePtr Response, FImageResponse& ImageResponse);

    bool Success(FHttpResponsePtr Response, bool WasSuccessful);
    void Log(const FString& Info) const;
    void LogResponse(FHttpResponsePtr Response) const;
    void LogError(const FString& ErrorText) const;

    template <typename OutStructType>
    FString SerializeRequest(const OutStructType& OutStruct) const
    {
        TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(OutStruct);
        FString RequestBodyStr;
        UOpenAIFuncLib::JsonToString(Json, RequestBodyStr);
        return RequestBodyStr;
    }

    template <typename OutStructType>
    FHttpRequestRef MakeRequest(const OutStructType& OutStruct, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
    {
        auto HttpRequest = CreateRequest();
        HttpRequest->SetHeader("Content-Type", "application/json");
        HttpRequest->SetHeader("Authorization", FString("Bearer ").Append(Auth.APIKey));
        HttpRequest->SetHeader("OpenAI-Organization", Auth.OrganizationID);
        HttpRequest->SetHeader("OpenAI-Project", Auth.ProjectID);
        HttpRequest->SetURL(URL);
        HttpRequest->SetVerb(Method);
        const FString Content = SerializeRequest(OutStruct);
        Log(FString("Content was set as: ").Append(Content));
        HttpRequest->SetContentAsString(Content);
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

        ParsedResponseType ParsedResponse;
        if (UOpenAIFuncLib::ParseJSONToStruct(Response->GetContentAsString(), &ParsedResponse))
        {
            Delegate.Broadcast(ParsedResponse);
        }
        else
        {
            LogError("JSON deserialization error");
            RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
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
    bool HandleString(FString& IncomeString, bool& LastString) const;

    template <typename ResponseType>
    bool ParseStreamRequest(FHttpResponsePtr Response, TArray<ResponseType>& Responses)
    {
        if (!Response) return false;

        TArray<FString> StringArray;
        Response->GetContentAsString().ParseIntoArrayLines(StringArray);

        for (auto& String : StringArray)
        {
            bool LastString{false};
            if (HandleString(String, LastString))
            {
                if (LastString)
                {
                    break;
                }
                ResponseType ParsedResponse;
                if (!UOpenAIFuncLib::ParseJSONToStruct(String, &ParsedResponse)) continue;

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
            Delegate.Broadcast(ParsedResponses);
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
            Delegate.Broadcast(ParsedResponses);
        }
        else
        {
            LogError("JSON deserialization error");
            LogError(Response->GetContentAsString());
            RequestError.Broadcast(Response->GetURL(), Response->GetContentAsString());
        }
    }

    void CleanChatCompletionFieldsThatCantBeEmpty(const FChatCompletion& ChatCompletion, TSharedPtr<FJsonObject>& Json) const;
};

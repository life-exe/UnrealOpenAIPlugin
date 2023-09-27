// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HTTP.h"
#include "ResponseTypes.h"
#include "RequestTypes.h"
#include "Delegates.h"
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
      Creates a new edit for the provided input, instruction, and parameters.
      https://platform.openai.com/docs/api-reference/edits/create
    */
    void CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth);

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
      Creates a job that fine-tunes a specified model from a given dataset.
      https://platform.openai.com/docs/api-reference/fine-tunes/create
    */
    void CreateFineTune(const FFineTune& FineTune, const FOpenAIAuth& Auth);

    /**
      List your organization's fine-tuning jobs.
      https://platform.openai.com/docs/api-reference/fine-tunes/list
    */
    void ListFineTunes(const FOpenAIAuth& Auth);

    /**
      Gets info about the fine-tune job.
      https://platform.openai.com/docs/api-reference/fine-tunes/retrieve
    */
    void RetrieveFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth);

    /**
      Immediately cancel a fine-tune job.
      https://platform.openai.com/docs/api-reference/fine-tunes/cancel
    */
    void CancelFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth);

    /**
      Get fine-grained status updates for a fine-tune job.
      https://platform.openai.com/docs/api-reference/fine-tunes/events
    */
    void ListFineTuneEvents(const FString& FineTuneID, const FOpenAIAuth& Auth);

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
      Print response to console
    */
    void SetLogEnabled(bool LogEnabled) { bLogEnabled = LogEnabled; }

public:
    FOnRequestError& OnRequestError() { return RequestError; };
    FOnListModelsCompleted& OnListModelsCompleted() { return ListModelsCompleted; }
    FOnRetrieveModelCompleted& OnRetrieveModelCompleted() { return RetrieveModelCompleted; }
    FOnCreateCompletionCompleted& OnCreateCompletionCompleted() { return CreateCompletionCompleted; }
    FOnCreateChatCompletionCompleted& OnCreateChatCompletionCompleted() { return CreateChatCompletionCompleted; }
    FOnCreateChatCompletionStreamCompleted& OnCreateChatCompletionStreamCompleted() { return CreateChatCompletionStreamCompleted; }
    FOnCreateChatCompletionStreamCompleted& OnCreateChatCompletionStreamProgresses() { return CreateChatCompletionStreamProgresses; }
    FOnCreateEditCompleted& OnCreateEditCompleted() { return CreateEditCompleted; }
    FOnCreateImageCompleted& OnCreateImageCompleted() { return CreateImageCompleted; }
    FOnCreateImageEditCompleted& OnCreateImageEditCompleted() { return CreateImageEditCompleted; }
    FOnCreateImageVariationCompleted& OnCreateImageVariationCompleted() { return CreateImageVariationCompleted; }
    FOnCreateEmbeddingsCompleted& OnCreateEmbeddingsCompleted() { return CreateEmbeddingsCompleted; }
    FOnCreateAudioTranscriptionCompleted& OnCreateAudioTranscriptionCompleted() { return CreateAudioTranscriptionCompleted; }
    FOnCreateAudioTranslationCompleted& OnCreateAudioTranslationCompleted() { return CreateAudioTranslationCompleted; }
    FOnListFilesCompleted& OnListFilesCompleted() { return ListFilesCompleted; }
    FOnUploadFileCompleted& OnUploadFileCompleted() { return UploadFileCompleted; }
    FOnDeleteFileCompleted& OnDeleteFileCompleted() { return DeleteFileCompleted; }
    FOnRetrieveFileCompleted& OnRetrieveFileCompleted() { return RetrieveFileCompleted; }
    FOnRetrieveFileContentCompleted& OnRetrieveFileContentCompleted() { return RetrieveFileContentCompleted; }
    FOnCreateFineTuneCompleted& OnCreateFineTuneCompleted() { return CreateFineTuneCompleted; }
    FOnListFineTunesCompleted& OnListFineTunesCompleted() { return ListFineTunesCompleted; }
    FOnRetrieveFineTuneCompleted& OnRetrieveFineTuneCompleted() { return RetrieveFineTuneCompleted; }
    FOnCancelFineTuneCompleted& OnCancelFineTuneCompleted() { return CancelFineTuneCompleted; }
    FOnListFineTuneEventsCompleted& OnListFineTuneEventsCompleted() { return ListFineTuneEventsCompleted; }
    FOnDeleteFineTunedModelCompleted& OnDeleteFineTunedModelCompleted() { return DeleteFineTunedModelCompleted; }
    FOnCreateModerationsCompleted& OnCreateModerationsCompleted() { return CreateModerationsCompleted; }

private:
    TSharedPtr<OpenAI::IAPI> API;
    bool bLogEnabled{true};

    FOnRequestError RequestError;
    FOnListModelsCompleted ListModelsCompleted;
    FOnRetrieveModelCompleted RetrieveModelCompleted;
    FOnCreateCompletionCompleted CreateCompletionCompleted;
    FOnCreateChatCompletionCompleted CreateChatCompletionCompleted;
    FOnCreateChatCompletionStreamCompleted CreateChatCompletionStreamCompleted;
    FOnCreateChatCompletionStreamProgresses CreateChatCompletionStreamProgresses;
    FOnCreateEditCompleted CreateEditCompleted;
    FOnCreateImageCompleted CreateImageCompleted;
    FOnCreateImageEditCompleted CreateImageEditCompleted;
    FOnCreateImageVariationCompleted CreateImageVariationCompleted;
    FOnCreateEmbeddingsCompleted CreateEmbeddingsCompleted;
    FOnCreateAudioTranscriptionCompleted CreateAudioTranscriptionCompleted;
    FOnCreateAudioTranslationCompleted CreateAudioTranslationCompleted;
    FOnListFilesCompleted ListFilesCompleted;
    FOnUploadFileCompleted UploadFileCompleted;
    FOnDeleteFileCompleted DeleteFileCompleted;
    FOnRetrieveFileCompleted RetrieveFileCompleted;
    FOnRetrieveFileContentCompleted RetrieveFileContentCompleted;
    FOnCreateFineTuneCompleted CreateFineTuneCompleted;
    FOnListFineTunesCompleted ListFineTunesCompleted;
    FOnRetrieveFineTuneCompleted RetrieveFineTuneCompleted;
    FOnCancelFineTuneCompleted CancelFineTuneCompleted;
    FOnListFineTuneEventsCompleted ListFineTuneEventsCompleted;
    FOnDeleteFineTunedModelCompleted DeleteFineTunedModelCompleted;
    FOnCreateModerationsCompleted CreateModerationsCompleted;

    virtual void OnListModelsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnRetrieveModelCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateCompletionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateChatCompletionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateChatCompletionStreamCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateChatCompletionStreamProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
    virtual void OnCreateEditCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateImageCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateImageEditCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateImageVariationCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateEmbeddingsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateAudioTranscriptionCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateAudioTranslationCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnListFilesCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnUploadFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnDeleteFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnRetrieveFileCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnRetrieveFileContentCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateFineTuneCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnListFineTunesCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnRetrieveFineTuneCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCancelFineTuneCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnListFineTuneEventsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnDeleteFineTunedModelCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    virtual void OnCreateModerationsCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);

    void ProcessRequest(FHttpRequestRef HttpRequest);
    bool ParseImageRequest(FHttpResponsePtr Response, FImageResponse& ImageResponse);
    bool ParseChatCompletionStreamRequest(FHttpResponsePtr Response, TArray<FChatCompletionStreamResponse>& Responses);

    bool Success(FHttpResponsePtr Response, bool WasSuccessful);
    void LogResponse(FHttpResponsePtr Response);
    void LogError(const FString& ErrorText);

    template <typename OutStructType>
    FString SerializeRequest(const OutStructType& OutStruct) const
    {
        TSharedPtr<FJsonObject> RequestBody = FJsonObjectConverter::UStructToJsonObject(OutStruct);

        FString RequestBodyStr;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBodyStr);
        FJsonSerializer::Serialize(RequestBody.ToSharedRef(), Writer);

        return RequestBodyStr.ToLower();
    }

    template <typename OutStructType>
    FHttpRequestRef MakeRequest(const OutStructType& OutStruct, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
    {
        auto HttpRequest = CreateRequest();
        HttpRequest->SetHeader("Content-Type", "application/json");
        HttpRequest->SetHeader("Authorization", FString("Bearer ").Append(Auth.APIKey));
        HttpRequest->SetHeader("OpenAI-Organization", Auth.OrganizationID);
        HttpRequest->SetURL(URL);
        HttpRequest->SetVerb(Method);
        HttpRequest->SetContentAsString(SerializeRequest(OutStruct));
        return HttpRequest;
    }

    FHttpRequestRef MakeRequest(const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const
    {
        auto HttpRequest = CreateRequest();
        HttpRequest->SetHeader("Content-Type", "application/json");
        HttpRequest->SetHeader("Authorization", FString("Bearer ").Append(Auth.APIKey));
        HttpRequest->SetHeader("OpenAI-Organization", Auth.OrganizationID);
        HttpRequest->SetURL(URL);
        HttpRequest->SetVerb(Method);
        return HttpRequest;
    }

    template <typename OutStructType>
    bool ParseJSONToStruct(const FString& Data, OutStructType* OutStruct)
    {
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Data);
        TSharedPtr<FJsonObject> JsonObject;
        if (!FJsonSerializer::Deserialize(JsonReader, JsonObject)) return false;

        FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), OutStruct, 0, 0);
        return true;
    }

    template <typename ParsedResponseType, typename DelegateType>
    void HandleResponse(FHttpResponsePtr Response, bool WasSuccessful, DelegateType& Delegate)
    {
        if (!Success(Response, WasSuccessful)) return;

        ParsedResponseType ParsedResponse;
        if (ParseJSONToStruct(Response->GetContentAsString(), &ParsedResponse))
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
};

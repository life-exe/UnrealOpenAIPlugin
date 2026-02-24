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
#include "Provider/JsonParsers/FineTuningParser.h"
#include "Provider/JsonParsers/EvalParser.h"
#include "Provider/JsonParsers/VectorStoreParser.h"
#include "Provider/JsonParsers/ChatKitParser.h"
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
      Creates a custom voice.
      https://platform.openai.com/docs/api-reference/audio/createVoice
    */
    void CreateVoice(const FCreateVoice& CreateVoice, const FOpenAIAuth& Auth);

    /**
      Translates audio into into English.
      https://platform.openai.com/docs/api-reference/audio/create
    */
    void CreateAudioTranslation(const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth);

    /**
      Returns a list of files that belong to the user's organization.
      https://platform.openai.com/docs/api-reference/files/list
    */
    void ListFiles(const FListFilesParams& ListFilesParams, const FOpenAIAuth& Auth);

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
    void PauseFineTuningJob(const FString& FineTuneID, const FOpenAIAuth& Auth);
    void ResumeFineTuningJob(const FString& FineTuneID, const FOpenAIAuth& Auth);

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
      Create an assistant with a model and instructions.
      https://platform.openai.com/docs/api-reference/assistants/createAssistant
    */
    void CreateAssistant(const FCreateAssistant& CreateAssistant, const FOpenAIAuth& Auth);

    /**
      Returns a list of assistants.
      https://platform.openai.com/docs/api-reference/assistants/listAssistants
    */
    void ListAssistants(const FListAssistants& ListAssistants, const FOpenAIAuth& Auth);

    /**
      Retrieves an assistant.
      https://platform.openai.com/docs/api-reference/assistants/getAssistant
    */
    void RetrieveAssistant(const FString& AssistantId, const FOpenAIAuth& Auth);

    /**
      Modifies an assistant.
      https://platform.openai.com/docs/api-reference/assistants/modifyAssistant
    */
    void ModifyAssistant(const FString& AssistantId, const FModifyAssistant& ModifyAssistant, const FOpenAIAuth& Auth);

    /**
      Delete an assistant.
      https://platform.openai.com/docs/api-reference/assistants/deleteAssistant
    */
    void DeleteAssistant(const FString& AssistantId, const FOpenAIAuth& Auth);

    /**
      Creates a video generation job.
      https://platform.openai.com/docs/api-reference/videos/create
    */
    void CreateVideo(const FCreateVideo& CreateVideo, const FOpenAIAuth& Auth);

    /**
      Retrieves a video generation job.
      https://platform.openai.com/docs/api-reference/videos/retrieve
    */
    void RetrieveVideo(const FString& VideoId, const FOpenAIAuth& Auth);

    /**
      List recently generated videos.
      https://platform.openai.com/docs/api-reference/videos/list
    */
    void ListVideos(const FListVideos& ListVideos, const FOpenAIAuth& Auth);

    /**
      Delete a video generation job.
      https://platform.openai.com/docs/api-reference/videos/delete
    */
    void DeleteVideo(const FString& VideoId, const FOpenAIAuth& Auth);

    /**
      Create a remix of a completed video using a refreshed prompt.
      https://platform.openai.com/docs/api-reference/videos/remix
    */
    void RemixVideo(const FString& VideoId, const FRemixVideo& RemixVideo, const FOpenAIAuth& Auth);

    /**
      Download video content (video, thumbnail, or spritesheet).
      https://platform.openai.com/docs/api-reference/videos/download-content
    */
    void DownloadVideoContent(const FString& VideoId, const FDownloadVideoContent& DownloadVideoContent, const FOpenAIAuth& Auth);

    /**
      Create the structure of an evaluation that can be used to test a model's performance.
      https://platform.openai.com/docs/api-reference/evals/create
    */
    void CreateEval(const FCreateEval& CreateEval, const FOpenAIAuth& Auth);

    /**
      List evaluations for a project.
      https://platform.openai.com/docs/api-reference/evals/list
    */
    void ListEvals(const FEvalQueryParams& QueryParams, const FOpenAIAuth& Auth);

    /**
      Retrieves an evaluation.
      https://platform.openai.com/docs/api-reference/evals/retrieve
    */
    void RetrieveEval(const FString& EvalId, const FOpenAIAuth& Auth);

    /**
      Update an evaluation.
      https://platform.openai.com/docs/api-reference/evals/update
    */
    void UpdateEval(const FString& EvalId, const FUpdateEval& UpdateEval, const FOpenAIAuth& Auth);

    /**
      Delete an evaluation.
      https://platform.openai.com/docs/api-reference/evals/delete
    */
    void DeleteEval(const FString& EvalId, const FOpenAIAuth& Auth);

    /**
      Create a vector store.
      https://platform.openai.com/docs/api-reference/vector-stores/create
    */
    void CreateVectorStore(const FCreateVectorStore& CreateVectorStore, const FOpenAIAuth& Auth);

    /**
      Returns a list of vector stores.
      https://platform.openai.com/docs/api-reference/vector-stores/list
    */
    void ListVectorStores(const FVectorStoreQueryParams& QueryParams, const FOpenAIAuth& Auth);

    /**
      Retrieves a vector store.
      https://platform.openai.com/docs/api-reference/vector-stores/retrieve
    */
    void RetrieveVectorStore(const FString& VectorStoreId, const FOpenAIAuth& Auth);

    /**
      Modifies a vector store.
      https://platform.openai.com/docs/api-reference/vector-stores/update
    */
    void UpdateVectorStore(const FString& VectorStoreId, const FUpdateVectorStore& UpdateVectorStore, const FOpenAIAuth& Auth);

    /**
      Delete a vector store.
      https://platform.openai.com/docs/api-reference/vector-stores/delete
    */
    void DeleteVectorStore(const FString& VectorStoreId, const FOpenAIAuth& Auth);

    /**
      Create a vector store file by attaching a File to a vector store.
      https://platform.openai.com/docs/api-reference/vector-stores-files/create
    */
    void CreateVectorStoreFile(const FString& VectorStoreId, const FString& FileId, const FOpenAIAuth& Auth);

    /**
      Returns a list of vector store files.
      https://platform.openai.com/docs/api-reference/vector-stores-files/list
    */
    void ListVectorStoreFiles(const FString& VectorStoreId, const FVectorStoreFileQueryParams& QueryParams, const FOpenAIAuth& Auth);

    /**
      Retrieves a vector store file.
      https://platform.openai.com/docs/api-reference/vector-stores-files/retrieve
    */
    void RetrieveVectorStoreFile(const FString& VectorStoreId, const FString& FileId, const FOpenAIAuth& Auth);

    /**
      Delete a vector store file. This will remove the file from the vector store but the file itself will not be deleted.
      To delete the file, use the delete file endpoint.
      https://platform.openai.com/docs/api-reference/vector-stores-files/delete
    */
    void DeleteVectorStoreFile(const FString& VectorStoreId, const FString& FileId, const FOpenAIAuth& Auth);

    /**
      Create a vector store file batch.
      https://platform.openai.com/docs/api-reference/vector-stores-file-batches/create
    */
    void CreateVectorStoreFileBatch(const FString& VectorStoreId, const FCreateVectorStoreFileBatch& CreateBatch, const FOpenAIAuth& Auth);

    /**
      Retrieves a vector store file batch.
      https://platform.openai.com/docs/api-reference/vector-stores-file-batches/retrieve
    */
    void RetrieveVectorStoreFileBatch(const FString& VectorStoreId, const FString& BatchId, const FOpenAIAuth& Auth);

    /**
      Cancel a vector store file batch. This can only be done for batches that are in_progress.
      https://platform.openai.com/docs/api-reference/vector-stores-file-batches/cancel
    */
    void CancelVectorStoreFileBatch(const FString& VectorStoreId, const FString& BatchId, const FOpenAIAuth& Auth);

    /**
      Search a vector store for relevant chunks based on a query and file attributes filter.
      https://platform.openai.com/docs/api-reference/vector-stores/search
    */
    void SearchVectorStore(const FString& VectorStoreId, const FVectorStoreSearch& Search, const FOpenAIAuth& Auth);

    /**
      Create a ChatKit session.
    */
    void CreateChatKitSession(const FCreateChatKitSession& CreateSession, const FOpenAIAuth& Auth);

    /**
      Cancel an active ChatKit session.
    */
    void CancelChatKitSession(const FString& SessionId, const FOpenAIAuth& Auth);

    /**
      List ChatKit threads.
    */
    void ListChatKitThreads(const FListChatKitThreads& ListThreads, const FOpenAIAuth& Auth);

    /**
      Retrieve a ChatKit thread.
    */
    void RetrieveChatKitThread(const FString& ThreadId, const FOpenAIAuth& Auth);

    /**
      Delete a ChatKit thread.
    */
    void DeleteChatKitThread(const FString& ThreadId, const FOpenAIAuth& Auth);

    /**
      List items that belong to a ChatKit thread.
    */
    void ListChatKitThreadItems(const FString& ThreadId, const FListChatKitThreadItems& ListItems, const FOpenAIAuth& Auth);

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

#define DECLARE_HTTP_CALLBACK(Callback) virtual void Callback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
#define DECLARE_HTTP_CALLBACK_PROGRESS(Callback) virtual void Callback(FHttpRequestPtr Request, uint64 BytesSent, uint64 BytesReceived);

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
    DEFINE_EVENT_GETTER(CreateVoiceCompleted)
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
    DEFINE_EVENT_GETTER(PauseFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(ResumeFineTuningJobCompleted)
    DEFINE_EVENT_GETTER(ListBatchCompleted)
    DEFINE_EVENT_GETTER(CreateBatchCompleted)
    DEFINE_EVENT_GETTER(RetrieveBatchCompleted)
    DEFINE_EVENT_GETTER(CancelBatchCompleted)
    DEFINE_EVENT_GETTER(CreateUploadCompleted)
    DEFINE_EVENT_GETTER(AddUploadPartCompleted)
    DEFINE_EVENT_GETTER(CompleteUploadCompleted)
    DEFINE_EVENT_GETTER(CancelUploadCompleted)
    DEFINE_EVENT_GETTER(CreateAssistantCompleted)
    DEFINE_EVENT_GETTER(ListAssistantsCompleted)
    DEFINE_EVENT_GETTER(RetrieveAssistantCompleted)
    DEFINE_EVENT_GETTER(ModifyAssistantCompleted)
    DEFINE_EVENT_GETTER(DeleteAssistantCompleted)
    DEFINE_EVENT_GETTER(CreateVideoCompleted)
    DEFINE_EVENT_GETTER(RetrieveVideoCompleted)
    DEFINE_EVENT_GETTER(ListVideosCompleted)
    DEFINE_EVENT_GETTER(DeleteVideoCompleted)
    DEFINE_EVENT_GETTER(RemixVideoCompleted)
    DEFINE_EVENT_GETTER(DownloadVideoContentCompleted)
    DEFINE_EVENT_GETTER(CreateEvalCompleted)
    DEFINE_EVENT_GETTER(ListEvalsCompleted)
    DEFINE_EVENT_GETTER(RetrieveEvalCompleted)
    DEFINE_EVENT_GETTER(UpdateEvalCompleted)
    DEFINE_EVENT_GETTER(DeleteEvalCompleted)

    DEFINE_EVENT_GETTER(CreateVectorStoreCompleted)
    DEFINE_EVENT_GETTER(ListVectorStoresCompleted)
    DEFINE_EVENT_GETTER(RetrieveVectorStoreCompleted)
    DEFINE_EVENT_GETTER(UpdateVectorStoreCompleted)
    DEFINE_EVENT_GETTER(DeleteVectorStoreCompleted)
    DEFINE_EVENT_GETTER(CreateVectorStoreFileCompleted)
    DEFINE_EVENT_GETTER(ListVectorStoreFilesCompleted)
    DEFINE_EVENT_GETTER(RetrieveVectorStoreFileCompleted)
    DEFINE_EVENT_GETTER(DeleteVectorStoreFileCompleted)
    DEFINE_EVENT_GETTER(CreateVectorStoreFileBatchCompleted)
    DEFINE_EVENT_GETTER(RetrieveVectorStoreFileBatchCompleted)
    DEFINE_EVENT_GETTER(CancelVectorStoreFileBatchCompleted)
    DEFINE_EVENT_GETTER(SearchVectorStoreCompleted)

    DEFINE_EVENT_GETTER(CreateChatKitSessionCompleted)
    DEFINE_EVENT_GETTER(CancelChatKitSessionCompleted)
    DEFINE_EVENT_GETTER(ListChatKitThreadsCompleted)
    DEFINE_EVENT_GETTER(RetrieveChatKitThreadCompleted)
    DEFINE_EVENT_GETTER(DeleteChatKitThreadCompleted)
    DEFINE_EVENT_GETTER(ListChatKitThreadItemsCompleted)

private:
    TSharedPtr<OpenAI::IAPI> API;
    bool bLogEnabled{true};
    FOnRequestError RequestError;

#define DECLARE_HTTP_CALLBACK(Callback) virtual void Callback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
#define DECLARE_HTTP_CALLBACK_PROGRESS(Callback) virtual void Callback(FHttpRequestPtr Request, uint64 BytesSent, uint64 BytesReceived);

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
    DECLARE_HTTP_CALLBACK(OnCreateVoiceCompleted)
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
    DECLARE_HTTP_CALLBACK(OnPauseFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnResumeFineTuningJobCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnListBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateUploadCompleted)
    DECLARE_HTTP_CALLBACK(OnAddUploadPartCompleted)
    DECLARE_HTTP_CALLBACK(OnCompleteUploadCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelUploadCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateAssistantCompleted)
    DECLARE_HTTP_CALLBACK(OnListAssistantsCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveAssistantCompleted)
    DECLARE_HTTP_CALLBACK(OnModifyAssistantCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteAssistantCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateVideoCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveVideoCompleted)
    DECLARE_HTTP_CALLBACK(OnListVideosCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteVideoCompleted)
    DECLARE_HTTP_CALLBACK(OnRemixVideoCompleted)
    DECLARE_HTTP_CALLBACK(OnDownloadVideoContentCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateEvalCompleted)
    DECLARE_HTTP_CALLBACK(OnListEvalsCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveEvalCompleted)
    DECLARE_HTTP_CALLBACK(OnUpdateEvalCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteEvalCompleted)

    DECLARE_HTTP_CALLBACK(OnCreateVectorStoreCompleted)
    DECLARE_HTTP_CALLBACK(OnListVectorStoresCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveVectorStoreCompleted)
    DECLARE_HTTP_CALLBACK(OnUpdateVectorStoreCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteVectorStoreCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateVectorStoreFileCompleted)
    DECLARE_HTTP_CALLBACK(OnListVectorStoreFilesCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveVectorStoreFileCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteVectorStoreFileCompleted)
    DECLARE_HTTP_CALLBACK(OnCreateVectorStoreFileBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveVectorStoreFileBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelVectorStoreFileBatchCompleted)
    DECLARE_HTTP_CALLBACK(OnSearchVectorStoreCompleted)

    DECLARE_HTTP_CALLBACK(OnCreateChatKitSessionCompleted)
    DECLARE_HTTP_CALLBACK(OnCancelChatKitSessionCompleted)
    DECLARE_HTTP_CALLBACK(OnListChatKitThreadsCompleted)
    DECLARE_HTTP_CALLBACK(OnRetrieveChatKitThreadCompleted)
    DECLARE_HTTP_CALLBACK(OnDeleteChatKitThreadCompleted)
    DECLARE_HTTP_CALLBACK(OnListChatKitThreadItemsCompleted)

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
    FHttpRequestRef MakeRequest(
        const FFineTuningJob& FineTuningJob, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;
    FHttpRequestRef MakeRequest(const FCreateEval& CreateEval, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;
    FHttpRequestRef MakeRequest(const FUpdateEval& UpdateEval, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;
    FHttpRequestRef MakeRequest(
        const FCreateVectorStore& CreateVectorStore, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;
    FHttpRequestRef MakeRequest(
        const FUpdateVectorStore& UpdateVectorStore, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;
    FHttpRequestRef MakeRequest(
        const FCreateVectorStoreFileBatch& CreateBatch, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;
    FHttpRequestRef MakeRequest(const FVectorStoreSearch& Search, const FString& URL, const FString& Method, const FOpenAIAuth& Auth) const;

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
            LogError(FString::Format(TEXT("JSON deserialization error: {0}"), {Content}));
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
    void OnStreamProgress(FHttpRequestPtr Request, uint64 BytesSent, uint64 BytesReceived, DelegateType& Delegate)
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

            LogError(FString::Format(TEXT("JSON deserialization error: {0}"), {Content}));
            LogError(Content);
            RequestError.Broadcast(ResponseURL, Content);
        }
    }
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/AllTypesHeader.h"

// ============================ C++ delegates ============================
// common
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestCompleted, const FString& /* URL */, const FString& /* Response content */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestProgresses, const FString& /* URL */, const FString& /* Response content */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestError, const FString& /* URL */, const FString& /* Response content */);
// models
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListModelsCompleted, const FListModelsResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRetrieveModelCompleted, const FRetrieveModelResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnDeleteFineTunedModelCompleted, const FDeleteFineTunedModelResponse&, const FOpenAIResponseMetadata&);
// chat
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateCompletionCompleted, const FCompletionResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnCreateCompletionStreamCompleted, const TArray<FCompletionStreamResponse>&, const FOpenAIResponseMetadata&);
using FOnCreateCompletionStreamProgresses = FOnCreateCompletionStreamCompleted;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateChatCompletionCompleted, const FChatCompletionResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnCreateChatCompletionStreamCompleted, const TArray<FChatCompletionStreamResponse>&, const FOpenAIResponseMetadata&);
using FOnCreateChatCompletionStreamProgresses = FOnCreateChatCompletionStreamCompleted;
// images
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateImageCompleted, const FImageResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateImageEditCompleted, const FImageEditResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateImageVariationCompleted, const FImageVariationResponse&, const FOpenAIResponseMetadata&);
// embeddings
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateEmbeddingsCompleted, const FEmbeddingsResponse&, const FOpenAIResponseMetadata&);
// audio + speech
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateSpeechCompleted, const FSpeechResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnCreateAudioTranscriptionCompleted, const FAudioTranscriptionResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnCreateAudioTranscriptionVerboseCompleted, const FAudioTranscriptionVerboseResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateAudioTranslationCompleted, const FAudioTranslationResponse&, const FOpenAIResponseMetadata&);
// files
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListFilesCompleted, const FListFilesResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUploadFileCompleted, const FUploadFileResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDeleteFileCompleted, const FDeleteFileResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRetrieveFileCompleted, const FRetrieveFileResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRetrieveFileContentCompleted, const FRetrieveFileContentResponse&, const FOpenAIResponseMetadata&);
// fine tunes
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateFineTuningJobCompleted, const FFineTuningJobObjectResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListFineTuningJobsCompleted, const FListFineTuningJobsResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnListFineTuningEventsCompleted, const FListFineTuningEventsResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnListFineTuningCheckpointsCompleted, const FListFineTuningCheckpointsResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnRetrieveFineTuningJobCompleted, const FFineTuningJobObjectResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCancelFineTuningJobCompleted, const FFineTuningJobObjectResponse&, const FOpenAIResponseMetadata&);
// moderations
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateModerationsCompleted, const FModerationsResponse&, const FOpenAIResponseMetadata&);
// batches
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListBatchCompleted, const FListBatchResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateBatchCompleted, const FCreateBatchResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRetrieveBatchCompleted, const FRetrieveBatchResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCancelBatchCompleted, const FCancelBatchResponse&, const FOpenAIResponseMetadata&);
// uploads
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCreateUploadCompleted, const FUploadObjectResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAddUploadPartCompleted, const FUploadPartObjectResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCompleteUploadCompleted, const FUploadObjectResponse&, const FOpenAIResponseMetadata&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCancelUploadCompleted, const FUploadObjectResponse&, const FOpenAIResponseMetadata&);

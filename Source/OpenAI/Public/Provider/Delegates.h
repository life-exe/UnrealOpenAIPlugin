// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ResponseTypes.h"

// C++ delegates
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestCompleted, const FString& /* URL */, const FString& /* Response content */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestProgresses, const FString& /* URL */, const FString& /* Response content */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestError, const FString& /* URL */, const FString& /* Response content */);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnListModelsCompleted, const FListModelsResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRetrieveModelCompleted, const FRetrieveModelResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateCompletionCompleted, const FCompletionResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateCompletionStreamCompleted, const TArray<FCompletionStreamResponse>&);
using FOnCreateCompletionStreamProgresses = FOnCreateCompletionStreamCompleted;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateChatCompletionCompleted, const FChatCompletionResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateChatCompletionStreamCompleted, const TArray<FChatCompletionStreamResponse>&);
using FOnCreateChatCompletionStreamProgresses = FOnCreateChatCompletionStreamCompleted;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateEditCompleted, const FEditResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateImageCompleted, const FImageResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateImageEditCompleted, const FImageEditResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateImageVariationCompleted, const FImageVariationResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateEmbeddingsCompleted, const FEmbeddingsResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateAudioTranscriptionCompleted, const FAudioTranscriptionResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateAudioTranslationCompleted, const FAudioTranslationResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnListFilesCompleted, const FListFilesResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUploadFileCompleted, const FUploadFileResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeleteFileCompleted, const FDeleteFileResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRetrieveFileCompleted, const FRetrieveFileResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRetrieveFileContentCompleted, const FRetrieveFileContentResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateFineTuneCompleted, const FFineTuneResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnListFineTunesCompleted, const FListFineTuneResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRetrieveFineTuneCompleted, const FFineTuneResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCancelFineTuneCompleted, const FFineTuneResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnListFineTuneEventsCompleted, const FFineTuneEventsResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeleteFineTunedModelCompleted, const FDeleteFineTuneResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateModerationsCompleted, const FModerationsResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnListFineTuningJobsCompleted, const FListFineTuningJobsResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateFineTuningJobCompleted, const FFineTuningJobObjectResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRetrieveFineTuningJobCompleted, const FFineTuningJobObjectResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCancelFineTuningJobCompleted, const FFineTuningJobObjectResponse&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnListFineTuningEventsCompleted, const FFineTuningJobEventResponse&);

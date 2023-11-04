// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/CreateFineTuningJobAction.h"
#include "Provider/OpenAIProvider.h"

UCreateFineTuningJobAction* UCreateFineTuningJobAction::CreateFineTuningJob(const FFineTuningJob& FineTuningJob, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UCreateFineTuningJobAction>();
    CompletionAction->FineTuningJob = FineTuningJob;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UCreateFineTuningJobAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateFineTuningJobCompleted().AddUObject(this, &ThisClass::OnCreateFineTuningJobCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateFineTuningJob(FineTuningJob, Auth);
}

void UCreateFineTuningJobAction::OnCreateFineTuningJobCompleted(const FFineTuningJobObjectResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UCreateFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

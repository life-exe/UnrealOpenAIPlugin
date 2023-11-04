// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningJobsAction.h"
#include "Provider/OpenAIProvider.h"

UListFineTuningJobsAction* UListFineTuningJobsAction::ListFineTuningJobs(
    const FFineTuningQueryParameters& FineTuningQueryParameters, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UListFineTuningJobsAction>();
    CompletionAction->FineTuningQueryParameters = FineTuningQueryParameters;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UListFineTuningJobsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTuningJobsCompleted().AddUObject(this, &ThisClass::OnListFineTuningJobsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->ListFineTuningJobs(Auth);
}

void UListFineTuningJobsAction::OnListFineTuningJobsCompleted(const FListFineTuningJobsResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UListFineTuningJobsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

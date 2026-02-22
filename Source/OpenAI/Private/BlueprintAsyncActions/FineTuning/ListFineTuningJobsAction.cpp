// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningJobsAction.h"

UListFineTuningJobsAction* UListFineTuningJobsAction::ListFineTuningJobs(
    const FFineTuningQueryParameters& FineTuningQueryParameters, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ListFineTuningJobsAction = NewObject<UListFineTuningJobsAction>();
    ListFineTuningJobsAction->FineTuningQueryParameters = FineTuningQueryParameters;
    ListFineTuningJobsAction->Auth = Auth;
    ListFineTuningJobsAction->URLOverride = URLOverride;
    return ListFineTuningJobsAction;
}

void UListFineTuningJobsAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnListFineTuningJobsCompleted().AddUObject(this, &ThisClass::OnListFineTuningJobsCompleted);
    Provider->ListFineTuningJobs(Auth);
}

void UListFineTuningJobsAction::OnListFineTuningJobsCompleted(
    const FListFineTuningJobsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListFineTuningJobsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListFineTuningJobsAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

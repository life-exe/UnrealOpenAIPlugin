// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/CancelFineTuningJobAction.h"

UCancelFineTuningJobAction* UCancelFineTuningJobAction::CancelFineTuningJob(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CancelFineTuningJobAction = NewObject<UCancelFineTuningJobAction>();
    CancelFineTuningJobAction->FineTuningJobID = FineTuningJobID;
    CancelFineTuningJobAction->Auth = Auth;
    CancelFineTuningJobAction->URLOverride = URLOverride;
    return CancelFineTuningJobAction;
}

void UCancelFineTuningJobAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCancelFineTuningJobCompleted().AddUObject(this, &ThisClass::OnCancelFineTuningJobCompleted);
    Provider->CancelFineTuningJob(FineTuningJobID, Auth);
}

void UCancelFineTuningJobAction::OnCancelFineTuningJobCompleted(
    const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCancelFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCancelFineTuningJobAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

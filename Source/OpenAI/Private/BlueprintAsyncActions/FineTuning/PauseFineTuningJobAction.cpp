// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/PauseFineTuningJobAction.h"

UPauseFineTuningJobAction* UPauseFineTuningJobAction::PauseFineTuningJob(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* PauseFineTuningJobAction = NewObject<UPauseFineTuningJobAction>();
    PauseFineTuningJobAction->FineTuningJobID = FineTuningJobID;
    PauseFineTuningJobAction->Auth = Auth;
    PauseFineTuningJobAction->URLOverride = URLOverride;
    return PauseFineTuningJobAction;
}

void UPauseFineTuningJobAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnPauseFineTuningJobCompleted().AddUObject(this, &ThisClass::OnPauseFineTuningJobCompleted);
    Provider->PauseFineTuningJob(FineTuningJobID, Auth);
}

void UPauseFineTuningJobAction::OnPauseFineTuningJobCompleted(
    const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UPauseFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UPauseFineTuningJobAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

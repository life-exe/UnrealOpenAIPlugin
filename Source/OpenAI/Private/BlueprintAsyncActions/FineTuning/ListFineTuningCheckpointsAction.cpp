// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningCheckpointsAction.h"

UListFineTuningCheckpointsAction* UListFineTuningCheckpointsAction::ListFineTuningCheckpoints(
    const FString& FineTuningID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ListFineTuningCheckpointsAction = NewObject<UListFineTuningCheckpointsAction>();
    ListFineTuningCheckpointsAction->FineTuningID = FineTuningID;
    ListFineTuningCheckpointsAction->Auth = Auth;
    ListFineTuningCheckpointsAction->URLOverride = URLOverride;
    return ListFineTuningCheckpointsAction;
}

void UListFineTuningCheckpointsAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnListFineTuningCheckpointsCompleted().AddUObject(this, &ThisClass::OnListFineTuningCheckpointsCompleted);
    Provider->ListFineTuningEvents(FineTuningID, Auth);
}

void UListFineTuningCheckpointsAction::OnListFineTuningCheckpointsCompleted(
    const FListFineTuningCheckpointsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListFineTuningCheckpointsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListFineTuningCheckpointsAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

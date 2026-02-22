// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningEventsAction.h"

UListFineTuningEventsAction* UListFineTuningEventsAction::ListFineTuningEvents(
    const FString& FineTuningID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ListFineTuningEventsAction = NewObject<UListFineTuningEventsAction>();
    ListFineTuningEventsAction->FineTuningID = FineTuningID;
    ListFineTuningEventsAction->Auth = Auth;
    ListFineTuningEventsAction->URLOverride = URLOverride;
    return ListFineTuningEventsAction;
}

void UListFineTuningEventsAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnListFineTuningEventsCompleted().AddUObject(this, &ThisClass::OnListFineTuningEventsCompleted);
    Provider->ListFineTuningEvents(FineTuningID, Auth);
}

void UListFineTuningEventsAction::OnListFineTuningEventsCompleted(
    const FListFineTuningEventsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListFineTuningEventsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListFineTuningEventsAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

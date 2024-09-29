// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningCheckpointsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTuningCheckpointsCompleted().AddUObject(this, &ThisClass::OnListFineTuningCheckpointsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UListFineTuningCheckpointsAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTuningJobs = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

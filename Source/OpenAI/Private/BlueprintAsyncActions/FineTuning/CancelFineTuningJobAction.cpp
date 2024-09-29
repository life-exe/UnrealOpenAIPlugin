// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/CancelFineTuningJobAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCancelFineTuningJobCompleted().AddUObject(this, &ThisClass::OnCancelFineTuningJobCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UCancelFineTuningJobAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTuningJobs = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

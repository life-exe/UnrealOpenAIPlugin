// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/CancelFineTuningJobAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UCancelFineTuningJobAction* UCancelFineTuningJobAction::CancelFineTuningJob(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompletionAction = NewObject<UCancelFineTuningJobAction>();
    CompletionAction->FineTuningJobID = FineTuningJobID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UCancelFineTuningJobAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCancelFineTuningJobCompleted().AddUObject(this, &ThisClass::OnCancelFineTuningJobCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->CancelFineTuningJob(FineTuningJobID, Auth);
}

void UCancelFineTuningJobAction::OnCancelFineTuningJobCompleted(const FFineTuningJobObjectResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UCancelFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

void UCancelFineTuningJobAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTuningJobs = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

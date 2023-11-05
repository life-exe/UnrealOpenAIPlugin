// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningJobsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UListFineTuningJobsAction* UListFineTuningJobsAction::ListFineTuningJobs(
    const FFineTuningQueryParameters& FineTuningQueryParameters, const FOpenAIAuth& Auth, const FString& URLOverride)
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
    TryToOverrideURL(Provider);
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

void UListFineTuningJobsAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTuningJobs = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

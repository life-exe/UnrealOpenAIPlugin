// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningJobsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTuningJobsCompleted().AddUObject(this, &ThisClass::OnListFineTuningJobsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UListFineTuningJobsAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTuningJobs = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/RetrieveFineTuningJobAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

URetrieveFineTuningJobAction* URetrieveFineTuningJobAction::RetrieveFineTuningJob(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveFineTuningJobAction = NewObject<URetrieveFineTuningJobAction>();
    RetrieveFineTuningJobAction->FineTuningJobID = FineTuningJobID;
    RetrieveFineTuningJobAction->Auth = Auth;
    RetrieveFineTuningJobAction->URLOverride = URLOverride;
    return RetrieveFineTuningJobAction;
}

void URetrieveFineTuningJobAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveFineTuningJobCompleted().AddUObject(this, &ThisClass::OnRetrieveFineTuningJobCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->RetrieveFineTuningJob(FineTuningJobID, Auth);
}

void URetrieveFineTuningJobAction::OnRetrieveFineTuningJobCompleted(
    const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveFineTuningJobAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTuningJobs = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

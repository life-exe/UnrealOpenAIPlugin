// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/RetrieveFineTuningJobAction.h"

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
    auto* Provider = CreateProvider();
    Provider->OnRetrieveFineTuningJobCompleted().AddUObject(this, &ThisClass::OnRetrieveFineTuningJobCompleted);
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

void URetrieveFineTuningJobAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

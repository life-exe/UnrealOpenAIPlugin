// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/CreateFineTuningJobAction.h"

UCreateFineTuningJobAction* UCreateFineTuningJobAction::CreateFineTuningJob(
    const FFineTuningJob& FineTuningJob, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CreateFineTuningJobAction = NewObject<UCreateFineTuningJobAction>();
    CreateFineTuningJobAction->FineTuningJob = FineTuningJob;
    CreateFineTuningJobAction->Auth = Auth;
    CreateFineTuningJobAction->URLOverride = URLOverride;
    return CreateFineTuningJobAction;
}

void UCreateFineTuningJobAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateFineTuningJobCompleted().AddUObject(this, &ThisClass::OnCreateFineTuningJobCompleted);
    Provider->CreateFineTuningJob(FineTuningJob, Auth);
}

void UCreateFineTuningJobAction::OnCreateFineTuningJobCompleted(
    const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCreateFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCreateFineTuningJobAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

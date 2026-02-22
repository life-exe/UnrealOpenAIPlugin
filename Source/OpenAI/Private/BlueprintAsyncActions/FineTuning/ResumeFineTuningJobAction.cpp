// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ResumeFineTuningJobAction.h"

UResumeFineTuningJobAction* UResumeFineTuningJobAction::ResumeFineTuningJob(
    const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ResumeFineTuningJobAction = NewObject<UResumeFineTuningJobAction>();
    ResumeFineTuningJobAction->FineTuningJobID = FineTuningJobID;
    ResumeFineTuningJobAction->Auth = Auth;
    ResumeFineTuningJobAction->URLOverride = URLOverride;
    return ResumeFineTuningJobAction;
}

void UResumeFineTuningJobAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnResumeFineTuningJobCompleted().AddUObject(this, &ThisClass::OnResumeFineTuningJobCompleted);
    Provider->ResumeFineTuningJob(FineTuningJobID, Auth);
}

void UResumeFineTuningJobAction::OnResumeFineTuningJobCompleted(
    const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UResumeFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UResumeFineTuningJobAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.FineTuningJobs = URL;
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/RetrieveFineTuningJobAction.h"
#include "Provider/OpenAIProvider.h"

URetrieveFineTuningJobAction* URetrieveFineTuningJobAction::RetrieveFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<URetrieveFineTuningJobAction>();
    CompletionAction->FineTuningJobID = FineTuningJobID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void URetrieveFineTuningJobAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveFineTuningJobCompleted().AddUObject(this, &ThisClass::OnRetrieveFineTuningJobCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->RetrieveFineTuningJob(FineTuningJobID, Auth);
}

void URetrieveFineTuningJobAction::OnRetrieveFineTuningJobCompleted(const FFineTuningJobObjectResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void URetrieveFineTuningJobAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

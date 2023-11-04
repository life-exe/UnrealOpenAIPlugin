// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/CancelFineTuningJobAction.h"
#include "Provider/OpenAIProvider.h"

UCancelFineTuningJobAction* UCancelFineTuningJobAction::CancelFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth)
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

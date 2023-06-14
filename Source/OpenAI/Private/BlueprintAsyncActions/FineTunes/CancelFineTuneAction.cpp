// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/CancelFineTuneAction.h"
#include "Provider/OpenAIProvider.h"

UCancelFineTuneAction* UCancelFineTuneAction::CancelFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UCancelFineTuneAction>();
    CompletionAction->FineTuneID = FineTuneID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UCancelFineTuneAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCancelFineTuneCompleted().AddUObject(this, &ThisClass::OnCancelFineTuneCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CancelFineTune(FineTuneID, Auth);
}

void UCancelFineTuneAction::OnCancelFineTuneCompleted(const FFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UCancelFineTuneAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

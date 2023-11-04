// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTuning/ListFineTuningEventsAction.h"
#include "Provider/OpenAIProvider.h"

UListFineTuningEventsAction* UListFineTuningEventsAction::ListFineTuningEvents(const FString& FineTuningID, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UListFineTuningEventsAction>();
    CompletionAction->FineTuningID = FineTuningID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UListFineTuningEventsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTuneEventsCompleted().AddUObject(this, &ThisClass::OnListFineTuningEventsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->ListFineTuningEvents(FineTuningID, Auth);
}

void UListFineTuningEventsAction::OnListFineTuningEventsCompleted(const FFineTuneEventsResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UListFineTuningEventsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

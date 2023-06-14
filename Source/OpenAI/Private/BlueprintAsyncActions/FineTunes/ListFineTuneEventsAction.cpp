// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/ListFineTuneEventsAction.h"
#include "Provider/OpenAIProvider.h"

UListFineTuneEventsAction* UListFineTuneEventsAction::ListFineTuneEvents(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UListFineTuneEventsAction>();
    CompletionAction->FineTuneID = FineTuneID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UListFineTuneEventsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTuneEventsCompleted().AddUObject(this, &ThisClass::OnListFineTuneEventsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->ListFineTuneEvents(FineTuneID, Auth);
}

void UListFineTuneEventsAction::OnListFineTuneEventsCompleted(const FFineTuneEventsResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UListFineTuneEventsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

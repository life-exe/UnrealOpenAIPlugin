// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/ListFineTuneEventsAction.h"
#include "Provider/OpenAIProvider.h"

UDEPRECATED_ListFineTuneEventsAction* UDEPRECATED_ListFineTuneEventsAction::ListFineTuneEvents(
    const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UDEPRECATED_ListFineTuneEventsAction>();
    CompletionAction->FineTuneID = FineTuneID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UDEPRECATED_ListFineTuneEventsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTuneEventsCompleted().AddUObject(this, &ThisClass::OnListFineTuneEventsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    Provider->ListFineTuneEvents(FineTuneID, Auth);
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UDEPRECATED_ListFineTuneEventsAction::OnListFineTuneEventsCompleted(const FFineTuneEventsResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDEPRECATED_ListFineTuneEventsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

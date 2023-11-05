// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/ListFineTuneEventsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UDEPRECATED_ListFineTuneEventsAction* UDEPRECATED_ListFineTuneEventsAction::ListFineTuneEvents(
    const FString& FineTuneID, const FOpenAIAuth& Auth, const FString& URLOverride)
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
    TryToOverrideURL(Provider);
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

void UDEPRECATED_ListFineTuneEventsAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTunes = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

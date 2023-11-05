// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/ListFineTunesAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UDEPRECATED_ListFineTunesAction* UDEPRECATED_ListFineTunesAction::ListFineTunes(const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompletionAction = NewObject<UDEPRECATED_ListFineTunesAction>();
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UDEPRECATED_ListFineTunesAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTunesCompleted().AddUObject(this, &ThisClass::OnListFineTunesCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    TryToOverrideURL(Provider);
    Provider->ListFineTunes(Auth);
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UDEPRECATED_ListFineTunesAction::OnListFineTunesCompleted(const FListFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDEPRECATED_ListFineTunesAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

void UDEPRECATED_ListFineTunesAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTunes = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

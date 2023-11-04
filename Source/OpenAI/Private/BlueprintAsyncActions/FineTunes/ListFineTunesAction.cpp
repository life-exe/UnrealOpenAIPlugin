// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/ListFineTunesAction.h"
#include "Provider/OpenAIProvider.h"

UDEPRECATED_ListFineTunesAction* UDEPRECATED_ListFineTunesAction::ListFineTunes(const FOpenAIAuth& Auth)
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

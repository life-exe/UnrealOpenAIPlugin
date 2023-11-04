// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/ListFineTuneAction.h"
#include "Provider/OpenAIProvider.h"

UDEPRECATED_ListFineTuneAction* UDEPRECATED_ListFineTuneAction::ListFineTunes(const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UDEPRECATED_ListFineTuneAction>();
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UDEPRECATED_ListFineTuneAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTunesCompleted().AddUObject(this, &ThisClass::OnListFineTunesCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    Provider->ListFineTunes(Auth);
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UDEPRECATED_ListFineTuneAction::OnListFineTunesCompleted(const FListFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDEPRECATED_ListFineTuneAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

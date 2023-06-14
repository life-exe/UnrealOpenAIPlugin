// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/ListFineTuneAction.h"
#include "Provider/OpenAIProvider.h"

UListFineTuneAction* UListFineTuneAction::ListFineTunes(const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UListFineTuneAction>();
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UListFineTuneAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFineTunesCompleted().AddUObject(this, &ThisClass::OnListFineTunesCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->ListFineTunes(Auth);
}

void UListFineTuneAction::OnListFineTunesCompleted(const FListFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UListFineTuneAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

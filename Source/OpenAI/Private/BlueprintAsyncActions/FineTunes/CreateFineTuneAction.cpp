// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/CreateFineTuneAction.h"
#include "Provider/OpenAIProvider.h"

/*
UCreateFineTuneAction* UCreateFineTuneAction::CreateFineTune(const FFineTune& FineTune, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UCreateFineTuneAction>();
    CompletionAction->FineTune = FineTune;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UCreateFineTuneAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateFineTuneCompleted().AddUObject(this, &ThisClass::OnCreateFineTuneCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateFineTune(FineTune, Auth);
}

void UCreateFineTuneAction::OnCreateFineTuneCompleted(const FFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UCreateFineTuneAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

*/

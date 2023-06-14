// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Chat/CompletionAction.h"
#include "Provider/OpenAIProvider.h"

UCompletionAction* UCompletionAction::CreateCompletion(const FCompletion& Completion, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UCompletionAction>();
    CompletionAction->Completion = Completion;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UCompletionAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateCompletionCompleted().AddUObject(this, &ThisClass::OnCreateCompletionCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateCompletion(Completion, Auth);
}

void UCompletionAction::OnCreateCompletionCompleted(const FCompletionResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UCompletionAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

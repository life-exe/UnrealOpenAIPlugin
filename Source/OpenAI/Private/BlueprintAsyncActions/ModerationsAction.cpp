// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/ModerationsAction.h"
#include "Provider/OpenAIProvider.h"

UModerationsAction* UModerationsAction::CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UModerationsAction>();
    CompletionAction->Moderations = Moderations;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UModerationsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateModerationsCompleted().AddUObject(this, &ThisClass::OnCreateModerationsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateModerations(Moderations, Auth);
}

void UModerationsAction::OnCreateModerationsCompleted(const FModerationsResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UModerationsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

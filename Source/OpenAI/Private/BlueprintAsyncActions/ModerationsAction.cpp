// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/ModerationsAction.h"

UModerationsAction* UModerationsAction::CreateModerations(
    const FModerations& Moderations, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompletionAction = NewObject<UModerationsAction>();
    CompletionAction->Moderations = Moderations;
    CompletionAction->Auth = Auth;
    CompletionAction->URLOverride = URLOverride;
    return CompletionAction;
}

void UModerationsAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateModerationsCompleted().AddUObject(this, &ThisClass::OnCreateModerationsCompleted);
    Provider->CreateModerations(Moderations, Auth);
}

void UModerationsAction::OnCreateModerationsCompleted(const FModerationsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UModerationsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UModerationsAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Moderations = URL;
}

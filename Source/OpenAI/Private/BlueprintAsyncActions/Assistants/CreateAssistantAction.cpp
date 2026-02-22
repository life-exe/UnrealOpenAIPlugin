// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/CreateAssistantAction.h"

UCreateAssistantAction* UCreateAssistantAction::CreateAssistantAction(
    const FCreateAssistant& CreateAssistant, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CreateAssistantAction = NewObject<UCreateAssistantAction>();
    CreateAssistantAction->CreateAssistant = CreateAssistant;
    CreateAssistantAction->Auth = Auth;
    CreateAssistantAction->URLOverride = URLOverride;
    return CreateAssistantAction;
}

void UCreateAssistantAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateAssistantCompleted().AddUObject(this, &ThisClass::OnCreateAssistantCompleted);
    Provider->CreateAssistant(CreateAssistant, Auth);
}

void UCreateAssistantAction::OnCreateAssistantCompleted(
    const FAssistantObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCreateAssistantAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCreateAssistantAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Assistants = URL;
}

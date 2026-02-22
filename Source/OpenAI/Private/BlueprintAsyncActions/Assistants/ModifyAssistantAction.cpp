// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/ModifyAssistantAction.h"

UModifyAssistantAction* UModifyAssistantAction::ModifyAssistantAction(
    const FString& AssistantId, const FModifyAssistant& ModifyAssistant, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ModifyAssistantAction = NewObject<UModifyAssistantAction>();
    ModifyAssistantAction->AssistantId = AssistantId;
    ModifyAssistantAction->ModifyAssistant = ModifyAssistant;
    ModifyAssistantAction->Auth = Auth;
    ModifyAssistantAction->URLOverride = URLOverride;
    return ModifyAssistantAction;
}

void UModifyAssistantAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnModifyAssistantCompleted().AddUObject(this, &ThisClass::OnModifyAssistantCompleted);
    Provider->ModifyAssistant(AssistantId, ModifyAssistant, Auth);
}

void UModifyAssistantAction::OnModifyAssistantCompleted(
    const FAssistantObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UModifyAssistantAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UModifyAssistantAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Assistants = URL;
}

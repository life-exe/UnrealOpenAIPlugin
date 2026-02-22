// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/RetrieveAssistantAction.h"

URetrieveAssistantAction* URetrieveAssistantAction::RetrieveAssistantAction(
    const FString& AssistantID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveAssistantAction = NewObject<URetrieveAssistantAction>();
    RetrieveAssistantAction->AssistantID = AssistantID;
    RetrieveAssistantAction->AssistantID = AssistantID;
    RetrieveAssistantAction->URLOverride = URLOverride;
    return RetrieveAssistantAction;
}

void URetrieveAssistantAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnRetrieveAssistantCompleted().AddUObject(this, &ThisClass::OnRetrieveAssistantCompleted);
    Provider->RetrieveAssistant(AssistantID, Auth);
}

void URetrieveAssistantAction::OnRetrieveAssistantCompleted(
    const FAssistantObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveAssistantAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveAssistantAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Assistants = URL;
}

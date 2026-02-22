// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/DeleteAssistantAction.h"

UDeleteAssistantAction* UDeleteAssistantAction::DeleteAssistantAction(
    const FString& AssistantId, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* DeleteAssistantAction = NewObject<UDeleteAssistantAction>();
    DeleteAssistantAction->AssistantId = AssistantId;
    DeleteAssistantAction->Auth = Auth;
    DeleteAssistantAction->URLOverride = URLOverride;
    return DeleteAssistantAction;
}

void UDeleteAssistantAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnDeleteAssistantCompleted().AddUObject(this, &ThisClass::OnDeleteAssistantActionCompleted);
    Provider->DeleteAssistant(AssistantId, Auth);
}

void UDeleteAssistantAction::OnDeleteAssistantActionCompleted(
    const FDeleteAssistantResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UDeleteAssistantAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UDeleteAssistantAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Assistants = URL;
}

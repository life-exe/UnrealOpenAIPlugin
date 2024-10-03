// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/ModifyAssistantAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnModifyAssistantCompleted().AddUObject(this, &ThisClass::OnModifyAssistantCompleted);
    TryToOverrideURL();
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

void UModifyAssistantAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranscriptions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

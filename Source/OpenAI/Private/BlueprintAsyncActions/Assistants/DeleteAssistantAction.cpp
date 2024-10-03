// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/DeleteAssistantAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnDeleteAssistantCompleted().AddUObject(this, &ThisClass::OnDeleteAssistantActionCompleted);
    TryToOverrideURL();
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

void UDeleteAssistantAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranscriptions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

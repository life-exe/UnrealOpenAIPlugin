// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/RetrieveAssistantAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveAssistantCompleted().AddUObject(this, &ThisClass::OnRetrieveAssistantCompleted);
    TryToOverrideURL();
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

void URetrieveAssistantAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranscriptions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

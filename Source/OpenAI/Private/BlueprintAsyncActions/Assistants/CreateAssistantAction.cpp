// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/CreateAssistantAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateAssistantCompleted().AddUObject(this, &ThisClass::OnCreateAssistantCompleted);
    TryToOverrideURL();
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

void UCreateAssistantAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranscriptions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/ListAssistantsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UListAssistantsAction* UListAssistantsAction::ListAssistantsAction(
    const FListAssistants& ListAssistants, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ListAssistantsAction = NewObject<UListAssistantsAction>();
    ListAssistantsAction->ListAssistants = ListAssistants;
    ListAssistantsAction->Auth = Auth;
    ListAssistantsAction->URLOverride = URLOverride;
    return ListAssistantsAction;
}

void UListAssistantsAction::Activate()
{
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnListAssistantsCompleted().AddUObject(this, &ThisClass::OnListAssistantsActionCompleted);
    TryToOverrideURL();
    Provider->ListAssistants(ListAssistants, Auth);
}

void UListAssistantsAction::OnListAssistantsActionCompleted(
    const FListAssistantsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListAssistantsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListAssistantsAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranscriptions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

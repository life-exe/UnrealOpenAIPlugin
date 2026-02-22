// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Assistants/ListAssistantsAction.h"

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
    auto* Provider = CreateProvider();
    Provider->OnListAssistantsCompleted().AddUObject(this, &ThisClass::OnListAssistantsActionCompleted);
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

void UListAssistantsAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Assistants = URL;
}

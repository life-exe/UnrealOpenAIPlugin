// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/ModerationsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UModerationsAction* UModerationsAction::CreateModerations(
    const FModerations& Moderations, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompletionAction = NewObject<UModerationsAction>();
    CompletionAction->Moderations = Moderations;
    CompletionAction->Auth = Auth;
    CompletionAction->URLOverride = URLOverride;
    return CompletionAction;
}

void UModerationsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateModerationsCompleted().AddUObject(this, &ThisClass::OnCreateModerationsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->CreateModerations(Moderations, Auth);
}

void UModerationsAction::OnCreateModerationsCompleted(const FModerationsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UModerationsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UModerationsAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Moderations = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

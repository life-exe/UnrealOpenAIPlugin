// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Models/ListModelsAction.h"

UListModelsAction* UListModelsAction::ListModels(const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ModelsAction = NewObject<UListModelsAction>();
    ModelsAction->Auth = Auth;
    ModelsAction->URLOverride = URLOverride;
    return ModelsAction;
}

void UListModelsAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnListModelsCompleted().AddUObject(this, &ThisClass::OnListModelsCompleted);
    Provider->ListModels(Auth);
}

void UListModelsAction::OnListModelsCompleted(const FListModelsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListModelsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListModelsAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Models = URL;
}

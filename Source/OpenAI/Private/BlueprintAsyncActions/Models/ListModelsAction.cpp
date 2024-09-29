// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Models/ListModelsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UListModelsAction* UListModelsAction::ListModels(const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ModelsAction = NewObject<UListModelsAction>();
    ModelsAction->Auth = Auth;
    ModelsAction->URLOverride = URLOverride;
    return ModelsAction;
}

void UListModelsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListModelsCompleted().AddUObject(this, &ThisClass::OnListModelsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UListModelsAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Models = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

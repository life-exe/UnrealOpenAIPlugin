// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/ListModelsAction.h"
#include "Provider/OpenAIProvider.h"

UListModelsAction* UListModelsAction::ListModels(const FOpenAIAuth& Auth)
{
    auto* ModelsAction = NewObject<UListModelsAction>();
    ModelsAction->Auth = Auth;
    return ModelsAction;
}

void UListModelsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListModelsCompleted().AddUObject(this, &ThisClass::OnListModelsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->ListModels(Auth);
}

void UListModelsAction::OnListModelsCompleted(const FListModelsResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UListModelsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}
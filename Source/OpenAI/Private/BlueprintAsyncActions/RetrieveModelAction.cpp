// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/RetrieveModelAction.h"
#include "Provider/OpenAIProvider.h"

URetrieveModelAction* URetrieveModelAction::RetrieveModel(const FString& ModelName, const FOpenAIAuth& Auth)
{
    auto* RetrieveModelAction = NewObject<URetrieveModelAction>();
    RetrieveModelAction->ModelName = ModelName;
    RetrieveModelAction->Auth = Auth;
    return RetrieveModelAction;
}

void URetrieveModelAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveModelCompleted().AddUObject(this, &ThisClass::OnRetrieveModelCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->RetrieveModel(ModelName, Auth);
}

void URetrieveModelAction::OnRetrieveModelCompleted(const FRetrieveModelResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void URetrieveModelAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

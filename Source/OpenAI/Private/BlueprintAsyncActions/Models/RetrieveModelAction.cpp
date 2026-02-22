// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Models/RetrieveModelAction.h"

URetrieveModelAction* URetrieveModelAction::RetrieveModel(const FString& ModelName, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveModelAction = NewObject<URetrieveModelAction>();
    RetrieveModelAction->ModelName = ModelName;
    RetrieveModelAction->Auth = Auth;
    RetrieveModelAction->URLOverride = URLOverride;
    return RetrieveModelAction;
}

void URetrieveModelAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnRetrieveModelCompleted().AddUObject(this, &ThisClass::OnRetrieveModelCompleted);
    Provider->RetrieveModel(ModelName, Auth);
}

void URetrieveModelAction::OnRetrieveModelCompleted(const FRetrieveModelResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveModelAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveModelAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Models = URL;
}

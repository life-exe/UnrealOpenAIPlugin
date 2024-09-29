// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Models/RetrieveModelAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveModelCompleted().AddUObject(this, &ThisClass::OnRetrieveModelCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void URetrieveModelAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Models = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

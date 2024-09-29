// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/RetrieveBatchAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

URetrieveBatchAction* URetrieveBatchAction::RetrieveBatch(const FString& BatchID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveBatchAction = NewObject<URetrieveBatchAction>();
    RetrieveBatchAction->BatchID = BatchID;
    RetrieveBatchAction->Auth = Auth;
    RetrieveBatchAction->URLOverride = URLOverride;
    return RetrieveBatchAction;
}

void URetrieveBatchAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveBatchCompleted().AddUObject(this, &ThisClass::OnRetrieveBatchCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->RetrieveBatch(BatchID, Auth);
}

void URetrieveBatchAction::OnRetrieveBatchCompleted(const FRetrieveBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveBatchAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveBatchAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Batches = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/RetrieveBatchAction.h"

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
    auto* Provider = CreateProvider();
    Provider->OnRetrieveBatchCompleted().AddUObject(this, &ThisClass::OnRetrieveBatchCompleted);
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

void URetrieveBatchAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Batches = URL;
}

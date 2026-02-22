// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/ListBatchAction.h"

UListBatchAction* UListBatchAction::ListBatch(const FListBatch& ListBatchData, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ListBatchAction = NewObject<UListBatchAction>();
    ListBatchAction->Auth = Auth;
    ListBatchAction->ListBatchData = ListBatchData;
    ListBatchAction->URLOverride = URLOverride;
    return ListBatchAction;
}

void UListBatchAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnListBatchCompleted().AddUObject(this, &ThisClass::OnListBatchCompleted);
    Provider->ListBatch(ListBatchData, Auth);
}

void UListBatchAction::OnListBatchCompleted(const FListBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListBatchAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListBatchAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Batches = URL;
}

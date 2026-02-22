// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/CreateBatchAction.h"

UCreateBatchAction* UCreateBatchAction::CreateBatch(const FCreateBatch CreateBatchData, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CreateBatchAction = NewObject<UCreateBatchAction>();
    CreateBatchAction->CreateBatchData = CreateBatchData;
    CreateBatchAction->Auth = Auth;
    CreateBatchAction->URLOverride = URLOverride;
    return CreateBatchAction;
}

void UCreateBatchAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateBatchCompleted().AddUObject(this, &ThisClass::OnCreateBatchCompleted);
    Provider->CreateBatch(CreateBatchData, Auth);
}

void UCreateBatchAction::OnCreateBatchCompleted(const FCreateBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCreateBatchAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCreateBatchAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Batches = URL;
}

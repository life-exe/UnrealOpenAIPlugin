// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/CancelBatchAction.h"

UCancelBatchAction* UCancelBatchAction::CancelBatch(const FString& BatchID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CancelBatchAction = NewObject<UCancelBatchAction>();
    CancelBatchAction->BatchID = BatchID;
    CancelBatchAction->Auth = Auth;
    CancelBatchAction->URLOverride = URLOverride;
    return CancelBatchAction;
}

void UCancelBatchAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCancelBatchCompleted().AddUObject(this, &ThisClass::OnCancelBatchCompleted);
    Provider->CancelBatch(BatchID, Auth);
}

void UCancelBatchAction::OnCancelBatchCompleted(const FCancelBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCancelBatchAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCancelBatchAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Batches = URL;
}

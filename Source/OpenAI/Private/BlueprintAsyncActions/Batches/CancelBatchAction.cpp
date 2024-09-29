// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/CancelBatchAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCancelBatchCompleted().AddUObject(this, &ThisClass::OnCancelBatchCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UCancelBatchAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Batches = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

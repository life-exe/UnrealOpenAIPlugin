// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/ListBatchAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListBatchCompleted().AddUObject(this, &ThisClass::OnListBatchCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UListBatchAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Batches = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Batches/CreateBatchAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateBatchCompleted().AddUObject(this, &ThisClass::OnCreateBatchCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UCreateBatchAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Batches = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

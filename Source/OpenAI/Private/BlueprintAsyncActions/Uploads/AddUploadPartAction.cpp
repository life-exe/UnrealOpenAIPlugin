// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/AddUploadPartAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UAddUploadPartAction* UAddUploadPartAction::AddUploadPart(
    const FString& UploadId, const FAddUploadPart& UploadPartRequest, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* AddUploadPartAction = NewObject<UAddUploadPartAction>();
    AddUploadPartAction->Auth = Auth;
    AddUploadPartAction->UploadId = UploadId;
    AddUploadPartAction->UploadPartRequest = UploadPartRequest;
    AddUploadPartAction->URLOverride = URLOverride;
    return AddUploadPartAction;
}

void UAddUploadPartAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnAddUploadPartCompleted().AddUObject(this, &ThisClass::OnAddUploadPartCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->AddUploadPart(UploadId, UploadPartRequest, Auth);
}

void UAddUploadPartAction::OnAddUploadPartCompleted(
    const FUploadPartObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UAddUploadPartAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UAddUploadPartAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Uploads = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

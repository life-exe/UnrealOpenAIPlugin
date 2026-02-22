// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/AddUploadPartAction.h"

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
    auto* Provider = CreateProvider();
    Provider->OnAddUploadPartCompleted().AddUObject(this, &ThisClass::OnAddUploadPartCompleted);
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

void UAddUploadPartAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Uploads = URL;
}

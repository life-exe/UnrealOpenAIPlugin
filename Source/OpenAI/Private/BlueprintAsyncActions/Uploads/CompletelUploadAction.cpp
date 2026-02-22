// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/CompleteUploadAction.h"

UCompleteUploadAction* UCompleteUploadAction::CompleteUpload(
    const FString& UploadId, const FCompleteUpload& CompleteUploadRequest, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompleteUploadAction = NewObject<UCompleteUploadAction>();
    CompleteUploadAction->Auth = Auth;
    CompleteUploadAction->UploadId = UploadId;
    CompleteUploadAction->CompleteUploadRequest = CompleteUploadRequest;
    CompleteUploadAction->URLOverride = URLOverride;
    return CompleteUploadAction;
}

void UCompleteUploadAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCompleteUploadCompleted().AddUObject(this, &ThisClass::OnCompleteUploadCompleted);
    Provider->CompleteUpload(UploadId, CompleteUploadRequest, Auth);
}

void UCompleteUploadAction::OnCompleteUploadCompleted(
    const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCompleteUploadAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCompleteUploadAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Uploads = URL;
}

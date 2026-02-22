// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/CancelUploadAction.h"

UCancelUploadAction* UCancelUploadAction::CancelUpload(const FString& UploadId, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CancelUploadAction = NewObject<UCancelUploadAction>();
    CancelUploadAction->Auth = Auth;
    CancelUploadAction->UploadId = UploadId;
    CancelUploadAction->URLOverride = URLOverride;
    return CancelUploadAction;
}

void UCancelUploadAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCancelUploadCompleted().AddUObject(this, &ThisClass::OnCancelUploadCompleted);
    Provider->CancelUpload(UploadId, Auth);
}

void UCancelUploadAction::OnCancelUploadCompleted(const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCancelUploadAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCancelUploadAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Uploads = URL;
}

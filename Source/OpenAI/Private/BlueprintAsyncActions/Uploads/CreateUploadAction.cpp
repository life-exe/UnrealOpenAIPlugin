// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/CreateUploadAction.h"

UCreateUploadAction* UCreateUploadAction::CreateUpload(
    const FCreateUpload& CreateUploadRequest, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CreateUploadAction = NewObject<UCreateUploadAction>();
    CreateUploadAction->Auth = Auth;
    CreateUploadAction->CreateUploadRequest = CreateUploadRequest;
    CreateUploadAction->URLOverride = URLOverride;
    return CreateUploadAction;
}

void UCreateUploadAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateUploadCompleted().AddUObject(this, &ThisClass::OnCreateUploadCompleted);
    Provider->CreateUpload(CreateUploadRequest, Auth);
}

void UCreateUploadAction::OnCreateUploadCompleted(const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCreateUploadAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCreateUploadAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Uploads = URL;
}

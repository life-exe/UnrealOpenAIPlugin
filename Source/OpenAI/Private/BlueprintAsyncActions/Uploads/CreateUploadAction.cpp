// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/CreateUploadAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateUploadCompleted().AddUObject(this, &ThisClass::OnCreateUploadCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UCreateUploadAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Uploads = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

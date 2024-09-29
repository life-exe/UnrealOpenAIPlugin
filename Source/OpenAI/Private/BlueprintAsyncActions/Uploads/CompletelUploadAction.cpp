// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/CompleteUploadAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCompleteUploadCompleted().AddUObject(this, &ThisClass::OnCompleteUploadCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UCompleteUploadAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Uploads = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

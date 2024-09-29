// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Uploads/CancelUploadAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCancelUploadCompleted().AddUObject(this, &ThisClass::OnCancelUploadCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UCancelUploadAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Uploads = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/UploadFileAction.h"

UUploadFileAction* UUploadFileAction::UploadFile(const FUploadFile& File, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* UploadFileAction = NewObject<UUploadFileAction>();
    UploadFileAction->File = File;
    UploadFileAction->Auth = Auth;
    UploadFileAction->URLOverride = URLOverride;
    return UploadFileAction;
}

void UUploadFileAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnUploadFileCompleted().AddUObject(this, &ThisClass::OnUploadFileCompleted);
    Provider->UploadFile(File, Auth);
}

void UUploadFileAction::OnUploadFileCompleted(const FUploadFileResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UUploadFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UUploadFileAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Files = URL;
}

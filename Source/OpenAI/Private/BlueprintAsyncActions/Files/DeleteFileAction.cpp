// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/DeleteFileAction.h"

UDeleteFileAction* UDeleteFileAction::DeleteFile(const FString& FileID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* DeleteFileAction = NewObject<UDeleteFileAction>();
    DeleteFileAction->FileID = FileID;
    DeleteFileAction->Auth = Auth;
    DeleteFileAction->URLOverride = URLOverride;
    return DeleteFileAction;
}

void UDeleteFileAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnDeleteFileCompleted().AddUObject(this, &ThisClass::OnDeleteFileCompleted);
    Provider->DeleteFile(FileID, Auth);
}

void UDeleteFileAction::OnDeleteFileCompleted(const FDeleteFileResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UDeleteFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UDeleteFileAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Files = URL;
}

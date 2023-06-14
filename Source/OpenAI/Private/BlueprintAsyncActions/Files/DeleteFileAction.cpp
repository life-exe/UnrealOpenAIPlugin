// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/DeleteFileAction.h"
#include "Provider/OpenAIProvider.h"

UDeleteFileAction* UDeleteFileAction::DeleteFile(const FString& FileID, const FOpenAIAuth& Auth)
{
    auto* DeleteFileAction = NewObject<UDeleteFileAction>();
    DeleteFileAction->FileID = FileID;
    DeleteFileAction->Auth = Auth;
    return DeleteFileAction;
}

void UDeleteFileAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnDeleteFileCompleted().AddUObject(this, &ThisClass::OnDeleteFileCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->DeleteFile(FileID, Auth);
}

void UDeleteFileAction::OnDeleteFileCompleted(const FDeleteFileResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDeleteFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

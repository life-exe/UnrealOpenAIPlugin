// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/UploadFileAction.h"
#include "Provider/OpenAIProvider.h"

UUploadFileAction* UUploadFileAction::UploadFile(const FUploadFile& File, const FOpenAIAuth& Auth)
{
    auto* UploadFileAction = NewObject<UUploadFileAction>();
    UploadFileAction->File = File;
    UploadFileAction->Auth = Auth;
    return UploadFileAction;
}

void UUploadFileAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnUploadFileCompleted().AddUObject(this, &ThisClass::OnUploadFileCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->UploadFile(File, Auth);
}

void UUploadFileAction::OnUploadFileCompleted(const FUploadFileResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UUploadFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

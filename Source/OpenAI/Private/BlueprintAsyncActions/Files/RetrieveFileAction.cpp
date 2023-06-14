// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/RetrieveFileAction.h"
#include "Provider/OpenAIProvider.h"

URetrieveFileAction* URetrieveFileAction::RetrieveFile(const FString& FileID, const FOpenAIAuth& Auth)
{
    auto* RetrieveFileAction = NewObject<URetrieveFileAction>();
    RetrieveFileAction->FileID = FileID;
    RetrieveFileAction->Auth = Auth;
    return RetrieveFileAction;
}

void URetrieveFileAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveFileCompleted().AddUObject(this, &ThisClass::OnRetrieveFileCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->RetrieveFile(FileID, Auth);
}

void URetrieveFileAction::OnRetrieveFileCompleted(const FRetrieveFileResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void URetrieveFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

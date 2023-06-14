// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/RetrieveFileContentAction.h"
#include "Provider/OpenAIProvider.h"

URetrieveFileContentAction* URetrieveFileContentAction::RetrieveFileContent(const FString& FileID, const FOpenAIAuth& Auth)
{
    auto* RetrieveFileContentAction = NewObject<URetrieveFileContentAction>();
    RetrieveFileContentAction->FileID = FileID;
    RetrieveFileContentAction->Auth = Auth;
    return RetrieveFileContentAction;
}

void URetrieveFileContentAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveFileContentCompleted().AddUObject(this, &ThisClass::OnRetrieveFileContentCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->RetrieveFileContent(FileID, Auth);
}

void URetrieveFileContentAction::OnRetrieveFileContentCompleted(const FRetrieveFileContentResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void URetrieveFileContentAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

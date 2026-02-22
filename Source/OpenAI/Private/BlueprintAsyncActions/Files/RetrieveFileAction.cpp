// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/RetrieveFileAction.h"

URetrieveFileAction* URetrieveFileAction::RetrieveFile(const FString& FileID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveFileAction = NewObject<URetrieveFileAction>();
    RetrieveFileAction->FileID = FileID;
    RetrieveFileAction->Auth = Auth;
    RetrieveFileAction->URLOverride = URLOverride;
    return RetrieveFileAction;
}

void URetrieveFileAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnRetrieveFileCompleted().AddUObject(this, &ThisClass::OnRetrieveFileCompleted);
    Provider->RetrieveFile(FileID, Auth);
}

void URetrieveFileAction::OnRetrieveFileCompleted(const FRetrieveFileResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveFileAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Files = URL;
}

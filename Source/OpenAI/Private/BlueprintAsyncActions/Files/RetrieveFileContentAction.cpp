// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/RetrieveFileContentAction.h"

URetrieveFileContentAction* URetrieveFileContentAction::RetrieveFileContent(
    const FString& FileID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveFileContentAction = NewObject<URetrieveFileContentAction>();
    RetrieveFileContentAction->FileID = FileID;
    RetrieveFileContentAction->Auth = Auth;
    RetrieveFileContentAction->URLOverride = URLOverride;
    return RetrieveFileContentAction;
}

void URetrieveFileContentAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnRetrieveFileContentCompleted().AddUObject(this, &ThisClass::OnRetrieveFileContentCompleted);
    Provider->RetrieveFileContent(FileID, Auth);
}

void URetrieveFileContentAction::OnRetrieveFileContentCompleted(
    const FRetrieveFileContentResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveFileContentAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveFileContentAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Files = URL;
}

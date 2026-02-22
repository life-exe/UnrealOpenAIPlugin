// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/ListFilesAction.h"

UListFilesAction* UListFilesAction::ListFiles(const FListFilesParams& ListFilesParams, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ListFilesAction = NewObject<UListFilesAction>();
    ListFilesAction->ListFilesParams = ListFilesParams;
    ListFilesAction->Auth = Auth;
    ListFilesAction->URLOverride = URLOverride;
    return ListFilesAction;
}

void UListFilesAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnListFilesCompleted().AddUObject(this, &ThisClass::OnListFilesCompleted);
    Provider->ListFiles(ListFilesParams, Auth);
}

void UListFilesAction::OnListFilesCompleted(const FListFilesResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListFilesAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListFilesAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Files = URL;
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Videos/DownloadVideoContentAction.h"

UDownloadVideoContentAction* UDownloadVideoContentAction::DownloadVideoContentAction(
    const FString& VideoId, const FDownloadVideoContent& DownloadVideoContent, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<UDownloadVideoContentAction>();
    Action->VideoId = VideoId;
    Action->DownloadVideoContent = DownloadVideoContent;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void UDownloadVideoContentAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnDownloadVideoContentCompleted().AddUObject(this, &ThisClass::OnDownloadVideoContentActionCompleted);
    Provider->DownloadVideoContent(VideoId, DownloadVideoContent, Auth);
}

void UDownloadVideoContentAction::OnDownloadVideoContentActionCompleted(
    const FDownloadVideoContentResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UDownloadVideoContentAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UDownloadVideoContentAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Videos = URL;
}

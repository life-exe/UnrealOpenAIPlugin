// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Videos/RemixVideoAction.h"

URemixVideoAction* URemixVideoAction::RemixVideoAction(
    const FString& VideoId, const FRemixVideo& RemixVideo, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<URemixVideoAction>();
    Action->VideoId = VideoId;
    Action->RemixVideo = RemixVideo;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void URemixVideoAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnRemixVideoCompleted().AddUObject(this, &ThisClass::OnRemixVideoActionCompleted);
    Provider->RemixVideo(VideoId, RemixVideo, Auth);
}

void URemixVideoAction::OnRemixVideoActionCompleted(const FVideoObject& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URemixVideoAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URemixVideoAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Videos = URL;
}

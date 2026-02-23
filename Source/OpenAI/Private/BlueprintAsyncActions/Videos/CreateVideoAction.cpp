// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Videos/CreateVideoAction.h"

UCreateVideoAction* UCreateVideoAction::CreateVideoAction(
    const FCreateVideo& CreateVideo, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<UCreateVideoAction>();
    Action->CreateVideo = CreateVideo;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void UCreateVideoAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateVideoCompleted().AddUObject(this, &ThisClass::OnCreateVideoActionCompleted);
    Provider->CreateVideo(CreateVideo, Auth);
}

void UCreateVideoAction::OnCreateVideoActionCompleted(const FVideoObject& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCreateVideoAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCreateVideoAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Videos = URL;
}

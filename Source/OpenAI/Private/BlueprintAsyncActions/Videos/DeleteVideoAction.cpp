// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Videos/DeleteVideoAction.h"

UDeleteVideoAction* UDeleteVideoAction::DeleteVideoAction(const FString& VideoId, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<UDeleteVideoAction>();
    Action->VideoId = VideoId;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void UDeleteVideoAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnDeleteVideoCompleted().AddUObject(this, &ThisClass::OnDeleteVideoActionCompleted);
    Provider->DeleteVideo(VideoId, Auth);
}

void UDeleteVideoAction::OnDeleteVideoActionCompleted(const FDeleteVideoResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UDeleteVideoAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UDeleteVideoAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Videos = URL;
}

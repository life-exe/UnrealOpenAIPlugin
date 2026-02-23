// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Videos/ListVideosAction.h"

UListVideosAction* UListVideosAction::ListVideosAction(const FListVideos& ListVideos, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<UListVideosAction>();
    Action->ListVideos = ListVideos;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void UListVideosAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnListVideosCompleted().AddUObject(this, &ThisClass::OnListVideosActionCompleted);
    Provider->ListVideos(ListVideos, Auth);
}

void UListVideosAction::OnListVideosActionCompleted(const FListVideosResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListVideosAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListVideosAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Videos = URL;
}

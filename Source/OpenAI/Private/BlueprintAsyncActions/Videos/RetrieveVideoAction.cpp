// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Videos/RetrieveVideoAction.h"

URetrieveVideoAction* URetrieveVideoAction::RetrieveVideoAction(const FString& VideoId, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<URetrieveVideoAction>();
    Action->VideoId = VideoId;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void URetrieveVideoAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnRetrieveVideoCompleted().AddUObject(this, &ThisClass::OnRetrieveVideoActionCompleted);
    Provider->RetrieveVideo(VideoId, Auth);
}

void URetrieveVideoAction::OnRetrieveVideoActionCompleted(const FVideoObject& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveVideoAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveVideoAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Videos = URL;
}

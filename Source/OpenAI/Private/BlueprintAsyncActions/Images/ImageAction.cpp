// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageAction.h"

UImageAction* UImageAction::CreateImage(const FOpenAIImage& Image, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ImageAction = NewObject<UImageAction>();
    ImageAction->Image = Image;
    ImageAction->Auth = Auth;
    ImageAction->URLOverride = URLOverride;
    return ImageAction;
}

void UImageAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateImageCompleted().AddUObject(this, &ThisClass::OnCreateImageCompleted);
    Provider->CreateImage(Image, Auth);
}

void UImageAction::OnCreateImageCompleted(const FImageResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UImageAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UImageAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.ImageGenerations = URL;
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageVariationAction.h"

UImageVariationAction* UImageVariationAction::CreateImageVariation(
    const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ImageVariationAction = NewObject<UImageVariationAction>();
    ImageVariationAction->ImageVariation = ImageVariation;
    ImageVariationAction->Auth = Auth;
    ImageVariationAction->URLOverride = URLOverride;
    return ImageVariationAction;
}

void UImageVariationAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateImageVariationCompleted().AddUObject(this, &ThisClass::OnCreateImageVariationCompleted);
    Provider->CreateImageVariation(ImageVariation, Auth);
}

void UImageVariationAction::OnCreateImageVariationCompleted(
    const FImageVariationResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UImageVariationAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UImageVariationAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.ImageVariations = URL;
}

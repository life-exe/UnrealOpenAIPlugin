// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageVariationAction.h"
#include "Provider/OpenAIProvider.h"

UImageVariationAction* UImageVariationAction::CreateImageVariation(const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth)
{
    auto* ImageVariationAction = NewObject<UImageVariationAction>();
    ImageVariationAction->ImageVariation = ImageVariation;
    ImageVariationAction->Auth = Auth;
    return ImageVariationAction;
}

void UImageVariationAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateImageVariationCompleted().AddUObject(this, &ThisClass::OnCreateImageVariationCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateImageVariation(ImageVariation, Auth);
}

void UImageVariationAction::OnCreateImageVariationCompleted(const FImageVariationResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UImageVariationAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageAction.h"
#include "Provider/OpenAIProvider.h"

UImageAction* UImageAction::CreateImage(const FOpenAIImage& Image, const FOpenAIAuth& Auth)
{
    auto* ImageAction = NewObject<UImageAction>();
    ImageAction->Image = Image;
    ImageAction->Auth = Auth;
    return ImageAction;
}

void UImageAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateImageCompleted().AddUObject(this, &ThisClass::OnCreateImageCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateImage(Image, Auth);
}

void UImageAction::OnCreateImageCompleted(const FImageResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UImageAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

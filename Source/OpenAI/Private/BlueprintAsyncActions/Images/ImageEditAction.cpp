// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageEditAction.h"
#include "Provider/OpenAIProvider.h"

UImageEditAction* UImageEditAction::CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth)
{
    auto* ImageEditAction = NewObject<UImageEditAction>();
    ImageEditAction->ImageEdit = ImageEdit;
    ImageEditAction->Auth = Auth;
    return ImageEditAction;
}

void UImageEditAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateImageEditCompleted().AddUObject(this, &ThisClass::OnCreateImageEditCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateImageEdit(ImageEdit, Auth);
}

void UImageEditAction::OnCreateImageEditCompleted(const FImageEditResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UImageEditAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

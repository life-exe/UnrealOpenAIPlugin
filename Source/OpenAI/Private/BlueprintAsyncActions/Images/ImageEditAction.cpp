// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageEditAction.h"

UImageEditAction* UImageEditAction::CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ImageEditAction = NewObject<UImageEditAction>();
    ImageEditAction->ImageEdit = ImageEdit;
    ImageEditAction->Auth = Auth;
    ImageEditAction->URLOverride = URLOverride;
    return ImageEditAction;
}

void UImageEditAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateImageEditCompleted().AddUObject(this, &ThisClass::OnCreateImageEditCompleted);
    Provider->CreateImageEdit(ImageEdit, Auth);
}

void UImageEditAction::OnCreateImageEditCompleted(const FImageEditResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UImageEditAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UImageEditAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.ImageEdits = URL;
}

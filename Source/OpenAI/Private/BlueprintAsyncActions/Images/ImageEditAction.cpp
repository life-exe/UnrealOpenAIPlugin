// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageEditAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateImageEditCompleted().AddUObject(this, &ThisClass::OnCreateImageEditCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL();
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

void UImageEditAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.ImageEdits = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

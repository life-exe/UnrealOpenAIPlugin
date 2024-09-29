// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateImageCompleted().AddUObject(this, &ThisClass::OnCreateImageCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL();
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

void UImageAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.ImageGenerations = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

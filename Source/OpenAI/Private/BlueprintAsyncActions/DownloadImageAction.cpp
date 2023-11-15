// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/DownloadImageAction.h"
#include "FuncLib/ImageFuncLib.h"

UDownloadImageAction* UDownloadImageAction::DownloadImage(const FString& URL)
{
    auto* ModelsAction = NewObject<UDownloadImageAction>();
    ModelsAction->URL = URL;
    return ModelsAction;
}

void UDownloadImageAction::Activate()
{
    auto HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleImageRequest);
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->ProcessRequest();
}

void UDownloadImageAction::HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
    if (bSucceeded && HttpResponse)
    {
        auto* Texture = UImageFuncLib::CreateTexture(HttpResponse->GetContent());
        OnCompleted.Broadcast(Texture);
        return;
    }
    OnCompleted.Broadcast(nullptr);
}

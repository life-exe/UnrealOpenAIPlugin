// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/UploadFileAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UUploadFileAction* UUploadFileAction::UploadFile(const FUploadFile& File, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* UploadFileAction = NewObject<UUploadFileAction>();
    UploadFileAction->File = File;
    UploadFileAction->Auth = Auth;
    UploadFileAction->URLOverride = URLOverride;
    return UploadFileAction;
}

void UUploadFileAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnUploadFileCompleted().AddUObject(this, &ThisClass::OnUploadFileCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->UploadFile(File, Auth);
}

void UUploadFileAction::OnUploadFileCompleted(const FUploadFileResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UUploadFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UUploadFileAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Files = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

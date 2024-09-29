// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/DeleteFileAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UDeleteFileAction* UDeleteFileAction::DeleteFile(const FString& FileID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* DeleteFileAction = NewObject<UDeleteFileAction>();
    DeleteFileAction->FileID = FileID;
    DeleteFileAction->Auth = Auth;
    DeleteFileAction->URLOverride = URLOverride;
    return DeleteFileAction;
}

void UDeleteFileAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnDeleteFileCompleted().AddUObject(this, &ThisClass::OnDeleteFileCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->DeleteFile(FileID, Auth);
}

void UDeleteFileAction::OnDeleteFileCompleted(const FDeleteFileResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UDeleteFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UDeleteFileAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Files = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

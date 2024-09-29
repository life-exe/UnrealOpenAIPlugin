// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/RetrieveFileAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

URetrieveFileAction* URetrieveFileAction::RetrieveFile(const FString& FileID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveFileAction = NewObject<URetrieveFileAction>();
    RetrieveFileAction->FileID = FileID;
    RetrieveFileAction->Auth = Auth;
    RetrieveFileAction->URLOverride = URLOverride;
    return RetrieveFileAction;
}

void URetrieveFileAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveFileCompleted().AddUObject(this, &ThisClass::OnRetrieveFileCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->RetrieveFile(FileID, Auth);
}

void URetrieveFileAction::OnRetrieveFileCompleted(const FRetrieveFileResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveFileAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveFileAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Files = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/RetrieveFileContentAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

URetrieveFileContentAction* URetrieveFileContentAction::RetrieveFileContent(
    const FString& FileID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* RetrieveFileContentAction = NewObject<URetrieveFileContentAction>();
    RetrieveFileContentAction->FileID = FileID;
    RetrieveFileContentAction->Auth = Auth;
    RetrieveFileContentAction->URLOverride = URLOverride;
    return RetrieveFileContentAction;
}

void URetrieveFileContentAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveFileContentCompleted().AddUObject(this, &ThisClass::OnRetrieveFileContentCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->RetrieveFileContent(FileID, Auth);
}

void URetrieveFileContentAction::OnRetrieveFileContentCompleted(
    const FRetrieveFileContentResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void URetrieveFileContentAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void URetrieveFileContentAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Files = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

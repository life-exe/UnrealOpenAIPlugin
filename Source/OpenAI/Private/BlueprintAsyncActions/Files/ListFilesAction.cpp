// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/ListFilesAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UListFilesAction* UListFilesAction::ListFiles(const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ListFilesAction = NewObject<UListFilesAction>();
    ListFilesAction->Auth = Auth;
    ListFilesAction->URLOverride = URLOverride;
    return ListFilesAction;
}

void UListFilesAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFilesCompleted().AddUObject(this, &ThisClass::OnListFilesCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->ListFiles(Auth);
}

void UListFilesAction::OnListFilesCompleted(const FListFilesResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UListFilesAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UListFilesAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Files = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

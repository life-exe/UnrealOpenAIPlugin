// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Files/ListFilesAction.h"
#include "Provider/OpenAIProvider.h"

UListFilesAction* UListFilesAction::ListFiles(const FOpenAIAuth& Auth)
{
    auto* ListFilesAction = NewObject<UListFilesAction>();
    ListFilesAction->Auth = Auth;
    return ListFilesAction;
}

void UListFilesAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnListFilesCompleted().AddUObject(this, &ThisClass::OnListFilesCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->ListFiles(Auth);
}

void UListFilesAction::OnListFilesCompleted(const FListFilesResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UListFilesAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

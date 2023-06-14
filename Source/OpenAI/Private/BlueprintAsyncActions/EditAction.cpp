// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/EditAction.h"
#include "Provider/OpenAIProvider.h"

UEditAction* UEditAction::CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth)
{
    auto* EditAction = NewObject<UEditAction>();
    EditAction->Edit = Edit;
    EditAction->Auth = Auth;
    return EditAction;
}

void UEditAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateEditCompleted().AddUObject(this, &ThisClass::OnCreateEditCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateEdit(Edit, Auth);
}

void UEditAction::OnCreateEditCompleted(const FEditResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UEditAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

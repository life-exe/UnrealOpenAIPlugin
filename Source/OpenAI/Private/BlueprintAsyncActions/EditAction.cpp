// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/EditAction.h"
#include "Provider/OpenAIProvider.h"

UDEPRECATED_EditAction* UDEPRECATED_EditAction::CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth)
{
    auto* EditAction = NewObject<UDEPRECATED_EditAction>();
    EditAction->Edit = Edit;
    EditAction->Auth = Auth;
    return EditAction;
}

void UDEPRECATED_EditAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateEditCompleted().AddUObject(this, &ThisClass::OnCreateEditCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    Provider->CreateEdit(Edit, Auth);
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UDEPRECATED_EditAction::OnCreateEditCompleted(const FEditResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDEPRECATED_EditAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

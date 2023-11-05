// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/EditAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UDEPRECATED_EditAction* UDEPRECATED_EditAction::CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* EditAction = NewObject<UDEPRECATED_EditAction>();
    EditAction->Edit = Edit;
    EditAction->Auth = Auth;
    EditAction->URLOverride = URLOverride;
    return EditAction;
}

void UDEPRECATED_EditAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateEditCompleted().AddUObject(this, &ThisClass::OnCreateEditCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    TryToOverrideURL(Provider);
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

void UDEPRECATED_EditAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Edits = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/CancelFineTuneAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UDEPRECATED_CancelFineTuneAction* UDEPRECATED_CancelFineTuneAction::CancelFineTune(
    const FString& FineTuneID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompletionAction = NewObject<UDEPRECATED_CancelFineTuneAction>();
    CompletionAction->FineTuneID = FineTuneID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UDEPRECATED_CancelFineTuneAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCancelFineTuneCompleted().AddUObject(this, &ThisClass::OnCancelFineTuneCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    TryToOverrideURL(Provider);
    Provider->CancelFineTune(FineTuneID, Auth);
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UDEPRECATED_CancelFineTuneAction::OnCancelFineTuneCompleted(const FFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDEPRECATED_CancelFineTuneAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

void UDEPRECATED_CancelFineTuneAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.FineTunes = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/RetrieveFineTuneAction.h"
#include "Provider/OpenAIProvider.h"

UDEPRECATED_RetrieveFineTuneAction* UDEPRECATED_RetrieveFineTuneAction::RetrieveFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UDEPRECATED_RetrieveFineTuneAction>();
    CompletionAction->FineTuneID = FineTuneID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UDEPRECATED_RetrieveFineTuneAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnRetrieveFineTuneCompleted().AddUObject(this, &ThisClass::OnRetrieveFineTuneCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    Provider->RetrieveFineTune(FineTuneID, Auth);
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UDEPRECATED_RetrieveFineTuneAction::OnRetrieveFineTuneCompleted(const FFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDEPRECATED_RetrieveFineTuneAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

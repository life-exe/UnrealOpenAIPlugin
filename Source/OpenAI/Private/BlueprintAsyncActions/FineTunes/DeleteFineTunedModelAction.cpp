// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/FineTunes/DeleteFineTunedModelAction.h"
#include "Provider/OpenAIProvider.h"

UDeleteFineTunedModelAction* UDeleteFineTunedModelAction::DeleteFineTuneModel(const FString& ModelID, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UDeleteFineTunedModelAction>();
    CompletionAction->ModelID = ModelID;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UDeleteFineTunedModelAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnDeleteFineTunedModelCompleted().AddUObject(this, &ThisClass::OnDeleteFineTunedModelCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->DeleteFineTunedModel(ModelID, Auth);
}

void UDeleteFineTunedModelAction::OnDeleteFineTunedModelCompleted(const FDeleteFineTuneResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UDeleteFineTunedModelAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

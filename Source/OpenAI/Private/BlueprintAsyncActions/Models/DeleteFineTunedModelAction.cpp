// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Models/DeleteFineTunedModelAction.h"

UDeleteFineTunedModelAction* UDeleteFineTunedModelAction::DeleteFineTuneModel(
    const FString& ModelID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* DeleteFineTunedModelAction = NewObject<UDeleteFineTunedModelAction>();
    DeleteFineTunedModelAction->ModelID = ModelID;
    DeleteFineTunedModelAction->Auth = Auth;
    DeleteFineTunedModelAction->URLOverride = URLOverride;
    return DeleteFineTunedModelAction;
}

void UDeleteFineTunedModelAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnDeleteFineTunedModelCompleted().AddUObject(this, &ThisClass::OnDeleteFineTunedModelCompleted);
    Provider->DeleteFineTunedModel(ModelID, Auth);
}

void UDeleteFineTunedModelAction::OnDeleteFineTunedModelCompleted(
    const FDeleteFineTunedModelResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UDeleteFineTunedModelAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UDeleteFineTunedModelAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Models = URL;
}

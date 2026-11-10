// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/DeleteVoiceAction.h"

UDeleteVoiceAction* UDeleteVoiceAction::DeleteVoice(const FString& VoiceID, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* DeleteVoiceAction = NewObject<UDeleteVoiceAction>();
    DeleteVoiceAction->VoiceID = VoiceID;
    DeleteVoiceAction->Auth = Auth;
    DeleteVoiceAction->URLOverride = URLOverride;
    return DeleteVoiceAction;
}

void UDeleteVoiceAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnDeleteVoiceCompleted().AddUObject(this, &ThisClass::OnDeleteVoiceCompleted);
    Provider->DeleteVoice(VoiceID, Auth);
}

void UDeleteVoiceAction::OnDeleteVoiceCompleted(const FVoiceDeletedResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UDeleteVoiceAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UDeleteVoiceAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.AudioVoices = URL;
}

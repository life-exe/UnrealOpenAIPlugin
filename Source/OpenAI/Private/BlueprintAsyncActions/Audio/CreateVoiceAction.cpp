// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/CreateVoiceAction.h"

UCreateVoiceAction* UCreateVoiceAction::CreateVoice(
    const FCreateVoice& CreateVoice, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<UCreateVoiceAction>();
    Action->CreateVoiceParams = CreateVoice;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void UCreateVoiceAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateVoiceCompleted().AddUObject(this, &ThisClass::OnCreateVoiceCompleted);
    Provider->CreateVoice(CreateVoiceParams, Auth);
}

void UCreateVoiceAction::OnCreateVoiceCompleted(
    const FCreateVoiceResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCreateVoiceAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCreateVoiceAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.AudioVoices = URL;
}

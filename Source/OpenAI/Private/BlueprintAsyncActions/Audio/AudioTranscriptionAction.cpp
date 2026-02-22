// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranscriptionAction.h"

UAudioTranscriptionAction* UAudioTranscriptionAction::CreateAudioTranscription(
    const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* AudioTranscriptionAction = NewObject<UAudioTranscriptionAction>();
    AudioTranscriptionAction->AudioTranscription = AudioTranscription;
    AudioTranscriptionAction->Auth = Auth;
    AudioTranscriptionAction->URLOverride = URLOverride;
    return AudioTranscriptionAction;
}

void UAudioTranscriptionAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateAudioTranscriptionCompleted().AddUObject(this, &ThisClass::OnCreateAudioTranscriptionCompleted);
    Provider->CreateAudioTranscription(AudioTranscription, Auth);
}

void UAudioTranscriptionAction::OnCreateAudioTranscriptionCompleted(
    const FAudioTranscriptionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UAudioTranscriptionAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UAudioTranscriptionAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.AudioTranscriptions = URL;
}

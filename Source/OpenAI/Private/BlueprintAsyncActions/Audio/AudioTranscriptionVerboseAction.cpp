// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranscriptionVerboseAction.h"

UAudioTranscriptionVerboseAction* UAudioTranscriptionVerboseAction::CreateAudioTranscriptionVerbose(
    const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* AudioTranscriptionAction = NewObject<UAudioTranscriptionVerboseAction>();
    AudioTranscriptionAction->AudioTranscription = AudioTranscription;
    AudioTranscriptionAction->Auth = Auth;
    AudioTranscriptionAction->URLOverride = URLOverride;
    return AudioTranscriptionAction;
}

void UAudioTranscriptionVerboseAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateAudioTranscriptionVerboseCompleted().AddUObject(this, &ThisClass::OnCreateAudioTranscriptionVerboseCompleted);
    Provider->CreateAudioTranscription(AudioTranscription, Auth);
}

void UAudioTranscriptionVerboseAction::OnCreateAudioTranscriptionVerboseCompleted(
    const FAudioTranscriptionVerboseResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UAudioTranscriptionVerboseAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UAudioTranscriptionVerboseAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.AudioTranscriptions = URL;
}

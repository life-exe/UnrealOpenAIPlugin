// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranscriptionAction.h"
#include "Provider/OpenAIProvider.h"

UAudioTranscriptionAction* UAudioTranscriptionAction::CreateAudioTranscription(
    const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth)
{
    auto* AudioTranscriptionAction = NewObject<UAudioTranscriptionAction>();
    AudioTranscriptionAction->AudioTranscription = AudioTranscription;
    AudioTranscriptionAction->Auth = Auth;
    return AudioTranscriptionAction;
}

void UAudioTranscriptionAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateAudioTranscriptionCompleted().AddUObject(this, &ThisClass::OnCreateAudioTranscriptionCompleted);
    Provider->CreateAudioTranscription(AudioTranscription, Auth);
}

void UAudioTranscriptionAction::OnCreateAudioTranscriptionCompleted(const FAudioTranscriptionResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UAudioTranscriptionAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

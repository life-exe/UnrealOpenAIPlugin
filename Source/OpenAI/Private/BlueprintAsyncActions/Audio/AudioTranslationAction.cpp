// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranslationAction.h"
#include "Provider/OpenAIProvider.h"

UAudioTranslationAction* UAudioTranslationAction::CreateAudioTranslation(const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth)
{
    auto* AudioTranslationAction = NewObject<UAudioTranslationAction>();
    AudioTranslationAction->AudioTranslation = AudioTranslation;
    AudioTranslationAction->Auth = Auth;
    return AudioTranslationAction;
}

void UAudioTranslationAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateAudioTranslationCompleted().AddUObject(this, &ThisClass::OnAudioTranslationCompleted);
    Provider->CreateAudioTranslation(AudioTranslation, Auth);
}

void UAudioTranslationAction::OnAudioTranslationCompleted(const FAudioTranslationResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UAudioTranslationAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

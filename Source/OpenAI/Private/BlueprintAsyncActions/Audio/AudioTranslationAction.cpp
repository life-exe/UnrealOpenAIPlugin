// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranslationAction.h"

UAudioTranslationAction* UAudioTranslationAction::CreateAudioTranslation(
    const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* AudioTranslationAction = NewObject<UAudioTranslationAction>();
    AudioTranslationAction->AudioTranslation = AudioTranslation;
    AudioTranslationAction->Auth = Auth;
    AudioTranslationAction->URLOverride = URLOverride;
    return AudioTranslationAction;
}

void UAudioTranslationAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateAudioTranslationCompleted().AddUObject(this, &ThisClass::OnCreateAudioTranslationCompleted);
    Provider->CreateAudioTranslation(AudioTranslation, Auth);
}

void UAudioTranslationAction::OnCreateAudioTranslationCompleted(
    const FAudioTranslationResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UAudioTranslationAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UAudioTranslationAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.AudioTranslations = URL;
}

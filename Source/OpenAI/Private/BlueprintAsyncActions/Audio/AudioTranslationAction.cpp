// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranslationAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UAudioTranslationAction* UAudioTranslationAction::CreateAudioTranslation(
    const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth, const FString& URLOverride)
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
    TryToOverrideURL(Provider);
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

void UAudioTranslationAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranslations = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranscriptionAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateAudioTranscriptionCompleted().AddUObject(this, &ThisClass::OnCreateAudioTranscriptionCompleted);
    TryToOverrideURL();
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

void UAudioTranscriptionAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranscriptions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

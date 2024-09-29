// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/AudioTranscriptionVerboseAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateAudioTranscriptionVerboseCompleted().AddUObject(this, &ThisClass::OnCreateAudioTranscriptionVerboseCompleted);
    TryToOverrideURL();
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

void UAudioTranscriptionVerboseAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranscriptions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

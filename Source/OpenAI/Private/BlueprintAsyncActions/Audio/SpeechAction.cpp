// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/SpeechAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

DEFINE_LOG_CATEGORY_STATIC(LogSpeechAction, All, All);

USpeechAction* USpeechAction::CreateSpeech(
    const FSpeech& Speech, const FOpenAIAuth& Auth, const FString& URLOverride, const FSpeechSettings& Settings)
{
    auto* AudioTranslationAction = NewObject<USpeechAction>();
    AudioTranslationAction->Speech = Speech;
    AudioTranslationAction->Auth = Auth;
    AudioTranslationAction->Settings = Settings;
    return AudioTranslationAction;
}

void USpeechAction::Activate()
{
    Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateSpeechCompleted().AddUObject(this, &ThisClass::OnCreateSpeechCompleted);
    TryToOverrideURL();
    Provider->CreateSpeech(Speech, Auth);
}

void USpeechAction::OnCreateSpeechCompleted(const FSpeechResponse& Response)
{
    FString FilePath{};
    if (Settings.SaveToFile && !Settings.AbsolutePath.IsEmpty() && !Settings.BaseName.IsEmpty())
    {
        FilePath = Settings.AbsolutePath.Append("/").Append(Settings.BaseName);
        if (Settings.AppendDate)
        {
            const FString Date = FDateTime::Now().ToString();
            FilePath.Append("_").Append(Date);
        }

        const FString Extension = Speech.Response_Format;
        FilePath.Append(".").Append(Extension);

        if (FFileHelper::SaveArrayToFile(Response.Bytes, *FilePath))
        {
            UE_LOG(LogSpeechAction, Display, TEXT("File was successfully saved to: %s"), *FilePath);
        }
    }
    OnCompleted.Broadcast(FSpeechPayload{Response, FilePath}, {});
}

void USpeechAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

void USpeechAction::TryToOverrideURL()
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.AudioTranslations = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

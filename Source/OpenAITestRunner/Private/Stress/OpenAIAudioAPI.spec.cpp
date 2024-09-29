// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/AudioTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIProviderAudio, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderAudio)

using namespace OpenAI::Tests;

void FOpenAIProviderAudio::Define()
{
    Describe("ActualAPI",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));
                    OpenAIProvider = NewObject<UOpenAIProvider>();
                    OpenAIProvider->SetLogEnabled(true);
                    OpenAIProvider->OnRequestError().AddLambda(
                        [&](const FString& URL, const FString& Content)
                        {
                            RequestCompleted = true;
                            AddError(FString::Format(TEXT("URL: {0} Content: {1}"), {URL, Content}));
                        });
                    RequestCompleted = false;
                });

            It("Audio.CreateTranscriptionRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateAudioTranscriptionCompleted().AddLambda(
                        [&](const FAudioTranscriptionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(TestUtils::RemovePunctuation(Response.Text.ToLower()).Equals("hello whats up"));
                            RequestCompleted = true;
                        });

                    FAudioTranscription AudioTranscription;
                    AudioTranscription.Language = "en";
                    AudioTranscription.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModel::Whisper_1);
                    AudioTranscription.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
                    AudioTranscription.Temperature = 0.0f;
                    AudioTranscription.File = TestUtils::FileFullPath("hello.mp3");

                    OpenAIProvider->CreateAudioTranscription(AudioTranscription, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Audio.CreateTranscriptionVerboseRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateAudioTranscriptionVerboseCompleted().AddLambda(
                        [&](const FAudioTranscriptionVerboseResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(TestUtils::RemovePunctuation(Response.Text.ToLower()).Equals("hello whats up"));
                            TestTrueExpr(Response.Segments.Num() > 0);
                            TestTrueExpr(Response.Segments[0].Compression_Ratio > 0.1);
                            RequestCompleted = true;
                        });

                    FAudioTranscription AudioTranscriptionVerbose;
                    AudioTranscriptionVerbose.Language = "en";
                    AudioTranscriptionVerbose.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModel::Whisper_1);
                    AudioTranscriptionVerbose.Response_Format =
                        UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::Verbose_JSON);
                    AudioTranscriptionVerbose.Temperature = 0.0f;
                    AudioTranscriptionVerbose.File = TestUtils::FileFullPath("hello.mp3");

                    OpenAIProvider->CreateAudioTranscription(AudioTranscriptionVerbose, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Audio.CreateTranslationRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateAudioTranslationCompleted().AddLambda(
                        [&](const FAudioTranslationResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(TestUtils::RemovePunctuation(Response.Text.ToLower()).Equals("hi how are you"));
                            RequestCompleted = true;
                        });

                    FAudioTranslation AudioTranslation;
                    AudioTranslation.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModel::Whisper_1);
                    AudioTranslation.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
                    AudioTranslation.Temperature = 0.0f;
                    AudioTranslation.File = TestUtils::FileFullPath("bonjour.mp3");

                    OpenAIProvider->CreateAudioTranslation(AudioTranslation, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Audio.CreateSpeechRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Format = UOpenAIFuncLib::OpenAITTSAudioFormatToString(ETTSAudioFormat::MP3);

                    OpenAIProvider->OnCreateAudioTranscriptionCompleted().AddLambda(
                        [&](const FAudioTranscriptionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(TestUtils::RemovePunctuation(Response.Text.ToLower()).Equals("hi how are you"));
                            RequestCompleted = true;
                        });

                    OpenAIProvider->OnCreateSpeechCompleted().AddLambda([&, Format](const FSpeechResponse& Response,
                                                                            const FOpenAIResponseMetadata& ResponseMetadata) {  //
                        const FString Date = FDateTime::Now().ToString();
                        const FString FileName = FString("speech_").Append(Date).Append(".").Append(Format);
                        const FString FilePath = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
                            TEXT("OpenAI"), TEXT("Saved"), FileName);

                        if (!FFileHelper::SaveArrayToFile(Response.Bytes, *FilePath))
                        {
                            AddError(FString::Format(TEXT("File wasn't successfully saved to: {0}"), {FilePath}));
                            RequestCompleted = true;
                            return;
                        }

                        FAudioTranscription AudioTranscription;
                        AudioTranscription.Language = "en";
                        AudioTranscription.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModel::Whisper_1);
                        AudioTranscription.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
                        AudioTranscription.Temperature = 0.0f;
                        AudioTranscription.File = FilePath;
                        OpenAIProvider->CreateAudioTranscription(AudioTranscription, Auth);
                    });

                    FSpeech Speech;
                    Speech.Input = "hi how are you";
                    Speech.Model = UOpenAIFuncLib::OpenAITTSModelToString(ETTSModel::TTS_1);
                    Speech.Response_Format = Format;
                    Speech.Speed = 1.0f;

                    OpenAIProvider->CreateSpeech(Speech, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

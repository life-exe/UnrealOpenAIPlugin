// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "API/API.h"

DEFINE_SPEC(FAPIProvider, "OpenAI",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FAPIProvider::Define()
{
    Describe("API.OpenAI",
        [this]()
        {
            It("V1_URLs_ShouldBeCorrect",
                [this]()
                {
                    const auto API = MakeShared<OpenAI::V1::OpenAIAPI>();
                    const auto BaseURL = API->BaseURL();
                    TestTrueExpr(BaseURL.Equals("https://api.openai.com"));

                    TestTrueExpr(API->Models().Equals(BaseURL + "/v1/models"));
                    TestTrueExpr(API->Completion().Equals(BaseURL + "/v1/completions"));
                    TestTrueExpr(API->ChatCompletion().Equals(BaseURL + "/v1/chat/completions"));
                    TestTrueExpr(API->ImageGenerations().Equals(BaseURL + "/v1/images/generations"));
                    TestTrueExpr(API->ImageEdits().Equals(BaseURL + "/v1/images/edits"));
                    TestTrueExpr(API->ImageVariations().Equals(BaseURL + "/v1/images/variations"));
                    TestTrueExpr(API->Embeddings().Equals(BaseURL + "/v1/embeddings"));
                    TestTrueExpr(API->Speech().Equals(BaseURL + "/v1/audio/speech"));
                    TestTrueExpr(API->AudioTranscriptions().Equals(BaseURL + "/v1/audio/transcriptions"));
                    TestTrueExpr(API->AudioTranslations().Equals(BaseURL + "/v1/audio/translations"));
                    TestTrueExpr(API->Files().Equals(BaseURL + "/v1/files"));
                    TestTrueExpr(API->FineTuningJobs().Equals(BaseURL + "/v1/fine_tuning/jobs"));
                    TestTrueExpr(API->Moderations().Equals(BaseURL + "/v1/moderations"));
                });
        });
}

#endif

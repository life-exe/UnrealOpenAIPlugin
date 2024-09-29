// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/ModerationTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIProviderModeration, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderModeration)

using namespace OpenAI::Tests;

void FOpenAIProviderModeration::Define()
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

            It("Moderation.RequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateModerationsCompleted().AddLambda(
                        [&](const FModerationsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(!Response.Model.IsEmpty());
                            TestTrueExpr(Response.Results.Num() == 2);

                            TestTrueExpr(!Response.Results[0].Flagged);
                            TestTrueExpr(Response.Results[1].Flagged);
                            RequestCompleted = true;
                        });

                    FModerations Moderations;
                    Moderations.Input = {"I love you.", "I despise you fiercely."};
                    Moderations.Model = UOpenAIFuncLib::OpenAIModerationModelToString(EModerationsModelEnum::Text_Moderation_Latest);

                    OpenAIProvider->CreateModerations(Moderations, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

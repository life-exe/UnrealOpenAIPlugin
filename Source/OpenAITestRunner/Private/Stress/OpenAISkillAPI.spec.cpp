// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/SkillTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAISkill, "OpenAI.Provider",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
FString SkillId;
END_DEFINE_SPEC(FOpenAISkill)

using namespace OpenAI::Tests;

void FOpenAISkill::Define()
{
    Describe("ActualAPI",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append(TEXT("OpenAIAuth.ini")));
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

            It("Skills.ListSkillsShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListSkillsCompleted().AddLambda(
                        [&](const FListSkillsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals(TEXT("list")));
                            RequestCompleted = true;
                        });

                    OpenAIProvider->ListSkills({}, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Skills.RetrieveAndListVersionsShouldWorkIfSkillsExist",
                [this]()
                {
                    OpenAIProvider->OnListSkillsCompleted().AddLambda(
                        [&](const FListSkillsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            if (Response.Data.Num() > 0)
                            {
                                SkillId = Response.Data[0].Id;
                                OpenAIProvider->RetrieveSkill(SkillId, Auth);
                            }
                            else
                            {
                                AddInfo(TEXT("No skills found to test Retrieve"));
                                RequestCompleted = true;
                            }
                        });

                    OpenAIProvider->OnRetrieveSkillCompleted().AddLambda(
                        [&](const FSkillResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Id.Equals(SkillId));
                            OpenAIProvider->ListSkillVersions(SkillId, {}, Auth);
                        });

                    OpenAIProvider->OnListSkillVersionsCompleted().AddLambda(
                        [&](const FListSkillVersionsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals(TEXT("list")));
                            RequestCompleted = true;
                        });

                    OpenAIProvider->ListSkills({}, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

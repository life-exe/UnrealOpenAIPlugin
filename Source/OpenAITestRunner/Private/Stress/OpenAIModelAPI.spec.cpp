// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIProviderModel, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderModel)

using namespace OpenAI::Tests;

void FOpenAIProviderModel::Define()
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

            It("Models.ListModelsRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListModelsCompleted().AddLambda(
                        [&](const FListModelsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(!Response.Object.IsEmpty());
                            TestTrueExpr(Response.Data.Num() != 0);

                            TSet<FString> ModelNames;
                            for (const auto& Model : Response.Data)
                            {
                                // My models will also be returned to the main list, so I have to ignore them
                                if (!Model.ID.Contains("lifeexe"))
                                {
                                    ModelNames.Add(Model.ID);
                                }
                                TestTrueExpr(Model.Created > 0);
                                TestTrueExpr(!Model.ID.IsEmpty());
                                TestTrueExpr(!Model.Object.IsEmpty());
                                TestTrueExpr(!Model.Owned_By.IsEmpty());
                            }

                            // Determine the difference between plug-in and remote OpenAI model names
                            TSet<FString> PluginModelNames;
                            const UEnum* Enum = StaticEnum<EAllModelEnum>();
                            for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
                            {
                                const FString OpenAIModelName = TestUtils::PluginEnumToOpenAIModelName(static_cast<EAllModelEnum>(i));
                                PluginModelNames.Add(OpenAIModelName);
                            }

                            const TSet<FString> Intersection = ModelNames.Intersect(PluginModelNames);
                            ModelNames = ModelNames.Difference(Intersection);
                            PluginModelNames = PluginModelNames.Difference(Intersection);

                            for (const auto& Name : ModelNames)
                            {
                                const FString PluginEnumName = TestUtils::OpenAIModelNameToPluginEnum(Name);
                                AddWarning(FString::Format(TEXT("Remote OpenAI model name that doesn't exist in plugin EAllModelEnum: {0}, "
                                                                "consider to add it as {1}"),
                                    {Name, PluginEnumName}));
                            }

                            for (const auto& Name : PluginModelNames)
                            {
                                AddError(FString::Format(TEXT("Plugin model name that doesn't exist in remote OpenAI: {0}"), {Name}));
                            }

                            RequestCompleted = true;
                        });
                    OpenAIProvider->ListModels(Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Models.RetrieveModelRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnRetrieveModelCompleted().AddLambda(
                        [&](const FRetrieveModelResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(!Response.Object.IsEmpty());
                            TestTrueExpr(!Response.Owned_By.IsEmpty());
                            RequestCompleted = true;
                        });
                    OpenAIProvider->RetrieveModel("gpt-4", Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

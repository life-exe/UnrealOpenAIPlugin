// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "OpenAIProviderFake.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "Provider/CommonTypes.h"
#include "Funclib/ModelTypes.h"
#include "FuncLib/OpenAIFuncLib.h"

BEGIN_DEFINE_SPEC(FOpenAIProviderActual, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderActual)

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForRequestCompleted, bool&, RequestCompleted);
bool FWaitForRequestCompleted::Update()
{
    return RequestCompleted;
}

void FOpenAIProviderActual::Define()
{
    Describe("Actual",
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

            It("ListModelsRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListModelsCompleted().AddLambda([&](const FListModelsResponse& Response)  //
                        {                                                                                       //
                            RequestCompleted = true;

                            TestTrueExpr(!Response.Object.IsEmpty());
                            TestTrueExpr(Response.Data.Num() != 0);

                            TSet<FString> ModelNames;
                            for (const auto& Model : Response.Data)
                            {
                                if (!Model.ID.Contains("lifeexe"))  // my models that also returns in the main list
                                {
                                    ModelNames.Add(Model.ID);
                                }
                                TestTrueExpr(Model.Created > 0);
                                TestTrueExpr(!Model.ID.IsEmpty());
                                TestTrueExpr(!Model.Object.IsEmpty());
                                TestTrueExpr(!Model.Owned_By.IsEmpty());
                                TestTrueExpr(!Model.Root.IsEmpty());
                                for (const auto& OnePermission : Model.Permission)
                                {
                                    TestTrueExpr(!OnePermission.ID.IsEmpty());
                                    TestTrueExpr(!OnePermission.Object.IsEmpty());
                                    TestTrueExpr(!OnePermission.Organization.IsEmpty());
                                    TestTrueExpr(OnePermission.Created > 0);
                                }
                            }

                            // determine difference between Plugin and OpenAI model names
                            TSet<FString> PluginModelNames;
                            const UEnum* Enum = StaticEnum<EAllModelEnum>();
                            for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
                            {
                                FString EnumName = UEnum::GetValueAsString(static_cast<EAllModelEnum>(i));
                                EnumName = EnumName.ToLower().Replace(TEXT("_"), TEXT("-"));
                                EnumName = EnumName.Replace(
                                    TEXT("3-5"), TEXT("3.5"));  // *gpt-3.5-* in plugin looks as *gpt_3_5* because of '.' symbol
                                FString LeftStr, RightStr;
                                EnumName.Split("::", &LeftStr, &RightStr);  // remove enum name: eallmodelenum::ada
                                PluginModelNames.Add(RightStr);
                            }

                            const TSet<FString> Intersection = ModelNames.Intersect(PluginModelNames);
                            ModelNames = ModelNames.Difference(Intersection);
                            PluginModelNames = PluginModelNames.Difference(Intersection);

                            for (const auto& Name : ModelNames)
                            {
                                AddWarning(FString::Format(TEXT("Model name that is not in plugin EAllModelEnum: {0}"), {Name}));
                            }

                            for (const auto& Name : PluginModelNames)
                            {
                                AddWarning(FString::Format(TEXT("Model name that is not in OpenAI: {0}"), {Name}));
                            }
                        });
                    OpenAIProvider->ListModels(Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("RetrieveModelRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnRetrieveModelCompleted().AddLambda([&](const FRetrieveModelResponse& Response)  //
                        {                                                                                             //
                            RequestCompleted = true;
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(!Response.Object.IsEmpty());
                            TestTrueExpr(!Response.Owned_By.IsEmpty());
                            TestTrueExpr(!Response.Root.IsEmpty());
                            for (const auto& Permission : Response.Permission)
                            {
                                TestTrueExpr(!Permission.ID.IsEmpty());
                                TestTrueExpr(!Permission.Object.IsEmpty());
                                TestTrueExpr(!Permission.Organization.IsEmpty());
                                TestTrueExpr(Permission.Created > 0);
                            }
                        });
                    OpenAIProvider->RetrieveModel("gpt-4", Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

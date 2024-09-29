// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/Legacy/CompletionTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAICompletionAPI, All, All);

BEGIN_DEFINE_SPEC(FOpenAIProviderCompletion, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderCompletion)

using namespace OpenAI::Tests;

void FOpenAIProviderCompletion::Define()
{
    Describe("ActualAPI.Legacy",
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

            It("Completions.CreateCompletionRequestShouldResponseCorrectly.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct);

                    OpenAIProvider->OnCreateCompletionCompleted().AddLambda(
                        [&, Model](const FCompletionResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestUtils::TestStreamResponse<FCompletionResponse>(this, Response, Model, "text_completion");
                            RequestCompleted = true;
                        });

                    FCompletion Completion;
                    Completion.Model = Model;
                    Completion.Prompt = "What is Unreal Engine?";
                    Completion.Max_Tokens = 100;
                    Completion.Stream = false;

                    OpenAIProvider->CreateCompletion(Completion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Completions.CreateCompletionRequestShouldResponseCorrectly.Streaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct);

                    OpenAIProvider->OnCreateCompletionStreamCompleted().AddLambda(
                        [&, Model](const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& Metadata)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestUtils::TestStreamResponse<FCompletionStreamResponse>(this, Response, Model, "text_completion");
                            }
                            RequestCompleted = true;
                        });

                    OpenAIProvider->OnCreateCompletionStreamProgresses().AddLambda(
                        [&, Model](const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& Metadata)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestUtils::TestStreamResponse<FCompletionStreamResponse>(this, Response, Model, "text_completion");
                            }
                        });

                    FCompletion Completion;
                    Completion.Model = Model;
                    Completion.Prompt = "What is Unreal Engine?";
                    Completion.Max_Tokens = 100;
                    Completion.Stream = true;

                    OpenAIProvider->CreateCompletion(Completion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

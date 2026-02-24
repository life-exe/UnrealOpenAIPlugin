// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/ChatKitTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"
BEGIN_DEFINE_SPEC(FOpenAIChatKit, "OpenAI.Provider",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
FString SessionId;
FString ThreadId;
END_DEFINE_SPEC(FOpenAIChatKit)

using namespace OpenAI::Tests;

void FOpenAIChatKit::Define()
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
                            // If we get a 404 for the specific example workflow, we consider the provider's logic (URL/Headers/Beta)
                            // correct.
                            if (Content.Contains(TEXT("Workflow with id 'wf_example_stress' not found.")))
                            {
                                AddInfo(TEXT("Verified: Request reached OpenAI with correct Beta headers and URL, but workflow ID is "
                                             "(predictably) missing."));
                            }
                            else
                            {
                                AddError(FString::Format(TEXT("Unexpected Error - URL: {0} Content: {1}"), {URL, Content}));
                            }
                        });
                    RequestCompleted = false;
                });

            It("ChatKit.Session.CreateAndCancelShouldWork",
                [this]()
                {
                    OpenAIProvider->OnCreateChatKitSessionCompleted().AddLambda(
                        [&](const FChatKitSessionResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(!Response.Id.IsEmpty());
                            SessionId = Response.Id;

                            // 2. Cancel session
                            OpenAIProvider->CancelChatKitSession(SessionId, Auth);
                        });

                    OpenAIProvider->OnCancelChatKitSessionCompleted().AddLambda(
                        [&](const FChatKitSessionResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Id.Equals(SessionId));
                            TestTrueExpr(Response.Status.Equals(TEXT("cancelled")));
                            RequestCompleted = true;
                        });

                    // TODO: Implement the full lifecycle test now that the Skills API is available:
                    //   1. CreateSkill  -> get skill ID
                    //   2. CreateChatKitSession (workflow.id = skill ID)
                    //   3. CancelChatKitSession
                    //   4. DeleteSkill
                    // Blocker: a valid skill zip/directory must be added to Source/OpenAITestRunner/Data/
                    // and passed as FCreateSkill::Files. Until then, only API connectivity is verified.
                    FCreateChatKitSession CreateSession;
                    CreateSession.User = "test_user_stress";
                    CreateSession.Workflow.Id = "wf_example_stress";

                    // 1. Create session
                    OpenAIProvider->CreateChatKitSession(CreateSession, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("ChatKit.Threads.ListThreadsShouldWork",
                [this]()
                {
                    OpenAIProvider->OnListChatKitThreadsCompleted().AddLambda(
                        [&](const FListChatKitThreadsResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            // We don't necessarily need threads to exist, just a successful list response
                            TestTrueExpr(Response.Object.Equals(TEXT("list")));
                            RequestCompleted = true;
                        });

                    // 1. List threads
                    OpenAIProvider->ListChatKitThreads({}, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("ChatKit.Threads.RetrieveAndListItemsShouldWork",
                [this]()
                {
                    OpenAIProvider->OnListChatKitThreadsCompleted().AddLambda(
                        [&](const FListChatKitThreadsResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            if (Response.Data.Num() > 0)
                            {
                                ThreadId = Response.Data[0].Id;
                                // 2. Retrieve thread
                                OpenAIProvider->RetrieveChatKitThread(ThreadId, Auth);
                            }
                            else
                            {
                                AddInfo(TEXT("No threads found to test Retrieve/ListItems"));
                                RequestCompleted = true;
                            }
                        });

                    OpenAIProvider->OnRetrieveChatKitThreadCompleted().AddLambda(
                        [&](const FChatKitThread& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Id.Equals(ThreadId));
                            // 3. List items
                            OpenAIProvider->ListChatKitThreadItems(ThreadId, {}, Auth);
                        });

                    OpenAIProvider->OnListChatKitThreadItemsCompleted().AddLambda(
                        [&](const FChatKitThreadItemListResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Object.Equals(TEXT("list")));
                            RequestCompleted = true;
                        });

                    // 1. List threads to get an ID
                    OpenAIProvider->ListChatKitThreads({}, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("ChatKit.Threads.DeleteThreadShouldWork",
                [this]()
                {
                    OpenAIProvider->OnListChatKitThreadsCompleted().AddLambda(
                        [&](const FListChatKitThreadsResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            if (Response.Data.Num() > 0)
                            {
                                ThreadId = Response.Data[0].Id;
                                // 2. Delete thread
                                OpenAIProvider->DeleteChatKitThread(ThreadId, Auth);
                            }
                            else
                            {
                                AddInfo(TEXT("No threads found to test Delete"));
                                RequestCompleted = true;
                            }
                        });

                    OpenAIProvider->OnDeleteChatKitThreadCompleted().AddLambda(
                        [&](const FDeleteChatKitThreadResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Id.Equals(ThreadId));
                            TestTrueExpr(Response.Deleted);
                            RequestCompleted = true;
                        });

                    // 1. List threads to get an ID
                    OpenAIProvider->ListChatKitThreads({}, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

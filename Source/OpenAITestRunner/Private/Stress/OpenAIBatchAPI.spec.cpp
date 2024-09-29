// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/FileTypes.h"
#include "Provider/Types/BatchTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "TestUtils.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIBatchAPI, All, All);

BEGIN_DEFINE_SPEC(FOpenAIProviderBatch, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderBatch)

using namespace OpenAI::Tests;

namespace
{
const FString BatchFile = "test_file_batch.jsonl";
}  // namespace

void FOpenAIProviderBatch::Define()
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

            It("Batch.CreateRequestShouldResponseCorrectly",
                [this]()
                {
                    // 1. upload file with batch purpose
                    FUploadFile UploadFile;
                    UploadFile.File = TestUtils::FileFullPath(BatchFile);
                    UploadFile.Purpose = UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::Batch);

                    const auto Endpoint = UOpenAIFuncLib::OpenAIBatchEndpointToString(EBatchEndpoint::ChatCompletions);

                    OpenAIProvider->OnUploadFileCompleted().AddLambda(
                        [&, Endpoint](const FUploadFileResponse& File, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIBatchAPI, Display, "File upload request completed, id={0}", File.ID);
                            // 2. Create batch
                            FCreateBatch Batch;
                            Batch.Input_File_Id = File.ID;
                            Batch.Endpoint = Endpoint;
                            Batch.Completion_Window =
                                UOpenAIFuncLib::OpenAIBatchCompletionWindowToString(EBatchCompletionWindow::Window_24h);
                            Batch.Metadata.Add("purpose", "plugin test");
                            Batch.Metadata.Add("user_name", "John Doe");
                            OpenAIProvider->CreateBatch(Batch, Auth);
                        });

                    OpenAIProvider->OnCreateBatchCompleted().AddLambda(
                        [&, Endpoint](const FCreateBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIBatchAPI, Display, "Batch request completed, id={0}", Response.Id);
                            TestTrueExpr(Response.Metadata["purpose"].Equals("plugin test"));
                            TestTrueExpr(Response.Metadata["user_name"].Equals("John Doe"));
                            TestTrueExpr(Response.Object.Equals("batch"));
                            TestTrueExpr(Response.Endpoint.Equals(Endpoint));
                            RequestCompleted = true;
                        });

                    OpenAIProvider->UploadFile(UploadFile, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Batch.RetrieveRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnRetrieveBatchCompleted().AddLambda(
                        [&](const FRetrieveBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIBatchAPI, Display, "RetrieveBatch request completed!");
                            TestTrueExpr(Response.Metadata["purpose"].Equals("plugin test"));
                            TestTrueExpr(Response.Metadata["user_name"].Equals("John Doe"));
                            TestTrueExpr(Response.Object.Equals("batch"));
                            RequestCompleted = true;
                        });
                    const FString PersistentBatchId = "batch_66f59b416b448190b5155325219d2f7f";
                    OpenAIProvider->RetrieveBatch(PersistentBatchId, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Batch.ListRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListBatchCompleted().AddLambda(
                        [&](const FListBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIBatchAPI, Display, "ListBatch request completed!");
                            RequestCompleted = true;
                        });
                    FListBatch ListBatch;
                    ListBatch.Limit.Set(20);
                    OpenAIProvider->ListBatch(ListBatch, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Batch.CancelRequestShouldResponseCorrectly",
                [this]()
                {
                    // 1. upload file with batch purpose
                    FUploadFile UploadFile;
                    UploadFile.File = TestUtils::FileFullPath(BatchFile);
                    UploadFile.Purpose = UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::Batch);

                    const auto Endpoint = UOpenAIFuncLib::OpenAIBatchEndpointToString(EBatchEndpoint::ChatCompletions);

                    OpenAIProvider->OnUploadFileCompleted().AddLambda(
                        [&, Endpoint](const FUploadFileResponse& File, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIBatchAPI, Display, "File upload request completed, id={0}", File.ID);
                            // 2. Create batch
                            FCreateBatch Batch;
                            Batch.Input_File_Id = File.ID;
                            Batch.Endpoint = Endpoint;
                            Batch.Completion_Window =
                                UOpenAIFuncLib::OpenAIBatchCompletionWindowToString(EBatchCompletionWindow::Window_24h);
                            Batch.Metadata.Add("purpose", "plugin test");
                            Batch.Metadata.Add("user_name", "John Doe");
                            OpenAIProvider->CreateBatch(Batch, Auth);
                        });

                    OpenAIProvider->OnCreateBatchCompleted().AddLambda(
                        [&, Endpoint](const FCreateBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIBatchAPI, Display, "Batch request completed, id={0}", Response.Id);
                            TestTrueExpr(Response.Metadata["purpose"].Equals("plugin test"));
                            TestTrueExpr(Response.Metadata["user_name"].Equals("John Doe"));
                            TestTrueExpr(Response.Object.Equals("batch"));
                            TestTrueExpr(Response.Endpoint.Equals(Endpoint));

                            // 3. Cancel batch
                            OpenAIProvider->CancelBatch(Response.Id, Auth);
                        });

                    OpenAIProvider->OnCancelBatchCompleted().AddLambda(
                        [&](const FCancelBatchResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIBatchAPI, Display, "CancelBatch request completed!");
                            RequestCompleted = true;
                        });

                    OpenAIProvider->UploadFile(UploadFile, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

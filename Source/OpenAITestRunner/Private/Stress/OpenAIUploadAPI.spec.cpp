// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/UploadTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIProviderUpload, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)

TObjectPtr<UOpenAIProvider> OpenAIProvider;

FOpenAIAuth Auth;
bool RequestCompleted{false};
FString UploadId{};

const FString Filename{"test_file.jsonl"};
const FString Purpose{UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::FineTune)};
// Console command to get the number of bytes in a file:
// forfiles /p "c:\Projects\UnrealEngine\ProjectX\Plugins\OpenAI\Source\OpenAITestRunner\Data\" /m "test_file.jsonl" /c "cmd /c echo @fsize"
const int64 Bytes{3138};

END_DEFINE_SPEC(FOpenAIProviderUpload)

using namespace OpenAI::Tests;

void FOpenAIProviderUpload::Define()
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

            It("Upload.CreateShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateUploadCompleted().AddLambda(
                        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals("upload"));
                            TestTrueExpr(Response.Filename.Equals(Filename));
                            TestTrueExpr(Response.Purpose.Equals(Purpose));
                            TestTrueExpr(Response.Status.Equals(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Pending)));
                            TestTrueExpr(Response.Bytes == Bytes);

                            RequestCompleted = true;
                        });

                    FCreateUpload CreateUpload;
                    CreateUpload.Purpose = Purpose;
                    CreateUpload.Filename = Filename;
                    CreateUpload.Bytes = Bytes;
                    CreateUpload.Mime_Type = "text/jsonl";

                    OpenAIProvider->CreateUpload(CreateUpload, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Upload.AddPartShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateUploadCompleted().AddLambda(
                        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals("upload"));
                            TestTrueExpr(Response.Filename.Equals(Filename));
                            TestTrueExpr(Response.Status.Equals(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Pending)));
                            TestTrueExpr(Response.Bytes == Bytes);

                            // 2. add upload
                            FAddUploadPart AddUploadPart;
                            AddUploadPart.Data = TestUtils::FileFullPath(Filename);
                            OpenAIProvider->AddUploadPart(Response.Id, AddUploadPart, Auth);
                        });

                    OpenAIProvider->OnAddUploadPartCompleted().AddLambda(
                        [&](const FUploadPartObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals("upload.part"));
                            RequestCompleted = true;
                        });

                    // 1. create upload
                    FCreateUpload CreateUpload;
                    CreateUpload.Purpose = Purpose;
                    CreateUpload.Filename = Filename;
                    CreateUpload.Bytes = Bytes;
                    CreateUpload.Mime_Type = "text/jsonl";

                    OpenAIProvider->CreateUpload(CreateUpload, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Upload.CompleteShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateUploadCompleted().AddLambda(
                        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals("upload"));
                            TestTrueExpr(Response.Filename.Equals(Filename));
                            TestTrueExpr(Response.Status.Equals(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Pending)));
                            TestTrueExpr(Response.Bytes == Bytes);

                            // 2. add upload
                            FAddUploadPart AddUploadPart;
                            AddUploadPart.Data = TestUtils::FileFullPath(Filename);
                            OpenAIProvider->AddUploadPart(Response.Id, AddUploadPart, Auth);
                        });

                    OpenAIProvider->OnAddUploadPartCompleted().AddLambda(
                        [&](const FUploadPartObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals("upload.part"));

                            // 3. complete upload
                            FCompleteUpload CompleteUpload;
                            CompleteUpload.Part_Ids.Add(Response.Id);
                            OpenAIProvider->CompleteUpload(Response.Upload_Id, CompleteUpload, Auth);
                        });

                    OpenAIProvider->OnCompleteUploadCompleted().AddLambda(
                        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Status.Equals(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Completed)));
                            TestTrueExpr(Response.Filename.Equals(Filename));
                            TestTrueExpr(Response.Purpose.Equals(Purpose));
                            TestTrueExpr(Response.Bytes == Bytes);
                            RequestCompleted = true;
                        });

                    // 1. create upload
                    FCreateUpload CreateUpload;
                    CreateUpload.Purpose = Purpose;
                    CreateUpload.Filename = Filename;
                    CreateUpload.Bytes = Bytes;
                    CreateUpload.Mime_Type = "text/jsonl";

                    OpenAIProvider->CreateUpload(CreateUpload, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Upload.CancelShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateUploadCompleted().AddLambda(
                        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals("upload"));
                            TestTrueExpr(Response.Filename.Equals(Filename));
                            TestTrueExpr(Response.Status.Equals(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Pending)));
                            TestTrueExpr(Response.Bytes == Bytes);

                            // 2. add upload
                            FAddUploadPart AddUploadPart;
                            AddUploadPart.Data = TestUtils::FileFullPath(Filename);
                            UploadId = Response.Id;
                            OpenAIProvider->AddUploadPart(UploadId, AddUploadPart, Auth);
                        });

                    OpenAIProvider->OnAddUploadPartCompleted().AddLambda(
                        [&](const FUploadPartObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Object.Equals("upload.part"));
                            // 3. cancel upload
                            OpenAIProvider->CancelUpload(UploadId, Auth);
                        });

                    OpenAIProvider->OnCancelUploadCompleted().AddLambda(
                        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Filename.Equals(Filename));
                            TestTrueExpr(Response.Purpose.Equals(Purpose));
                            TestTrueExpr(Response.Status.Equals(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Cancelled)));
                            TestTrueExpr(Response.Id.Equals(Response.Id));
                            TestTrueExpr(Response.Bytes == Bytes);
                            RequestCompleted = true;
                        });

                    // 1. create upload
                    FCreateUpload CreateUpload;
                    CreateUpload.Purpose = Purpose;
                    CreateUpload.Filename = Filename;
                    CreateUpload.Bytes = Bytes;
                    CreateUpload.Mime_Type = "text/jsonl";

                    OpenAIProvider->CreateUpload(CreateUpload, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

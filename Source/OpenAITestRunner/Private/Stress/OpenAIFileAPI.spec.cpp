// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/FileTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIProviderFile, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderFile)

using namespace OpenAI::Tests;

namespace
{
namespace File
{
const FString FileID = "file-xm3aonDNpFhE4CMnzUdgoKUU";
const int32 FileBytes = 3138;
}  // namespace File
}  // namespace

void FOpenAIProviderFile::Define()
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

            It("File.ListFilesRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListFilesCompleted().AddLambda(
                        [&](const FListFilesResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            for (const auto& File : Response.Data)
                            {
                                TestTrueExpr(File.Object.Equals("file"));
                                TestTrueExpr(!File.ID.IsEmpty());
                                TestTrueExpr(!File.Purpose.IsEmpty());
                                TestTrueExpr(!File.FileName.IsEmpty());
                                TestTrueExpr(File.Bytes > 0);
                                TestTrueExpr(File.Created_At > 0);
                                TestTrueExpr(!File.Status.IsEmpty());
                            }
                            TestTrueExpr(Response.Object.Equals("list"));

                            RequestCompleted = true;
                        });

                    OpenAIProvider->ListFiles(Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("File.UploadAndDeleteFileRequestsShouldResponseCorrectly",
                [this]()
                {
                    const FString FileName = "test_file.jsonl";

                    OpenAIProvider->OnUploadFileCompleted().AddLambda(
                        [&, FileName](const FUploadFileResponse& File, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(File.FileName.Equals(FileName));
                            TestTrueExpr(File.Purpose.Equals("fine-tune"));
                            TestTrueExpr(File.Object.Equals("file"));
                            TestTrueExpr(File.Bytes == File::FileBytes);
                            TestTrueExpr(File.Status.Equals("processed"));
                            TestTrueExpr(!File.ID.IsEmpty());
                            TestTrueExpr(File.Created_At > 0);

                            OpenAIProvider->DeleteFile(File.ID, Auth);
                        });

                    // delete file after upload
                    OpenAIProvider->OnDeleteFileCompleted().AddLambda(
                        [&](const FDeleteFileResponse& File, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(File.Object.Equals("file"));
                            TestTrueExpr(File.Deleted);
                            RequestCompleted = true;
                        });

                    FUploadFile UploadFile;
                    UploadFile.File = TestUtils::FileFullPath(FileName);
                    UploadFile.Purpose = "fine-tune";

                    OpenAIProvider->UploadFile(UploadFile, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("File.RetrieveFileRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString FileName = "test_file.jsonl";
                    const FString _FileID{File::FileID};

                    OpenAIProvider->OnRetrieveFileCompleted().AddLambda(
                        [&, FileName, _FileID](const FRetrieveFileResponse& File, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(File.FileName.Equals(FileName));
                            TestTrueExpr(File.Purpose.Equals("fine-tune"));
                            TestTrueExpr(File.Object.Equals("file"));
                            TestTrueExpr(File.Bytes == File::FileBytes);
                            TestTrueExpr(File.Status.Equals("processed"));
                            TestTrueExpr(File.ID.Equals(_FileID));
                            TestTrueExpr(File.Created_At > 0);

                            RequestCompleted = true;
                        });

                    OpenAIProvider->RetrieveFile(File::FileID, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("File.RetrieveFileContentRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString FilePath = TestUtils::FileFullPath("test_file.jsonl");
                    FString FileContent;
                    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
                    {
                        AddError(FString::Format(TEXT("Can't open the file {0}"), {FilePath}));
                        return;
                    }

                    OpenAIProvider->OnRetrieveFileContentCompleted().AddLambda(
                        [&, FileContent](const FRetrieveFileContentResponse& File, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(FileContent.Equals(File.Content));
                            RequestCompleted = true;
                        });

                    OpenAIProvider->RetrieveFileContent(File::FileID, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

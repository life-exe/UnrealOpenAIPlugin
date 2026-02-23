// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/VectorStoreTypes.h"
#include "Provider/Types/FileTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIVectorStore, "OpenAI.Provider",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
FString VectorStoreId;
FString FileId;
FString BatchId;
END_DEFINE_SPEC(FOpenAIVectorStore)

using namespace OpenAI::Tests;

void FOpenAIVectorStore::Define()
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

            It("VectorStore.CRUD.LifecycleShouldBeCorrect",
                [this]()
                {
                    const FString VSName = TEXT("Test Vector Store ") + FGuid::NewGuid().ToString();

                    // 1. Create Vector Store
                    OpenAIProvider->OnCreateVectorStoreCompleted().AddLambda(
                        [&, VSName](const FVectorStore& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Name.Equals(VSName));
                            VectorStoreId = Response.Id;

                            // 2. Update Vector Store
                            FUpdateVectorStore Update;
                            Update.Name = VSName + TEXT("_Updated");
                            OpenAIProvider->UpdateVectorStore(VectorStoreId, Update, Auth);
                        });

                    // 2. Update Vector Store Callback
                    OpenAIProvider->OnUpdateVectorStoreCompleted().AddLambda(
                        [&, VSName](const FVectorStore& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Name.Equals(VSName + TEXT("_Updated")));

                            // 3. Retrieve Vector Store
                            OpenAIProvider->RetrieveVectorStore(VectorStoreId, Auth);
                        });

                    // 3. Retrieve Vector Store Callback
                    OpenAIProvider->OnRetrieveVectorStoreCompleted().AddLambda(
                        [&](const FVectorStore& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Id.Equals(VectorStoreId));

                            // 4. List Vector Stores
                            OpenAIProvider->ListVectorStores(FVectorStoreQueryParams{}, Auth);
                        });

                    // 4. List Vector Stores Callback
                    OpenAIProvider->OnListVectorStoresCompleted().AddLambda(
                        [&](const FListVectorStoresResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Object.Equals(TEXT("list")));

                            // 5. Delete Vector Store
                            OpenAIProvider->DeleteVectorStore(VectorStoreId, Auth);
                        });

                    // 5. Delete Vector Store Callback
                    OpenAIProvider->OnDeleteVectorStoreCompleted().AddLambda(
                        [&](const FVectorStoreDeletedResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Id.Equals(VectorStoreId));
                            TestTrueExpr(Response.Deleted);
                            RequestCompleted = true;
                        });

                    FCreateVectorStore Create;
                    Create.Name = VSName;
                    OpenAIProvider->CreateVectorStore(Create, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("VectorStore.Files.LifecycleShouldBeCorrect",
                [this]()
                {
                    const FString VSName = TEXT("Test VS Files ") + FGuid::NewGuid().ToString();

                    // 0. Upload File first
                    OpenAIProvider->OnUploadFileCompleted().AddLambda(
                        [&, VSName](const FUploadFileResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            FileId = Response.ID;

                            // 1. Create Vector Store
                            FCreateVectorStore Create;
                            Create.Name = VSName;
                            OpenAIProvider->CreateVectorStore(Create, Auth);
                        });

                    OpenAIProvider->OnCreateVectorStoreCompleted().AddLambda(
                        [&](const FVectorStore& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            VectorStoreId = Response.Id;

                            // 2. Create Vector Store File
                            OpenAIProvider->CreateVectorStoreFile(VectorStoreId, FileId, Auth);
                        });

                    OpenAIProvider->OnCreateVectorStoreFileCompleted().AddLambda(
                        [&](const FVectorStoreFile& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(!Response.Id.IsEmpty());

                            // 3. Retrieve Vector Store File
                            OpenAIProvider->RetrieveVectorStoreFile(VectorStoreId, FileId, Auth);
                        });

                    OpenAIProvider->OnRetrieveVectorStoreFileCompleted().AddLambda(
                        [&](const FVectorStoreFile& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Id.Equals(FileId));

                            // 4. List Vector Store Files
                            OpenAIProvider->ListVectorStoreFiles(VectorStoreId, FVectorStoreFileQueryParams{}, Auth);
                        });

                    OpenAIProvider->OnListVectorStoreFilesCompleted().AddLambda(
                        [&](const FListVectorStoreFilesResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Object.Equals(TEXT("list")));

                            // 5. Delete Vector Store File
                            OpenAIProvider->DeleteVectorStoreFile(VectorStoreId, FileId, Auth);
                        });

                    OpenAIProvider->OnDeleteVectorStoreFileCompleted().AddLambda(
                        [&](const FVectorStoreDeletedResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Id.Equals(FileId));
                            TestTrueExpr(Response.Deleted);

                            // 6. Delete Vector Store
                            OpenAIProvider->DeleteVectorStore(VectorStoreId, Auth);
                        });

                    OpenAIProvider->OnDeleteVectorStoreCompleted().AddLambda(
                        [&](const FVectorStoreDeletedResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            // 7. Delete File
                            OpenAIProvider->DeleteFile(FileId, Auth);
                        });

                    OpenAIProvider->OnDeleteFileCompleted().AddLambda(
                        [&](const FDeleteFileResponse& Response, const FOpenAIResponseMetadata& Meta) { RequestCompleted = true; });

                    FUploadFile Upload;
                    Upload.File = TestUtils::FileFullPath(TEXT("test_file_vs.txt"));
                    Upload.Purpose = TEXT("assistants");
                    OpenAIProvider->UploadFile(Upload, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("VectorStore.FileBatches.LifecycleShouldBeCorrect",
                [this]()
                {
                    const FString VSName = TEXT("Test VS Batches ") + FGuid::NewGuid().ToString();

                    // 0. Upload File first
                    OpenAIProvider->OnUploadFileCompleted().AddLambda(
                        [&, VSName](const FUploadFileResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            FileId = Response.ID;

                            // 1. Create Vector Store
                            FCreateVectorStore Create;
                            Create.Name = VSName;
                            OpenAIProvider->CreateVectorStore(Create, Auth);
                        });

                    OpenAIProvider->OnCreateVectorStoreCompleted().AddLambda(
                        [&](const FVectorStore& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            VectorStoreId = Response.Id;

                            // 2. Create Vector Store File Batch
                            FCreateVectorStoreFileBatch CreateBatch;
                            CreateBatch.File_Ids.Add(FileId);
                            OpenAIProvider->CreateVectorStoreFileBatch(VectorStoreId, CreateBatch, Auth);
                        });

                    OpenAIProvider->OnCreateVectorStoreFileBatchCompleted().AddLambda(
                        [&](const FVectorStoreFileBatch& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(!Response.Id.IsEmpty());
                            BatchId = Response.Id;

                            // 3. Retrieve Vector Store File Batch
                            OpenAIProvider->RetrieveVectorStoreFileBatch(VectorStoreId, BatchId, Auth);
                        });

                    OpenAIProvider->OnRetrieveVectorStoreFileBatchCompleted().AddLambda(
                        [&](const FVectorStoreFileBatch& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(!Response.Id.IsEmpty());

                            // 4. Delete Vector Store
                            OpenAIProvider->DeleteVectorStore(VectorStoreId, Auth);
                        });

                    OpenAIProvider->OnDeleteVectorStoreCompleted().AddLambda(
                        [&](const FVectorStoreDeletedResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            // 5. Delete File
                            OpenAIProvider->DeleteFile(FileId, Auth);
                        });

                    OpenAIProvider->OnDeleteFileCompleted().AddLambda(
                        [&](const FDeleteFileResponse& Response, const FOpenAIResponseMetadata& Meta) { RequestCompleted = true; });

                    FUploadFile Upload;
                    Upload.File = TestUtils::FileFullPath(TEXT("test_file_vs.txt"));
                    Upload.Purpose = TEXT("assistants");
                    OpenAIProvider->UploadFile(Upload, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("VectorStore.Search.RequestShouldResponseCorrectly",
                [this]()
                {
                    const FString VSName = TEXT("Test VS Search ") + FGuid::NewGuid().ToString();

                    // Search requires a vector store, optionally with files.
                    // For now just test the request/response structure.

                    OpenAIProvider->OnCreateVectorStoreCompleted().AddLambda(
                        [&](const FVectorStore& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            VectorStoreId = Response.Id;

                            // 2. Search Vector Store
                            FVectorStoreSearch Search;
                            Search.Query = TEXT("What is this?");
                            OpenAIProvider->SearchVectorStore(VectorStoreId, Search, Auth);
                        });

                    OpenAIProvider->OnSearchVectorStoreCompleted().AddLambda(
                        [&](const FVectorStoreSearchResponse& Response, const FOpenAIResponseMetadata& Meta)
                        {
                            TestTrueExpr(Response.Object.Contains(TEXT("vector_store.search_results")));

                            // 3. Delete Vector Store
                            OpenAIProvider->DeleteVectorStore(VectorStoreId, Auth);
                        });

                    OpenAIProvider->OnDeleteVectorStoreCompleted().AddLambda(
                        [&](const FVectorStoreDeletedResponse& Response, const FOpenAIResponseMetadata& Meta) { RequestCompleted = true; });

                    FCreateVectorStore Create;
                    Create.Name = VSName;
                    OpenAIProvider->CreateVectorStore(Create, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

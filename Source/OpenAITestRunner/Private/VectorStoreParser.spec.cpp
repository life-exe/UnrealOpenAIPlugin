// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/JsonParsers/VectorStoreParser.h"
#include "Provider/Types/VectorStoreTypes.h"
#include "FuncLib/JsonFuncLib.h"

DEFINE_SPEC(FVectorStoreParserTest, "OpenAI.Parser.VectorStore",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FVectorStoreParserTest::Define()
{
    Describe("JSON",
        [this]()
        {
            It("CreateVectorStoreWithAutoChunkingShouldBeCorrect",
                [this]()
                {
                    FCreateVectorStore CreateVS;
                    CreateVS.Name = "Test VS";
                    CreateVS.Chunking_Strategy.Type = EChunkingStrategyType::Auto;
                    CreateVS.Metadata.Add("key", "value");

                    const FString Result = OpenAI::VectorStoreParser::CreateVectorStoreToJsonRepresentation(CreateVS);

                    TSharedPtr<FJsonObject> Json;
                    UJsonFuncLib::StringToJson(Result, Json);

                    TestTrueExpr(Json->GetStringField(TEXT("name")).Equals(TEXT("Test VS")));

                    auto Strategy = Json->GetObjectField(TEXT("chunking_strategy"));
                    TestTrueExpr(Strategy->GetStringField(TEXT("type")).Equals(TEXT("auto")));
                    TestFalseExpr(Strategy->HasField(TEXT("static")));

                    auto Metadata = Json->GetObjectField(TEXT("metadata"));
                    TestTrueExpr(Metadata->GetStringField(TEXT("key")).Equals(TEXT("value")));
                });

            It("CreateVectorStoreWithStaticChunkingShouldBeCorrect",
                [this]()
                {
                    FCreateVectorStore CreateVS;
                    CreateVS.Chunking_Strategy.Type = EChunkingStrategyType::Static;
                    CreateVS.Chunking_Strategy.Static.Max_Chunk_Size_Tokens = 500;
                    CreateVS.Chunking_Strategy.Static.Chunk_Overlap_Tokens = 200;

                    const FString Result = OpenAI::VectorStoreParser::CreateVectorStoreToJsonRepresentation(CreateVS);

                    TSharedPtr<FJsonObject> Json;
                    UJsonFuncLib::StringToJson(Result, Json);

                    auto Strategy = Json->GetObjectField(TEXT("chunking_strategy"));
                    TestTrueExpr(Strategy->GetStringField(TEXT("type")).Equals(TEXT("static")));

                    auto Static = Strategy->GetObjectField(TEXT("static"));
                    TestTrueExpr(Static->GetNumberField(TEXT("max_chunk_size_tokens")) == 500);
                    TestTrueExpr(Static->GetNumberField(TEXT("chunk_overlap_tokens")) == 200);
                });

            It("CreateVectorStoreFileBatchShouldConvertMetadataToAttributes",
                [this]()
                {
                    FCreateVectorStoreFileBatch CreateBatch;
                    CreateBatch.File_Ids = {"file-1", "file-2"};
                    CreateBatch.Metadata.Add("batch-key", "batch-value");

                    FVectorStoreFileBatchFile File;
                    File.File_Id = "file-3";
                    File.Metadata.Add("file-key", "file-value");
                    CreateBatch.Files.Add(File);

                    const FString Result = OpenAI::VectorStoreParser::CreateVectorStoreFileBatchToJsonRepresentation(CreateBatch);

                    TSharedPtr<FJsonObject> Json;
                    UJsonFuncLib::StringToJson(Result, Json);

                    TestTrueExpr(Json->HasField(TEXT("attributes")));
                    TestFalseExpr(Json->HasField(TEXT("metadata")));
                    TestTrueExpr(Json->GetObjectField(TEXT("attributes"))->GetStringField(TEXT("batch-key")).Equals(TEXT("batch-value")));

                    auto Files = Json->GetArrayField(TEXT("files"));
                    TestTrueExpr(Files.Num() == 1);
                    auto FileObj = Files[0]->AsObject();
                    TestTrueExpr(FileObj->GetStringField(TEXT("file_id")).Equals(TEXT("file-3")));
                    TestTrueExpr(FileObj->HasField(TEXT("attributes")));
                    TestTrueExpr(FileObj->GetObjectField(TEXT("attributes"))->GetStringField(TEXT("file-key")).Equals(TEXT("file-value")));
                });

            It("VectorStoreQueryParamsShouldBeCorrect",
                [this]()
                {
                    FVectorStoreQueryParams Params;
                    Params.Limit = 10;
                    Params.Order = "asc";
                    Params.After.Set("obj_123");

                    const FString Query = Params.ToQuery();
                    TestTrueExpr(Query.Contains("limit=10"));
                    TestTrueExpr(Query.Contains("order=asc"));
                    TestTrueExpr(Query.Contains("after=obj_123"));
                    TestFalseExpr(Query.Contains("before="));
                });
        });
}

#endif

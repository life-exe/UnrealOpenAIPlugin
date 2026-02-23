// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Provider/JsonParsers/VectorStoreParser.h"
#include "JsonObjectConverter.h"
#include "FuncLib/JsonFuncLib.h"

using namespace OpenAI;

namespace
{
void CleanChunkingStrategy(TSharedPtr<FJsonObject>& StrategyObj)
{
    if (!StrategyObj.IsValid()) return;

    FString TypeStr = StrategyObj->GetStringField(TEXT("Type")).ToLower();
    StrategyObj->SetStringField(TEXT("Type"), TypeStr);

    if (TypeStr.Equals(TEXT("auto")))
    {
        StrategyObj->RemoveField(TEXT("Static"));
    }
}
}  // namespace

FString VectorStoreParser::CreateVectorStoreToJsonRepresentation(const FCreateVectorStore& CreateVectorStore)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(CreateVectorStore);
    if (!Json.IsValid()) return {};

    UJsonFuncLib::RemoveEmptyArrays(Json);

    if (Json->HasField(TEXT("Chunking_Strategy")))
    {
        auto StrategyObj = Json->GetObjectField(TEXT("Chunking_Strategy"));
        CleanChunkingStrategy(StrategyObj);
    }

    if (CreateVectorStore.Metadata.IsEmpty())
    {
        Json->RemoveField(TEXT("Metadata"));
    }

    if (CreateVectorStore.Expires_After.Days == 0)
    {
        Json->RemoveField(TEXT("Expires_After"));
    }

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    return TransformedString;
}

FString VectorStoreParser::UpdateVectorStoreToJsonRepresentation(const FUpdateVectorStore& UpdateVectorStore)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(UpdateVectorStore);
    if (!Json.IsValid()) return {};

    if (UpdateVectorStore.Metadata.IsEmpty())
    {
        Json->RemoveField(TEXT("Metadata"));
    }

    if (UpdateVectorStore.Expires_After.Days == 0)
    {
        Json->RemoveField(TEXT("Expires_After"));
    }

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    return TransformedString;
}

FString VectorStoreParser::CreateVectorStoreFileBatchToJsonRepresentation(const FCreateVectorStoreFileBatch& CreateBatch)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(CreateBatch);
    if (!Json.IsValid()) return {};

    UJsonFuncLib::RemoveEmptyArrays(Json);

    if (Json->HasField(TEXT("Chunking_Strategy")))
    {
        auto StrategyObj = Json->GetObjectField(TEXT("Chunking_Strategy"));
        CleanChunkingStrategy(StrategyObj);
    }

    // Attributes vs Metadata
    if (Json->HasField(TEXT("Metadata")))
    {
        Json->SetObjectField(TEXT("Attributes"), Json->GetObjectField(TEXT("Metadata")));
        Json->RemoveField(TEXT("Metadata"));
    }

    if (CreateBatch.Metadata.IsEmpty())
    {
        Json->RemoveField(TEXT("Attributes"));
    }

    if (Json->HasField(TEXT("Files")))
    {
        auto FilesArray = Json->GetArrayField(TEXT("Files"));
        for (auto& FileVal : FilesArray)
        {
            auto FileObj = FileVal->AsObject();
            if (FileObj->HasField(TEXT("Chunking_Strategy")))
            {
                auto StrategyObj = FileObj->GetObjectField(TEXT("Chunking_Strategy"));
                CleanChunkingStrategy(StrategyObj);
            }
            if (FileObj->HasField(TEXT("Metadata")))
            {
                FileObj->SetObjectField(TEXT("Attributes"), FileObj->GetObjectField(TEXT("Metadata")));
                FileObj->RemoveField(TEXT("Metadata"));
            }
        }
    }

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    return TransformedString;
}

FString VectorStoreParser::SearchVectorStoreToJsonRepresentation(const FVectorStoreSearch& Search)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(Search);
    if (!Json.IsValid()) return {};

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    return TransformedString;
}

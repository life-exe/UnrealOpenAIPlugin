// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "VectorStoreTypes.generated.h"

UENUM(BlueprintType)
enum class EVectorStoreStatus : uint8
{
    Expired,
    In_Progress,
    Completed
};

UENUM(BlueprintType)
enum class EVectorStoreFileStatus : uint8
{
    In_Progress,
    Completed,
    Cancelled,
    Failed
};

UENUM(BlueprintType)
enum class EVectorStoreFileErrorCode : uint8
{
    Server_Error,
    Unsupported_File,
    Invalid_File,
    None
};

UENUM(BlueprintType)
enum class EChunkingStrategyType : uint8
{
    Auto,
    Static,
    Other
};

USTRUCT(BlueprintType)
struct FStaticFileChunkingStrategy
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    int32 Chunk_Overlap_Tokens{400};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    int32 Max_Chunk_Size_Tokens{800};
};

USTRUCT(BlueprintType)
struct FFileChunkingStrategy
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    EChunkingStrategyType Type{EChunkingStrategyType::Auto};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FStaticFileChunkingStrategy Static;
};

USTRUCT(BlueprintType)
struct FVectorStoreExpiresAfter
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Anchor{"last_active_at"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    int32 Days{0};
};

USTRUCT(BlueprintType)
struct FVectorStoreFileCounts
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Cancelled{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Completed{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Failed{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 In_Progress{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Total{0};
};

USTRUCT(BlueprintType)
struct FVectorStore
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FVectorStoreFileCounts File_Counts;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Last_Active_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    EVectorStoreStatus Status{EVectorStoreStatus::In_Progress};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Usage_Bytes{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FOptionalInt Expires_At;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FVectorStoreExpiresAfter Expires_After;
};

USTRUCT(BlueprintType)
struct FCreateVectorStore
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FString> File_Ids;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FVectorStoreExpiresAfter Expires_After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FFileChunkingStrategy Chunking_Strategy;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FUpdateVectorStore
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FVectorStoreExpiresAfter Expires_After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FVectorStoreFileBatch
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Vector_Store_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    EVectorStoreFileStatus Status{EVectorStoreFileStatus::In_Progress};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FVectorStoreFileCounts File_Counts;
};

USTRUCT(BlueprintType)
struct FVectorStoreFileBatchFile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString File_Id;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FFileChunkingStrategy Chunking_Strategy;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FCreateVectorStoreFileBatch
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FString> File_Ids;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FVectorStoreFileBatchFile> Files;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FFileChunkingStrategy Chunking_Strategy;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FVectorStoreFile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Usage_Bytes{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Vector_Store_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    EVectorStoreFileStatus Status{EVectorStoreFileStatus::In_Progress};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    EVectorStoreFileErrorCode Last_Error_Code{EVectorStoreFileErrorCode::None};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Error_Message;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FFileChunkingStrategy Chunking_Strategy;
};

USTRUCT(BlueprintType)
struct FVectorStoreDeletedResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{false};
};

USTRUCT(BlueprintType)
struct FVectorStoreQueryParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 Limit{20};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Order{"desc"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Before;

    FString ToQuery() const
    {
        FString Query{"?"};

        Query.Append("limit=").Append(FString::FromInt(Limit)).Append("&");
        Query.Append("order=").Append(Order).Append("&");

        if (After.IsSet)
        {
            Query.Append("after=").Append(After.Value).Append("&");
        }

        if (Before.IsSet)
        {
            Query.Append("before=").Append(Before.Value).Append("&");
        }

        return Query.LeftChop(1);
    }
};

USTRUCT(BlueprintType)
struct FVectorStoreFileQueryParams : public FVectorStoreQueryParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Filter;  // in_progress, completed, failed, cancelled

    FString ToQuery() const
    {
        FString Query = FVectorStoreQueryParams::ToQuery();

        if (Filter.IsSet)
        {
            Query.Append("&filter=").Append(Filter.Value);
        }

        return Query;
    }
};

USTRUCT(BlueprintType)
struct FListVectorStoresResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FVectorStore> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};
};

USTRUCT(BlueprintType)
struct FListVectorStoreFilesResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FVectorStoreFile> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};
};

USTRUCT(BlueprintType)
struct FVectorStoreSearch
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Query;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 Max_Num_Results{20};
};

USTRUCT(BlueprintType)
struct FVectorStoreSearchContent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Text;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Type;
};

USTRUCT(BlueprintType)
struct FVectorStoreSearchResultItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TMap<FString, FString> Attributes;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FVectorStoreSearchContent> Content;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString File_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Filename;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    float Score{0.0f};
};

USTRUCT(BlueprintType)
struct FVectorStoreSearchResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FVectorStoreSearchResultItem> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Next_Page;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FString> Search_Query;
};

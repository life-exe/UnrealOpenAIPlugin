// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "FileTypes.generated.h"

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class EUploadFilePurpose : uint8
{
    Assistants = 0,
    Vision,
    Batch,
    FineTune
};

USTRUCT(BlueprintType)
struct FListFilesParams
{
    GENERATED_BODY()

    /**
      A cursor for use in pagination. After is an object ID that defines your place in the list.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    /**
      A limit on the number of objects to be returned. Limit can range between 1 and 10,000. Default is 10,000.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Limit;

    /**
      Sort order by the created_at timestamp. "asc" for ascending, "desc" for descending.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Order;

    /**
      Only return files with the given purpose.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Purpose;
};

/**
  Upload a file that can be used across various endpoints.
  Individual files can be up to 512 MB, and the size of all files uploaded by one
  organization can be up to 100 GB.
*/
USTRUCT(BlueprintType)
struct FUploadFile
{
    GENERATED_BODY()

    /**
      The File object (not file name) to be uploaded.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString File;

    /**
      The intended purpose of the uploaded file.

      Use "assistants" for Assistants and Message files,
      "vision" for Assistants image file inputs,
      "batch" for Batch API, and
      "fine-tune" for Fine-tuning.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Purpose;
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FOpenAIFileBase
{
    GENERATED_BODY()

    /**
      The file identifier, which can be referenced in the API endpoints.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID;

    /**
      The object type, which is always "file".
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The size of the file in bytes.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Bytes{};

    /**
      The Unix timestamp (in seconds) for when the file was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    /**
      The name of the file.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString FileName;

    /**
      The intended purpose of the file.
      Supported values are assistants, assistants_output, batch, batch_output, fine-tune, fine-tune-results, vision, and user_data.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Purpose;

    /**
      The Unix timestamp (in seconds) for when the file will expire.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Expires_At{};
};

USTRUCT(BlueprintType)
struct FOpenAIFile : public FOpenAIFileBase
{
    GENERATED_BODY()

    /**
      The current status of the file,
      which can be either uploaded, processed, pending, error, deleting or deleted.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status;

    /**
      Additional details about the status of the file.
      If the file is in the error state,
      this will include a message describing the error.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status_Details;
};

USTRUCT(BlueprintType)
struct FListFilesResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FOpenAIFile> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{};
};

USTRUCT(BlueprintType)
struct FUploadFileResponse : public FOpenAIFile
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FDeleteFileResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{};
};

USTRUCT(BlueprintType)
struct FRetrieveFileResponse : public FOpenAIFile
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FRetrieveFileContentResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Content;
};

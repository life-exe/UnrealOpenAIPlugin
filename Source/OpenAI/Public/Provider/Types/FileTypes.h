// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
      Supported values are fine-tune or fine-tune-results.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Purpose;
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

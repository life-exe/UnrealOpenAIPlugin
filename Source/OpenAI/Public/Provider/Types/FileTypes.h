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

USTRUCT(BlueprintType)
struct FUploadFile
{
    GENERATED_BODY()

    /**
      Name of the JSON Lines file to be uploaded.
      If the purpose is set to "fine-tune", each line is a JSON record with "prompt" and "completion"
      fields representing your training examples.
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
struct FOpenAIFile
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

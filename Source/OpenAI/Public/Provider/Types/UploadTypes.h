// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "Provider/Types/FileTypes.h"
#include "UploadTypes.generated.h"

UENUM(BlueprintType)
enum class EUploadStatus : uint8
{
    Pending = 0,
    Completed,
    Cancelled
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCreateUpload
{
    GENERATED_BODY()

    /**
      The name of the file to upload.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Filename;

    /**
      The intended purpose of the uploaded file.
      See the documentation on File purposes:
      https://platform.openai.com/docs/api-reference/files/create#files-create-purpose
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Purpose;

    /**
      The number of bytes in the file you are uploading.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    int64 Bytes{};

    /**
      The MIME type of the file.
      This must fall within the supported MIME types for your file purpose.
      See the supported MIME types for assistants and vision.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Mime_Type;
};

USTRUCT(BlueprintType)
struct FAddUploadPart
{
    GENERATED_BODY()

    /**
      The chunk of bytes for this Part.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Data;
};

USTRUCT(BlueprintType)
struct FCompleteUpload
{
    GENERATED_BODY()

    /**
      The ordered list of Part IDs.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    TArray<FString> Part_Ids;

    /**
      The optional md5 checksum for the file contents to verify
      if the bytes uploaded matches what you expect.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FOptionalString Md5;
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

/**
  The Upload object can accept byte chunks in the form of Parts.
*/
USTRUCT(BlueprintType)
struct FUploadObjectResponse
{
    GENERATED_BODY()

    /**
       The Upload unique identifier, which can be referenced in API endpoints.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    /**
      The Unix timestamp (in seconds) for when the Upload was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    /**
      The name of the file to be uploaded.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Filename;

    /**
      The intended number of bytes to be uploaded.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int64 Bytes{};

    /**
      The intended purpose of the file. Please refer here for acceptable values.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Purpose;

    /**
      The status of the Upload.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status;

    /**
      The Unix timestamp (in seconds) for when the Upload was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Expires_At{};

    /**
      The object type, which is always "upload".
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The File object represents a document that has been uploaded to OpenAI.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FOpenAIFileBase File;
};

/**
  The upload Part represents a chunk of bytes we can add to an Upload object.
*/
USTRUCT(BlueprintType)
struct FUploadPartObjectResponse
{
    GENERATED_BODY()

    /**
       The upload Part unique identifier, which can be referenced in API endpoints.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    /**
      The Unix timestamp (in seconds) for when the Part was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    /**
      The ID of the Upload object that this Part was added to.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Upload_Id;

    /**
      The object type, which is always upload.part.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

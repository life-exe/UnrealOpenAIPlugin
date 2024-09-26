// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "BatchTypes.generated.h"

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class EBatchEndpoint : uint8
{
    ChatCompletions = 0,
    Embeddings,
    Completions
};

UENUM(BlueprintType)
enum class EBatchCompletionWindow : uint8
{
    Window_24h = 0,
};

USTRUCT(BlueprintType)
struct FCreateBatch
{
    GENERATED_BODY()

    /**
      The ID of an uploaded file that contains requests for the new batch.
      See upload file for how to upload a file.

      Your input file must be formatted as a JSONL file,
      and must be uploaded with the purpose batch.
      The file can contain up to 50,000 requests, and can be up to 100 MB in size.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Input_File_Id{};

    /**
      The endpoint to be used for all requests in the batch.
      Currently /v1/chat/completions, /v1/embeddings,
      and /v1/completions are supported.
      Note that /v1/embeddings batches are also restricted
      to a maximum of 50,000 embedding inputs across all requests in the batch.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Endpoint{"/v1/chat/completions"};

    /**
      The time frame within which the batch should be processed.Currently only 24h is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Completion_Window{"24h"};

    /**
      Optional custom metadata for the batch.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FListBatch
{
    GENERATED_BODY()

    /**
      A cursor for use in pagination. after is an object ID
      that defines your place in the list. For instance,
      if you make a list request and receive 100 objects, ending with obj_foo,
      your subsequent call can include after=obj_foo in order to fetch the next page of the list.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    /**
      A limit on the number of objects to be returned.
      Limit can range between 1 and 100, and the default is 20.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Limit;

    FString ToQuery() const
    {
        FString Query{"?"};
        if (After.IsSet)
        {
            Query.Append("after=").Append(After.Value).Append("&");
        }

        if (Limit.IsSet)
        {
            Query.Append("limit=").Append(FString::FromInt(Limit.Value)).Append("&");
        }

        return Query.LeftChop(1);
    }
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FOpenAIBatchErrorData
{
    GENERATED_BODY()

    /**
      An error code identifying the error type.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Code;

    /**
      A human-readable message providing more details about the error.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Message;

    /**
      The name of the parameter that caused the error, if applicable.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Param;

    /**
      The line number of the input file where the error occurred, if applicable.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Line{};
};

USTRUCT(BlueprintType)
struct FOpenAIBatchErrors
{
    GENERATED_BODY()

    /**
      The object type, which is always list.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The object type, which is always list.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FOpenAIBatchErrorData> Data;
};

USTRUCT(BlueprintType)
struct FOpenAIRequestCounts
{
    GENERATED_BODY()

    /**
      Total number of requests in the batch.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Total{};

    /**
      Number of requests that have been completed successfully.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Completed{};

    /**
      Number of requests that have failed.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Failed{};
};

USTRUCT(BlueprintType)
struct FOpenAIBatch
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    /**
      The object type, which is always batch.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The OpenAI API endpoint used by the batch.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Endpoint;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FOpenAIBatchErrors Errors;

    /**
      The ID of the input file for the batch.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Input_File_Id;

    /**
      The time frame within which the batch should be processed.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Completion_Window;

    /**
      The current status of the batch.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status;

    /**
      The ID of the file containing the outputs of successfully executed requests.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Output_File_Id;

    /**
      The ID of the file containing the outputs of requests with errors.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Error_File_Id;

    /**
      The Unix timestamp (in seconds) for when the batch was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    /**
      The Unix timestamp (in seconds) for when the batch started processing.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 In_Progress_At{};

    /**
      The Unix timestamp (in seconds) for when the batch will expire.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Expires_At{};

    /**
      The Unix timestamp (in seconds) for when the batch started finalizing.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Finalizing_At{};

    /**
      The Unix timestamp (in seconds) for when the batch was completed.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Completed_At{};

    /**
      The Unix timestamp (in seconds) for when the batch failed.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Failed_At{};

    /**
      The Unix timestamp (in seconds) for when the batch expired.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Expired_At{};

    /**
      The Unix timestamp (in seconds) for when the batch started cancelling.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Cancelling_At{};

    /**
      The Unix timestamp (in seconds) for when the batch was cancelled.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Cancelled_At{};

    /**
      The request counts for different statuses within the batch.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FOpenAIRequestCounts Request_Counts;

    /**
      Set of 16 key-value pairs that can be attached to an object.
      This can be useful for storing additional information about
      the object in a structured format. Keys can be a maximum of
      64 characters long and values can be a maxium of 512 characters long.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FCreateBatchResponse : public FOpenAIBatch
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FRetrieveBatchResponse : public FOpenAIBatch
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FCancelBatchResponse : public FOpenAIBatch
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FListBatchResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FOpenAIBatch> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{};
};

// @todo: https://platform.openai.com/docs/api-reference/batch/request-input
// @todo: https://platform.openai.com/docs/api-reference/batch/request-output

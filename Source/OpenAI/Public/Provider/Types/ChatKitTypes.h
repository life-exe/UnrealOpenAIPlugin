// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "ChatKitTypes.generated.h"

///////////////////////////////////////////////////////
//                 COMMON TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FChatKitTracing
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Enabled;
};

USTRUCT(BlueprintType)
struct FChatKitWorkflow
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Id;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, FString> State_Variables;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FChatKitTracing Tracing;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Version;
};

USTRUCT(BlueprintType)
struct FChatKitAutomaticThreadTitling
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Enabled;
};

USTRUCT(BlueprintType)
struct FChatKitFileUpload
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Enabled;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Max_File_Size;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Max_Files;
};

USTRUCT(BlueprintType)
struct FChatKitHistory
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Enabled;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Recent_Threads;
};

USTRUCT(BlueprintType)
struct FChatKitConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FChatKitAutomaticThreadTitling Automatic_Thread_Titling;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FChatKitFileUpload File_Upload;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FChatKitHistory History;
};

USTRUCT(BlueprintType)
struct FChatKitExpiresAfter
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Anchor{"created_at"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    int32 Seconds{600};
};

USTRUCT(BlueprintType)
struct FChatKitRateLimits
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Max_Requests_Per_1_Minute;
};

///////////////////////////////////////////////////////
//                 SESSIONS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCreateChatKitSession
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString User;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FChatKitWorkflow Workflow;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FChatKitConfiguration Chatkit_Configuration;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FChatKitExpiresAfter Expires_After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FChatKitRateLimits Rate_Limits;
};

USTRUCT(BlueprintType)
struct FChatKitSessionResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FChatKitConfiguration Chatkit_Configuration;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Client_Secret;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Expires_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Max_Requests_Per_1_Minute{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FChatKitRateLimits Rate_Limits;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString User;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FChatKitWorkflow Workflow;
};

///////////////////////////////////////////////////////
//                 THREADS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FChatKitThreadStatus
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Type;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Reason;
};

USTRUCT(BlueprintType)
struct FChatKitThread
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FChatKitThreadStatus Status;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Title;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString User;
};

USTRUCT(BlueprintType)
struct FListChatKitThreads
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Before;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Limit;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Order;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString User;

    FString ToQuery() const
    {
        FString Query{"?"};
        if (After.IsSet) Query.Append("after=").Append(After.Value).Append("&");
        if (Before.IsSet) Query.Append("before=").Append(Before.Value).Append("&");
        if (Limit.IsSet) Query.Append("limit=").Append(FString::FromInt(Limit.Value)).Append("&");
        if (Order.IsSet) Query.Append("order=").Append(Order.Value).Append("&");
        if (User.IsSet) Query.Append("user=").Append(User.Value).Append("&");
        return Query.LeftChop(1);
    }
};

USTRUCT(BlueprintType)
struct FListChatKitThreadsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChatKitThread> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

USTRUCT(BlueprintType)
struct FDeleteChatKitThreadResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

///////////////////////////////////////////////////////
//                 THREAD ITEMS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FChatKitAttachment
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Mime_Type;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Preview_Url;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Type;
};

USTRUCT(BlueprintType)
struct FChatKitContent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Text;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Type;
};

USTRUCT(BlueprintType)
struct FChatKitInferenceOptions
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model;
};

USTRUCT(BlueprintType)
struct FChatKitTaskEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Heading;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Summary;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Type;
};

USTRUCT(BlueprintType)
struct FChatKitThreadItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChatKitAttachment> Attachments;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChatKitContent> Content;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FChatKitInferenceOptions Inference_Options;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Thread_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Type;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Widget;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Arguments;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Call_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Output;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Heading;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Summary;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Task_Type;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChatKitTaskEntry> Tasks;
};

USTRUCT(BlueprintType)
struct FChatKitThreadItemListResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChatKitThreadItem> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

USTRUCT(BlueprintType)
struct FListChatKitThreadItems
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Before;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Limit;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Order;

    FString ToQuery() const
    {
        FString Query{"?"};
        if (After.IsSet) Query.Append("after=").Append(After.Value).Append("&");
        if (Before.IsSet) Query.Append("before=").Append(Before.Value).Append("&");
        if (Limit.IsSet) Query.Append("limit=").Append(FString::FromInt(Limit.Value)).Append("&");
        if (Order.IsSet) Query.Append("order=").Append(Order.Value).Append("&");
        return Query.LeftChop(1);
    }
};

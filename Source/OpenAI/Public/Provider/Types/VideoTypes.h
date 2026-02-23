// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "VideoTypes.generated.h"

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class EVideoModel : uint8
{
    Sora_2 = 0,  // "sora-2"
    Sora_2_Pro,  // "sora-2-pro"
};

UENUM(BlueprintType)
enum class EVideoStatus : uint8
{
    Queued = 0,
    InProgress,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EVideoVariant : uint8
{
    Video = 0,    // MP4 video (default)
    Thumbnail,    // Preview thumbnail image
    Spritesheet,  // Sprite sheet of frames
};

UENUM(BlueprintType)
enum class EVideoSize : uint8
{
    Size_720x1280 = 0,  // "720x1280" (default, portrait)
    Size_1280x720,      // "1280x720" (landscape)
    Size_1024x1792,     // "1024x1792" (tall portrait)
    Size_1792x1024,     // "1792x1024" (wide landscape)
};

/**
  Create a new video generation job.
  https://platform.openai.com/docs/api-reference/videos/create
*/
USTRUCT(BlueprintType)
struct FCreateVideo
{
    GENERATED_BODY()

    /**
      Text prompt describing the video to generate.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt{};

    /**
      The model to use for video generation. Defaults to "sora-2".
      Valid values: "sora-2", "sora-2-pro".
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"sora-2"};

    /**
      Optional image reference URL guiding generation.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Input_Reference;

    /**
      Clip duration in seconds. Must be 4, 8, or 12. Defaults to 4.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Seconds;

    /**
      Output resolution. Defaults to "720x1280".
      Valid values: "720x1280", "1280x720", "1024x1792", "1792x1024".
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Size;
};

/**
  List recently generated videos.
  https://platform.openai.com/docs/api-reference/videos/list
*/
USTRUCT(BlueprintType)
struct FListVideos
{
    GENERATED_BODY()

    /**
      Identifier for the last item from the previous pagination request.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    /**
      Number of items to retrieve.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Limit;

    /**
      Sort order by timestamp. "asc" for ascending, "desc" for descending.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Order;

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
        if (Order.IsSet)
        {
            Query.Append("order=").Append(Order.Value).Append("&");
        }
        return Query.LeftChop(1);
    }
};

/**
  Create a remix of a completed video using a refreshed prompt.
  https://platform.openai.com/docs/api-reference/videos/remix
*/
USTRUCT(BlueprintType)
struct FRemixVideo
{
    GENERATED_BODY()

    /**
      Updated text prompt directing remix generation.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt{};
};

/**
  Query parameters for downloading video content.
  https://platform.openai.com/docs/api-reference/videos/download-content
*/
USTRUCT(BlueprintType)
struct FDownloadVideoContent
{
    GENERATED_BODY()

    /**
      Which downloadable asset to return.
      Valid values: "video" (default), "thumbnail", "spritesheet".
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Variant;

    FString ToQuery() const
    {
        FString Query{"?"};
        if (Variant.IsSet)
        {
            Query.Append("variant=").Append(Variant.Value).Append("&");
        }
        return Query.LeftChop(1);
    }
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FVideoCreateError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Code{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Message{};
};

/**
  Represents a video generation job.
*/
USTRUCT(BlueprintType)
struct FVideoObject
{
    GENERATED_BODY()

    /** Unique job identifier. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id{};

    /** Unix timestamp when the job completed. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Completed_At{};

    /** Unix timestamp when the job was created. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    /** Error details if the job failed. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FVideoCreateError Error{};

    /** Unix timestamp when the downloadable assets expire. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Expires_At{};

    /** Model used for generation. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model{};

    /** Object type, always "video". */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object{};

    /** Approximate completion percentage (0–100). */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    float Progress{};

    /** Prompt used for generation. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Prompt{};

    /** Source video ID if this is a remix. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Remixed_From_Video_Id{};

    /** Duration in seconds of the generated clip. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Seconds{};

    /** Resolution of the generated video (e.g. "720x1280"). */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Size{};

    /** Current lifecycle status: "queued", "in_progress", "completed", or "failed". */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status{};
};

USTRUCT(BlueprintType)
struct FListVideosResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FVideoObject> Data{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object{};
};

USTRUCT(BlueprintType)
struct FDeleteVideoResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object{};
};

USTRUCT(BlueprintType)
struct FDownloadVideoContentResponse
{
    GENERATED_BODY()

    /** Raw binary content of the downloaded asset. */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<uint8> Content{};
};

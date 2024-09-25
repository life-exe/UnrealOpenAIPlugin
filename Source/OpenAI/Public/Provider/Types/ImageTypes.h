// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "ImageTypes.generated.h"

UENUM(BlueprintType)
enum class EImageModelEnum : uint8
{
    DALL_E_2,
    DALL_E_3
};

UENUM(BlueprintType)
enum class EImageSizeDalle2 : uint8
{
    Size_256x256,
    Size_512x512,
    Size_1024x1024
};

UENUM(BlueprintType)
enum EImageSizeDalle3 : uint8
{
    Size_1024x1024,
    Size_1792x1024,
    Size_1024x1792
};

UENUM(BlueprintType)
enum EOpenAIImageQuality
{
    Standard,
    HD
};

UENUM(BlueprintType)
enum EOpenAIImageStyle
{
    // Vivid causes the model to lean towards generating hyper - real and dramatic images.
    Vivid,
    // Natural causes the model to produce more natural, less hyper - real looking images.
    Natural
};

UENUM(BlueprintType)
enum class EOpenAIImageFormat : uint8
{
    URL,
    B64_JSON
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FOpenAIImage
{
    GENERATED_BODY()

    /**
      A text description of the desired image(s). The maximum length is 1000 characters for dall-e-2 and 4000 characters for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt;

    /**
      The model to use for image generation.
      Defaults to dall-e-2.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      The number of images to generate. Must be between 1 and 10.
      For dall-e-3, only n=1 is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The quality of the image that will be generated. hd creates images with finer details and greater consistency across the image.
      This param is only supported for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Quality{"standard"};

    /**
      The format in which the generated images are returned. Must be one of url or b64_json.
      URLs are only valid for 60 minutes after the image has been generated.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"url"};

    /**
      The size of the generated images. Must be one of 256x256, 512x512, or 1024x1024 for dall-e-2.
      Must be one of 1024x1024, 1792x1024, or 1024x1792 for dall-e-3 models.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

    /**
      The style of the generated images.
      Must be one of vivid or natural.
      Vivid causes the model to lean towards generating hyper-real and dramatic images.
      Natural causes the model to produce more natural, less hyper-real looking images.
      This param is only supported for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Style{"vivid"};

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString User;
};

USTRUCT(BlueprintType)
struct FOpenAIImageEdit
{
    GENERATED_BODY()

    /**
      The image to edit. Must be a valid PNG file, less than 4MB, and square.
      If mask is not provided, image must have transparency, which will be used as the mask.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Image;

    /**
      A text description of the desired image(s). The maximum length is 1000 characters.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt;

    /**
      An additional image whose fully transparent areas (e.g. where alpha is zero) indicate where image should be edited.
      Must be a valid PNG file, less than 4MB, and have the same dimensions as image.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Mask;

    /**
      The model to use for image generation.
      Defaults to dall-e-2.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      The number of images to generate. Must be between 1 and 10.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The size of the generated images. Must be one of 256x256, 512x512, or 1024x1024.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

    /**
      The format in which the generated images are returned.
      Must be one of url or b64_json.
      URLs are only valid for 60 minutes after the image has been generated.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"url"};

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString User;
};

USTRUCT(BlueprintType)
struct FOpenAIImageVariation
{
    GENERATED_BODY()

    /**
      The image to use as the basis for the variation(s).
      Must be a valid PNG file, less than 4MB, and square.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Image;

    /**
      The model to use for image generation.
      Only dall-e-2 is supported at this time.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      The number of images to generate.
      Must be between 1 and 10. For dall-e-3, only n=1 is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The format in which the generated images are returned.
      Must be one of url or b64_json.
      URLs are only valid for 60 minutes after the image has been generated.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"url"};

    /**
      The size of the generated images. Must be one of 256x256, 512x512, or 1024x1024.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString User;
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

/**
  Represents the url or the content of an image generated by the OpenAI API.
*/
USTRUCT(BlueprintType)
struct FImageObject
{
    GENERATED_BODY()

    /**
       The base64 - encoded JSON of the generated image, if response_format is b64_json.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString B64_JSON;

    /**
      The URL of the generated image, if response_format is url (default).
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString URL;

    /**
      The prompt that was used to generate the image, if there was any revision to the prompt.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Revised_Prompt;
};

USTRUCT(BlueprintType)
struct FImageResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FImageObject> Data;
};

USTRUCT(BlueprintType)
struct FImageEditResponse : public FImageResponse
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FImageVariationResponse : public FImageResponse
{
    GENERATED_BODY()
};

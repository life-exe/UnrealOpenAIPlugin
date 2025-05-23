// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "ImageTypes.generated.h"

UENUM(BlueprintType)
enum class EImageModelEnum : uint8
{
    DALL_E_2,
    DALL_E_3,
    GPT_Image_1
};

UENUM(BlueprintType)
enum class EImageSizeDalle2 : uint8
{
    Size_256x256,
    Size_512x512,
    Size_1024x1024
};

UENUM(BlueprintType)
enum class EImageSizeDalle3 : uint8
{
    Size_1024x1024,
    Size_1792x1024,
    Size_1024x1792
};

UENUM(BlueprintType)
enum class EImageSizeGptImage1 : uint8
{
    Auto,
    Size_1024x1024,
    Size_1536x1024,
    Size_1024x1536
};

UENUM(BlueprintType)
enum class EOpenAIImageQuality : uint8
{
    Standard,
    HD,
    High,
    Medium,
    Low
};

UENUM(BlueprintType)
enum class EOpenAIImageStyle : uint8
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

UENUM(BlueprintType)
enum class EOpenAIImageBackground : uint8
{
    Auto,
    Transparent,
    Opaque
};

UENUM(BlueprintType)
enum class EOpenAIImageModeration : uint8
{
    Auto,
    Low
};

UENUM(BlueprintType)
enum class EOpenAIImageOutputFormat : uint8
{
    Png,
    Jpeg,
    Webp
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FOpenAIImage
{
    GENERATED_BODY()

    /**
      A text description of the desired image(s).
      The maximum length is 32000 characters for gpt-image-1, 1000 characters for dall-e-2 and 4000 characters for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt;

    /**
      Allows to set transparency for the background of the generated image(s).
      This parameter is only supported for gpt-image-1.
      Must be one of transparent, opaque or auto (default value).
      When auto is used, the model will automatically determine the best background for the image.
      If transparent, the output format needs to support transparency, so it should be set to either png (default value) or webp.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Background;

    /**
      The model to use for image generation. One of dall-e-2, dall-e-3, or gpt-image-1.
      Defaults to dall-e-2 unless a parameter specific to gpt-image-1 is used.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      Control the content-moderation level for images generated by gpt-image-1.
      Must be either low for less restrictive filtering or auto (default value).
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Moderation;

    /**
      The number of images to generate. Must be between 1 and 10.
      For dall-e-3, only n=1 is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The compression level (0-100%) for the generated images.
      This parameter is only supported for gpt-image-1 with the webp or jpeg output formats, and defaults to 100.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Output_Compression{100};

    /**
      The format in which the generated images are returned.
      This parameter is only supported for gpt-image-1. Must be one of png, jpeg, or webp.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Output_Format{};

    /**
      The quality of the image that will be generated.

      auto (default value) will automatically select the best quality for the given model.
      high, medium and low are supported for gpt-image-1.
      hd and standard are supported for dall-e-3.
      standard is the only option for dall-e-2.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Quality{};

    /**
      The format in which generated images with dall-e-2 and dall-e-3 are returned. Must be one of url or b64_json.
      URLs are only valid for 60 minutes after the image has been generated.
      This parameter isn't supported for gpt-image-1 which will always return base64-encoded images.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Response_Format{};

    /**
      The size of the generated images.
      Must be one of 1024x1024, 1536x1024 (landscape), 1024x1536 (portrait), or auto (default value) for gpt-image-1,
      one of 256x256, 512x512, or 1024x1024 for dall-e-2, and one of 1024x1024, 1792x1024, or 1024x1792 for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

    /**
      The style of the generated images.
      This parameter is only supported for dall-e-3.
      Must be one of vivid or natural.
      Vivid causes the model to lean towards generating hyper-real and dramatic images.
      Natural causes the model to produce more natural, less hyper-real looking images.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Style{};

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
      The image(s) to edit. Must be a supported image file or an array of images.
      For gpt-image-1, each image should be a png, webp, or jpg file less than 25MB.
      For dall-e-2, you can only provide one image, and it should be a square png file less than 4MB.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    TArray<FString> Image;

    /**
      A text description of the desired image(s).
      The maximum length is 1000 characters for dall-e-2, and 32000 characters for gpt-image-1.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt;

    /**
      Allows to set transparency for the background of the generated image(s).
      This parameter is only supported for gpt-image-1.
      Must be one of transparent, opaque or auto (default value).
      When auto is used, the model will automatically determine the best background for the image.
      If transparent, the output format needs to support transparency, so it should be set to either png (default value) or webp.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Background;

    /**
      An additional image whose fully transparent areas (e.g. where alpha is zero) indicate where image should be edited. If there are
      multiple images provided, the mask will be applied on the first image. Must be a valid PNG file, less than 4MB, and have the same
      dimensions as image.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Mask;

    /**
      The model to use for image generation.
      Only dall-e-2 and gpt-image-1 are supported.
      Defaults to dall-e-2 unless a parameter specific to gpt-image-1 is used.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      The number of images to generate. Must be between 1 and 10.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The quality of the image that will be generated.
      high, medium and low are only supported for gpt-image-1.
      dall-e-2 only supports standard quality. Defaults to auto.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Quality{"auto"};

    /**
      The format in which the generated images are returned. Must be one of url or b64_json.
      URLs are only valid for 60 minutes after the image has been generated.
      This parameter is only supported for dall-e-2, as gpt-image-1 will always return base64-encoded images.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"url"};

    /**
      The size of the generated images.
      Must be one of 1024x1024, 1536x1024 (landscape), 1024x1536 (portrait), or auto (default value) for gpt-image-1,
      and one of 256x256, 512x512, or 1024x1024 for dall-e-2.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

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
struct FInputTokensDetails
{
    GENERATED_BODY()
    /**
      The number of image tokens in the input prompt.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Image_Tokens{};

    /**
      The number of text tokens in the input prompt.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Text_Tokens{};
};

/**
  For gpt-image-1 only, the token usage information for the image generation.
*/
USTRUCT(BlueprintType)
struct FImageUsage
{
    GENERATED_BODY()

    /**
      The number of tokens (images and text) in the input prompt.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Input_Tokens{};

    /**
      The input tokens detailed information for the image generation.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FInputTokensDetails Input_Tokens_Details{};

    /**
      The number of image tokens in the output image.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Output_Tokens{};

    /**
      The total number of tokens (images and text) used for the image generation.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Total_Tokens{};
};

USTRUCT(BlueprintType)
struct FImageResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FImageObject> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FImageUsage> Usage;
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

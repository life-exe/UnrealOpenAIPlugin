// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Provider/Types/AllTypesHeader.h"
#include "OpenAIFuncLib.generated.h"

UCLASS()
class OPENAI_API UOpenAIFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "OpenAI | Model")
    static FString OpenAIAllModelToString(EAllModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Model")
    static FString OpenAIMainModelToString(EMainModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Model")
    static FString OpenAIModelToString(const FOpenAIModel& OpenAIModel);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Moderation")
    static FString OpenAIModerationModelToString(EModerationsModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Moderations")
    static FString OpenAIModerationsToString(const FModerationResults& ModerationResults);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Model | Vision")
    static bool ModelSupportsVision(const FString& Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAIAudioModelToString(EAudioModel Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAITTSModelToString(ETTSModel Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAIVoiceToString(EVoice Voice);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAITTSAudioFormatToString(ETTSAudioFormat Format);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Audio")
    static FString OpenAIAudioTranscriptToString(ETranscriptFormat TranscriptFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static FString OpenAIImageModelToString(EImageModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static EImageModelEnum StringToOpenAIImageModel(const FString& Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static FString OpenAIImageSizeDalle2ToString(EImageSizeDalle2 ImageSize);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static EImageSizeDalle2 StringToOpenAIImageSizeDalle2(const FString& ImageSize);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static FString OpenAIImageSizeDalle3ToString(EImageSizeDalle3 ImageSize);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static EImageSizeDalle3 StringToOpenAIImageSizeDalle3(const FString& ImageSize);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static FString OpenAIImageFormatToString(EOpenAIImageFormat ImageFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static EOpenAIImageFormat StringToOpenAIImageFormat(const FString& ImageFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static FString OpenAIImageQualityToString(EOpenAIImageQuality ImageQuality);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static EOpenAIImageQuality StringToOpenAIImageQuality(const FString& ImageQuality);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static FString OpenAIImageStyleToString(EOpenAIImageStyle ImageStyle);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Image")
    static EOpenAIImageStyle StringToOpenAIImageStyle(const FString& ImageStyle);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Chat")
    static FString OpenAIChatResponseFormatToString(EChatResponseFormat ChatResponseFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Chat")
    static FString OpenAIMessageContentTypeToString(EMessageContentType MessageContentType);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Chat")
    static FString OpenAIServiceTierToString(EServiceTier ServiceTier);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Embeddings")
    static FString OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat EmbeddingsEncodingFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI | File")
    static FString OpenAIUploadFilePurposeToString(EUploadFilePurpose UploadFilePurpose);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Batch")
    static FString OpenAIBatchEndpointToString(EBatchEndpoint BatchEndpoint);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Batch")
    static FString OpenAIBatchCompletionWindowToString(EBatchCompletionWindow BatchCompletionWindow);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Upload")
    static FString OpenAIUploadStatusToString(EUploadStatus UploadStatus);

    // common
    UFUNCTION(BlueprintPure, Category = "OpenAI | Common")
    static FString OpenAIRoleToString(ERole Role);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Common")
    static FString OpenAIFinishReasonToString(EOpenAIFinishReason FinishReason);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Common")
    static EOpenAIFinishReason StringToOpenAIFinishReason(const FString& FinishReason);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Common")
    static ERole StringToOpenAIRole(const FString& Role);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Common")
    static FString OpenAIHeaderTypeToString(EOpenAIHttpHeaderType Type);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Common")
    static FString FindOpenAIHttpHeaderByType(const FOpenAIResponseMetadata& Headers, EOpenAIHttpHeaderType Type);

    // errors
    UFUNCTION(BlueprintPure, Category = "OpenAI | Error")
    static EOpenAIResponseError GetErrorCode(const FString& RawError);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Error")
    static FString GetErrorMessage(const FString& RawError);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Error")
    static FString ResponseErrorToString(EOpenAIResponseError Code);

    // base64
    UFUNCTION(BlueprintPure, Category = "OpenAI | Base64")
    static FString WrapBase64(const FString& Base64String);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Base64")
    static FString UnWrapBase64(const FString& Base64String);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Base64")
    static FString FilePathToBase64(const FString& FilePath);

    // API
    /**
      File content consits of two lines:
      APIKey=sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      OrganizationID=org-xxxxxxxxxxxxxxxxxxxxxx
      ProjectID=proj_xxxxxxxxxxxxxxxxxxxxxx
    */
    UFUNCTION(BlueprintPure, Category = "OpenAI | API")
    static FOpenAIAuth LoadAPITokensFromFile(const FString& FilePath);

    UFUNCTION(BlueprintPure, Category = "OpenAI | API")
    static FOpenAIAuth LoadAPITokensFromFileOnce(const FString& FilePath);

    // misc helpers
    static FString BoolToString(bool Value);
    static FString RemoveWhiteSpaces(const FString& Input);
    static OpenAI::ServiceSecrets LoadServiceSecretsFromFile(const FString& FilePath);
    static bool LoadSecretByName(const OpenAI::ServiceSecrets& Secrets, const FString& SecretName, FString& SecretValue);
    static FString MakeURLWithQuery(const FString& URL, const OpenAI::QueryPairs& Args);
};

// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/BatchTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/Types/AudioTypes.h"
#include "Provider/Types/ImageTypes.h"
#include "Provider/Types/ModerationTypes.h"
#include "Provider/Types/EmbeddingTypes.h"
#include "Provider/Types/FileTypes.h"
#include "Provider/Types/ChatTypes.h"
#include "JsonObjectConverter.h"
#include "OpenAIFuncLib.generated.h"

UCLASS()
class OPENAI_API UOpenAIFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIAllModelToString(EAllModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIMainModelToString(EMainModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIModerationModelToString(EModerationsModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static bool ModelSupportsVision(const FString& Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAIAudioModelToString(EAudioModel Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAITTSModelToString(ETTSModel Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAIVoiceToString(EVoice Voice);

    UFUNCTION(BlueprintPure, Category = "OpenAI| Audio")
    static FString OpenAITTSAudioFormatToString(ETTSAudioFormat Format);

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

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIRoleToString(ERole Role);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIFinishReasonToString(EOpenAIFinishReason FinishReason);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static EOpenAIFinishReason StringToOpenAIFinishReason(const FString& FinishReason);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static ERole StringToOpenAIRole(const FString& Role);

    /**
      File content consits of two lines:
      APIKey=sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      OrganizationID=org-xxxxxxxxxxxxxxxxxxxxxx
      ProjectID=proj_xxxxxxxxxxxxxxxxxxxxxx
    */
    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FOpenAIAuth LoadAPITokensFromFile(const FString& FilePath);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FOpenAIAuth LoadAPITokensFromFileOnce(const FString& FilePath);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIAudioTranscriptToString(ETranscriptFormat TranscriptFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat EmbeddingsEncodingFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIChatResponseFormatToString(EChatResponseFormat ChatResponseFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIModelToString(const FOpenAIModel& OpenAIModel);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Chat")
    static FString OpenAIMessageContentTypeToString(EMessageContentType MessageContentType);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIModerationsToString(const FModerationResults& ModerationResults);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static EOpenAIResponseError GetErrorCode(const FString& RawError);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString GetErrorMessage(const FString& RawError);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString ResponseErrorToString(EOpenAIResponseError Code);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString WrapBase64(const FString& Base64String);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString UnWrapBase64(const FString& Base64String);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString FilePathToBase64(const FString& FilePath);

    UFUNCTION(BlueprintPure, Category = "OpenAI | File")
    static FString OpenAIUploadFilePurposeToString(EUploadFilePurpose UploadFilePurpose);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Batch")
    static FString OpenAIBatchEndpointToString(EBatchEndpoint BatchEndpoint);

    UFUNCTION(BlueprintPure, Category = "OpenAI | Batch")
    static FString OpenAIBatchCompletionWindowToString(EBatchCompletionWindow BatchCompletionWindow);

    static FString BoolToString(bool Value);
    static FString RemoveWhiteSpaces(const FString& Input);

    static OpenAI::ServiceSecrets LoadServiceSecretsFromFile(const FString& FilePath);
    static bool LoadSecretByName(const OpenAI::ServiceSecrets& Secrets, const FString& SecretName, FString& SecretValue);

    static bool StringToJson(const FString& JsonString, TSharedPtr<FJsonObject>& JsonObject);
    static bool JsonToString(const TSharedPtr<FJsonObject>& JsonObject, FString& JsonString);

    template <typename OutStructType>
    static bool ParseJSONToStruct(const FString& Data, OutStructType* OutStruct)
    {
        TSharedPtr<FJsonObject> JsonObject;
        if (!UOpenAIFuncLib::StringToJson(Data, JsonObject)) return false;

        FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), OutStruct, 0, 0);
        return true;
    }

    static FString MakeURLWithQuery(const FString& URL, const OpenAI::QueryPairs& Args);

public:
    // helpers for OpeanAI 'functions'
    static FString MakeFunctionsString(const TSharedPtr<FJsonObject>& Json);
    static FString CleanUpFunctionsObject(const FString& Input);

private:
    static const FString START_FUNCTION_OBJECT_MARKER;
    static const FString END_FUNCTION_OBJECT_MARKER;
};

// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModelTypes.h"
#include "Provider/CommonTypes.h"
#include "Provider/ResponseTypes.h"
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
    static FString OpenAIAudioModelToString(EAudioModelEnum Model);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIImageSizeToString(EImageSize ImageSize);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIImageFormatToString(EOpenAIImageFormat ImageFormat);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIRoleToString(ERole Role);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIFinishReasonToString(EOpenAIFinishReason FinishReason);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static ERole StringToOpenAIRole(const FString& Role);

    /**
      File content consits of two lines:
      APIKey=sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
      OrganizationID=org-xxxxxxxxxxxxxxxxxxxxxx
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
    static FString OpenAIModelToString(const FOpenAIModel& OpenAIModel);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString OpenAIModerationsToString(const FModerationResults& ModerationResults);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static EOpenAIResponseError GetErrorCode(const FString& RawError);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString GetErrorMessage(const FString& RawError);

    UFUNCTION(BlueprintPure, Category = "OpenAI")
    static FString ResponseErrorToString(EOpenAIResponseError Code);

    static FString BoolToString(bool Value);
};

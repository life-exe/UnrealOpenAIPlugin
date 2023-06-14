// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "AudioTranslationAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnAudioTranslation, const FAudioTranslationResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UAudioTranslationAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnAudioTranslation OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UAudioTranslationAction* CreateAudioTranslation(const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth);

    void OnAudioTranslationCompleted(const FAudioTranslationResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FAudioTranslation AudioTranslation;
    FOpenAIAuth Auth;
};

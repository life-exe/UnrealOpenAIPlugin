// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/AudioTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "AudioTranslationAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAudioTranslation, const FAudioTranslationResponse&, Response,
    const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UAudioTranslationAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnAudioTranslation OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UAudioTranslationAction* CreateAudioTranslation(
        const FAudioTranslation& AudioTranslation, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL();

    void OnCreateAudioTranslationCompleted(const FAudioTranslationResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;

    FAudioTranslation AudioTranslation;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

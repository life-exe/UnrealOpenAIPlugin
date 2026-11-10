// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "BlueprintAsyncActions/OpenAIActionBase.h"
#include "Provider/Types/AudioTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "CreateVoiceConsentAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreateVoiceConsent, const FVoiceConsentResponse&, Response,
    const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

UCLASS()
class OPENAI_API UCreateVoiceConsentAction : public UOpenAIActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCreateVoiceConsent OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UCreateVoiceConsentAction* CreateVoiceConsent(
        const FCreateVoiceConsent& CreateVoiceConsent, const FOpenAIAuth& Auth, const FString& URLOverride);

    void OnCreateVoiceConsentCompleted(const FVoiceConsentResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    virtual void OnRequestError(const FString& URL, const FString& Content) override;
    virtual void SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const override;

private:
    FCreateVoiceConsent CreateVoiceConsentParams;
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/ImageTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "ImageVariationAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnImageVariation, const FImageVariationResponse&, Response, const FOpenAIResponseMetadata&,
    ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UImageVariationAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnImageVariation OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Images")
    static UImageVariationAction* CreateImageVariation(
        const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL();

    void OnCreateImageVariationCompleted(const FImageVariationResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;
    FOpenAIImageVariation ImageVariation;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

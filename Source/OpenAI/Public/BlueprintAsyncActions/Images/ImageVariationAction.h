// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ImageVariationAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageVariation, const FImageVariationResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UImageVariationAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnImageVariation OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Images")
    static UImageVariationAction* CreateImageVariation(const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth);

    void OnCreateImageVariationCompleted(const FImageVariationResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FOpenAIImageVariation ImageVariation;
    FOpenAIAuth Auth;
};

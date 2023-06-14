// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ImageAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImage, const FImageResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UImageAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnImage OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Images")
    static UImageAction* CreateImage(const FOpenAIImage& Image, const FOpenAIAuth& Auth);

    void OnCreateImageCompleted(const FImageResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FOpenAIImage Image;
    FOpenAIAuth Auth;
};

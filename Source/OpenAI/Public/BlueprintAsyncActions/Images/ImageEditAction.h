// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ImageEditAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageEdit, const FImageEditResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UImageEditAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnImageEdit OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Images")
    static UImageEditAction* CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth);

    void OnCreateImageEditCompleted(const FImageEditResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FOpenAIImageEdit ImageEdit;
    FOpenAIAuth Auth;
};

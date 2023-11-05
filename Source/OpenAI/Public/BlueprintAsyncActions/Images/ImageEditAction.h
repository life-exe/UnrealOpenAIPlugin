// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ImageEditAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageEdit, const FImageEditResponse&, Response, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UImageEditAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnImageEdit OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Images")
    static UImageEditAction* CreateImageEdit(const FOpenAIImageEdit& ImageEdit, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnCreateImageEditCompleted(const FImageEditResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FOpenAIImageEdit ImageEdit;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

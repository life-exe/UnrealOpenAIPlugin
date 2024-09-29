// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/ImageTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "ImageEditAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FOnImageEdit, const FImageEditResponse&, Response, const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

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

    void TryToOverrideURL();

    void OnCreateImageEditCompleted(const FImageEditResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;
    FOpenAIImageEdit ImageEdit;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

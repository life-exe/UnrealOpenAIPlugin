// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "BlueprintAsyncActions/OpenAIActionBase.h"
#include "Provider/Types/VideoTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "RemixVideoAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FOnRemixVideo, const FVideoObject&, Response, const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

UCLASS()
class OPENAI_API URemixVideoAction : public UOpenAIActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnRemixVideo OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Videos")
    static URemixVideoAction* RemixVideoAction(
        const FString& VideoId, const FRemixVideo& RemixVideo, const FOpenAIAuth& Auth, const FString& URLOverride);

    void OnRemixVideoActionCompleted(const FVideoObject& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    virtual void OnRequestError(const FString& URL, const FString& Content) override;
    virtual void SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const override;

private:
    FString VideoId;
    FRemixVideo RemixVideo;
};

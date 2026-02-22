// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "BlueprintAsyncActions/OpenAIActionBase.h"
#include "Provider/Types/UploadTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "AddUploadPartAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAddUploadPart, const FUploadPartObjectResponse&, Response, const FOpenAIResponseMetadata&,
    ResponseMetadata, const FOpenAIError&, RawError);

UCLASS()
class OPENAI_API UAddUploadPartAction : public UOpenAIActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnAddUploadPart OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Uploads")
    static UAddUploadPartAction* AddUploadPart(
        const FString& UploadId, const FAddUploadPart& UploadPartRequest, const FOpenAIAuth& Auth, const FString& URLOverride);

    void OnAddUploadPartCompleted(const FUploadPartObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    virtual void OnRequestError(const FString& URL, const FString& Content) override;
    virtual void SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const override;

private:
    FString UploadId;
    FAddUploadPart UploadPartRequest;
};

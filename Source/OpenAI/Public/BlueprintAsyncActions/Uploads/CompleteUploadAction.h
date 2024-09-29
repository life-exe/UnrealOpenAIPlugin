// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/UploadTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "CompleteUploadAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCompleteUpload, const FUploadObjectResponse&, Response, const FOpenAIResponseMetadata&,
    ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UCompleteUploadAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCompleteUpload OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Embeddings")
    static UCompleteUploadAction* CompleteUpload(
        const FString& UploadId, const FCompleteUpload& CompleteUploadRequest, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnCompleteUploadCompleted(const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString UploadId;
    FCompleteUpload CompleteUploadRequest;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

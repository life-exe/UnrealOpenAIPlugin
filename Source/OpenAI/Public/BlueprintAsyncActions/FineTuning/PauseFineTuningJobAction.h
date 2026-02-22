// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "BlueprintAsyncActions/OpenAIActionBase.h"
#include "Provider/Types/FineTuneTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "PauseFineTuningJobAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPauseFineTuningJob, const FFineTuningJobObjectResponse&, Response,
    const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

UCLASS()
class OPENAI_API UPauseFineTuningJobAction : public UOpenAIActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnPauseFineTuningJob OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UPauseFineTuningJobAction* PauseFineTuningJob(
        const FString& FineTuningJobID, const FOpenAIAuth& Auth, const FString& URLOverride);

    void OnPauseFineTuningJobCompleted(const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    virtual void OnRequestError(const FString& URL, const FString& Content) override;
    virtual void SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const override;

private:
    FString FineTuningJobID;
};

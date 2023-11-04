// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "CancelFineTuningJobAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnCancelFineTuningJob, const FFineTuningJobObjectResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UCancelFineTuningJobAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCancelFineTuningJob OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UCancelFineTuningJobAction* CancelFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth);

    void OnCancelFineTuningJobCompleted(const FFineTuningJobObjectResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FineTuningJobID;
    FOpenAIAuth Auth;
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "RetrieveFineTuningJobAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnRetrieveFineTuningJob, const FFineTuningJobObjectResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class URetrieveFineTuningJobAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnRetrieveFineTuningJob OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static URetrieveFineTuningJobAction* RetrieveFineTuningJob(const FString& FineTuningJobID, const FOpenAIAuth& Auth);

    void OnRetrieveFineTuningJobCompleted(const FFineTuningJobObjectResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FineTuningJobID;
    FOpenAIAuth Auth;
};

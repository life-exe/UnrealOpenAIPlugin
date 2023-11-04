// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "CreateFineTuningJobAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnCreateFineTuningJob, const FFineTuningJobObjectResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UCreateFineTuningJobAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCreateFineTuningJob OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UCreateFineTuningJobAction* CreateFineTuningJob(const FFineTuningJob& FineTuningJob, const FOpenAIAuth& Auth);

    void OnCreateFineTuningJobCompleted(const FFineTuningJobObjectResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FFineTuningJob FineTuningJob;
    FOpenAIAuth Auth;
};

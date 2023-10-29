// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "CancelFineTuneAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCancelFineTune, const FFineTuneResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UCancelFineTuneAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCancelFineTune OnCompleted;

    virtual void Activate() override;

private:
    UE_DEPRECATED("5.3", "Deprecated in OpenAI API, use fine-tuning job object requests instead")
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UCancelFineTuneAction* CancelFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth);

    void OnCancelFineTuneCompleted(const FFineTuneResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FineTuneID;
    FOpenAIAuth Auth;
};

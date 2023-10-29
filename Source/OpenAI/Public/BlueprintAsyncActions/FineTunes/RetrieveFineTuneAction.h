// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "RetrieveFineTuneAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRetrieveFineTune, const FFineTuneResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class URetrieveFineTuneAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnRetrieveFineTune OnCompleted;

    virtual void Activate() override;

private:
    UE_DEPRECATED("5.3", "Deprecated in OpenAI API, use fine-tuning job object requests instead")
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static URetrieveFineTuneAction* RetrieveFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth);

    void OnRetrieveFineTuneCompleted(const FFineTuneResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FineTuneID;
    FOpenAIAuth Auth;
};

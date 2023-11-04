// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "RetrieveFineTuneAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRetrieveFineTune, const FFineTuneResponse&, Response, const FOpenAIError&, RawError);

UCLASS(Deprecated, meta = (DeprecationMessage = "Deprecated in OpenAI API, use fine-tuning job object requests instead"))
class UDEPRECATED_RetrieveFineTuneAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnRetrieveFineTune OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UDEPRECATED_RetrieveFineTuneAction* RetrieveFineTune(const FString& FineTuneID, const FOpenAIAuth& Auth);

    void OnRetrieveFineTuneCompleted(const FFineTuneResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FineTuneID;
    FOpenAIAuth Auth;
};

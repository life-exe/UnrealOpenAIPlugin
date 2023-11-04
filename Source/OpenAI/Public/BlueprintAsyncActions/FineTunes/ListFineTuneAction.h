// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ListFineTuneAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFineTune, const FListFineTuneResponse&, Response, const FOpenAIError&, RawError);

UCLASS(Deprecated, meta = (DeprecationMessage = "Deprecated in OpenAI API, use fine-tuning job object requests instead"))
class UDEPRECATED_ListFineTuneAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnListFineTune OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UDEPRECATED_ListFineTuneAction* ListFineTunes(const FOpenAIAuth& Auth);

    void OnListFineTunesCompleted(const FListFineTuneResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FOpenAIAuth Auth;
};

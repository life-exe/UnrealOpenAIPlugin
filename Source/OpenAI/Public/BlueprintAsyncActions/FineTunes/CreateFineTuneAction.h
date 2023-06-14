// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
// #include "CreateFineTuneAction.generated.h"

/* !!! Currently is not available in blueprints !!!

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateFineTune, const FFineTuneResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UCreateFineTuneAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCreateFineTune OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UCreateFineTuneAction* CreateFineTune(const FFineTune& FineTune, const FOpenAIAuth& Auth);

    void OnCreateFineTuneCompleted(const FFineTuneResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FFineTune FineTune;
    FOpenAIAuth Auth;
};

*/

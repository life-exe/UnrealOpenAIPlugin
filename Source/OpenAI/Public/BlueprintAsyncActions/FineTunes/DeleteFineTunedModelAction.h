// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "DeleteFineTunedModelAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnDeleteFineTuneModel, const FDeleteFineTuneResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UDeleteFineTunedModelAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnDeleteFineTuneModel OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UDeleteFineTunedModelAction* DeleteFineTuneModel(const FString& ModelID, const FOpenAIAuth& Auth);

    void OnDeleteFineTunedModelCompleted(const FDeleteFineTuneResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString ModelID;
    FOpenAIAuth Auth;
};

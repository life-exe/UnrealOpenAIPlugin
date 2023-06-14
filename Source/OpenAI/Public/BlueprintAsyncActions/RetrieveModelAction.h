// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "RetrieveModelAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnRetrieveModelResponse, const FRetrieveModelResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class URetrieveModelAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnRetrieveModelResponse OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Models")
    static URetrieveModelAction* RetrieveModel(const FString& ModelName, const FOpenAIAuth& Auth);

    void OnRetrieveModelCompleted(const FRetrieveModelResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString ModelName;
    FOpenAIAuth Auth;
};

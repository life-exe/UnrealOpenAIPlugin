// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "CompletionAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompletion, const FCompletionResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UCompletionAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCompletion OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Completion")
    static UCompletionAction* CreateCompletion(const FCompletion& CompletionRequest, const FOpenAIAuth& Auth);

    void OnCreateCompletionCompleted(const FCompletionResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FCompletion Completion;
    FOpenAIAuth Auth;
};

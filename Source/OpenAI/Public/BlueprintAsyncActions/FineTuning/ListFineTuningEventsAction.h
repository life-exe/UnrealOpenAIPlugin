// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ListFineTuningEventsAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnListFineTuningEvents, const FFineTuneEventsResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UListFineTuningEventsAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnListFineTuningEvents OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UListFineTuningEventsAction* ListFineTuningEvents(const FString& FineTuningID, const FOpenAIAuth& Auth);

    void OnListFineTuningEventsCompleted(const FFineTuneEventsResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FineTuningID;
    FOpenAIAuth Auth;
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ListFineTuneEventsAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnListFineTuneEvents, const FFineTuneEventsResponse&, Response, const FOpenAIError&, RawError);

UCLASS(Deprecated, meta = (DeprecationMessage = "Deprecated in OpenAI API, use fine-tuning job object requests instead"))
class UDEPRECATED_ListFineTuneEventsAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnListFineTuneEvents OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UDEPRECATED_ListFineTuneEventsAction* ListFineTuneEvents(const FString& FineTuneID, const FOpenAIAuth& Auth);

    void OnListFineTuneEventsCompleted(const FFineTuneEventsResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FineTuneID;
    FOpenAIAuth Auth;
};

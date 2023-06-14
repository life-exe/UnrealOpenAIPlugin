// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ModerationsAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModerations, const FModerationsResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UModerationsAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnModerations OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Moderations")
    static UModerationsAction* CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth);

    void OnCreateModerationsCompleted(const FModerationsResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FModerations Moderations;
    FOpenAIAuth Auth;
};

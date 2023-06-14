// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "EditAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEdit, const FEditResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UEditAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnEdit OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Edit")
    static UEditAction* CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth);

    void OnCreateEditCompleted(const FEditResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FEdit Edit;
    FOpenAIAuth Auth;
};

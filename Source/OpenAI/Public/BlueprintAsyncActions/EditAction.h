// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "EditAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEdit, const FEditResponse&, Response, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS(Deprecated, meta = (DeprecationMessage = "Deprecated in OpenAI API"))
class UDEPRECATED_EditAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnEdit OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, BlueprintInternalUseOnly = "true"), Category = "OpenAI | Edit")
    static UDEPRECATED_EditAction* CreateEdit(const FEdit& Edit, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnCreateEditCompleted(const FEditResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FEdit Edit;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

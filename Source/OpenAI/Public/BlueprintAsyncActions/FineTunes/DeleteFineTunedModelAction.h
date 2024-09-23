// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/FineTuneTypes.h"
#include "DeleteFineTunedModelAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnDeleteFineTuneModel, const FDeleteFineTuneResponse&, Response, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UDeleteFineTunedModelAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnDeleteFineTuneModel OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UDeleteFineTunedModelAction* DeleteFineTuneModel(const FString& ModelID, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnDeleteFineTunedModelCompleted(const FDeleteFineTuneResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString ModelID;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

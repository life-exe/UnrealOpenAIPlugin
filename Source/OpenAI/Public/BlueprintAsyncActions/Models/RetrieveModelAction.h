// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "RetrieveModelAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRetrieveModel, const FRetrieveModelResponse&, Response, const FOpenAIResponseMetadata&,
    ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class URetrieveModelAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnRetrieveModel OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Models")
    static URetrieveModelAction* RetrieveModel(const FString& ModelName, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnRetrieveModelCompleted(const FRetrieveModelResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString ModelName;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

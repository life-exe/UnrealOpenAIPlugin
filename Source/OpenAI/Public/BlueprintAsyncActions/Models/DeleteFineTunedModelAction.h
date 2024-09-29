// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/FineTuneTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "DeleteFineTunedModelAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteFineTunedModel, const FDeleteFineTunedModelResponse&, Response,
    const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UDeleteFineTunedModelAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnDeleteFineTunedModel OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | FineTunes")
    static UDeleteFineTunedModelAction* DeleteFineTuneModel(const FString& ModelID, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnDeleteFineTunedModelCompleted(const FDeleteFineTunedModelResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString ModelID;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

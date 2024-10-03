// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/AssistantTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "DeleteAssistantAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteAssistant, const FDeleteAssistantResponse&, Response,
    const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UDeleteAssistantAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnDeleteAssistant OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UDeleteAssistantAction* DeleteAssistantAction(const FString& AssistantId, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL();

    void OnDeleteAssistantActionCompleted(const FDeleteAssistantResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;

    FString AssistantId;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

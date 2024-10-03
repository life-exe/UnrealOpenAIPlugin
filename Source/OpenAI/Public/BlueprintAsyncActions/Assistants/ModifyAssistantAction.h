// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/AssistantTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "ModifyAssistantAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnModifyAssistant, const FAssistantObjectResponse&, Response,
    const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UModifyAssistantAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnModifyAssistant OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UModifyAssistantAction* ModifyAssistantAction(
        const FString& AssistantId, const FModifyAssistant& ModifyAssistant, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL();

    void OnModifyAssistantCompleted(const FAssistantObjectResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;

    FModifyAssistant ModifyAssistant;
    FString AssistantId;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

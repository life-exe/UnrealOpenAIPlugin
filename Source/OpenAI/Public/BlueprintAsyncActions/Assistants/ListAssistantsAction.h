// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/AssistantTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "ListAssistantsAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnListAssistants, const FListAssistantsResponse&, Response,
    const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UListAssistantsAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnListAssistants OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UListAssistantsAction* ListAssistantsAction(
        const FListAssistants& ListAssistants, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL();

    void OnListAssistantsActionCompleted(const FListAssistantsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;

    FListAssistants ListAssistants;
    FOpenAIAuth Auth;
    FString URLOverride{};
};
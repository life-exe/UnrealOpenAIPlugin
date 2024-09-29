// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/ModerationTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "ModerationsAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FOnModerations, const FModerationsResponse&, Response, const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UModerationsAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnModerations OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Moderations")
    static UModerationsAction* CreateModerations(const FModerations& Moderations, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnCreateModerationsCompleted(const FModerationsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FModerations Moderations;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

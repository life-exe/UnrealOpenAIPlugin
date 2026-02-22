// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"
#include "OpenAIActionBase.generated.h"

UCLASS(Abstract)
class OPENAI_API UOpenAIActionBase : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

protected:
    FOpenAIAuth Auth;
    FString URLOverride;

    // Creates provider, stores it in ActiveProvider (preventing GC), binds OnRequestError, applies URL override.
    UOpenAIProvider* CreateProvider();

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> ActiveProvider;

    // Override to set the action-specific endpoint field.
    virtual void SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const {}

    // Override to broadcast to the action's specific delegate on error.
    virtual void OnRequestError(const FString& URL, const FString& Content) {}
};

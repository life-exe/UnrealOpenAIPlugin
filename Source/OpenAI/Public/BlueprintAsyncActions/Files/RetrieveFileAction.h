// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/FileTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "RetrieveFileAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRetrieveFile, const FRetrieveFileResponse&, Response, const FOpenAIResponseMetadata&,
    ResponseMetadata, const FOpenAIError&, RawError);
class UOpenAIProvider;

UCLASS()
class URetrieveFileAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnRetrieveFile OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Files")
    static URetrieveFileAction* RetrieveFile(const FString& FileID, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnRetrieveFileCompleted(const FRetrieveFileResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FOpenAIAuth Auth;
    FString FileID;
    FString URLOverride{};
};

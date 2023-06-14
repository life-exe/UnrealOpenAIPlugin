// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "ListFilesAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFiles, const FListFilesResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UListFilesAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnListFiles OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Files")
    static UListFilesAction* ListFiles(const FOpenAIAuth& Auth);

    void OnListFilesCompleted(const FListFilesResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FOpenAIAuth Auth;
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "DeleteFileAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeleteFile, const FDeleteFileResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UDeleteFileAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnDeleteFile OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Files")
    static UDeleteFileAction* DeleteFile(const FString& FileID, const FOpenAIAuth& Auth);

    void OnDeleteFileCompleted(const FDeleteFileResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FString FileID;
    FOpenAIAuth Auth;
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "UploadFileAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUploadFile, const FUploadFileResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UUploadFileAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnUploadFile OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Files")
    static UUploadFileAction* UploadFile(const FUploadFile& File, const FOpenAIAuth& Auth);

    void OnUploadFileCompleted(const FUploadFileResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FUploadFile File;
    FOpenAIAuth Auth;
};

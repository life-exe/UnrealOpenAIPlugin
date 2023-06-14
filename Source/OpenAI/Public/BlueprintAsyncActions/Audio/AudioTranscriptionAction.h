// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "AudioTranscriptionAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnAudioTranscription, const FAudioTranscriptionResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UAudioTranscriptionAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnAudioTranscription OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UAudioTranscriptionAction* CreateAudioTranscription(const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth);

    void OnCreateAudioTranscriptionCompleted(const FAudioTranscriptionResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FAudioTranscription AudioTranscription;
    FOpenAIAuth Auth;
};

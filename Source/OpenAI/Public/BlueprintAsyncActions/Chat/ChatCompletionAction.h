// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "Provider/CommonTypes.h"
#include "ChatCompletionAction.generated.h"

USTRUCT(BlueprintType)
struct FChatCompletionPayload
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FChatCompletionResponse Response;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString StreamResponse{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool bStream{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool bCompleted{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatCompletion, const FChatCompletionPayload&, Payload, const FOpenAIError&, RawError);

UCLASS()
class UChatCompletionAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnChatCompletion OnUpdate;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Chat")
    static UChatCompletionAction* CreateChatCompletion(const FChatCompletion& ChatCompletion, const FOpenAIAuth& Auth);

    void OnCreateChatCompletionCompleted(const FChatCompletionResponse& Response);
    void OnCreateChatCompletionStreamProgresses(const TArray<FChatCompletionStreamResponse>& Responses);
    void OnCreateChatCompletionStreamCompleted(const TArray<FChatCompletionStreamResponse>& Responses);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FChatCompletion ChatCompletion;
    FOpenAIAuth Auth;

    FString ParseResponses(const TArray<FChatCompletionStreamResponse>& Responses) const;
};

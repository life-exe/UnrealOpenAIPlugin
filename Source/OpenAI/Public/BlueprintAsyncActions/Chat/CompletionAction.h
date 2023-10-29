// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "CompletionAction.generated.h"

USTRUCT(BlueprintType)
struct FCompletionPayload
{
    GENERATED_BODY()

    /** Response struct to be used (if bStream = false) */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FCompletionResponse Response;

    /** Array of response structs to be used (if bStream = true) */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FCompletionStreamResponse> StreamResponse;

    /** Response structs combined into the plain string (if bStream = true) */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString StreamResponseString{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool bStream{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool bCompleted{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompletion, const FCompletionPayload&, Payload, const FOpenAIError&, RawError);

UCLASS()
class UCompletionAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCompletion OnUpdate;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Completion")
    static UCompletionAction* CreateCompletion(const FCompletion& CompletionRequest, const FOpenAIAuth& Auth);

    void OnCreateCompletionCompleted(const FCompletionResponse& Response);
    void OnCreateCompletionStreamProgresses(const TArray<FCompletionStreamResponse>& Responses);
    void OnCreateCompletionStreamCompleted(const TArray<FCompletionStreamResponse>& Responses);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FCompletion Completion;
    FOpenAIAuth Auth;
};

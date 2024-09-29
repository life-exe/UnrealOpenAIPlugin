// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/Chat/ChatCompletionTypes.h"
#include "Provider/Types/Chat/ChatCompletionChunkTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "ChatCompletionAction.generated.h"

USTRUCT(BlueprintType)
struct FChatCompletionPayload
{
    GENERATED_BODY()

    /** Response struct to be used (if bStream = false) */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FChatCompletionResponse Response;

    /** Array of response structs to be used (if bStream = true) */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChatCompletionStreamResponse> StreamResponse;

    /** Response structs combined into the plain string (if bStream = true) */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString StreamResponseString{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool bStream{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool bCompleted{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChatCompletion, const FChatCompletionPayload&, Payload, const FOpenAIResponseMetadata&,
    ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UChatCompletionAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnChatCompletion OnUpdate;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Chat")
    static UChatCompletionAction* CreateChatCompletion(
        const FChatCompletion& ChatCompletion, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnCreateChatCompletionCompleted(const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnCreateChatCompletionStreamProgresses(
        const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnCreateChatCompletionStreamCompleted(
        const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FChatCompletion ChatCompletion;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

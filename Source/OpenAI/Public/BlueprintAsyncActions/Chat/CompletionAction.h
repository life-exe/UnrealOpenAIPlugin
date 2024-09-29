// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/Legacy/CompletionTypes.h"
#include "Provider/Types/CommonTypes.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FOnCompletion, const FCompletionPayload&, Payload, const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UCompletionAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCompletion OnUpdate;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Completion")
    static UCompletionAction* CreateCompletion(const FCompletion& CompletionRequest, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnCreateCompletionCompleted(const FCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnCreateCompletionStreamProgresses(
        const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnCreateCompletionStreamCompleted(
        const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FCompletion Completion;
    FOpenAIAuth Auth;
    FString URLOverride{};
};

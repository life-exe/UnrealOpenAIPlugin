// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/Chat/ChatCompletionChunkTypes.h"
#include "ChatGPTWorld.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGPTRequestUpdated, const FMessage&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGPTRequestCompleted, const FMessage&, Message);

class UChatGPT;

UCLASS(Blueprintable, BlueprintType)
class OPENAI_API AChatGPTWorld : public AInfo
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OpenAI")
    EMainModelEnum Model{EMainModelEnum::GPT_4_Vision_Preview};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OpenAI")
    int32 MaxTokens{2000};

    UFUNCTION(BlueprintCallable, Category = "OpenAI")
    void SetAuth(const FOpenAIAuth& OpenAIAuth);

    UFUNCTION(BlueprintCallable, Category = "OpenAI")
    void SetModel(const FString& ModelName);

    UFUNCTION(BlueprintCallable, Category = "OpenAI")
    bool IsInProgress() const { return bIsInProgress; }

    UFUNCTION(BlueprintCallable, Category = "OpenAI")
    void MakeRequest(const FMessage& Message);

    UFUNCTION(BlueprintCallable, Category = "OpenAI")
    FMessage GetLastMessage() const { return LastMessage; }

protected:
    UPROPERTY(BlueprintAssignable)
    FOnGPTRequestUpdated OnGPTRequestUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnGPTRequestCompleted OnGPTRequestCompleted;

private:
    FOpenAIAuth OpenAIAuth;

    UPROPERTY()
    TObjectPtr<UChatGPT> ChatGPT;

    bool bIsInProgress{false};
    FMessage LastMessage;

private:
    void OnRequestCompleted();
    void OnRequestUpdated(const FMessage& Message, bool WasError);
};

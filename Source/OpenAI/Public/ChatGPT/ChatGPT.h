// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/Types/Chat/ChatCommonTypes.h"
#include "Logging/LogVerbosity.h"
#include "Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "ChatGPT.generated.h"

class UOpenAIProvider;
class UBaseService;

DECLARE_MULTICAST_DELEGATE(FOnChatGPTRequestCompleted);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChatGPTRequestUpdated, const FMessage&, bool);

UCLASS()
class OPENAI_API UChatGPT : public UObject
{
    GENERATED_BODY()

public:
    UChatGPT();

    void SetAuth(const FOpenAIAuth& OpenAIAuth);
    void SetModel(const FString& Model);
    FString GetModel() const;
    void SetMaxTokens(int32 Tokens);

    void SetLogEnabled(bool Enabled);

    bool RegisterService(const TSubclassOf<UBaseService>& ServiceClass, const OpenAI::ServiceSecrets& Secrets);
    void UnRegisterService(const TSubclassOf<UBaseService>& ServiceClass);

    void AddMessage(const FMessage& Message);
    void SetAssistantMessage(const FMessage& Message);
    FMessage GetAssistantMessage() const;

    void MakeRequest();

    void ClearHistory();
    TArray<FMessage> GetHistory() const;

    FOnChatGPTRequestCompleted& OnRequestCompleted() { return RequestCompleted; }
    FOnChatGPTRequestUpdated& OnRequestUpdated() { return RequestUpdated; }

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;

    UPROPERTY()
    TArray<TObjectPtr<UBaseService>> Services;

    FOpenAIAuth Auth;
    FString OpenAIModel;
    int32 MaxCompletionTokens{100};

    TArray<FMessage> ChatHistory;
    FMessage AssistantMessage;

private:
    FOnChatGPTRequestCompleted RequestCompleted;
    FOnChatGPTRequestUpdated RequestUpdated;

    void HandleRequestCompletion();
    void UpdateAssistantMessage(const FString& Message, bool WasError = false);

    void HandleError(const FString& Content);
    bool HandleFunctionCall(const FFunctionCommon& FunctionCall, const FString& ID);
};

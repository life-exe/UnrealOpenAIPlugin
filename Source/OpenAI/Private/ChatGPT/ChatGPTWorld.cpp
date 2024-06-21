// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/ChatGPTWorld.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "ChatGPT/ChatGPT.h"

void AChatGPTWorld::BeginPlay()
{
    Super::BeginPlay();

    ChatGPT = NewObject<UChatGPT>();
    check(ChatGPT);

    ChatGPT->SetLogEnabled(true);
    ChatGPT->SetMaxTokens(MaxTokens);
    ChatGPT->SetModel(UOpenAIFuncLib::OpenAIMainModelToString(Model));

    ChatGPT->OnRequestCompleted().AddUObject(this, &ThisClass::OnRequestCompleted);
    ChatGPT->OnRequestUpdated().AddUObject(this, &ThisClass::OnRequestUpdated);
}

void AChatGPTWorld::SetAuth(const FOpenAIAuth& Auth)
{
    if (ChatGPT)
    {
        ChatGPT->SetAuth(Auth);
    }
}

void AChatGPTWorld::SetModel(const FString& ModelName)
{
    if (ChatGPT)
    {
        ChatGPT->SetModel(ModelName);
    }
}

void AChatGPTWorld::MakeRequest(const FMessage& Message)
{
    if (bIsInProgress) return;

    bIsInProgress = true;

    ChatGPT->AddMessage(Message);

    const auto AssistantMessage = FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant), {}};
    ChatGPT->SetAssistantMessage(AssistantMessage);

    ChatGPT->MakeRequest();
}

void AChatGPTWorld::OnRequestUpdated(const FMessage& Message, bool WasError)
{
    LastMessage = Message;
    OnGPTRequestUpdated.Broadcast(Message);
}

void AChatGPTWorld::OnRequestCompleted()
{
    bIsInProgress = false;
    OnGPTRequestCompleted.Broadcast(LastMessage);
}

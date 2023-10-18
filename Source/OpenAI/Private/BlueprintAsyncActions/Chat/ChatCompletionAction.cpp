// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Chat/ChatCompletionAction.h"
#include "Provider/OpenAIProvider.h"
#include "Algo/ForEach.h"

UChatCompletionAction* UChatCompletionAction::CreateChatCompletion(const FChatCompletion& ChatCompletion, const FOpenAIAuth& Auth)
{
    auto* ChatCompletionAction = NewObject<UChatCompletionAction>();
    ChatCompletionAction->ChatCompletion = ChatCompletion;
    ChatCompletionAction->Auth = Auth;
    return ChatCompletionAction;
}

void UChatCompletionAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    if (ChatCompletion.Stream)
    {
        Provider->OnCreateChatCompletionStreamProgresses().AddUObject(this, &ThisClass::OnCreateChatCompletionStreamProgresses);
        Provider->OnCreateChatCompletionStreamCompleted().AddUObject(this, &ThisClass::OnCreateChatCompletionStreamCompleted);
    }
    else
    {
        Provider->OnCreateChatCompletionCompleted().AddUObject(this, &ThisClass::OnCreateChatCompletionCompleted);
    }
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateChatCompletion(ChatCompletion, Auth);
}

void UChatCompletionAction::OnCreateChatCompletionCompleted(const FChatCompletionResponse& Response)
{
    FChatCompletionPayload Payload;
    Payload.bStream = false;
    Payload.bCompleted = true;
    Payload.Response = Response;
    OnUpdate.Broadcast(Payload, {});
}

void UChatCompletionAction::OnCreateChatCompletionStreamProgresses(const TArray<FChatCompletionStreamResponse>& Responses)
{
    FChatCompletionPayload Payload;
    Payload.bStream = true;
    Payload.bCompleted = false;
    Payload.StreamResponseString = ParseResponses(Responses);
    Payload.StreamResponse = Responses;
    OnUpdate.Broadcast(Payload, {});
}

void UChatCompletionAction::OnCreateChatCompletionStreamCompleted(const TArray<FChatCompletionStreamResponse>& Responses)
{
    FChatCompletionPayload Payload;
    Payload.bStream = true;
    Payload.bCompleted = true;
    Payload.StreamResponseString = ParseResponses(Responses);
    Payload.StreamResponse = Responses;
    OnUpdate.Broadcast(Payload, {});
}

FString UChatCompletionAction::ParseResponses(const TArray<FChatCompletionStreamResponse>& Responses) const
{
    FString OutputString{};
    Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
        Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
            OutputString.Append(Choice.Delta.Content);
        });
    });
    return OutputString;
}

void UChatCompletionAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnUpdate.Broadcast({}, FOpenAIError{Content, true});
}

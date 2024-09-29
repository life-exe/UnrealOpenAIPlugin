// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Chat/ChatCompletionAction.h"
#include "Provider/OpenAIProvider.h"
#include "Algo/ForEach.h"
#include "API/API.h"

namespace
{
FString ParseResponses(const TArray<FChatCompletionStreamResponse>& Responses)
{
    FString OutputString{};
    Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
        Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
            OutputString.Append(Choice.Delta.Content);
        });
    });
    return OutputString;
}
}  // namespace

UChatCompletionAction* UChatCompletionAction::CreateChatCompletion(
    const FChatCompletion& ChatCompletion, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ChatCompletionAction = NewObject<UChatCompletionAction>();
    ChatCompletionAction->ChatCompletion = ChatCompletion;
    ChatCompletionAction->Auth = Auth;
    ChatCompletionAction->URLOverride = URLOverride;
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
    TryToOverrideURL(Provider);
    Provider->CreateChatCompletion(ChatCompletion, Auth);
}

void UChatCompletionAction::OnCreateChatCompletionCompleted(
    const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    FChatCompletionPayload Payload;
    Payload.bStream = false;
    Payload.bCompleted = true;
    Payload.Response = Response;
    OnUpdate.Broadcast(Payload, ResponseMetadata, {});
}

void UChatCompletionAction::OnCreateChatCompletionStreamProgresses(
    const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
{
    FChatCompletionPayload Payload;
    Payload.bStream = true;
    Payload.bCompleted = false;
    Payload.StreamResponseString = ParseResponses(Responses);
    Payload.StreamResponse = Responses;
    OnUpdate.Broadcast(Payload, ResponseMetadata, {});
}

void UChatCompletionAction::OnCreateChatCompletionStreamCompleted(
    const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
{
    FChatCompletionPayload Payload;
    Payload.bStream = true;
    Payload.bCompleted = true;
    Payload.StreamResponseString = ParseResponses(Responses);
    Payload.StreamResponse = Responses;
    OnUpdate.Broadcast(Payload, ResponseMetadata, {});
}

void UChatCompletionAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnUpdate.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UChatCompletionAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.ChatCompletions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

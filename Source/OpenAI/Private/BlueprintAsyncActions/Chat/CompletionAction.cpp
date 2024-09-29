// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Chat/CompletionAction.h"
#include "Provider/OpenAIProvider.h"
#include "Algo/ForEach.h"
#include "API/API.h"

namespace
{
FString ParseResponses(const TArray<FCompletionStreamResponse>& Responses)
{
    FString OutputString{};
    Algo::ForEach(Responses, [&](const FCompletionStreamResponse& StreamResponse) {  //
        Algo::ForEach(StreamResponse.Choices, [&](const FChoice& Choice) {           //
            OutputString.Append(Choice.Text);
        });
    });
    return OutputString;
}
}  // namespace

UCompletionAction* UCompletionAction::CreateCompletion(const FCompletion& Completion, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompletionAction = NewObject<UCompletionAction>();
    CompletionAction->Completion = Completion;
    CompletionAction->Auth = Auth;
    CompletionAction->URLOverride = URLOverride;
    return CompletionAction;
}

void UCompletionAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    if (Completion.Stream)
    {
        Provider->OnCreateCompletionStreamProgresses().AddUObject(this, &ThisClass::OnCreateCompletionStreamProgresses);
        Provider->OnCreateCompletionStreamCompleted().AddUObject(this, &ThisClass::OnCreateCompletionStreamCompleted);
    }
    else
    {
        Provider->OnCreateCompletionCompleted().AddUObject(this, &ThisClass::OnCreateCompletionCompleted);
    }

    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->CreateCompletion(Completion, Auth);
}

void UCompletionAction::OnCreateCompletionCompleted(const FCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    FCompletionPayload Payload;
    Payload.bStream = false;
    Payload.bCompleted = true;
    Payload.Response = Response;
    OnUpdate.Broadcast(Payload, ResponseMetadata, {});
}

void UCompletionAction::OnCreateCompletionStreamProgresses(
    const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
{
    FCompletionPayload Payload;
    Payload.bStream = true;
    Payload.bCompleted = false;
    Payload.StreamResponseString = ParseResponses(Responses);
    Payload.StreamResponse = Responses;
    OnUpdate.Broadcast(Payload, ResponseMetadata, {});
}

void UCompletionAction::OnCreateCompletionStreamCompleted(
    const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
{
    FCompletionPayload Payload;
    Payload.bStream = true;
    Payload.bCompleted = true;
    Payload.StreamResponseString = ParseResponses(Responses);
    Payload.StreamResponse = Responses;
    OnUpdate.Broadcast(Payload, ResponseMetadata, {});
}

void UCompletionAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnUpdate.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCompletionAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Completions = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

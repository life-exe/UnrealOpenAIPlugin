// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/EmbeddingsAction.h"

UEmbeddingsAction* UEmbeddingsAction::CreateEmbeddings(const FEmbeddings& Embeddings, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* CompletionAction = NewObject<UEmbeddingsAction>();
    CompletionAction->Embeddings = Embeddings;
    CompletionAction->Auth = Auth;
    CompletionAction->URLOverride = URLOverride;
    return CompletionAction;
}

void UEmbeddingsAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateEmbeddingsCompleted().AddUObject(this, &ThisClass::OnCreateEmbeddingsCompleted);
    Provider->CreateEmbeddings(Embeddings, Auth);
}

void UEmbeddingsAction::OnCreateEmbeddingsCompleted(const FEmbeddingsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UEmbeddingsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UEmbeddingsAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.Embeddings = URL;
}

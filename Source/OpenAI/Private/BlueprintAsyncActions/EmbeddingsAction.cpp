// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/EmbeddingsAction.h"
#include "Provider/OpenAIProvider.h"

UEmbeddingsAction* UEmbeddingsAction::CreateEmbeddings(const FEmbeddings& Embeddings, const FOpenAIAuth& Auth)
{
    auto* CompletionAction = NewObject<UEmbeddingsAction>();
    CompletionAction->Embeddings = Embeddings;
    CompletionAction->Auth = Auth;
    return CompletionAction;
}

void UEmbeddingsAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateEmbeddingsCompleted().AddUObject(this, &ThisClass::OnCreateEmbeddingsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->CreateEmbeddings(Embeddings, Auth);
}

void UEmbeddingsAction::OnCreateEmbeddingsCompleted(const FEmbeddingsResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UEmbeddingsAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

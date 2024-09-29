// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/EmbeddingsAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

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
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateEmbeddingsCompleted().AddUObject(this, &ThisClass::OnCreateEmbeddingsCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
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

void UEmbeddingsAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.Embeddings = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}

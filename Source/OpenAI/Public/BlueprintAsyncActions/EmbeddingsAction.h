// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "EmbeddingsAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmbeddings, const FEmbeddingsResponse&, Response, const FOpenAIError&, RawError);

UCLASS()
class UEmbeddingsAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnEmbeddings OnCompleted;

    virtual void Activate() override;

private:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Embeddings")
    static UEmbeddingsAction* CreateEmbeddings(const FEmbeddings& Embeddings, const FOpenAIAuth& Auth);

    void OnCreateEmbeddingsCompleted(const FEmbeddingsResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FEmbeddings Embeddings;
    FOpenAIAuth Auth;
};

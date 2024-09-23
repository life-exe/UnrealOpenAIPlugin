// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EmbeddingTypes.generated.h"

UENUM(BlueprintType)
enum class EEmbeddingsEncodingFormat : uint8
{
    Float,
    Base64
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FEmbeddings
{
    GENERATED_BODY()

    /**
      ID of the model to use. You can use the List models API to see all of your available models,
      or see our Model overview for descriptions of them.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model;

    /**
      Input text to embed, encoded as a string or array of tokens.
      To embed multiple inputs in a single request, pass an array of strings or array of token arrays.
      The input must not exceed the max input tokens for the model (8192 tokens for text-embedding-ada-002)
      and cannot be an empty string.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    TArray<FString> Input;

    /**
      The format to return the embeddings in. Can be either float or base64.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Encoding_Format{"float"};

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString User;
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FEmbeddingsUsage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Prompt_Tokens{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Total_Tokens{};
};

USTRUCT(BlueprintType)
struct FEmbeddingsData
{
    GENERATED_BODY()

    /**
      The object type, which is always "embedding".
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The index of the embedding in the list of embeddings.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Index{};

    /**
      The embedding vector, which is a list of floats. The length of vector depends on the model as listed in the embedding guide:
      https://platform.openai.com/docs/guides/embeddings
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<float> Embedding;
};

USTRUCT(BlueprintType)
struct FEmbeddingsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FEmbeddingsData> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FEmbeddingsUsage Usage;
};

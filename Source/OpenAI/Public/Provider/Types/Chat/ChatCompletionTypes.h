// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/CommonTypes.h"
#include "ChatCommonTypes.h"
#include "ChatCompletionTypes.generated.h"

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FChatChoice
{
    GENERATED_BODY()

    /**
      The reason the model stopped generating tokens.
      This will be stop if the model hit a natural stop point or a provided stop sequence,
      length if the maximum number of tokens specified in the request was reached,
      content_filter if content was omitted due to a flag from our content filters,
      or function_call if the model called a function.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Finish_Reason{};

    /**
      The index of the choice in the list of choices.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Index{};

    /**
      A chat completion message generated by the model.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FMessage Message;

    /**
      Log probability information for the choice.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FLogProbs Logprobs;
};

USTRUCT(BlueprintType)
struct FCompletionTokensDetails
{
    GENERATED_BODY()

    /**
      Audio input tokens generated by the model.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Audio_Tokens{};

    /**
      Tokens generated by the model for reasoning.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Reasoning_Tokens{};
};

USTRUCT(BlueprintType)
struct FPromptTokensDetails
{
    GENERATED_BODY()

    /**
      Audio input tokens present in the prompt.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Audio_Tokens{};

    /**
      Cached tokens present in the prompt.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Cached_Tokens{};
};

USTRUCT(BlueprintType)
struct FCompletionUsage : public FUsage
{
    GENERATED_BODY()

    /**
       Breakdown of tokens used in a completion.
     */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FCompletionTokensDetails Completion_Tokens_Details;

    /**
       Breakdown of tokens used in the prompt.
     */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FPromptTokensDetails Prompt_Tokens_Details;
};

USTRUCT(BlueprintType)
struct FChatCompletionResponse : public FChatCompletionResponseBase
{
    GENERATED_BODY()
    /**
       A list of chat completion choices. Can be more than one if n is greater than 1.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChatChoice> Choices;

    /**
       Usage statistics for the completion request.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FCompletionUsage Usage;
};
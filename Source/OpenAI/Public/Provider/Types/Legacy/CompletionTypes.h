// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/Types/Chat/ChatTypesHeader.h"
#include "CompletionTypes.generated.h"

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCompletion
{
    GENERATED_BODY()

    /**
      ID of the model to use.
      You can use the List models API to see all of your available models,
      or see our Model overview for descriptions of them.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model;

    /**
      The prompt(s) to generate completions for, encoded as a string,
      array of strings, array of tokens, or array of token arrays.

      Note that <|endoftext|> is the document separator that the model sees during training,
      so if a prompt is not specified the model will generate as if from the beginning of a new document.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Prompt;

    /**
      The suffix that comes after a completion of inserted text.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Suffix;

    /**
      The maximum number of tokens to generate in the chat completion.
      The total length of input tokens and generated tokens is limited by the model's context length.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 Max_Tokens{16};

    /**
      What sampling temperature to use, between 0 and 2.
      Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic.
      We generally recommend altering this or top_p but not both.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Temperature{1.0f};

    /**
      An alternative to sampling with temperature, called nucleus sampling,
      where the model considers the results of the tokens with top_p probability mass.
      So 0.1 means only the tokens comprising the top 10% probability mass are considered.
      We generally recommend altering this or temperature but not both.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Top_P{1.0f};

    /**
      How many completions to generate for each prompt.
      Note: Because this parameter generates many completions,
      it can quickly consume your token quota.
      Use carefully and ensure that you have reasonable settings for max_tokens and stop.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      Whether to stream back partial progress.
      If set, tokens will be sent as data-only server-sent events as they become available,
      with the stream terminated by a data: [DONE] message.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    bool Stream{false};

    /**
      Include the log probabilities on the logprobs most likely tokens, as well the chosen tokens.
      For example, if logprobs is 5, the API will return a list of the 5 most likely tokens.
      The API will always return the logprob of the sampled token, so there may be up to logprobs+1 elements in the response.
      The maximum value for logprobs is 5.
      If you need more than this, please contact us through our Help center and describe your use case.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 Logprobs{0};

    /**
      Echo back the prompt in addition to the completion.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    bool Echo{false};

    /**
      Number between -2.0 and 2.0.
      Positive values penalize new tokens based on whether they appear in the text so far,
      increasing the model's likelihood to talk about new topics.

      See more information about frequency and presence penalties:
      https://platform.openai.com/docs/api-reference/parameter-details
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Presence_Penalty{0.0f};

    /**
      Number between -2.0 and 2.0.
      Positive values penalize new tokens based on their existing frequency in the text so far,
      decreasing the model's likelihood to repeat the same line verbatim.

      See more information about frequency and presence penalties:
      https://platform.openai.com/docs/api-reference/parameter-details
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Frequency_Penalty{0.0f};

    /**
      Generates best_of completions server-side and returns the "best"
      (the one with the highest log probability per token).
      Results cannot be streamed.
      When used with n, best_of controls the number of candidate completions
      and n specifies how many to return – best_of must be greater than n.

      Note: Because this parameter generates many completions, it can quickly consume your token quota.
      Use carefully and ensure that you have reasonable settings for max_tokens and stop.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 Best_Of{1};

    /**
      Modify the likelihood of specified tokens appearing in the completion.
      Accepts a json object that maps tokens
      (specified by their token ID in the GPT tokenizer) to an associated bias value from -100 to 100.
      You can use this tokenizer tool (which works for both GPT-2 and GPT-3) to convert text to token IDs.
      Mathematically, the bias is added to the logits generated by the model prior to sampling.
      The exact effect will vary per model, but values between -1 and 1 should decrease or increase
      likelihood of selection; values like -100 or 100 should result in a ban or exclusive selection of the relevant token.

      As an example, you can pass {"50256": -100} to prevent the <|endoftext|> token from being generated.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, int32> Logit_Bias;

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString User;

    /**
      Up to 4 sequences where the API will stop generating further tokens. The returned text will not contain the stop sequence.
    */
    // UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    // TArray<FString> Stop;
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCompletionLogProbs
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<int32> Text_Offset;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<double> Token_Logprobs;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FString> Tokens;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Top_Logprobs{};
};

USTRUCT(BlueprintType)
struct FBaseChoice
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Text{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Index{};
};

USTRUCT(BlueprintType)
struct FChoice : public FBaseChoice
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FCompletionLogProbs LogProbs;

    /**
      The reason the model stopped generating tokens.
      This will be stop if the model hit a natural stop point or a provided stop sequence,
      length if the maximum number of tokens specified in the request was reached, or
      content_filter if content was omitted due to a flag from our content filters.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Finish_Reason{};
};

USTRUCT(BlueprintType)
struct FCompletionResponseBase
{
    GENERATED_BODY()

    /**
       A unique identifier for the completion.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID{};

    /**
      The list of completion choices the model generated for the input prompt.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FChoice> Choices;

    /**
      The Unix timestamp (in seconds) of when the completion was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created{};

    /**
       The model used for completion.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model{};

    /**
       The object type, which is always "chat.completion"
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object{};
};

USTRUCT(BlueprintType)
struct FCompletionResponse : public FCompletionResponseBase
{
    GENERATED_BODY()

    /**
       Usage statistics for the completion request.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FCompletionUsage Usage;
};

USTRUCT(BlueprintType)
struct FCompletionStreamResponse : public FCompletionResponseBase
{
    GENERATED_BODY()
};

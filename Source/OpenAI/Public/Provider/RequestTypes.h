// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.h"
#include "ResponseTypes.h"
#include "RequestTypes.generated.h"

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

USTRUCT(BlueprintType)
struct FFunctionOpenAI
{
    GENERATED_BODY()

    /**
      A description of what the function does, used by the model to choose when and how to call the function.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Description;

    /**
      The name of the function to be called. Must be a-z, A-Z, 0-9, or contain underscores and dashes, with a maximum length of 64.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Name;

    /**
      The parameters the functions accepts, described as a JSON Schema object.
      See the guide for examples:
      https://platform.openai.com/docs/guides/gpt/function-calling

      and the JSON Schema reference for documentation about the format:
      https://json-schema.org/understanding-json-schema

      To describe a function that accepts no parameters, provide the value {"type": "object", "properties": {}}.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Parameters;
};

USTRUCT(BlueprintType)
struct FChatCompletion
{
    GENERATED_BODY()

    /**
      ID of the model to use.
      See the model endpoint compatibility table for details on which models work with the Chat API.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model;

    /**
        A list of messages comprising the conversation so far.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    TArray<FMessage> Messages;

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
      How many chat completion choices to generate for each input message.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      If set, partial message deltas will be sent, like in ChatGPT.
      Tokens will be sent as data-only server-sent events as they become available,
      with the stream terminated by a data: [DONE] message.
      See the OpenAI Cookbook for example code
      [https://github.com/openai/openai-cookbook/blob/main/examples/How_to_stream_completions.ipynb]
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    bool Stream{false};

    /**
      The maximum number of tokens to generate in the chat completion.
      The total length of input tokens and generated tokens is limited by the model's context length.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 Max_Tokens{1};

    /**
      Number between -2.0 and 2.0.
      Positive values penalize new tokens based on whether they appear in the text so far,
      increasing the model's likelihood to talk about new topics.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Presence_Penalty{0.0f};

    /**
      Number between -2.0 and 2.0.
      Positive values penalize new tokens based on their existing frequency in the text so far,
      decreasing the model's likelihood to repeat the same line verbatim.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Frequency_Penalty{0.0f};

    /**
      Modify the likelihood of specified tokens appearing in the completion.
      Accepts a json object that maps tokens (specified by their token ID in the tokenizer)
      to an associated bias value from -100 to 100. Mathematically, the bias is added to
      the logits generated by the model prior to sampling.
      The exact effect will vary per model, but values between -1 and 1 should decrease
      or increase likelihood of selection; values like -100 or 100 should result in a ban
      or exclusive selection of the relevant token.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, int32> Logit_Bias;

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString User;

    /** Up to 4 sequences where the API will stop generating further tokens. */
    UPROPERTY(BlueprintReadWrite, Category = "Optional")
    TArray<FString> Stop;

    /**
      Controls how the model calls functions.
      "none" means the model will not call a function and instead generates a message.
      "auto" means the model can pick between generating a message or calling a function.
      Specifying a particular function via {"name": "my_function"} forces the model to call that function.
      "none" is the default when no functions are present. "auto" is the default if functions are present.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FFunctionCall Function_Call;

    /**
      A list of functions the model may generate JSON inputs for.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FFunctionOpenAI> Functions;
};

USTRUCT(BlueprintType)
struct FEdit
{
    GENERATED_BODY()

    /**
      ID of the model to use.
      You can use the text-davinci-edit-001 or code-davinci-edit-001 model with this endpoint.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model{"text-davinci-edit-001"};

    /** The input text to use as a starting point for the edit. */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Input;

    /** The instruction that tells the model how to edit the prompt. */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Instruction;

    /** How many edits to generate for the input and instruction. */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

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
};

USTRUCT(BlueprintType)
struct FOpenAIImage
{
    GENERATED_BODY()

    /**
      A text description of the desired image(s). The maximum length is 1000 characters for dall-e-2 and 4000 characters for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt;

    /**
      The model to use for image generation.
      Defaults to dall-e-2.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      The number of images to generate. Must be between 1 and 10.
      For dall-e-3, only n=1 is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The quality of the image that will be generated. hd creates images with finer details and greater consistency across the image.
      This param is only supported for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Quality{"standard"};

    /**
      The format in which the generated images are returned. Must be one of url or b64_json.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"url"};

    /**
      The size of the generated images. Must be one of 256x256, 512x512, or 1024x1024 for dall-e-2.
      Must be one of 1024x1024, 1792x1024, or 1024x1792 for dall-e-3 models.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

    /**
      The style of the generated images.
      Must be one of vivid or natural.
      Vivid causes the model to lean towards generating hyper-real and dramatic images.
      Natural causes the model to produce more natural, less hyper-real looking images.
      This param is only supported for dall-e-3.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Style{"vivid"};

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString User;
};

USTRUCT(BlueprintType)
struct FOpenAIImageEdit
{
    GENERATED_BODY()

    /**
      The image to edit. Must be a valid PNG file, less than 4MB, and square.
      If mask is not provided, image must have transparency, which will be used as the mask.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Image;

    /**
      A text description of the desired image(s). The maximum length is 1000 characters.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Prompt;

    /**
      An additional image whose fully transparent areas (e.g. where alpha is zero) indicate where image should be edited.
      Must be a valid PNG file, less than 4MB, and have the same dimensions as image.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Mask;

    /**
      The model to use for image generation.
      Defaults to dall-e-2.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      The number of images to generate. Must be between 1 and 10.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The size of the generated images. Must be one of 256x256, 512x512, or 1024x1024.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

    /**
      The format in which the generated images are returned. Must be one of url or b64_json.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"url"};

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString User;
};

USTRUCT(BlueprintType)
struct FOpenAIImageVariation
{
    GENERATED_BODY()

    /**
      The image to use as the basis for the variation(s).
      Must be a valid PNG file, less than 4MB, and square.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Image;

    /**
      The model to use for image generation.
      Defaults to dall-e-2.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model{"dall-e-2"};

    /**
      The number of images to generate. Must be between 1 and 10.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 N{1};

    /**
      The format in which the generated images are returned. Must be one of url or b64_json.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"url"};

    /**
      The size of the generated images. Must be one of 256x256, 512x512, or 1024x1024.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Size{"1024x1024"};

    /**
      A unique identifier representing your end-user, which can help OpenAI to monitor and detect abuse.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString User;
};

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

USTRUCT(BlueprintType)
struct FAudioBase
{
    GENERATED_BODY()

    /**
      The audio file to transcribe, in one of these formats: mp3, mp4, mpeg, mpga, m4a, wav, or webm.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString File;

    /**
      ID of the model to use. Only whisper-1 is currently available.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model{"whisper-1"};

    /**
      An optional text to guide the model's style or continue a previous audio segment.
      The prompt should match the audio language.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Prompt;

    /**
      The format of the transcript output, in one of these options: json, text, srt, verbose_json, or vtt.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"json"};

    /**
      The sampling temperature, between 0 and 1.
      Higher values like 0.8 will make the output more random,
      while lower values like 0.2 will make it more focused and deterministic.
      If set to 0, the model will use log probability to automatically increase
      the temperature until certain thresholds are hit.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Temperature{0.0f};
};

USTRUCT(BlueprintType)
struct FAudioTranscription : public FAudioBase
{
    GENERATED_BODY()

    /**
      The language of the input audio.
      Supplying the input language in ISO-639-1 (https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) format
      will improve accuracy and latency.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Language;
};

USTRUCT(BlueprintType)
struct FAudioTranslation : public FAudioBase
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FUploadFile
{
    GENERATED_BODY()

    /**
      Name of the JSON Lines file to be uploaded.
      If the purpose is set to "fine-tune", each line is a JSON record with "prompt" and "completion"
      fields representing your training examples.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString File;

    /**
      The intended purpose of the uploaded documents.
      Use "fine-tune" for Fine-tuning. This allows us to validate the format of the uploaded file.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Purpose;
};

/**
 * Available in C++ only
 */
USTRUCT()
struct FFineTune
{
    GENERATED_BODY()

    /**
      The ID of an uploaded file that contains training data.
      See upload file for how to upload a file.

      Your dataset must be formatted as a JSONL file,
      where each training example is a JSON object with the keys "prompt" and "completion".
      Additionally, you must upload your file with the purpose fine-tune.
      See the fine-tuning guide for more details.
    */
    FString Training_File;

    /**
      The ID of an uploaded file that contains validation data.
      If you provide this file, the data is used to generate validation metrics periodically during fine-tuning.
      These metrics can be viewed in the fine-tuning results file.
      Your train and validation data should be mutually exclusive.
      Your dataset must be formatted as a JSONL file,
      where each validation example is a JSON object with the keys "prompt" and "completion".
      Additionally, you must upload your file with the purpose fine-tune.
      See the fine-tuning guide for more details.
    */
    TOptional<FString> Validation_File;

    /**
      The name of the base model to fine-tune.
      You can select one of "ada", "babbage", "curie", "davinci",
      or a fine-tuned model created after 2022-04-21.
      To learn more about these models, see the Models documentation.
    */
    FString Model{"curie"};

    /**
      The number of epochs to train the model for.
      An epoch refers to one full cycle through the training dataset.
    */
    TOptional<int32> N_Epochs{4};

    /**
      The batch size to use for training.
      The batch size is the number of training examples used to train a single forward and backward pass.
      By default, the batch size will be dynamically configured to be ~0.2% of the number
      of examples in the training set, capped at 256 - in general,
      we've found that larger batch sizes tend to work better for larger datasets.
    */
    TOptional<int32> Batch_Size;

    /**
      The learning rate multiplier to use for training.
      The fine-tuning learning rate is the original learning rate used for pretraining multiplied by this value.
      By default, the learning rate multiplier is the 0.05, 0.1, or 0.2 depending on final batch_size
      (larger learning rates tend to perform better with larger batch sizes).
      We recommend experimenting with values in the range 0.02 to 0.2 to see what produces the best results.
    */
    TOptional<float> Learning_Rate_Multiplier;

    /**
      The weight to use for loss on the prompt tokens.
      This controls how much the model tries to learn to generate the prompt
      (as compared to the completion which always has a weight of 1.0),
      and can add a stabilizing effect to training when completions are short.
      If prompts are extremely long (relative to completions),
      it may make sense to reduce this weight so as to avoid over-prioritizing learning the prompt.
    */
    TOptional<float> Prompt_Loss_Weight;

    /**
      If set, we calculate classification-specific metrics such as accuracy and F-1 score
      using the validation set at the end of every epoch. These metrics can be viewed in the results file.
      In order to compute classification metrics, you must provide a validation_file.
      Additionally, you must specify classification_n_classes for multiclass classification
      or classification_positive_class for binary classification.
    */
    TOptional<bool> Compute_Classification_Metrics;

    /**
      The number of classes in a classification task.
      This parameter is required for multiclass classification.
    */
    // UPROPERTY(BlueprintReadWrite, Category = "Optional")
    TOptional<int32> Classification_N_Classes;

    /**
      The positive class in binary classification.
      This parameter is needed to generate precision, recall,
      and F1 metrics when doing binary classification.
    */
    TOptional<FString> Classification_Positive_Class;

    /**
      If this is provided, we calculate F-beta scores at the specified beta values.
      The F-beta score is a generalization of F-1 score.
      This is only used for binary classification.
      With a beta of 1 (i.e. the F-1 score), precision and recall are given the same weight.
      A larger beta score puts more weight on recall and less on precision.
      A smaller beta score puts more weight on precision and less on recall.
    */
    TOptional<TArray<float>> Classification_Betas;

    /**
      A string of up to 40 characters that will be added to your fine-tuned model name.
      For example, a suffix of "custom-model-name" would produce
      a model name like ada:ft-your-org:custom-model-name-2022-02-15-04-21-04.
    */
    TOptional<FString> Suffix;
};

USTRUCT(BlueprintType)
struct FModerations
{
    GENERATED_BODY()

    /**
      The input text to classify
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    TArray<FString> Input;

    /**
      Two content moderations models are available: text-moderation-stable and text-moderation-latest.

      The default is text-moderation-latest which will be automatically upgraded over time.
      This ensures you are always using our most accurate model.
      If you use text-moderation-stable, we will provide advanced notice before updating the model.
      Accuracy of text-moderation-stable may be slightly lower than for text-moderation-latest.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model{"text-moderation-stable"};
};

USTRUCT(BlueprintType)
struct FFineTuningJob
{
    GENERATED_BODY()

    /**
      The name of the model to fine-tune. You can select one of the supported models:
      https://platform.openai.com/docs/guides/fine-tuning/what-models-can-be-fine-tuned
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI | Required")
    FString Model;

    /**
      The ID of an uploaded file that contains training data.
      See upload file for how to upload a file:
      https://platform.openai.com/docs/api-reference/files/upload
      Your dataset must be formatted as a JSONL file. Additionally, you must upload your file with the purpose fine-tune.
      See the fine-tuning guide for more details:
      https://platform.openai.com/docs/guides/fine-tuning
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI | Required")
    FString Training_File;

    /**
      The hyperparameters used for the fine-tuning job.
    */
    TOptional<FFineTuningJobHyperparams> Hyperparameters;

    /**
      A string of up to 18 characters that will be added to your fine-tuned model name.
      For example, a suffix of "custom-model-name" would produce a model name like ft:gpt-3.5-turbo:openai:custom-model-name:7p4lURel.
    */
    TOptional<FString> Suffix;

    /**
      The ID of an uploaded file that contains validation data.
      If you provide this file, the data is used to generate validation metrics periodically during fine-tuning.
      These metrics can be viewed in the fine-tuning results file. The same data should not be present in both train and validation files.
      Your dataset must be formatted as a JSONL file. You must upload your file with the purpose fine-tune.
      See the fine-tuning guide for more details:
      https://platform.openai.com/docs/guides/fine-tuning
    */
    TOptional<FString> Validation_File;
};

USTRUCT(BlueprintType)
struct FFineTuningQueryParameters
{
    GENERATED_BODY()

    /**
      Identifier for the last event from the previous pagination request.
    */
    TOptional<FString> After;

    /**
      Number of events to retrieve.
    */
    TOptional<int32> Limit;

    FString ToQuery() const
    {
        FString Query{"?"};
        if (After.IsSet())
        {
            Query.Append("after=").Append(After.GetValue()).Append("&");
        }

        if (Limit.IsSet())
        {
            Query.Append("limit=").Append(FString::FromInt(Limit.GetValue())).Append("&");
        }

        return Query.LeftChop(1);
    }
};

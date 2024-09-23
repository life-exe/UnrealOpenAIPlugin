// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/CommonTypes.h"
#include "FineTuneTypes.generated.h"

USTRUCT(BlueprintType)
struct FFineTuningJobHyperparams
{
    GENERATED_BODY()

    /**
      The number of epochs to train the model for.
      An epoch refers to one full cycle through the training dataset.
      "auto" decides the optimal number of epochs based on the size of the dataset.
      If setting the number manually, we support any number between 1 and 50 epochs.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString N_Epochs;
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

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
        if (After)
        {
            Query.Append("after=").Append(After.GetValue()).Append("&");
        }

        if (Limit)
        {
            Query.Append("limit=").Append(FString::FromInt(Limit.GetValue())).Append("&");
        }

        return Query.LeftChop(1);
    }
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FFineTuneEventsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FOpenAIEvent> Data;
};

USTRUCT(BlueprintType)
struct FDeleteFineTuneResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{};
};

USTRUCT(BlueprintType)
struct FFineTuningJobError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Code;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Param;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Message;
};

USTRUCT(BlueprintType)
struct FFineTuningJobObjectResponse
{
    GENERATED_BODY()

    /**
      The object identifier, which can be referenced in the API endpoints.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID;

    /**
      The Unix timestamp (in seconds) for when the fine-tuning job was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    /**
      For fine-tuning jobs that have failed, this will contain more information on the cause of the failure.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FFineTuningJobError Error;

    /**
      The name of the fine-tuned model that is being created. The value will be null if the fine-tuning job is still running.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Fine_Tuned_Model;

    /**
      The Unix timestamp (in seconds) for when the fine-tuning job was finished.
      The value will be null if the fine-tuning job is still running.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Finished_At{};

    /**
      The hyperparameters used for the fine-tuning job:
      https://platform.openai.com/docs/guides/fine-tuning
      See the fine-tuning guide for more details.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FFineTuningJobHyperparams Hyperparameters;

    /**
      The size of the file in bytes.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model;

    /**
      The object type, which is always "fine_tuning.job".
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The organization that owns the fine-tuning job.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Organization_ID;

    /**
      The compiled results file ID(s) for the fine-tuning job.
      You can retrieve the results with the Files API:
      https://platform.openai.com/docs/api-reference/files/retrieve-contents
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FString> Result_Files;

    /**
      The current status of the fine-tuning job,
      which can be either validating_files, queued, running, succeeded, failed, or cancelled.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Status;

    /**
      The total number of billable tokens processed by this fine-tuning job.
      The value will be null if the fine-tuning job is still running.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Trained_Tokens{};

    /**
      The file ID used for training. You can retrieve the training data with the Files API:
      https://platform.openai.com/docs/api-reference/files/retrieve-contents
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Training_File;

    /**
      The file ID used for validation. You can retrieve the validation results with the Files API:
      https://platform.openai.com/docs/api-reference/files/retrieve-contents
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Validation_File;
};

USTRUCT(BlueprintType)
struct FListFineTuningJobsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FFineTuningJobObjectResponse> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};
};

USTRUCT(BlueprintType)
struct FFineTuningJobEventResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString ID;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Level;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

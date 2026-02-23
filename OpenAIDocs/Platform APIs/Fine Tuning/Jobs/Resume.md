## Resume

**post** `/fine_tuning/jobs/{fine_tuning_job_id}/resume`

Resume a fine-tune job.

### Path Parameters

- `fine_tuning_job_id: string`

### Returns

- `FineTuningJob = object { id, created_at, error, 16 more }`

  The `fine_tuning.job` object represents a fine-tuning job that has been created through the API.

  - `id: string`

    The object identifier, which can be referenced in the API endpoints.

  - `created_at: number`

    The Unix timestamp (in seconds) for when the fine-tuning job was created.

  - `error: object { code, message, param }`

    For fine-tuning jobs that have `failed`, this will contain more information on the cause of the failure.

    - `code: string`

      A machine-readable error code.

    - `message: string`

      A human-readable error message.

    - `param: string`

      The parameter that was invalid, usually `training_file` or `validation_file`. This field will be null if the failure was not parameter-specific.

  - `fine_tuned_model: string`

    The name of the fine-tuned model that is being created. The value will be null if the fine-tuning job is still running.

  - `finished_at: number`

    The Unix timestamp (in seconds) for when the fine-tuning job was finished. The value will be null if the fine-tuning job is still running.

  - `hyperparameters: object { batch_size, learning_rate_multiplier, n_epochs }`

    The hyperparameters used for the fine-tuning job. This value will only be returned when running `supervised` jobs.

    - `batch_size: optional "auto" or number`

      Number of examples in each batch. A larger batch size means that model parameters
      are updated less frequently, but with lower variance.

      - `UnionMember0 = "auto"`

        - `"auto"`

      - `UnionMember1 = number`

    - `learning_rate_multiplier: optional "auto" or number`

      Scaling factor for the learning rate. A smaller learning rate may be useful to avoid
      overfitting.

      - `UnionMember0 = "auto"`

        - `"auto"`

      - `UnionMember1 = number`

    - `n_epochs: optional "auto" or number`

      The number of epochs to train the model for. An epoch refers to one full cycle
      through the training dataset.

      - `UnionMember0 = "auto"`

        - `"auto"`

      - `UnionMember1 = number`

  - `model: string`

    The base model that is being fine-tuned.

  - `object: "fine_tuning.job"`

    The object type, which is always "fine_tuning.job".

    - `"fine_tuning.job"`

  - `organization_id: string`

    The organization that owns the fine-tuning job.

  - `result_files: array of string`

    The compiled results file ID(s) for the fine-tuning job. You can retrieve the results with the [Files API](/docs/api-reference/files/retrieve-contents).

  - `seed: number`

    The seed used for the fine-tuning job.

  - `status: "validating_files" or "queued" or "running" or 3 more`

    The current status of the fine-tuning job, which can be either `validating_files`, `queued`, `running`, `succeeded`, `failed`, or `cancelled`.

    - `"validating_files"`

    - `"queued"`

    - `"running"`

    - `"succeeded"`

    - `"failed"`

    - `"cancelled"`

  - `trained_tokens: number`

    The total number of billable tokens processed by this fine-tuning job. The value will be null if the fine-tuning job is still running.

  - `training_file: string`

    The file ID used for training. You can retrieve the training data with the [Files API](/docs/api-reference/files/retrieve-contents).

  - `validation_file: string`

    The file ID used for validation. You can retrieve the validation results with the [Files API](/docs/api-reference/files/retrieve-contents).

  - `estimated_finish: optional number`

    The Unix timestamp (in seconds) for when the fine-tuning job is estimated to finish. The value will be null if the fine-tuning job is not running.

  - `integrations: optional array of FineTuningJobWandbIntegrationObject`

    A list of integrations to enable for this fine-tuning job.

    - `type: "wandb"`

      The type of the integration being enabled for the fine-tuning job

      - `"wandb"`

    - `wandb: FineTuningJobWandbIntegration`

      The settings for your integration with Weights and Biases. This payload specifies the project that
      metrics will be sent to. Optionally, you can set an explicit display name for your run, add tags
      to your run, and set a default entity (team, username, etc) to be associated with your run.

      - `project: string`

        The name of the project that the new run will be created under.

      - `entity: optional string`

        The entity to use for the run. This allows you to set the team or username of the WandB user that you would
        like associated with the run. If not set, the default entity for the registered WandB API key is used.

      - `name: optional string`

        A display name to set for the run. If not set, we will use the Job ID as the name.

      - `tags: optional array of string`

        A list of tags to be attached to the newly created run. These tags are passed through directly to WandB. Some
        default tags are generated by OpenAI: "openai/finetune", "openai/{base-model}", "openai/{ftjob-abcdef}".

  - `metadata: optional Metadata`

    Set of 16 key-value pairs that can be attached to an object. This can be
    useful for storing additional information about the object in a structured
    format, and querying for objects via API or the dashboard.

    Keys are strings with a maximum length of 64 characters. Values are strings
    with a maximum length of 512 characters.

  - `method: optional object { type, dpo, reinforcement, supervised }`

    The method used for fine-tuning.

    - `type: "supervised" or "dpo" or "reinforcement"`

      The type of method. Is either `supervised`, `dpo`, or `reinforcement`.

      - `"supervised"`

      - `"dpo"`

      - `"reinforcement"`

    - `dpo: optional DpoMethod`

      Configuration for the DPO fine-tuning method.

      - `hyperparameters: optional DpoHyperparameters`

        The hyperparameters used for the DPO fine-tuning job.

        - `batch_size: optional "auto" or number`

          Number of examples in each batch. A larger batch size means that model parameters are updated less frequently, but with lower variance.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `beta: optional "auto" or number`

          The beta value for the DPO method. A higher beta value will increase the weight of the penalty between the policy and reference model.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `learning_rate_multiplier: optional "auto" or number`

          Scaling factor for the learning rate. A smaller learning rate may be useful to avoid overfitting.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `n_epochs: optional "auto" or number`

          The number of epochs to train the model for. An epoch refers to one full cycle through the training dataset.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

    - `reinforcement: optional ReinforcementMethod`

      Configuration for the reinforcement fine-tuning method.

      - `grader: StringCheckGrader or TextSimilarityGrader or PythonGrader or 2 more`

        The grader used for the fine-tuning job.

        - `StringCheckGrader = object { input, name, operation, 2 more }`

          A StringCheckGrader object that performs a string comparison between input and reference using a specified operation.

          - `input: string`

            The input text. This may include template strings.

          - `name: string`

            The name of the grader.

          - `operation: "eq" or "ne" or "like" or "ilike"`

            The string check operation to perform. One of `eq`, `ne`, `like`, or `ilike`.

            - `"eq"`

            - `"ne"`

            - `"like"`

            - `"ilike"`

          - `reference: string`

            The reference text. This may include template strings.

          - `type: "string_check"`

            The object type, which is always `string_check`.

            - `"string_check"`

        - `TextSimilarityGrader = object { evaluation_metric, input, name, 2 more }`

          A TextSimilarityGrader object which grades text based on similarity metrics.

          - `evaluation_metric: "cosine" or "fuzzy_match" or "bleu" or 8 more`

            The evaluation metric to use. One of `cosine`, `fuzzy_match`, `bleu`,
            `gleu`, `meteor`, `rouge_1`, `rouge_2`, `rouge_3`, `rouge_4`, `rouge_5`,
            or `rouge_l`.

            - `"cosine"`

            - `"fuzzy_match"`

            - `"bleu"`

            - `"gleu"`

            - `"meteor"`

            - `"rouge_1"`

            - `"rouge_2"`

            - `"rouge_3"`

            - `"rouge_4"`

            - `"rouge_5"`

            - `"rouge_l"`

          - `input: string`

            The text being graded.

          - `name: string`

            The name of the grader.

          - `reference: string`

            The text being graded against.

          - `type: "text_similarity"`

            The type of grader.

            - `"text_similarity"`

        - `PythonGrader = object { name, source, type, image_tag }`

          A PythonGrader object that runs a python script on the input.

          - `name: string`

            The name of the grader.

          - `source: string`

            The source code of the python script.

          - `type: "python"`

            The object type, which is always `python`.

            - `"python"`

          - `image_tag: optional string`

            The image tag to use for the python script.

        - `ScoreModelGrader = object { input, model, name, 3 more }`

          A ScoreModelGrader object that uses a model to assign a score to the input.

          - `input: array of object { content, role, type }`

            The input messages evaluated by the grader. Supports text, output text, input image, and input audio content blocks, and may include template strings.

            - `content: string or ResponseInputText or object { text, type }  or 3 more`

              Inputs to the model - can contain template strings. Supports text, output text, input images, and input audio, either as a single item or an array of items.

              - `TextInput = string`

                A text input to the model.

              - `ResponseInputText = object { text, type }`

                A text input to the model.

                - `text: string`

                  The text input to the model.

                - `type: "input_text"`

                  The type of the input item. Always `input_text`.

                  - `"input_text"`

              - `OutputText = object { text, type }`

                A text output from the model.

                - `text: string`

                  The text output from the model.

                - `type: "output_text"`

                  The type of the output text. Always `output_text`.

                  - `"output_text"`

              - `InputImage = object { image_url, type, detail }`

                An image input block used within EvalItem content arrays.

                - `image_url: string`

                  The URL of the image input.

                - `type: "input_image"`

                  The type of the image input. Always `input_image`.

                  - `"input_image"`

                - `detail: optional string`

                  The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

              - `ResponseInputAudio = object { input_audio, type }`

                An audio input to the model.

                - `input_audio: object { data, format }`

                  - `data: string`

                    Base64-encoded audio data.

                  - `format: "mp3" or "wav"`

                    The format of the audio data. Currently supported formats are `mp3` and
                    `wav`.

                    - `"mp3"`

                    - `"wav"`

                - `type: "input_audio"`

                  The type of the input item. Always `input_audio`.

                  - `"input_audio"`

              - `GraderInputs = array of string or ResponseInputText or object { text, type }  or 2 more`

                A list of inputs, each of which may be either an input text, output text, input
                image, or input audio object.

                - `TextInput = string`

                  A text input to the model.

                - `ResponseInputText = object { text, type }`

                  A text input to the model.

                  - `text: string`

                    The text input to the model.

                  - `type: "input_text"`

                    The type of the input item. Always `input_text`.

                    - `"input_text"`

                - `OutputText = object { text, type }`

                  A text output from the model.

                  - `text: string`

                    The text output from the model.

                  - `type: "output_text"`

                    The type of the output text. Always `output_text`.

                    - `"output_text"`

                - `InputImage = object { image_url, type, detail }`

                  An image input block used within EvalItem content arrays.

                  - `image_url: string`

                    The URL of the image input.

                  - `type: "input_image"`

                    The type of the image input. Always `input_image`.

                    - `"input_image"`

                  - `detail: optional string`

                    The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

                - `ResponseInputAudio = object { input_audio, type }`

                  An audio input to the model.

                  - `input_audio: object { data, format }`

                    - `data: string`

                      Base64-encoded audio data.

                    - `format: "mp3" or "wav"`

                      The format of the audio data. Currently supported formats are `mp3` and
                      `wav`.

                      - `"mp3"`

                      - `"wav"`

                  - `type: "input_audio"`

                    The type of the input item. Always `input_audio`.

                    - `"input_audio"`

            - `role: "user" or "assistant" or "system" or "developer"`

              The role of the message input. One of `user`, `assistant`, `system`, or
              `developer`.

              - `"user"`

              - `"assistant"`

              - `"system"`

              - `"developer"`

            - `type: optional "message"`

              The type of the message input. Always `message`.

              - `"message"`

          - `model: string`

            The model to use for the evaluation.

          - `name: string`

            The name of the grader.

          - `type: "score_model"`

            The object type, which is always `score_model`.

            - `"score_model"`

          - `range: optional array of number`

            The range of the score. Defaults to `[0, 1]`.

          - `sampling_params: optional object { max_completions_tokens, reasoning_effort, seed, 2 more }`

            The sampling parameters for the model.

            - `max_completions_tokens: optional number`

              The maximum number of tokens the grader model may generate in its response.

            - `reasoning_effort: optional ReasoningEffort`

              Constrains effort on reasoning for
              [reasoning models](https://platform.openai.com/docs/guides/reasoning).
              Currently supported values are `none`, `minimal`, `low`, `medium`, `high`, and `xhigh`. Reducing
              reasoning effort can result in faster responses and fewer tokens used
              on reasoning in a response.

              - `gpt-5.1` defaults to `none`, which does not perform reasoning. The supported reasoning values for `gpt-5.1` are `none`, `low`, `medium`, and `high`. Tool calls are supported for all reasoning values in gpt-5.1.
              - All models before `gpt-5.1` default to `medium` reasoning effort, and do not support `none`.
              - The `gpt-5-pro` model defaults to (and only supports) `high` reasoning effort.
              - `xhigh` is supported for all models after `gpt-5.1-codex-max`.

              - `"none"`

              - `"minimal"`

              - `"low"`

              - `"medium"`

              - `"high"`

              - `"xhigh"`

            - `seed: optional number`

              A seed value to initialize the randomness, during sampling.

            - `temperature: optional number`

              A higher temperature increases randomness in the outputs.

            - `top_p: optional number`

              An alternative to temperature for nucleus sampling; 1.0 includes all tokens.

        - `MultiGrader = object { calculate_output, graders, name, type }`

          A MultiGrader object combines the output of multiple graders to produce a single score.

          - `calculate_output: string`

            A formula to calculate the output based on grader results.

          - `graders: StringCheckGrader or TextSimilarityGrader or PythonGrader or 2 more`

            A StringCheckGrader object that performs a string comparison between input and reference using a specified operation.

            - `StringCheckGrader = object { input, name, operation, 2 more }`

              A StringCheckGrader object that performs a string comparison between input and reference using a specified operation.

              - `input: string`

                The input text. This may include template strings.

              - `name: string`

                The name of the grader.

              - `operation: "eq" or "ne" or "like" or "ilike"`

                The string check operation to perform. One of `eq`, `ne`, `like`, or `ilike`.

                - `"eq"`

                - `"ne"`

                - `"like"`

                - `"ilike"`

              - `reference: string`

                The reference text. This may include template strings.

              - `type: "string_check"`

                The object type, which is always `string_check`.

                - `"string_check"`

            - `TextSimilarityGrader = object { evaluation_metric, input, name, 2 more }`

              A TextSimilarityGrader object which grades text based on similarity metrics.

              - `evaluation_metric: "cosine" or "fuzzy_match" or "bleu" or 8 more`

                The evaluation metric to use. One of `cosine`, `fuzzy_match`, `bleu`,
                `gleu`, `meteor`, `rouge_1`, `rouge_2`, `rouge_3`, `rouge_4`, `rouge_5`,
                or `rouge_l`.

                - `"cosine"`

                - `"fuzzy_match"`

                - `"bleu"`

                - `"gleu"`

                - `"meteor"`

                - `"rouge_1"`

                - `"rouge_2"`

                - `"rouge_3"`

                - `"rouge_4"`

                - `"rouge_5"`

                - `"rouge_l"`

              - `input: string`

                The text being graded.

              - `name: string`

                The name of the grader.

              - `reference: string`

                The text being graded against.

              - `type: "text_similarity"`

                The type of grader.

                - `"text_similarity"`

            - `PythonGrader = object { name, source, type, image_tag }`

              A PythonGrader object that runs a python script on the input.

              - `name: string`

                The name of the grader.

              - `source: string`

                The source code of the python script.

              - `type: "python"`

                The object type, which is always `python`.

                - `"python"`

              - `image_tag: optional string`

                The image tag to use for the python script.

            - `ScoreModelGrader = object { input, model, name, 3 more }`

              A ScoreModelGrader object that uses a model to assign a score to the input.

              - `input: array of object { content, role, type }`

                The input messages evaluated by the grader. Supports text, output text, input image, and input audio content blocks, and may include template strings.

                - `content: string or ResponseInputText or object { text, type }  or 3 more`

                  Inputs to the model - can contain template strings. Supports text, output text, input images, and input audio, either as a single item or an array of items.

                  - `TextInput = string`

                    A text input to the model.

                  - `ResponseInputText = object { text, type }`

                    A text input to the model.

                    - `text: string`

                      The text input to the model.

                    - `type: "input_text"`

                      The type of the input item. Always `input_text`.

                      - `"input_text"`

                  - `OutputText = object { text, type }`

                    A text output from the model.

                    - `text: string`

                      The text output from the model.

                    - `type: "output_text"`

                      The type of the output text. Always `output_text`.

                      - `"output_text"`

                  - `InputImage = object { image_url, type, detail }`

                    An image input block used within EvalItem content arrays.

                    - `image_url: string`

                      The URL of the image input.

                    - `type: "input_image"`

                      The type of the image input. Always `input_image`.

                      - `"input_image"`

                    - `detail: optional string`

                      The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

                  - `ResponseInputAudio = object { input_audio, type }`

                    An audio input to the model.

                    - `input_audio: object { data, format }`

                      - `data: string`

                        Base64-encoded audio data.

                      - `format: "mp3" or "wav"`

                        The format of the audio data. Currently supported formats are `mp3` and
                        `wav`.

                        - `"mp3"`

                        - `"wav"`

                    - `type: "input_audio"`

                      The type of the input item. Always `input_audio`.

                      - `"input_audio"`

                  - `GraderInputs = array of string or ResponseInputText or object { text, type }  or 2 more`

                    A list of inputs, each of which may be either an input text, output text, input
                    image, or input audio object.

                    - `TextInput = string`

                      A text input to the model.

                    - `ResponseInputText = object { text, type }`

                      A text input to the model.

                      - `text: string`

                        The text input to the model.

                      - `type: "input_text"`

                        The type of the input item. Always `input_text`.

                        - `"input_text"`

                    - `OutputText = object { text, type }`

                      A text output from the model.

                      - `text: string`

                        The text output from the model.

                      - `type: "output_text"`

                        The type of the output text. Always `output_text`.

                        - `"output_text"`

                    - `InputImage = object { image_url, type, detail }`

                      An image input block used within EvalItem content arrays.

                      - `image_url: string`

                        The URL of the image input.

                      - `type: "input_image"`

                        The type of the image input. Always `input_image`.

                        - `"input_image"`

                      - `detail: optional string`

                        The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

                    - `ResponseInputAudio = object { input_audio, type }`

                      An audio input to the model.

                      - `input_audio: object { data, format }`

                        - `data: string`

                          Base64-encoded audio data.

                        - `format: "mp3" or "wav"`

                          The format of the audio data. Currently supported formats are `mp3` and
                          `wav`.

                          - `"mp3"`

                          - `"wav"`

                      - `type: "input_audio"`

                        The type of the input item. Always `input_audio`.

                        - `"input_audio"`

                - `role: "user" or "assistant" or "system" or "developer"`

                  The role of the message input. One of `user`, `assistant`, `system`, or
                  `developer`.

                  - `"user"`

                  - `"assistant"`

                  - `"system"`

                  - `"developer"`

                - `type: optional "message"`

                  The type of the message input. Always `message`.

                  - `"message"`

              - `model: string`

                The model to use for the evaluation.

              - `name: string`

                The name of the grader.

              - `type: "score_model"`

                The object type, which is always `score_model`.

                - `"score_model"`

              - `range: optional array of number`

                The range of the score. Defaults to `[0, 1]`.

              - `sampling_params: optional object { max_completions_tokens, reasoning_effort, seed, 2 more }`

                The sampling parameters for the model.

                - `max_completions_tokens: optional number`

                  The maximum number of tokens the grader model may generate in its response.

                - `reasoning_effort: optional ReasoningEffort`

                  Constrains effort on reasoning for
                  [reasoning models](https://platform.openai.com/docs/guides/reasoning).
                  Currently supported values are `none`, `minimal`, `low`, `medium`, `high`, and `xhigh`. Reducing
                  reasoning effort can result in faster responses and fewer tokens used
                  on reasoning in a response.

                  - `gpt-5.1` defaults to `none`, which does not perform reasoning. The supported reasoning values for `gpt-5.1` are `none`, `low`, `medium`, and `high`. Tool calls are supported for all reasoning values in gpt-5.1.
                  - All models before `gpt-5.1` default to `medium` reasoning effort, and do not support `none`.
                  - The `gpt-5-pro` model defaults to (and only supports) `high` reasoning effort.
                  - `xhigh` is supported for all models after `gpt-5.1-codex-max`.

                  - `"none"`

                  - `"minimal"`

                  - `"low"`

                  - `"medium"`

                  - `"high"`

                  - `"xhigh"`

                - `seed: optional number`

                  A seed value to initialize the randomness, during sampling.

                - `temperature: optional number`

                  A higher temperature increases randomness in the outputs.

                - `top_p: optional number`

                  An alternative to temperature for nucleus sampling; 1.0 includes all tokens.

            - `LabelModelGrader = object { input, labels, model, 3 more }`

              A LabelModelGrader object which uses a model to assign labels to each item
              in the evaluation.

              - `input: array of object { content, role, type }`

                - `content: string or ResponseInputText or object { text, type }  or 3 more`

                  Inputs to the model - can contain template strings. Supports text, output text, input images, and input audio, either as a single item or an array of items.

                  - `TextInput = string`

                    A text input to the model.

                  - `ResponseInputText = object { text, type }`

                    A text input to the model.

                    - `text: string`

                      The text input to the model.

                    - `type: "input_text"`

                      The type of the input item. Always `input_text`.

                      - `"input_text"`

                  - `OutputText = object { text, type }`

                    A text output from the model.

                    - `text: string`

                      The text output from the model.

                    - `type: "output_text"`

                      The type of the output text. Always `output_text`.

                      - `"output_text"`

                  - `InputImage = object { image_url, type, detail }`

                    An image input block used within EvalItem content arrays.

                    - `image_url: string`

                      The URL of the image input.

                    - `type: "input_image"`

                      The type of the image input. Always `input_image`.

                      - `"input_image"`

                    - `detail: optional string`

                      The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

                  - `ResponseInputAudio = object { input_audio, type }`

                    An audio input to the model.

                    - `input_audio: object { data, format }`

                      - `data: string`

                        Base64-encoded audio data.

                      - `format: "mp3" or "wav"`

                        The format of the audio data. Currently supported formats are `mp3` and
                        `wav`.

                        - `"mp3"`

                        - `"wav"`

                    - `type: "input_audio"`

                      The type of the input item. Always `input_audio`.

                      - `"input_audio"`

                  - `GraderInputs = array of string or ResponseInputText or object { text, type }  or 2 more`

                    A list of inputs, each of which may be either an input text, output text, input
                    image, or input audio object.

                    - `TextInput = string`

                      A text input to the model.

                    - `ResponseInputText = object { text, type }`

                      A text input to the model.

                      - `text: string`

                        The text input to the model.

                      - `type: "input_text"`

                        The type of the input item. Always `input_text`.

                        - `"input_text"`

                    - `OutputText = object { text, type }`

                      A text output from the model.

                      - `text: string`

                        The text output from the model.

                      - `type: "output_text"`

                        The type of the output text. Always `output_text`.

                        - `"output_text"`

                    - `InputImage = object { image_url, type, detail }`

                      An image input block used within EvalItem content arrays.

                      - `image_url: string`

                        The URL of the image input.

                      - `type: "input_image"`

                        The type of the image input. Always `input_image`.

                        - `"input_image"`

                      - `detail: optional string`

                        The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

                    - `ResponseInputAudio = object { input_audio, type }`

                      An audio input to the model.

                      - `input_audio: object { data, format }`

                        - `data: string`

                          Base64-encoded audio data.

                        - `format: "mp3" or "wav"`

                          The format of the audio data. Currently supported formats are `mp3` and
                          `wav`.

                          - `"mp3"`

                          - `"wav"`

                      - `type: "input_audio"`

                        The type of the input item. Always `input_audio`.

                        - `"input_audio"`

                - `role: "user" or "assistant" or "system" or "developer"`

                  The role of the message input. One of `user`, `assistant`, `system`, or
                  `developer`.

                  - `"user"`

                  - `"assistant"`

                  - `"system"`

                  - `"developer"`

                - `type: optional "message"`

                  The type of the message input. Always `message`.

                  - `"message"`

              - `labels: array of string`

                The labels to assign to each item in the evaluation.

              - `model: string`

                The model to use for the evaluation. Must support structured outputs.

              - `name: string`

                The name of the grader.

              - `passing_labels: array of string`

                The labels that indicate a passing result. Must be a subset of labels.

              - `type: "label_model"`

                The object type, which is always `label_model`.

                - `"label_model"`

          - `name: string`

            The name of the grader.

          - `type: "multi"`

            The object type, which is always `multi`.

            - `"multi"`

      - `hyperparameters: optional ReinforcementHyperparameters`

        The hyperparameters used for the reinforcement fine-tuning job.

        - `batch_size: optional "auto" or number`

          Number of examples in each batch. A larger batch size means that model parameters are updated less frequently, but with lower variance.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `compute_multiplier: optional "auto" or number`

          Multiplier on amount of compute used for exploring search space during training.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `eval_interval: optional "auto" or number`

          The number of training steps between evaluation runs.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `eval_samples: optional "auto" or number`

          Number of evaluation samples to generate per training step.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `learning_rate_multiplier: optional "auto" or number`

          Scaling factor for the learning rate. A smaller learning rate may be useful to avoid overfitting.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `n_epochs: optional "auto" or number`

          The number of epochs to train the model for. An epoch refers to one full cycle through the training dataset.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `reasoning_effort: optional "default" or "low" or "medium" or "high"`

          Level of reasoning effort.

          - `"default"`

          - `"low"`

          - `"medium"`

          - `"high"`

    - `supervised: optional SupervisedMethod`

      Configuration for the supervised fine-tuning method.

      - `hyperparameters: optional SupervisedHyperparameters`

        The hyperparameters used for the fine-tuning job.

        - `batch_size: optional "auto" or number`

          Number of examples in each batch. A larger batch size means that model parameters are updated less frequently, but with lower variance.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `learning_rate_multiplier: optional "auto" or number`

          Scaling factor for the learning rate. A smaller learning rate may be useful to avoid overfitting.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

        - `n_epochs: optional "auto" or number`

          The number of epochs to train the model for. An epoch refers to one full cycle through the training dataset.

          - `UnionMember0 = "auto"`

            - `"auto"`

          - `UnionMember1 = number`

### Example

```http
curl https://api.openai.com/v1/fine_tuning/jobs/$FINE_TUNING_JOB_ID/resume \
    -X POST \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

## Run

**post** `/fine_tuning/alpha/graders/run`

Run a grader.

### Body Parameters

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

- `model_sample: string`

  The model sample to be evaluated. This value will be used to populate
  the `sample` namespace. See [the guide](/docs/guides/graders) for more details.
  The `output_json` variable will be populated if the model sample is a
  valid JSON string.

- `item: optional unknown`

  The dataset item provided to the grader. This will be used to populate
  the `item` namespace. See [the guide](/docs/guides/graders) for more details.

### Returns

- `metadata: object { errors, execution_time, name, 4 more }`

  - `errors: object { formula_parse_error, invalid_variable_error, model_grader_parse_error, 11 more }`

    - `formula_parse_error: boolean`

    - `invalid_variable_error: boolean`

    - `model_grader_parse_error: boolean`

    - `model_grader_refusal_error: boolean`

    - `model_grader_server_error: boolean`

    - `model_grader_server_error_details: string`

    - `other_error: boolean`

    - `python_grader_runtime_error: boolean`

    - `python_grader_runtime_error_details: string`

    - `python_grader_server_error: boolean`

    - `python_grader_server_error_type: string`

    - `sample_parse_error: boolean`

    - `truncated_observation_error: boolean`

    - `unresponsive_reward_error: boolean`

  - `execution_time: number`

  - `name: string`

  - `sampled_model_name: string`

  - `scores: map[unknown]`

  - `token_usage: number`

  - `type: string`

- `model_grader_token_usage_per_model: map[unknown]`

- `reward: number`

- `sub_rewards: map[unknown]`

### Example

```http
curl https://api.openai.com/v1/fine_tuning/alpha/graders/run \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "grader": {
            "input": "input",
            "name": "name",
            "operation": "eq",
            "reference": "reference",
            "type": "string_check"
          },
          "model_sample": "model_sample"
        }'
```

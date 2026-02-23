## Update

**post** `/chat/completions/{completion_id}`

Modify a stored chat completion. Only Chat Completions that have been
created with the `store` parameter set to `true` can be modified. Currently,
the only supported modification is to update the `metadata` field.

### Path Parameters

- `completion_id: string`

### Body Parameters

- `metadata: Metadata`

  Set of 16 key-value pairs that can be attached to an object. This can be
  useful for storing additional information about the object in a structured
  format, and querying for objects via API or the dashboard.

  Keys are strings with a maximum length of 64 characters. Values are strings
  with a maximum length of 512 characters.

### Returns

- `ChatCompletion = object { id, choices, created, 5 more }`

  Represents a chat completion response returned by model, based on the provided input.

  - `id: string`

    A unique identifier for the chat completion.

  - `choices: array of object { finish_reason, index, logprobs, message }`

    A list of chat completion choices. Can be more than one if `n` is greater than 1.

    - `finish_reason: "stop" or "length" or "tool_calls" or 2 more`

      The reason the model stopped generating tokens. This will be `stop` if the model hit a natural stop point or a provided stop sequence,
      `length` if the maximum number of tokens specified in the request was reached,
      `content_filter` if content was omitted due to a flag from our content filters,
      `tool_calls` if the model called a tool, or `function_call` (deprecated) if the model called a function.

      - `"stop"`

      - `"length"`

      - `"tool_calls"`

      - `"content_filter"`

      - `"function_call"`

    - `index: number`

      The index of the choice in the list of choices.

    - `logprobs: object { content, refusal }`

      Log probability information for the choice.

      - `content: array of ChatCompletionTokenLogprob`

        A list of message content tokens with log probability information.

        - `token: string`

          The token.

        - `bytes: array of number`

          A list of integers representing the UTF-8 bytes representation of the token. Useful in instances where characters are represented by multiple tokens and their byte representations must be combined to generate the correct text representation. Can be `null` if there is no bytes representation for the token.

        - `logprob: number`

          The log probability of this token, if it is within the top 20 most likely tokens. Otherwise, the value `-9999.0` is used to signify that the token is very unlikely.

        - `top_logprobs: array of object { token, bytes, logprob }`

          List of the most likely tokens and their log probability, at this token position. In rare cases, there may be fewer than the number of requested `top_logprobs` returned.

          - `token: string`

            The token.

          - `bytes: array of number`

            A list of integers representing the UTF-8 bytes representation of the token. Useful in instances where characters are represented by multiple tokens and their byte representations must be combined to generate the correct text representation. Can be `null` if there is no bytes representation for the token.

          - `logprob: number`

            The log probability of this token, if it is within the top 20 most likely tokens. Otherwise, the value `-9999.0` is used to signify that the token is very unlikely.

      - `refusal: array of ChatCompletionTokenLogprob`

        A list of message refusal tokens with log probability information.

        - `token: string`

          The token.

        - `bytes: array of number`

          A list of integers representing the UTF-8 bytes representation of the token. Useful in instances where characters are represented by multiple tokens and their byte representations must be combined to generate the correct text representation. Can be `null` if there is no bytes representation for the token.

        - `logprob: number`

          The log probability of this token, if it is within the top 20 most likely tokens. Otherwise, the value `-9999.0` is used to signify that the token is very unlikely.

        - `top_logprobs: array of object { token, bytes, logprob }`

          List of the most likely tokens and their log probability, at this token position. In rare cases, there may be fewer than the number of requested `top_logprobs` returned.

          - `token: string`

            The token.

          - `bytes: array of number`

            A list of integers representing the UTF-8 bytes representation of the token. Useful in instances where characters are represented by multiple tokens and their byte representations must be combined to generate the correct text representation. Can be `null` if there is no bytes representation for the token.

          - `logprob: number`

            The log probability of this token, if it is within the top 20 most likely tokens. Otherwise, the value `-9999.0` is used to signify that the token is very unlikely.

    - `message: ChatCompletionMessage`

      A chat completion message generated by the model.

      - `content: string`

        The contents of the message.

      - `refusal: string`

        The refusal message generated by the model.

      - `role: "assistant"`

        The role of the author of this message.

        - `"assistant"`

      - `annotations: optional array of object { type, url_citation }`

        Annotations for the message, when applicable, as when using the
        [web search tool](/docs/guides/tools-web-search?api-mode=chat).

        - `type: "url_citation"`

          The type of the URL citation. Always `url_citation`.

          - `"url_citation"`

        - `url_citation: object { end_index, start_index, title, url }`

          A URL citation when using web search.

          - `end_index: number`

            The index of the last character of the URL citation in the message.

          - `start_index: number`

            The index of the first character of the URL citation in the message.

          - `title: string`

            The title of the web resource.

          - `url: string`

            The URL of the web resource.

      - `audio: optional ChatCompletionAudio`

        If the audio output modality is requested, this object contains data
        about the audio response from the model. [Learn more](/docs/guides/audio).

        - `id: string`

          Unique identifier for this audio response.

        - `data: string`

          Base64 encoded audio bytes generated by the model, in the format
          specified in the request.

        - `expires_at: number`

          The Unix timestamp (in seconds) for when this audio response will
          no longer be accessible on the server for use in multi-turn
          conversations.

        - `transcript: string`

          Transcript of the audio generated by the model.

      - `function_call: optional object { arguments, name }`

        Deprecated and replaced by `tool_calls`. The name and arguments of a function that should be called, as generated by the model.

        - `arguments: string`

          The arguments to call the function with, as generated by the model in JSON format. Note that the model does not always generate valid JSON, and may hallucinate parameters not defined by your function schema. Validate the arguments in your code before calling your function.

        - `name: string`

          The name of the function to call.

      - `tool_calls: optional array of ChatCompletionMessageToolCall`

        The tool calls generated by the model, such as function calls.

        - `ChatCompletionMessageFunctionToolCall = object { id, function, type }`

          A call to a function tool created by the model.

          - `id: string`

            The ID of the tool call.

          - `function: object { arguments, name }`

            The function that the model called.

            - `arguments: string`

              The arguments to call the function with, as generated by the model in JSON format. Note that the model does not always generate valid JSON, and may hallucinate parameters not defined by your function schema. Validate the arguments in your code before calling your function.

            - `name: string`

              The name of the function to call.

          - `type: "function"`

            The type of the tool. Currently, only `function` is supported.

            - `"function"`

        - `ChatCompletionMessageCustomToolCall = object { id, custom, type }`

          A call to a custom tool created by the model.

          - `id: string`

            The ID of the tool call.

          - `custom: object { input, name }`

            The custom tool that the model called.

            - `input: string`

              The input for the custom tool call generated by the model.

            - `name: string`

              The name of the custom tool to call.

          - `type: "custom"`

            The type of the tool. Always `custom`.

            - `"custom"`

  - `created: number`

    The Unix timestamp (in seconds) of when the chat completion was created.

  - `model: string`

    The model used for the chat completion.

  - `object: "chat.completion"`

    The object type, which is always `chat.completion`.

    - `"chat.completion"`

  - `service_tier: optional "auto" or "default" or "flex" or 2 more`

    Specifies the processing type used for serving the request.

    - If set to 'auto', then the request will be processed with the service tier configured in the Project settings. Unless otherwise configured, the Project will use 'default'.
    - If set to 'default', then the request will be processed with the standard pricing and performance for the selected model.
    - If set to '[flex](/docs/guides/flex-processing)' or '[priority](https://openai.com/api-priority-processing/)', then the request will be processed with the corresponding service tier.
    - When not set, the default behavior is 'auto'.

    When the `service_tier` parameter is set, the response body will include the `service_tier` value based on the processing mode actually used to serve the request. This response value may be different from the value set in the parameter.

    - `"auto"`

    - `"default"`

    - `"flex"`

    - `"scale"`

    - `"priority"`

  - `system_fingerprint: optional string`

    This fingerprint represents the backend configuration that the model runs with.

    Can be used in conjunction with the `seed` request parameter to understand when backend changes have been made that might impact determinism.

  - `usage: optional CompletionUsage`

    Usage statistics for the completion request.

    - `completion_tokens: number`

      Number of tokens in the generated completion.

    - `prompt_tokens: number`

      Number of tokens in the prompt.

    - `total_tokens: number`

      Total number of tokens used in the request (prompt + completion).

    - `completion_tokens_details: optional object { accepted_prediction_tokens, audio_tokens, reasoning_tokens, rejected_prediction_tokens }`

      Breakdown of tokens used in a completion.

      - `accepted_prediction_tokens: optional number`

        When using Predicted Outputs, the number of tokens in the
        prediction that appeared in the completion.

      - `audio_tokens: optional number`

        Audio input tokens generated by the model.

      - `reasoning_tokens: optional number`

        Tokens generated by the model for reasoning.

      - `rejected_prediction_tokens: optional number`

        When using Predicted Outputs, the number of tokens in the
        prediction that did not appear in the completion. However, like
        reasoning tokens, these tokens are still counted in the total
        completion tokens for purposes of billing, output, and context window
        limits.

    - `prompt_tokens_details: optional object { audio_tokens, cached_tokens }`

      Breakdown of tokens used in the prompt.

      - `audio_tokens: optional number`

        Audio input tokens present in the prompt.

      - `cached_tokens: optional number`

        Cached tokens present in the prompt.

### Example

```http
curl https://api.openai.com/v1/chat/completions/$COMPLETION_ID \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "metadata": {
            "foo": "string"
          }
        }'
```

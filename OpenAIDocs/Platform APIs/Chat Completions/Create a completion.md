## Create

**post** `/chat/completions`

**Starting a new project?** We recommend trying [Responses](/docs/api-reference/responses)
to take advantage of the latest OpenAI platform features. Compare
[Chat Completions with Responses](/docs/guides/responses-vs-chat-completions?api-mode=responses).

---

Creates a model response for the given chat conversation. Learn more in the
[text generation](/docs/guides/text-generation), [vision](/docs/guides/vision),
and [audio](/docs/guides/audio) guides.

Parameter support can differ depending on the model used to generate the
response, particularly for newer reasoning models. Parameters that are only
supported for reasoning models are noted below. For the current state of
unsupported parameters in reasoning models,
[refer to the reasoning guide](/docs/guides/reasoning).

Returns a chat completion object, or a streamed sequence of chat completion
chunk objects if the request is streamed.

### Body Parameters

- `messages: array of ChatCompletionMessageParam`

  A list of messages comprising the conversation so far. Depending on the
  [model](/docs/models) you use, different message types (modalities) are
  supported, like [text](/docs/guides/text-generation),
  [images](/docs/guides/vision), and [audio](/docs/guides/audio).

  - `ChatCompletionDeveloperMessageParam = object { content, role, name }`

    Developer-provided instructions that the model should follow, regardless of
    messages sent by the user. With o1 models and newer, `developer` messages
    replace the previous `system` messages.

    - `content: string or array of ChatCompletionContentPartText`

      The contents of the developer message.

      - `TextContent = string`

        The contents of the developer message.

      - `ArrayOfContentParts = array of ChatCompletionContentPartText`

        An array of content parts with a defined type. For developer messages, only type `text` is supported.

        - `text: string`

          The text content.

        - `type: "text"`

          The type of the content part.

          - `"text"`

    - `role: "developer"`

      The role of the messages author, in this case `developer`.

      - `"developer"`

    - `name: optional string`

      An optional name for the participant. Provides the model information to differentiate between participants of the same role.

  - `ChatCompletionSystemMessageParam = object { content, role, name }`

    Developer-provided instructions that the model should follow, regardless of
    messages sent by the user. With o1 models and newer, use `developer` messages
    for this purpose instead.

    - `content: string or array of ChatCompletionContentPartText`

      The contents of the system message.

      - `TextContent = string`

        The contents of the system message.

      - `ArrayOfContentParts = array of ChatCompletionContentPartText`

        An array of content parts with a defined type. For system messages, only type `text` is supported.

        - `text: string`

          The text content.

        - `type: "text"`

          The type of the content part.

          - `"text"`

    - `role: "system"`

      The role of the messages author, in this case `system`.

      - `"system"`

    - `name: optional string`

      An optional name for the participant. Provides the model information to differentiate between participants of the same role.

  - `ChatCompletionUserMessageParam = object { content, role, name }`

    Messages sent by an end user, containing prompts or additional context
    information.

    - `content: string or array of ChatCompletionContentPart`

      The contents of the user message.

      - `TextContent = string`

        The text contents of the message.

      - `ArrayOfContentParts = array of ChatCompletionContentPart`

        An array of content parts with a defined type. Supported options differ based on the [model](/docs/models) being used to generate the response. Can contain text, image, or audio inputs.

        - `ChatCompletionContentPartText = object { text, type }`

          Learn about [text inputs](/docs/guides/text-generation).

          - `text: string`

            The text content.

          - `type: "text"`

            The type of the content part.

            - `"text"`

        - `ChatCompletionContentPartImage = object { image_url, type }`

          Learn about [image inputs](/docs/guides/vision).

          - `image_url: object { url, detail }`

            - `url: string`

              Either a URL of the image or the base64 encoded image data.

            - `detail: optional "auto" or "low" or "high"`

              Specifies the detail level of the image. Learn more in the [Vision guide](/docs/guides/vision#low-or-high-fidelity-image-understanding).

              - `"auto"`

              - `"low"`

              - `"high"`

          - `type: "image_url"`

            The type of the content part.

            - `"image_url"`

        - `ChatCompletionContentPartInputAudio = object { input_audio, type }`

          Learn about [audio inputs](/docs/guides/audio).

          - `input_audio: object { data, format }`

            - `data: string`

              Base64 encoded audio data.

            - `format: "wav" or "mp3"`

              The format of the encoded audio data. Currently supports "wav" and "mp3".

              - `"wav"`

              - `"mp3"`

          - `type: "input_audio"`

            The type of the content part. Always `input_audio`.

            - `"input_audio"`

        - `FileContentPart = object { file, type }`

          Learn about [file inputs](/docs/guides/text) for text generation.

          - `file: object { file_data, file_id, filename }`

            - `file_data: optional string`

              The base64 encoded file data, used when passing the file to the model
              as a string.

            - `file_id: optional string`

              The ID of an uploaded file to use as input.

            - `filename: optional string`

              The name of the file, used when passing the file to the model as a
              string.

          - `type: "file"`

            The type of the content part. Always `file`.

            - `"file"`

    - `role: "user"`

      The role of the messages author, in this case `user`.

      - `"user"`

    - `name: optional string`

      An optional name for the participant. Provides the model information to differentiate between participants of the same role.

  - `ChatCompletionAssistantMessageParam = object { role, audio, content, 4 more }`

    Messages sent by the model in response to user messages.

    - `role: "assistant"`

      The role of the messages author, in this case `assistant`.

      - `"assistant"`

    - `audio: optional object { id }`

      Data about a previous audio response from the model.
      [Learn more](/docs/guides/audio).

      - `id: string`

        Unique identifier for a previous audio response from the model.

    - `content: optional string or array of ChatCompletionContentPartText or ChatCompletionContentPartRefusal`

      The contents of the assistant message. Required unless `tool_calls` or `function_call` is specified.

      - `TextContent = string`

        The contents of the assistant message.

      - `ArrayOfContentParts = array of ChatCompletionContentPartText or ChatCompletionContentPartRefusal`

        An array of content parts with a defined type. Can be one or more of type `text`, or exactly one of type `refusal`.

        - `ChatCompletionContentPartText = object { text, type }`

          Learn about [text inputs](/docs/guides/text-generation).

          - `text: string`

            The text content.

          - `type: "text"`

            The type of the content part.

            - `"text"`

        - `ChatCompletionContentPartRefusal = object { refusal, type }`

          - `refusal: string`

            The refusal message generated by the model.

          - `type: "refusal"`

            The type of the content part.

            - `"refusal"`

    - `function_call: optional object { arguments, name }`

      Deprecated and replaced by `tool_calls`. The name and arguments of a function that should be called, as generated by the model.

      - `arguments: string`

        The arguments to call the function with, as generated by the model in JSON format. Note that the model does not always generate valid JSON, and may hallucinate parameters not defined by your function schema. Validate the arguments in your code before calling your function.

      - `name: string`

        The name of the function to call.

    - `name: optional string`

      An optional name for the participant. Provides the model information to differentiate between participants of the same role.

    - `refusal: optional string`

      The refusal message by the assistant.

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

  - `ChatCompletionToolMessageParam = object { content, role, tool_call_id }`

    - `content: string or array of ChatCompletionContentPartText`

      The contents of the tool message.

      - `TextContent = string`

        The contents of the tool message.

      - `ArrayOfContentParts = array of ChatCompletionContentPartText`

        An array of content parts with a defined type. For tool messages, only type `text` is supported.

        - `text: string`

          The text content.

        - `type: "text"`

          The type of the content part.

          - `"text"`

    - `role: "tool"`

      The role of the messages author, in this case `tool`.

      - `"tool"`

    - `tool_call_id: string`

      Tool call that this message is responding to.

  - `ChatCompletionFunctionMessageParam = object { content, name, role }`

    - `content: string`

      The contents of the function message.

    - `name: string`

      The name of the function to call.

    - `role: "function"`

      The role of the messages author, in this case `function`.

      - `"function"`

- `model: string or "gpt-5.2" or "gpt-5.2-2025-12-11" or "gpt-5.2-chat-latest" or 69 more`

  Model ID used to generate the response, like `gpt-4o` or `o3`. OpenAI
  offers a wide range of models with different capabilities, performance
  characteristics, and price points. Refer to the [model guide](/docs/models)
  to browse and compare available models.

  - `UnionMember0 = string`

  - `UnionMember1 = "gpt-5.2" or "gpt-5.2-2025-12-11" or "gpt-5.2-chat-latest" or 69 more`

    Model ID used to generate the response, like `gpt-4o` or `o3`. OpenAI
    offers a wide range of models with different capabilities, performance
    characteristics, and price points. Refer to the [model guide](/docs/models)
    to browse and compare available models.

    - `"gpt-5.2"`

    - `"gpt-5.2-2025-12-11"`

    - `"gpt-5.2-chat-latest"`

    - `"gpt-5.2-pro"`

    - `"gpt-5.2-pro-2025-12-11"`

    - `"gpt-5.1"`

    - `"gpt-5.1-2025-11-13"`

    - `"gpt-5.1-codex"`

    - `"gpt-5.1-mini"`

    - `"gpt-5.1-chat-latest"`

    - `"gpt-5"`

    - `"gpt-5-mini"`

    - `"gpt-5-nano"`

    - `"gpt-5-2025-08-07"`

    - `"gpt-5-mini-2025-08-07"`

    - `"gpt-5-nano-2025-08-07"`

    - `"gpt-5-chat-latest"`

    - `"gpt-4.1"`

    - `"gpt-4.1-mini"`

    - `"gpt-4.1-nano"`

    - `"gpt-4.1-2025-04-14"`

    - `"gpt-4.1-mini-2025-04-14"`

    - `"gpt-4.1-nano-2025-04-14"`

    - `"o4-mini"`

    - `"o4-mini-2025-04-16"`

    - `"o3"`

    - `"o3-2025-04-16"`

    - `"o3-mini"`

    - `"o3-mini-2025-01-31"`

    - `"o1"`

    - `"o1-2024-12-17"`

    - `"o1-preview"`

    - `"o1-preview-2024-09-12"`

    - `"o1-mini"`

    - `"o1-mini-2024-09-12"`

    - `"gpt-4o"`

    - `"gpt-4o-2024-11-20"`

    - `"gpt-4o-2024-08-06"`

    - `"gpt-4o-2024-05-13"`

    - `"gpt-4o-audio-preview"`

    - `"gpt-4o-audio-preview-2024-10-01"`

    - `"gpt-4o-audio-preview-2024-12-17"`

    - `"gpt-4o-audio-preview-2025-06-03"`

    - `"gpt-4o-mini-audio-preview"`

    - `"gpt-4o-mini-audio-preview-2024-12-17"`

    - `"gpt-4o-search-preview"`

    - `"gpt-4o-mini-search-preview"`

    - `"gpt-4o-search-preview-2025-03-11"`

    - `"gpt-4o-mini-search-preview-2025-03-11"`

    - `"chatgpt-4o-latest"`

    - `"codex-mini-latest"`

    - `"gpt-4o-mini"`

    - `"gpt-4o-mini-2024-07-18"`

    - `"gpt-4-turbo"`

    - `"gpt-4-turbo-2024-04-09"`

    - `"gpt-4-0125-preview"`

    - `"gpt-4-turbo-preview"`

    - `"gpt-4-1106-preview"`

    - `"gpt-4-vision-preview"`

    - `"gpt-4"`

    - `"gpt-4-0314"`

    - `"gpt-4-0613"`

    - `"gpt-4-32k"`

    - `"gpt-4-32k-0314"`

    - `"gpt-4-32k-0613"`

    - `"gpt-3.5-turbo"`

    - `"gpt-3.5-turbo-16k"`

    - `"gpt-3.5-turbo-0301"`

    - `"gpt-3.5-turbo-0613"`

    - `"gpt-3.5-turbo-1106"`

    - `"gpt-3.5-turbo-0125"`

    - `"gpt-3.5-turbo-16k-0613"`

- `audio: optional ChatCompletionAudioParam`

  Parameters for audio output. Required when audio output is requested with
  `modalities: ["audio"]`. [Learn more](/docs/guides/audio).

  - `format: "wav" or "aac" or "mp3" or 3 more`

    Specifies the output audio format. Must be one of `wav`, `mp3`, `flac`,
    `opus`, or `pcm16`.

    - `"wav"`

    - `"aac"`

    - `"mp3"`

    - `"flac"`

    - `"opus"`

    - `"pcm16"`

  - `voice: string or "alloy" or "ash" or "ballad" or 7 more or object { id }`

    The voice the model uses to respond. Supported built-in voices are
    `alloy`, `ash`, `ballad`, `coral`, `echo`, `fable`, `nova`, `onyx`,
    `sage`, `shimmer`, `marin`, and `cedar`. You may also provide a
    custom voice object with an `id`, for example `{ "id": "voice_1234" }`.

    - `VoiceIDsShared = string or "alloy" or "ash" or "ballad" or 7 more`

      - `UnionMember0 = string`

      - `UnionMember1 = "alloy" or "ash" or "ballad" or 7 more`

        - `"alloy"`

        - `"ash"`

        - `"ballad"`

        - `"coral"`

        - `"echo"`

        - `"sage"`

        - `"shimmer"`

        - `"verse"`

        - `"marin"`

        - `"cedar"`

    - `ID = object { id }`

      Custom voice reference.

      - `id: string`

        The custom voice ID, e.g. `voice_1234`.

- `frequency_penalty: optional number`

  Number between -2.0 and 2.0. Positive values penalize new tokens based on
  their existing frequency in the text so far, decreasing the model's
  likelihood to repeat the same line verbatim.

- `function_call: optional "none" or "auto" or ChatCompletionFunctionCallOption`

  Deprecated in favor of `tool_choice`.

  Controls which (if any) function is called by the model.

  `none` means the model will not call a function and instead generates a
  message.

  `auto` means the model can pick between generating a message or calling a
  function.

  Specifying a particular function via `{"name": "my_function"}` forces the
  model to call that function.

  `none` is the default when no functions are present. `auto` is the default
  if functions are present.

  - `UnionMember0 = "none" or "auto"`

    `none` means the model will not call a function and instead generates a message. `auto` means the model can pick between generating a message or calling a function.

    - `"none"`

    - `"auto"`

  - `ChatCompletionFunctionCallOption = object { name }`

    Specifying a particular function via `{"name": "my_function"}` forces the model to call that function.

    - `name: string`

      The name of the function to call.

- `functions: optional array of object { name, description, parameters }`

  Deprecated in favor of `tools`.

  A list of functions the model may generate JSON inputs for.

  - `name: string`

    The name of the function to be called. Must be a-z, A-Z, 0-9, or contain underscores and dashes, with a maximum length of 64.

  - `description: optional string`

    A description of what the function does, used by the model to choose when and how to call the function.

  - `parameters: optional FunctionParameters`

    The parameters the functions accepts, described as a JSON Schema object. See the [guide](/docs/guides/function-calling) for examples, and the [JSON Schema reference](https://json-schema.org/understanding-json-schema/) for documentation about the format.

    Omitting `parameters` defines a function with an empty parameter list.

- `logit_bias: optional map[number]`

  Modify the likelihood of specified tokens appearing in the completion.

  Accepts a JSON object that maps tokens (specified by their token ID in the
  tokenizer) to an associated bias value from -100 to 100. Mathematically,
  the bias is added to the logits generated by the model prior to sampling.
  The exact effect will vary per model, but values between -1 and 1 should
  decrease or increase likelihood of selection; values like -100 or 100
  should result in a ban or exclusive selection of the relevant token.

- `logprobs: optional boolean`

  Whether to return log probabilities of the output tokens or not. If true,
  returns the log probabilities of each output token returned in the
  `content` of `message`.

- `max_completion_tokens: optional number`

  An upper bound for the number of tokens that can be generated for a completion, including visible output tokens and [reasoning tokens](/docs/guides/reasoning).

- `max_tokens: optional number`

  The maximum number of [tokens](/tokenizer) that can be generated in the
  chat completion. This value can be used to control
  [costs](https://openai.com/api/pricing/) for text generated via API.

  This value is now deprecated in favor of `max_completion_tokens`, and is
  not compatible with [o-series models](/docs/guides/reasoning).

- `metadata: optional Metadata`

  Set of 16 key-value pairs that can be attached to an object. This can be
  useful for storing additional information about the object in a structured
  format, and querying for objects via API or the dashboard.

  Keys are strings with a maximum length of 64 characters. Values are strings
  with a maximum length of 512 characters.

- `modalities: optional array of "text" or "audio"`

  Output types that you would like the model to generate.
  Most models are capable of generating text, which is the default:

  `["text"]`

  The `gpt-4o-audio-preview` model can also be used to
  [generate audio](/docs/guides/audio). To request that this model generate
  both text and audio responses, you can use:

  `["text", "audio"]`

  - `"text"`

  - `"audio"`

- `n: optional number`

  How many chat completion choices to generate for each input message. Note that you will be charged based on the number of generated tokens across all of the choices. Keep `n` as `1` to minimize costs.

- `parallel_tool_calls: optional boolean`

  Whether to enable [parallel function calling](/docs/guides/function-calling#configuring-parallel-function-calling) during tool use.

- `prediction: optional ChatCompletionPredictionContent`

  Static predicted output content, such as the content of a text file that is
  being regenerated.

  - `content: string or array of ChatCompletionContentPartText`

    The content that should be matched when generating a model response.
    If generated tokens would match this content, the entire model response
    can be returned much more quickly.

    - `TextContent = string`

      The content used for a Predicted Output. This is often the
      text of a file you are regenerating with minor changes.

    - `ArrayOfContentParts = array of ChatCompletionContentPartText`

      An array of content parts with a defined type. Supported options differ based on the [model](/docs/models) being used to generate the response. Can contain text inputs.

      - `text: string`

        The text content.

      - `type: "text"`

        The type of the content part.

        - `"text"`

  - `type: "content"`

    The type of the predicted content you want to provide. This type is
    currently always `content`.

    - `"content"`

- `presence_penalty: optional number`

  Number between -2.0 and 2.0. Positive values penalize new tokens based on
  whether they appear in the text so far, increasing the model's likelihood
  to talk about new topics.

- `prompt_cache_key: optional string`

  Used by OpenAI to cache responses for similar requests to optimize your cache hit rates. Replaces the `user` field. [Learn more](/docs/guides/prompt-caching).

- `prompt_cache_retention: optional "in-memory" or "24h"`

  The retention policy for the prompt cache. Set to `24h` to enable extended prompt caching, which keeps cached prefixes active for longer, up to a maximum of 24 hours. [Learn more](/docs/guides/prompt-caching#prompt-cache-retention).

  - `"in-memory"`

  - `"24h"`

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

- `response_format: optional ResponseFormatText or ResponseFormatJSONSchema or ResponseFormatJSONObject`

  An object specifying the format that the model must output.

  Setting to `{ "type": "json_schema", "json_schema": {...} }` enables
  Structured Outputs which ensures the model will match your supplied JSON
  schema. Learn more in the [Structured Outputs
  guide](/docs/guides/structured-outputs).

  Setting to `{ "type": "json_object" }` enables the older JSON mode, which
  ensures the message the model generates is valid JSON. Using `json_schema`
  is preferred for models that support it.

  - `ResponseFormatText = object { type }`

    Default response format. Used to generate text responses.

    - `type: "text"`

      The type of response format being defined. Always `text`.

      - `"text"`

  - `ResponseFormatJSONSchema = object { json_schema, type }`

    JSON Schema response format. Used to generate structured JSON responses.
    Learn more about [Structured Outputs](/docs/guides/structured-outputs).

    - `json_schema: object { name, description, schema, strict }`

      Structured Outputs configuration options, including a JSON Schema.

      - `name: string`

        The name of the response format. Must be a-z, A-Z, 0-9, or contain
        underscores and dashes, with a maximum length of 64.

      - `description: optional string`

        A description of what the response format is for, used by the model to
        determine how to respond in the format.

      - `schema: optional map[unknown]`

        The schema for the response format, described as a JSON Schema object.
        Learn how to build JSON schemas [here](https://json-schema.org/).

      - `strict: optional boolean`

        Whether to enable strict schema adherence when generating the output.
        If set to true, the model will always follow the exact schema defined
        in the `schema` field. Only a subset of JSON Schema is supported when
        `strict` is `true`. To learn more, read the [Structured Outputs
        guide](/docs/guides/structured-outputs).

    - `type: "json_schema"`

      The type of response format being defined. Always `json_schema`.

      - `"json_schema"`

  - `ResponseFormatJSONObject = object { type }`

    JSON object response format. An older method of generating JSON responses.
    Using `json_schema` is recommended for models that support it. Note that the
    model will not generate JSON without a system or user message instructing it
    to do so.

    - `type: "json_object"`

      The type of response format being defined. Always `json_object`.

      - `"json_object"`

- `safety_identifier: optional string`

  A stable identifier used to help detect users of your application that may be violating OpenAI's usage policies.
  The IDs should be a string that uniquely identifies each user, with a maximum length of 64 characters. We recommend hashing their username or email address, in order to avoid sending us any identifying information. [Learn more](/docs/guides/safety-best-practices#safety-identifiers).

- `seed: optional number`

  This feature is in Beta.
  If specified, our system will make a best effort to sample deterministically, such that repeated requests with the same `seed` and parameters should return the same result.
  Determinism is not guaranteed, and you should refer to the `system_fingerprint` response parameter to monitor changes in the backend.

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

- `stop: optional string or array of string`

  Not supported with latest reasoning models `o3` and `o4-mini`.

  Up to 4 sequences where the API will stop generating further tokens. The
  returned text will not contain the stop sequence.

  - `UnionMember0 = string`

  - `UnionMember1 = array of string`

- `store: optional boolean`

  Whether or not to store the output of this chat completion request for
  use in our [model distillation](/docs/guides/distillation) or
  [evals](/docs/guides/evals) products.

  Supports text and image inputs. Note: image inputs over 8MB will be dropped.

- `stream: optional boolean`

  If set to true, the model response data will be streamed to the client
  as it is generated using [server-sent events](https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events#Event_stream_format).
  See the [Streaming section below](/docs/api-reference/chat/streaming)
  for more information, along with the [streaming responses](/docs/guides/streaming-responses)
  guide for more information on how to handle the streaming events.

- `stream_options: optional ChatCompletionStreamOptions`

  Options for streaming response. Only set this when you set `stream: true`.

  - `include_obfuscation: optional boolean`

    When true, stream obfuscation will be enabled. Stream obfuscation adds
    random characters to an `obfuscation` field on streaming delta events to
    normalize payload sizes as a mitigation to certain side-channel attacks.
    These obfuscation fields are included by default, but add a small amount
    of overhead to the data stream. You can set `include_obfuscation` to
    false to optimize for bandwidth if you trust the network links between
    your application and the OpenAI API.

  - `include_usage: optional boolean`

    If set, an additional chunk will be streamed before the `data: [DONE]`
    message. The `usage` field on this chunk shows the token usage statistics
    for the entire request, and the `choices` field will always be an empty
    array.

    All other chunks will also include a `usage` field, but with a null
    value. **NOTE:** If the stream is interrupted, you may not receive the
    final usage chunk which contains the total token usage for the request.

- `temperature: optional number`

  What sampling temperature to use, between 0 and 2. Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic.
  We generally recommend altering this or `top_p` but not both.

- `tool_choice: optional ChatCompletionToolChoiceOption`

  Controls which (if any) tool is called by the model.
  `none` means the model will not call any tool and instead generates a message.
  `auto` means the model can pick between generating a message or calling one or more tools.
  `required` means the model must call one or more tools.
  Specifying a particular tool via `{"type": "function", "function": {"name": "my_function"}}` forces the model to call that tool.

  `none` is the default when no tools are present. `auto` is the default if tools are present.

  - `ToolChoiceMode = "none" or "auto" or "required"`

    `none` means the model will not call any tool and instead generates a message. `auto` means the model can pick between generating a message or calling one or more tools. `required` means the model must call one or more tools.

    - `"none"`

    - `"auto"`

    - `"required"`

  - `ChatCompletionAllowedToolChoice = object { allowed_tools, type }`

    Constrains the tools available to the model to a pre-defined set.

    - `allowed_tools: ChatCompletionAllowedTools`

      Constrains the tools available to the model to a pre-defined set.

      - `mode: "auto" or "required"`

        Constrains the tools available to the model to a pre-defined set.

        `auto` allows the model to pick from among the allowed tools and generate a
        message.

        `required` requires the model to call one or more of the allowed tools.

        - `"auto"`

        - `"required"`

      - `tools: array of map[unknown]`

        A list of tool definitions that the model should be allowed to call.

        For the Chat Completions API, the list of tool definitions might look like:

        ```json
        [
          { "type": "function", "function": { "name": "get_weather" } },
          { "type": "function", "function": { "name": "get_time" } }
        ]
        ```

    - `type: "allowed_tools"`

      Allowed tool configuration type. Always `allowed_tools`.

      - `"allowed_tools"`

  - `ChatCompletionNamedToolChoice = object { function, type }`

    Specifies a tool the model should use. Use to force the model to call a specific function.

    - `function: object { name }`

      - `name: string`

        The name of the function to call.

    - `type: "function"`

      For function calling, the type is always `function`.

      - `"function"`

  - `ChatCompletionNamedToolChoiceCustom = object { custom, type }`

    Specifies a tool the model should use. Use to force the model to call a specific custom tool.

    - `custom: object { name }`

      - `name: string`

        The name of the custom tool to call.

    - `type: "custom"`

      For custom tool calling, the type is always `custom`.

      - `"custom"`

- `tools: optional array of ChatCompletionTool`

  A list of tools the model may call. You can provide either
  [custom tools](/docs/guides/function-calling#custom-tools) or
  [function tools](/docs/guides/function-calling).

  - `ChatCompletionFunctionTool = object { function, type }`

    A function tool that can be used to generate a response.

    - `function: FunctionDefinition`

      - `name: string`

        The name of the function to be called. Must be a-z, A-Z, 0-9, or contain underscores and dashes, with a maximum length of 64.

      - `description: optional string`

        A description of what the function does, used by the model to choose when and how to call the function.

      - `parameters: optional FunctionParameters`

        The parameters the functions accepts, described as a JSON Schema object. See the [guide](/docs/guides/function-calling) for examples, and the [JSON Schema reference](https://json-schema.org/understanding-json-schema/) for documentation about the format.

        Omitting `parameters` defines a function with an empty parameter list.

      - `strict: optional boolean`

        Whether to enable strict schema adherence when generating the function call. If set to true, the model will follow the exact schema defined in the `parameters` field. Only a subset of JSON Schema is supported when `strict` is `true`. Learn more about Structured Outputs in the [function calling guide](/docs/guides/function-calling).

    - `type: "function"`

      The type of the tool. Currently, only `function` is supported.

      - `"function"`

  - `ChatCompletionCustomTool = object { custom, type }`

    A custom tool that processes input using a specified format.

    - `custom: object { name, description, format }`

      Properties of the custom tool.

      - `name: string`

        The name of the custom tool, used to identify it in tool calls.

      - `description: optional string`

        Optional description of the custom tool, used to provide more context.

      - `format: optional object { type }  or object { grammar, type }`

        The input format for the custom tool. Default is unconstrained text.

        - `TextFormat = object { type }`

          Unconstrained free-form text.

          - `type: "text"`

            Unconstrained text format. Always `text`.

            - `"text"`

        - `GrammarFormat = object { grammar, type }`

          A grammar defined by the user.

          - `grammar: object { definition, syntax }`

            Your chosen grammar.

            - `definition: string`

              The grammar definition.

            - `syntax: "lark" or "regex"`

              The syntax of the grammar definition. One of `lark` or `regex`.

              - `"lark"`

              - `"regex"`

          - `type: "grammar"`

            Grammar format. Always `grammar`.

            - `"grammar"`

    - `type: "custom"`

      The type of the custom tool. Always `custom`.

      - `"custom"`

- `top_logprobs: optional number`

  An integer between 0 and 20 specifying the number of most likely tokens to
  return at each token position, each with an associated log probability.
  `logprobs` must be set to `true` if this parameter is used.

- `top_p: optional number`

  An alternative to sampling with temperature, called nucleus sampling,
  where the model considers the results of the tokens with top_p probability
  mass. So 0.1 means only the tokens comprising the top 10% probability mass
  are considered.

  We generally recommend altering this or `temperature` but not both.

- `user: optional string`

  This field is being replaced by `safety_identifier` and `prompt_cache_key`. Use `prompt_cache_key` instead to maintain caching optimizations.
  A stable identifier for your end-users.
  Used to boost cache hit rates by better bucketing similar requests and  to help OpenAI detect and prevent abuse. [Learn more](/docs/guides/safety-best-practices#safety-identifiers).

- `verbosity: optional "low" or "medium" or "high"`

  Constrains the verbosity of the model's response. Lower values will result in
  more concise responses, while higher values will result in more verbose responses.
  Currently supported values are `low`, `medium`, and `high`.

  - `"low"`

  - `"medium"`

  - `"high"`

- `web_search_options: optional object { search_context_size, user_location }`

  This tool searches the web for relevant results to use in a response.
  Learn more about the [web search tool](/docs/guides/tools-web-search?api-mode=chat).

  - `search_context_size: optional "low" or "medium" or "high"`

    High level guidance for the amount of context window space to use for the
    search. One of `low`, `medium`, or `high`. `medium` is the default.

    - `"low"`

    - `"medium"`

    - `"high"`

  - `user_location: optional object { approximate, type }`

    Approximate location parameters for the search.

    - `approximate: object { city, country, region, timezone }`

      Approximate location parameters for the search.

      - `city: optional string`

        Free text input for the city of the user, e.g. `San Francisco`.

      - `country: optional string`

        The two-letter
        [ISO country code](https://en.wikipedia.org/wiki/ISO_3166-1) of the user,
        e.g. `US`.

      - `region: optional string`

        Free text input for the region of the user, e.g. `California`.

      - `timezone: optional string`

        The [IANA timezone](https://timeapi.io/documentation/iana-timezones)
        of the user, e.g. `America/Los_Angeles`.

    - `type: "approximate"`

      The type of location approximation. Always `approximate`.

      - `"approximate"`

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
curl https://api.openai.com/v1/chat/completions \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "messages": [
            {
              "content": "string",
              "role": "developer"
            }
          ],
          "model": "gpt-4o",
          "n": 1,
          "prompt_cache_key": "prompt-cache-key-1234",
          "safety_identifier": "safety-identifier-1234",
          "temperature": 1,
          "top_p": 1,
          "user": "user-1234"
        }'
```

## Create

**post** `/realtime/client_secrets`

Create a Realtime client secret with an associated session configuration.

Client secrets are short-lived tokens that can be passed to a client app,
such as a web frontend or mobile client, which grants access to the Realtime API without
leaking your main API key. You can configure a custom TTL for each client secret.

You can also attach session configuration options to the client secret, which will be
applied to any sessions created using that client secret, but these can also be overridden
by the client connection.

[Learn more about authentication with client secrets over WebRTC](/docs/guides/realtime-webrtc).

Returns the created client secret and the effective session object. The client secret is a string that looks like `ek_1234`.

### Body Parameters

- `expires_after: optional object { anchor, seconds }`

  Configuration for the client secret expiration. Expiration refers to the time after which
  a client secret will no longer be valid for creating sessions. The session itself may
  continue after that time once started. A secret can be used to create multiple sessions
  until it expires.

  - `anchor: optional "created_at"`

    The anchor point for the client secret expiration, meaning that `seconds` will be added to the `created_at` time of the client secret to produce an expiration timestamp. Only `created_at` is currently supported.

    - `"created_at"`

  - `seconds: optional number`

    The number of seconds from the anchor point to the expiration. Select a value between `10` and `7200` (2 hours). This default to 600 seconds (10 minutes) if not specified.

- `session: optional RealtimeSessionCreateRequest or RealtimeTranscriptionSessionCreateRequest`

  Session configuration to use for the client secret. Choose either a realtime
  session or a transcription session.

  - `RealtimeSessionCreateRequest = object { type, audio, include, 9 more }`

    Realtime session object configuration.

    - `type: "realtime"`

      The type of session to create. Always `realtime` for the Realtime API.

      - `"realtime"`

    - `audio: optional RealtimeAudioConfig`

      Configuration for input and output audio.

      - `input: optional RealtimeAudioConfigInput`

        - `format: optional RealtimeAudioFormats`

          The format of the input audio.

          - `PCMAudioFormat = object { rate, type }`

            The PCM audio format. Only a 24kHz sample rate is supported.

            - `rate: optional 24000`

              The sample rate of the audio. Always `24000`.

              - `24000`

            - `type: optional "audio/pcm"`

              The audio format. Always `audio/pcm`.

              - `"audio/pcm"`

          - `PCMUAudioFormat = object { type }`

            The G.711 μ-law format.

            - `type: optional "audio/pcmu"`

              The audio format. Always `audio/pcmu`.

              - `"audio/pcmu"`

          - `PCMAAudioFormat = object { type }`

            The G.711 A-law format.

            - `type: optional "audio/pcma"`

              The audio format. Always `audio/pcma`.

              - `"audio/pcma"`

        - `noise_reduction: optional object { type }`

          Configuration for input audio noise reduction. This can be set to `null` to turn off.
          Noise reduction filters audio added to the input audio buffer before it is sent to VAD and the model.
          Filtering the audio can improve VAD and turn detection accuracy (reducing false positives) and model performance by improving perception of the input audio.

          - `type: optional NoiseReductionType`

            Type of noise reduction. `near_field` is for close-talking microphones such as headphones, `far_field` is for far-field microphones such as laptop or conference room microphones.

            - `"near_field"`

            - `"far_field"`

        - `transcription: optional AudioTranscription`

          Configuration for input audio transcription, defaults to off and can be set to `null` to turn off once on. Input audio transcription is not native to the model, since the model consumes audio directly. Transcription runs asynchronously through [the /audio/transcriptions endpoint](/docs/api-reference/audio/createTranscription) and should be treated as guidance of input audio content rather than precisely what the model heard. The client can optionally set the language and prompt for transcription, these offer additional guidance to the transcription service.

          - `language: optional string`

            The language of the input audio. Supplying the input language in
            [ISO-639-1](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) (e.g. `en`) format
            will improve accuracy and latency.

          - `model: optional string or "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

            The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

            - `UnionMember0 = string`

            - `UnionMember1 = "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

              The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

              - `"whisper-1"`

              - `"gpt-4o-mini-transcribe"`

              - `"gpt-4o-mini-transcribe-2025-12-15"`

              - `"gpt-4o-transcribe"`

              - `"gpt-4o-transcribe-diarize"`

          - `prompt: optional string`

            An optional text to guide the model's style or continue a previous audio
            segment.
            For `whisper-1`, the [prompt is a list of keywords](/docs/guides/speech-to-text#prompting).
            For `gpt-4o-transcribe` models (excluding `gpt-4o-transcribe-diarize`), the prompt is a free text string, for example "expect words related to technology".

        - `turn_detection: optional RealtimeAudioInputTurnDetection`

          Configuration for turn detection, ether Server VAD or Semantic VAD. This can be set to `null` to turn off, in which case the client must manually trigger model response.

          Server VAD means that the model will detect the start and end of speech based on audio volume and respond at the end of user speech.

          Semantic VAD is more advanced and uses a turn detection model (in conjunction with VAD) to semantically estimate whether the user has finished speaking, then dynamically sets a timeout based on this probability. For example, if user audio trails off with "uhhm", the model will score a low probability of turn end and wait longer for the user to continue speaking. This can be useful for more natural conversations, but may have a higher latency.

          - `ServerVad = object { type, create_response, idle_timeout_ms, 4 more }`

            Server-side voice activity detection (VAD) which flips on when user speech is detected and off after a period of silence.

            - `type: "server_vad"`

              Type of turn detection, `server_vad` to turn on simple Server VAD.

              - `"server_vad"`

            - `create_response: optional boolean`

              Whether or not to automatically generate a response when a VAD stop event occurs. If `interrupt_response` is set to `false` this may fail to create a response if the model is already responding.

              If both `create_response` and `interrupt_response` are set to `false`, the model will never respond automatically but VAD events will still be emitted.

            - `idle_timeout_ms: optional number`

              Optional timeout after which a model response will be triggered automatically. This is
              useful for situations in which a long pause from the user is unexpected, such as a phone
              call. The model will effectively prompt the user to continue the conversation based
              on the current context.

              The timeout value will be applied after the last model response's audio has finished playing,
              i.e. it's set to the `response.done` time plus audio playback duration.

              An `input_audio_buffer.timeout_triggered` event (plus events
              associated with the Response) will be emitted when the timeout is reached.
              Idle timeout is currently only supported for `server_vad` mode.

            - `interrupt_response: optional boolean`

              Whether or not to automatically interrupt (cancel) any ongoing response with output to the default
              conversation (i.e. `conversation` of `auto`) when a VAD start event occurs. If `true` then the response will be cancelled, otherwise it will continue until complete.

              If both `create_response` and `interrupt_response` are set to `false`, the model will never respond automatically but VAD events will still be emitted.

            - `prefix_padding_ms: optional number`

              Used only for `server_vad` mode. Amount of audio to include before the VAD detected speech (in
              milliseconds). Defaults to 300ms.

            - `silence_duration_ms: optional number`

              Used only for `server_vad` mode. Duration of silence to detect speech stop (in milliseconds). Defaults
              to 500ms. With shorter values the model will respond more quickly,
              but may jump in on short pauses from the user.

            - `threshold: optional number`

              Used only for `server_vad` mode. Activation threshold for VAD (0.0 to 1.0), this defaults to 0.5. A
              higher threshold will require louder audio to activate the model, and
              thus might perform better in noisy environments.

          - `SemanticVad = object { type, create_response, eagerness, interrupt_response }`

            Server-side semantic turn detection which uses a model to determine when the user has finished speaking.

            - `type: "semantic_vad"`

              Type of turn detection, `semantic_vad` to turn on Semantic VAD.

              - `"semantic_vad"`

            - `create_response: optional boolean`

              Whether or not to automatically generate a response when a VAD stop event occurs.

            - `eagerness: optional "low" or "medium" or "high" or "auto"`

              Used only for `semantic_vad` mode. The eagerness of the model to respond. `low` will wait longer for the user to continue speaking, `high` will respond more quickly. `auto` is the default and is equivalent to `medium`. `low`, `medium`, and `high` have max timeouts of 8s, 4s, and 2s respectively.

              - `"low"`

              - `"medium"`

              - `"high"`

              - `"auto"`

            - `interrupt_response: optional boolean`

              Whether or not to automatically interrupt any ongoing response with output to the default
              conversation (i.e. `conversation` of `auto`) when a VAD start event occurs.

      - `output: optional RealtimeAudioConfigOutput`

        - `format: optional RealtimeAudioFormats`

          The format of the output audio.

          - `PCMAudioFormat = object { rate, type }`

            The PCM audio format. Only a 24kHz sample rate is supported.

            - `rate: optional 24000`

              The sample rate of the audio. Always `24000`.

              - `24000`

            - `type: optional "audio/pcm"`

              The audio format. Always `audio/pcm`.

              - `"audio/pcm"`

          - `PCMUAudioFormat = object { type }`

            The G.711 μ-law format.

            - `type: optional "audio/pcmu"`

              The audio format. Always `audio/pcmu`.

              - `"audio/pcmu"`

          - `PCMAAudioFormat = object { type }`

            The G.711 A-law format.

            - `type: optional "audio/pcma"`

              The audio format. Always `audio/pcma`.

              - `"audio/pcma"`

        - `speed: optional number`

          The speed of the model's spoken response as a multiple of the original speed.
          1.0 is the default speed. 0.25 is the minimum speed. 1.5 is the maximum speed. This value can only be changed in between model turns, not while a response is in progress.

          This parameter is a post-processing adjustment to the audio after it is generated, it's
          also possible to prompt the model to speak faster or slower.

        - `voice: optional string or "alloy" or "ash" or "ballad" or 7 more or object { id }`

          The voice the model uses to respond. Supported built-in voices are
          `alloy`, `ash`, `ballad`, `coral`, `echo`, `sage`, `shimmer`, `verse`,
          `marin`, and `cedar`. You may also provide a custom voice object with
          an `id`, for example `{ "id": "voice_1234" }`. Voice cannot be changed
          during the session once the model has responded with audio at least once.
          We recommend `marin` and `cedar` for best quality.

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

    - `include: optional array of "item.input_audio_transcription.logprobs"`

      Additional fields to include in server outputs.

      `item.input_audio_transcription.logprobs`: Include logprobs for input audio transcription.

      - `"item.input_audio_transcription.logprobs"`

    - `instructions: optional string`

      The default system instructions (i.e. system message) prepended to model calls. This field allows the client to guide the model on desired responses. The model can be instructed on response content and format, (e.g. "be extremely succinct", "act friendly", "here are examples of good responses") and on audio behavior (e.g. "talk quickly", "inject emotion into your voice", "laugh frequently"). The instructions are not guaranteed to be followed by the model, but they provide guidance to the model on the desired behavior.

      Note that the server sets default instructions which will be used if this field is not set and are visible in the `session.created` event at the start of the session.

    - `max_output_tokens: optional number or "inf"`

      Maximum number of output tokens for a single assistant response,
      inclusive of tool calls. Provide an integer between 1 and 4096 to
      limit output tokens, or `inf` for the maximum available tokens for a
      given model. Defaults to `inf`.

      - `UnionMember0 = number`

      - `UnionMember1 = "inf"`

        - `"inf"`

    - `model: optional string or "gpt-realtime" or "gpt-realtime-1.5" or "gpt-realtime-2025-08-28" or 13 more`

      The Realtime model used for this session.

      - `UnionMember0 = string`

      - `UnionMember1 = "gpt-realtime" or "gpt-realtime-1.5" or "gpt-realtime-2025-08-28" or 13 more`

        The Realtime model used for this session.

        - `"gpt-realtime"`

        - `"gpt-realtime-1.5"`

        - `"gpt-realtime-2025-08-28"`

        - `"gpt-4o-realtime-preview"`

        - `"gpt-4o-realtime-preview-2024-10-01"`

        - `"gpt-4o-realtime-preview-2024-12-17"`

        - `"gpt-4o-realtime-preview-2025-06-03"`

        - `"gpt-4o-mini-realtime-preview"`

        - `"gpt-4o-mini-realtime-preview-2024-12-17"`

        - `"gpt-realtime-mini"`

        - `"gpt-realtime-mini-2025-10-06"`

        - `"gpt-realtime-mini-2025-12-15"`

        - `"gpt-audio-1.5"`

        - `"gpt-audio-mini"`

        - `"gpt-audio-mini-2025-10-06"`

        - `"gpt-audio-mini-2025-12-15"`

    - `output_modalities: optional array of "text" or "audio"`

      The set of modalities the model can respond with. It defaults to `["audio"]`, indicating
      that the model will respond with audio plus a transcript. `["text"]` can be used to make
      the model respond with text only. It is not possible to request both `text` and `audio` at the same time.

      - `"text"`

      - `"audio"`

    - `prompt: optional ResponsePrompt`

      Reference to a prompt template and its variables.
      [Learn more](/docs/guides/text?api-mode=responses#reusable-prompts).

      - `id: string`

        The unique identifier of the prompt template to use.

      - `variables: optional map[string or ResponseInputText or ResponseInputImage or ResponseInputFile]`

        Optional map of values to substitute in for variables in your
        prompt. The substitution values can either be strings, or other
        Response input types like images or files.

        - `UnionMember0 = string`

        - `ResponseInputText = object { text, type }`

          A text input to the model.

          - `text: string`

            The text input to the model.

          - `type: "input_text"`

            The type of the input item. Always `input_text`.

            - `"input_text"`

        - `ResponseInputImage = object { detail, type, file_id, image_url }`

          An image input to the model. Learn about [image inputs](/docs/guides/vision).

          - `detail: "low" or "high" or "auto"`

            The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

            - `"low"`

            - `"high"`

            - `"auto"`

          - `type: "input_image"`

            The type of the input item. Always `input_image`.

            - `"input_image"`

          - `file_id: optional string`

            The ID of the file to be sent to the model.

          - `image_url: optional string`

            The URL of the image to be sent to the model. A fully qualified URL or base64 encoded image in a data URL.

        - `ResponseInputFile = object { type, file_data, file_id, 2 more }`

          A file input to the model.

          - `type: "input_file"`

            The type of the input item. Always `input_file`.

            - `"input_file"`

          - `file_data: optional string`

            The content of the file to be sent to the model.

          - `file_id: optional string`

            The ID of the file to be sent to the model.

          - `file_url: optional string`

            The URL of the file to be sent to the model.

          - `filename: optional string`

            The name of the file to be sent to the model.

      - `version: optional string`

        Optional version of the prompt template.

    - `tool_choice: optional RealtimeToolChoiceConfig`

      How the model chooses tools. Provide one of the string modes or force a specific
      function/MCP tool.

      - `ToolChoiceOptions = "none" or "auto" or "required"`

        Controls which (if any) tool is called by the model.

        `none` means the model will not call any tool and instead generates a message.

        `auto` means the model can pick between generating a message or calling one or
        more tools.

        `required` means the model must call one or more tools.

        - `"none"`

        - `"auto"`

        - `"required"`

      - `ToolChoiceFunction = object { name, type }`

        Use this option to force the model to call a specific function.

        - `name: string`

          The name of the function to call.

        - `type: "function"`

          For function calling, the type is always `function`.

          - `"function"`

      - `ToolChoiceMcp = object { server_label, type, name }`

        Use this option to force the model to call a specific tool on a remote MCP server.

        - `server_label: string`

          The label of the MCP server to use.

        - `type: "mcp"`

          For MCP tools, the type is always `mcp`.

          - `"mcp"`

        - `name: optional string`

          The name of the tool to call on the server.

    - `tools: optional RealtimeToolsConfig`

      Tools available to the model.

      - `RealtimeFunctionTool = object { description, name, parameters, type }`

        - `description: optional string`

          The description of the function, including guidance on when and how
          to call it, and guidance about what to tell the user when calling
          (if anything).

        - `name: optional string`

          The name of the function.

        - `parameters: optional unknown`

          Parameters of the function in JSON Schema.

        - `type: optional "function"`

          The type of the tool, i.e. `function`.

          - `"function"`

      - `McpTool = object { server_label, type, allowed_tools, 6 more }`

        Give the model access to additional tools via remote Model Context Protocol
        (MCP) servers. [Learn more about MCP](/docs/guides/tools-remote-mcp).

        - `server_label: string`

          A label for this MCP server, used to identify it in tool calls.

        - `type: "mcp"`

          The type of the MCP tool. Always `mcp`.

          - `"mcp"`

        - `allowed_tools: optional array of string or object { read_only, tool_names }`

          List of allowed tool names or a filter object.

          - `McpAllowedTools = array of string`

            A string array of allowed tool names

          - `McpToolFilter = object { read_only, tool_names }`

            A filter object to specify which tools are allowed.

            - `read_only: optional boolean`

              Indicates whether or not a tool modifies data or is read-only. If an
              MCP server is [annotated with `readOnlyHint`](https://modelcontextprotocol.io/specification/2025-06-18/schema#toolannotations-readonlyhint),
              it will match this filter.

            - `tool_names: optional array of string`

              List of allowed tool names.

        - `authorization: optional string`

          An OAuth access token that can be used with a remote MCP server, either
          with a custom MCP server URL or a service connector. Your application
          must handle the OAuth authorization flow and provide the token here.

        - `connector_id: optional "connector_dropbox" or "connector_gmail" or "connector_googlecalendar" or 5 more`

          Identifier for service connectors, like those available in ChatGPT. One of
          `server_url` or `connector_id` must be provided. Learn more about service
          connectors [here](/docs/guides/tools-remote-mcp#connectors).

          Currently supported `connector_id` values are:

          - Dropbox: `connector_dropbox`
          - Gmail: `connector_gmail`
          - Google Calendar: `connector_googlecalendar`
          - Google Drive: `connector_googledrive`
          - Microsoft Teams: `connector_microsoftteams`
          - Outlook Calendar: `connector_outlookcalendar`
          - Outlook Email: `connector_outlookemail`
          - SharePoint: `connector_sharepoint`

          - `"connector_dropbox"`

          - `"connector_gmail"`

          - `"connector_googlecalendar"`

          - `"connector_googledrive"`

          - `"connector_microsoftteams"`

          - `"connector_outlookcalendar"`

          - `"connector_outlookemail"`

          - `"connector_sharepoint"`

        - `headers: optional map[string]`

          Optional HTTP headers to send to the MCP server. Use for authentication
          or other purposes.

        - `require_approval: optional object { always, never }  or "always" or "never"`

          Specify which of the MCP server's tools require approval.

          - `McpToolApprovalFilter = object { always, never }`

            Specify which of the MCP server's tools require approval. Can be
            `always`, `never`, or a filter object associated with tools
            that require approval.

            - `always: optional object { read_only, tool_names }`

              A filter object to specify which tools are allowed.

              - `read_only: optional boolean`

                Indicates whether or not a tool modifies data or is read-only. If an
                MCP server is [annotated with `readOnlyHint`](https://modelcontextprotocol.io/specification/2025-06-18/schema#toolannotations-readonlyhint),
                it will match this filter.

              - `tool_names: optional array of string`

                List of allowed tool names.

            - `never: optional object { read_only, tool_names }`

              A filter object to specify which tools are allowed.

              - `read_only: optional boolean`

                Indicates whether or not a tool modifies data or is read-only. If an
                MCP server is [annotated with `readOnlyHint`](https://modelcontextprotocol.io/specification/2025-06-18/schema#toolannotations-readonlyhint),
                it will match this filter.

              - `tool_names: optional array of string`

                List of allowed tool names.

          - `McpToolApprovalSetting = "always" or "never"`

            Specify a single approval policy for all tools. One of `always` or
            `never`. When set to `always`, all tools will require approval. When
            set to `never`, all tools will not require approval.

            - `"always"`

            - `"never"`

        - `server_description: optional string`

          Optional description of the MCP server, used to provide more context.

        - `server_url: optional string`

          The URL for the MCP server. One of `server_url` or `connector_id` must be
          provided.

    - `tracing: optional RealtimeTracingConfig`

      Realtime API can write session traces to the [Traces Dashboard](/logs?api=traces). Set to null to disable tracing. Once
      tracing is enabled for a session, the configuration cannot be modified.

      `auto` will create a trace for the session with default values for the
      workflow name, group id, and metadata.

      - `Auto = "auto"`

        Enables tracing and sets default values for tracing configuration options. Always `auto`.

        - `"auto"`

      - `TracingConfiguration = object { group_id, metadata, workflow_name }`

        Granular configuration for tracing.

        - `group_id: optional string`

          The group id to attach to this trace to enable filtering and
          grouping in the Traces Dashboard.

        - `metadata: optional unknown`

          The arbitrary metadata to attach to this trace to enable
          filtering in the Traces Dashboard.

        - `workflow_name: optional string`

          The name of the workflow to attach to this trace. This is used to
          name the trace in the Traces Dashboard.

    - `truncation: optional RealtimeTruncation`

      When the number of tokens in a conversation exceeds the model's input token limit, the conversation be truncated, meaning messages (starting from the oldest) will not be included in the model's context. A 32k context model with 4,096 max output tokens can only include 28,224 tokens in the context before truncation occurs.

      Clients can configure truncation behavior to truncate with a lower max token limit, which is an effective way to control token usage and cost.

      Truncation will reduce the number of cached tokens on the next turn (busting the cache), since messages are dropped from the beginning of the context. However, clients can also configure truncation to retain messages up to a fraction of the maximum context size, which will reduce the need for future truncations and thus improve the cache rate.

      Truncation can be disabled entirely, which means the server will never truncate but would instead return an error if the conversation exceeds the model's input token limit.

      - `UnionMember0 = "auto" or "disabled"`

        The truncation strategy to use for the session. `auto` is the default truncation strategy. `disabled` will disable truncation and emit errors when the conversation exceeds the input token limit.

        - `"auto"`

        - `"disabled"`

      - `RetentionRatioTruncation = object { retention_ratio, type, token_limits }`

        Retain a fraction of the conversation tokens when the conversation exceeds the input token limit. This allows you to amortize truncations across multiple turns, which can help improve cached token usage.

        - `retention_ratio: number`

          Fraction of post-instruction conversation tokens to retain (`0.0` - `1.0`) when the conversation exceeds the input token limit. Setting this to `0.8` means that messages will be dropped until 80% of the maximum allowed tokens are used. This helps reduce the frequency of truncations and improve cache rates.

        - `type: "retention_ratio"`

          Use retention ratio truncation.

          - `"retention_ratio"`

        - `token_limits: optional object { post_instructions }`

          Optional custom token limits for this truncation strategy. If not provided, the model's default token limits will be used.

          - `post_instructions: optional number`

            Maximum tokens allowed in the conversation after instructions (which including tool definitions). For example, setting this to 5,000 would mean that truncation would occur when the conversation exceeds 5,000 tokens after instructions. This cannot be higher than the model's context window size minus the maximum output tokens.

  - `RealtimeTranscriptionSessionCreateRequest = object { type, audio, include }`

    Realtime transcription session object configuration.

    - `type: "transcription"`

      The type of session to create. Always `transcription` for transcription sessions.

      - `"transcription"`

    - `audio: optional RealtimeTranscriptionSessionAudio`

      Configuration for input and output audio.

      - `input: optional RealtimeTranscriptionSessionAudioInput`

        - `format: optional RealtimeAudioFormats`

          The PCM audio format. Only a 24kHz sample rate is supported.

          - `PCMAudioFormat = object { rate, type }`

            The PCM audio format. Only a 24kHz sample rate is supported.

            - `rate: optional 24000`

              The sample rate of the audio. Always `24000`.

              - `24000`

            - `type: optional "audio/pcm"`

              The audio format. Always `audio/pcm`.

              - `"audio/pcm"`

          - `PCMUAudioFormat = object { type }`

            The G.711 μ-law format.

            - `type: optional "audio/pcmu"`

              The audio format. Always `audio/pcmu`.

              - `"audio/pcmu"`

          - `PCMAAudioFormat = object { type }`

            The G.711 A-law format.

            - `type: optional "audio/pcma"`

              The audio format. Always `audio/pcma`.

              - `"audio/pcma"`

        - `noise_reduction: optional object { type }`

          Configuration for input audio noise reduction. This can be set to `null` to turn off.
          Noise reduction filters audio added to the input audio buffer before it is sent to VAD and the model.
          Filtering the audio can improve VAD and turn detection accuracy (reducing false positives) and model performance by improving perception of the input audio.

          - `type: optional NoiseReductionType`

            Type of noise reduction. `near_field` is for close-talking microphones such as headphones, `far_field` is for far-field microphones such as laptop or conference room microphones.

            - `"near_field"`

            - `"far_field"`

        - `transcription: optional AudioTranscription`

          Configuration for input audio transcription, defaults to off and can be set to `null` to turn off once on. Input audio transcription is not native to the model, since the model consumes audio directly. Transcription runs asynchronously through [the /audio/transcriptions endpoint](/docs/api-reference/audio/createTranscription) and should be treated as guidance of input audio content rather than precisely what the model heard. The client can optionally set the language and prompt for transcription, these offer additional guidance to the transcription service.

          - `language: optional string`

            The language of the input audio. Supplying the input language in
            [ISO-639-1](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) (e.g. `en`) format
            will improve accuracy and latency.

          - `model: optional string or "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

            The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

            - `UnionMember0 = string`

            - `UnionMember1 = "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

              The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

              - `"whisper-1"`

              - `"gpt-4o-mini-transcribe"`

              - `"gpt-4o-mini-transcribe-2025-12-15"`

              - `"gpt-4o-transcribe"`

              - `"gpt-4o-transcribe-diarize"`

          - `prompt: optional string`

            An optional text to guide the model's style or continue a previous audio
            segment.
            For `whisper-1`, the [prompt is a list of keywords](/docs/guides/speech-to-text#prompting).
            For `gpt-4o-transcribe` models (excluding `gpt-4o-transcribe-diarize`), the prompt is a free text string, for example "expect words related to technology".

        - `turn_detection: optional RealtimeTranscriptionSessionAudioInputTurnDetection`

          Configuration for turn detection, ether Server VAD or Semantic VAD. This can be set to `null` to turn off, in which case the client must manually trigger model response.

          Server VAD means that the model will detect the start and end of speech based on audio volume and respond at the end of user speech.

          Semantic VAD is more advanced and uses a turn detection model (in conjunction with VAD) to semantically estimate whether the user has finished speaking, then dynamically sets a timeout based on this probability. For example, if user audio trails off with "uhhm", the model will score a low probability of turn end and wait longer for the user to continue speaking. This can be useful for more natural conversations, but may have a higher latency.

          - `ServerVad = object { type, create_response, idle_timeout_ms, 4 more }`

            Server-side voice activity detection (VAD) which flips on when user speech is detected and off after a period of silence.

            - `type: "server_vad"`

              Type of turn detection, `server_vad` to turn on simple Server VAD.

              - `"server_vad"`

            - `create_response: optional boolean`

              Whether or not to automatically generate a response when a VAD stop event occurs. If `interrupt_response` is set to `false` this may fail to create a response if the model is already responding.

              If both `create_response` and `interrupt_response` are set to `false`, the model will never respond automatically but VAD events will still be emitted.

            - `idle_timeout_ms: optional number`

              Optional timeout after which a model response will be triggered automatically. This is
              useful for situations in which a long pause from the user is unexpected, such as a phone
              call. The model will effectively prompt the user to continue the conversation based
              on the current context.

              The timeout value will be applied after the last model response's audio has finished playing,
              i.e. it's set to the `response.done` time plus audio playback duration.

              An `input_audio_buffer.timeout_triggered` event (plus events
              associated with the Response) will be emitted when the timeout is reached.
              Idle timeout is currently only supported for `server_vad` mode.

            - `interrupt_response: optional boolean`

              Whether or not to automatically interrupt (cancel) any ongoing response with output to the default
              conversation (i.e. `conversation` of `auto`) when a VAD start event occurs. If `true` then the response will be cancelled, otherwise it will continue until complete.

              If both `create_response` and `interrupt_response` are set to `false`, the model will never respond automatically but VAD events will still be emitted.

            - `prefix_padding_ms: optional number`

              Used only for `server_vad` mode. Amount of audio to include before the VAD detected speech (in
              milliseconds). Defaults to 300ms.

            - `silence_duration_ms: optional number`

              Used only for `server_vad` mode. Duration of silence to detect speech stop (in milliseconds). Defaults
              to 500ms. With shorter values the model will respond more quickly,
              but may jump in on short pauses from the user.

            - `threshold: optional number`

              Used only for `server_vad` mode. Activation threshold for VAD (0.0 to 1.0), this defaults to 0.5. A
              higher threshold will require louder audio to activate the model, and
              thus might perform better in noisy environments.

          - `SemanticVad = object { type, create_response, eagerness, interrupt_response }`

            Server-side semantic turn detection which uses a model to determine when the user has finished speaking.

            - `type: "semantic_vad"`

              Type of turn detection, `semantic_vad` to turn on Semantic VAD.

              - `"semantic_vad"`

            - `create_response: optional boolean`

              Whether or not to automatically generate a response when a VAD stop event occurs.

            - `eagerness: optional "low" or "medium" or "high" or "auto"`

              Used only for `semantic_vad` mode. The eagerness of the model to respond. `low` will wait longer for the user to continue speaking, `high` will respond more quickly. `auto` is the default and is equivalent to `medium`. `low`, `medium`, and `high` have max timeouts of 8s, 4s, and 2s respectively.

              - `"low"`

              - `"medium"`

              - `"high"`

              - `"auto"`

            - `interrupt_response: optional boolean`

              Whether or not to automatically interrupt any ongoing response with output to the default
              conversation (i.e. `conversation` of `auto`) when a VAD start event occurs.

    - `include: optional array of "item.input_audio_transcription.logprobs"`

      Additional fields to include in server outputs.

      `item.input_audio_transcription.logprobs`: Include logprobs for input audio transcription.

      - `"item.input_audio_transcription.logprobs"`

### Returns

- `expires_at: number`

  Expiration timestamp for the client secret, in seconds since epoch.

- `session: RealtimeSessionCreateResponse or RealtimeTranscriptionSessionCreateResponse`

  The session configuration for either a realtime or transcription session.

  - `RealtimeSessionCreateResponse = object { client_secret, type, audio, 10 more }`

    A new Realtime session configuration, with an ephemeral key. Default TTL
    for keys is one minute.

    - `client_secret: RealtimeSessionClientSecret`

      Ephemeral key returned by the API.

      - `expires_at: number`

        Timestamp for when the token expires. Currently, all tokens expire
        after one minute.

      - `value: string`

        Ephemeral key usable in client environments to authenticate connections to the Realtime API. Use this in client-side environments rather than a standard API token, which should only be used server-side.

    - `type: "realtime"`

      The type of session to create. Always `realtime` for the Realtime API.

      - `"realtime"`

    - `audio: optional object { input, output }`

      Configuration for input and output audio.

      - `input: optional object { format, noise_reduction, transcription, turn_detection }`

        - `format: optional RealtimeAudioFormats`

          The format of the input audio.

          - `PCMAudioFormat = object { rate, type }`

            The PCM audio format. Only a 24kHz sample rate is supported.

            - `rate: optional 24000`

              The sample rate of the audio. Always `24000`.

              - `24000`

            - `type: optional "audio/pcm"`

              The audio format. Always `audio/pcm`.

              - `"audio/pcm"`

          - `PCMUAudioFormat = object { type }`

            The G.711 μ-law format.

            - `type: optional "audio/pcmu"`

              The audio format. Always `audio/pcmu`.

              - `"audio/pcmu"`

          - `PCMAAudioFormat = object { type }`

            The G.711 A-law format.

            - `type: optional "audio/pcma"`

              The audio format. Always `audio/pcma`.

              - `"audio/pcma"`

        - `noise_reduction: optional object { type }`

          Configuration for input audio noise reduction. This can be set to `null` to turn off.
          Noise reduction filters audio added to the input audio buffer before it is sent to VAD and the model.
          Filtering the audio can improve VAD and turn detection accuracy (reducing false positives) and model performance by improving perception of the input audio.

          - `type: optional NoiseReductionType`

            Type of noise reduction. `near_field` is for close-talking microphones such as headphones, `far_field` is for far-field microphones such as laptop or conference room microphones.

            - `"near_field"`

            - `"far_field"`

        - `transcription: optional AudioTranscription`

          Configuration for input audio transcription, defaults to off and can be set to `null` to turn off once on. Input audio transcription is not native to the model, since the model consumes audio directly. Transcription runs asynchronously through [the /audio/transcriptions endpoint](/docs/api-reference/audio/createTranscription) and should be treated as guidance of input audio content rather than precisely what the model heard. The client can optionally set the language and prompt for transcription, these offer additional guidance to the transcription service.

          - `language: optional string`

            The language of the input audio. Supplying the input language in
            [ISO-639-1](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) (e.g. `en`) format
            will improve accuracy and latency.

          - `model: optional string or "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

            The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

            - `UnionMember0 = string`

            - `UnionMember1 = "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

              The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

              - `"whisper-1"`

              - `"gpt-4o-mini-transcribe"`

              - `"gpt-4o-mini-transcribe-2025-12-15"`

              - `"gpt-4o-transcribe"`

              - `"gpt-4o-transcribe-diarize"`

          - `prompt: optional string`

            An optional text to guide the model's style or continue a previous audio
            segment.
            For `whisper-1`, the [prompt is a list of keywords](/docs/guides/speech-to-text#prompting).
            For `gpt-4o-transcribe` models (excluding `gpt-4o-transcribe-diarize`), the prompt is a free text string, for example "expect words related to technology".

        - `turn_detection: optional object { type, create_response, idle_timeout_ms, 4 more }  or object { type, create_response, eagerness, interrupt_response }`

          Configuration for turn detection, ether Server VAD or Semantic VAD. This can be set to `null` to turn off, in which case the client must manually trigger model response.

          Server VAD means that the model will detect the start and end of speech based on audio volume and respond at the end of user speech.

          Semantic VAD is more advanced and uses a turn detection model (in conjunction with VAD) to semantically estimate whether the user has finished speaking, then dynamically sets a timeout based on this probability. For example, if user audio trails off with "uhhm", the model will score a low probability of turn end and wait longer for the user to continue speaking. This can be useful for more natural conversations, but may have a higher latency.

          - `ServerVad = object { type, create_response, idle_timeout_ms, 4 more }`

            Server-side voice activity detection (VAD) which flips on when user speech is detected and off after a period of silence.

            - `type: "server_vad"`

              Type of turn detection, `server_vad` to turn on simple Server VAD.

              - `"server_vad"`

            - `create_response: optional boolean`

              Whether or not to automatically generate a response when a VAD stop event occurs. If `interrupt_response` is set to `false` this may fail to create a response if the model is already responding.

              If both `create_response` and `interrupt_response` are set to `false`, the model will never respond automatically but VAD events will still be emitted.

            - `idle_timeout_ms: optional number`

              Optional timeout after which a model response will be triggered automatically. This is
              useful for situations in which a long pause from the user is unexpected, such as a phone
              call. The model will effectively prompt the user to continue the conversation based
              on the current context.

              The timeout value will be applied after the last model response's audio has finished playing,
              i.e. it's set to the `response.done` time plus audio playback duration.

              An `input_audio_buffer.timeout_triggered` event (plus events
              associated with the Response) will be emitted when the timeout is reached.
              Idle timeout is currently only supported for `server_vad` mode.

            - `interrupt_response: optional boolean`

              Whether or not to automatically interrupt (cancel) any ongoing response with output to the default
              conversation (i.e. `conversation` of `auto`) when a VAD start event occurs. If `true` then the response will be cancelled, otherwise it will continue until complete.

              If both `create_response` and `interrupt_response` are set to `false`, the model will never respond automatically but VAD events will still be emitted.

            - `prefix_padding_ms: optional number`

              Used only for `server_vad` mode. Amount of audio to include before the VAD detected speech (in
              milliseconds). Defaults to 300ms.

            - `silence_duration_ms: optional number`

              Used only for `server_vad` mode. Duration of silence to detect speech stop (in milliseconds). Defaults
              to 500ms. With shorter values the model will respond more quickly,
              but may jump in on short pauses from the user.

            - `threshold: optional number`

              Used only for `server_vad` mode. Activation threshold for VAD (0.0 to 1.0), this defaults to 0.5. A
              higher threshold will require louder audio to activate the model, and
              thus might perform better in noisy environments.

          - `SemanticVad = object { type, create_response, eagerness, interrupt_response }`

            Server-side semantic turn detection which uses a model to determine when the user has finished speaking.

            - `type: "semantic_vad"`

              Type of turn detection, `semantic_vad` to turn on Semantic VAD.

              - `"semantic_vad"`

            - `create_response: optional boolean`

              Whether or not to automatically generate a response when a VAD stop event occurs.

            - `eagerness: optional "low" or "medium" or "high" or "auto"`

              Used only for `semantic_vad` mode. The eagerness of the model to respond. `low` will wait longer for the user to continue speaking, `high` will respond more quickly. `auto` is the default and is equivalent to `medium`. `low`, `medium`, and `high` have max timeouts of 8s, 4s, and 2s respectively.

              - `"low"`

              - `"medium"`

              - `"high"`

              - `"auto"`

            - `interrupt_response: optional boolean`

              Whether or not to automatically interrupt any ongoing response with output to the default
              conversation (i.e. `conversation` of `auto`) when a VAD start event occurs.

      - `output: optional object { format, speed, voice }`

        - `format: optional RealtimeAudioFormats`

          The format of the output audio.

          - `PCMAudioFormat = object { rate, type }`

            The PCM audio format. Only a 24kHz sample rate is supported.

            - `rate: optional 24000`

              The sample rate of the audio. Always `24000`.

              - `24000`

            - `type: optional "audio/pcm"`

              The audio format. Always `audio/pcm`.

              - `"audio/pcm"`

          - `PCMUAudioFormat = object { type }`

            The G.711 μ-law format.

            - `type: optional "audio/pcmu"`

              The audio format. Always `audio/pcmu`.

              - `"audio/pcmu"`

          - `PCMAAudioFormat = object { type }`

            The G.711 A-law format.

            - `type: optional "audio/pcma"`

              The audio format. Always `audio/pcma`.

              - `"audio/pcma"`

        - `speed: optional number`

          The speed of the model's spoken response as a multiple of the original speed.
          1.0 is the default speed. 0.25 is the minimum speed. 1.5 is the maximum speed. This value can only be changed in between model turns, not while a response is in progress.

          This parameter is a post-processing adjustment to the audio after it is generated, it's
          also possible to prompt the model to speak faster or slower.

        - `voice: optional string or "alloy" or "ash" or "ballad" or 7 more`

          The voice the model uses to respond. Voice cannot be changed during the
          session once the model has responded with audio at least once. Current
          voice options are `alloy`, `ash`, `ballad`, `coral`, `echo`, `sage`,
          `shimmer`, `verse`, `marin`, and `cedar`. We recommend `marin` and `cedar` for
          best quality.

          - `UnionMember0 = string`

          - `UnionMember1 = "alloy" or "ash" or "ballad" or 7 more`

            The voice the model uses to respond. Voice cannot be changed during the
            session once the model has responded with audio at least once. Current
            voice options are `alloy`, `ash`, `ballad`, `coral`, `echo`, `sage`,
            `shimmer`, `verse`, `marin`, and `cedar`. We recommend `marin` and `cedar` for
            best quality.

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

    - `include: optional array of "item.input_audio_transcription.logprobs"`

      Additional fields to include in server outputs.

      `item.input_audio_transcription.logprobs`: Include logprobs for input audio transcription.

      - `"item.input_audio_transcription.logprobs"`

    - `instructions: optional string`

      The default system instructions (i.e. system message) prepended to model calls. This field allows the client to guide the model on desired responses. The model can be instructed on response content and format, (e.g. "be extremely succinct", "act friendly", "here are examples of good responses") and on audio behavior (e.g. "talk quickly", "inject emotion into your voice", "laugh frequently"). The instructions are not guaranteed to be followed by the model, but they provide guidance to the model on the desired behavior.

      Note that the server sets default instructions which will be used if this field is not set and are visible in the `session.created` event at the start of the session.

    - `max_output_tokens: optional number or "inf"`

      Maximum number of output tokens for a single assistant response,
      inclusive of tool calls. Provide an integer between 1 and 4096 to
      limit output tokens, or `inf` for the maximum available tokens for a
      given model. Defaults to `inf`.

      - `UnionMember0 = number`

      - `UnionMember1 = "inf"`

        - `"inf"`

    - `model: optional string or "gpt-realtime" or "gpt-realtime-1.5" or "gpt-realtime-2025-08-28" or 13 more`

      The Realtime model used for this session.

      - `UnionMember0 = string`

      - `UnionMember1 = "gpt-realtime" or "gpt-realtime-1.5" or "gpt-realtime-2025-08-28" or 13 more`

        The Realtime model used for this session.

        - `"gpt-realtime"`

        - `"gpt-realtime-1.5"`

        - `"gpt-realtime-2025-08-28"`

        - `"gpt-4o-realtime-preview"`

        - `"gpt-4o-realtime-preview-2024-10-01"`

        - `"gpt-4o-realtime-preview-2024-12-17"`

        - `"gpt-4o-realtime-preview-2025-06-03"`

        - `"gpt-4o-mini-realtime-preview"`

        - `"gpt-4o-mini-realtime-preview-2024-12-17"`

        - `"gpt-realtime-mini"`

        - `"gpt-realtime-mini-2025-10-06"`

        - `"gpt-realtime-mini-2025-12-15"`

        - `"gpt-audio-1.5"`

        - `"gpt-audio-mini"`

        - `"gpt-audio-mini-2025-10-06"`

        - `"gpt-audio-mini-2025-12-15"`

    - `output_modalities: optional array of "text" or "audio"`

      The set of modalities the model can respond with. It defaults to `["audio"]`, indicating
      that the model will respond with audio plus a transcript. `["text"]` can be used to make
      the model respond with text only. It is not possible to request both `text` and `audio` at the same time.

      - `"text"`

      - `"audio"`

    - `prompt: optional ResponsePrompt`

      Reference to a prompt template and its variables.
      [Learn more](/docs/guides/text?api-mode=responses#reusable-prompts).

      - `id: string`

        The unique identifier of the prompt template to use.

      - `variables: optional map[string or ResponseInputText or ResponseInputImage or ResponseInputFile]`

        Optional map of values to substitute in for variables in your
        prompt. The substitution values can either be strings, or other
        Response input types like images or files.

        - `UnionMember0 = string`

        - `ResponseInputText = object { text, type }`

          A text input to the model.

          - `text: string`

            The text input to the model.

          - `type: "input_text"`

            The type of the input item. Always `input_text`.

            - `"input_text"`

        - `ResponseInputImage = object { detail, type, file_id, image_url }`

          An image input to the model. Learn about [image inputs](/docs/guides/vision).

          - `detail: "low" or "high" or "auto"`

            The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

            - `"low"`

            - `"high"`

            - `"auto"`

          - `type: "input_image"`

            The type of the input item. Always `input_image`.

            - `"input_image"`

          - `file_id: optional string`

            The ID of the file to be sent to the model.

          - `image_url: optional string`

            The URL of the image to be sent to the model. A fully qualified URL or base64 encoded image in a data URL.

        - `ResponseInputFile = object { type, file_data, file_id, 2 more }`

          A file input to the model.

          - `type: "input_file"`

            The type of the input item. Always `input_file`.

            - `"input_file"`

          - `file_data: optional string`

            The content of the file to be sent to the model.

          - `file_id: optional string`

            The ID of the file to be sent to the model.

          - `file_url: optional string`

            The URL of the file to be sent to the model.

          - `filename: optional string`

            The name of the file to be sent to the model.

      - `version: optional string`

        Optional version of the prompt template.

    - `tool_choice: optional ToolChoiceOptions or ToolChoiceFunction or ToolChoiceMcp`

      How the model chooses tools. Provide one of the string modes or force a specific
      function/MCP tool.

      - `ToolChoiceOptions = "none" or "auto" or "required"`

        Controls which (if any) tool is called by the model.

        `none` means the model will not call any tool and instead generates a message.

        `auto` means the model can pick between generating a message or calling one or
        more tools.

        `required` means the model must call one or more tools.

        - `"none"`

        - `"auto"`

        - `"required"`

      - `ToolChoiceFunction = object { name, type }`

        Use this option to force the model to call a specific function.

        - `name: string`

          The name of the function to call.

        - `type: "function"`

          For function calling, the type is always `function`.

          - `"function"`

      - `ToolChoiceMcp = object { server_label, type, name }`

        Use this option to force the model to call a specific tool on a remote MCP server.

        - `server_label: string`

          The label of the MCP server to use.

        - `type: "mcp"`

          For MCP tools, the type is always `mcp`.

          - `"mcp"`

        - `name: optional string`

          The name of the tool to call on the server.

    - `tools: optional array of RealtimeFunctionTool or object { server_label, type, allowed_tools, 6 more }`

      Tools available to the model.

      - `RealtimeFunctionTool = object { description, name, parameters, type }`

        - `description: optional string`

          The description of the function, including guidance on when and how
          to call it, and guidance about what to tell the user when calling
          (if anything).

        - `name: optional string`

          The name of the function.

        - `parameters: optional unknown`

          Parameters of the function in JSON Schema.

        - `type: optional "function"`

          The type of the tool, i.e. `function`.

          - `"function"`

      - `McpTool = object { server_label, type, allowed_tools, 6 more }`

        Give the model access to additional tools via remote Model Context Protocol
        (MCP) servers. [Learn more about MCP](/docs/guides/tools-remote-mcp).

        - `server_label: string`

          A label for this MCP server, used to identify it in tool calls.

        - `type: "mcp"`

          The type of the MCP tool. Always `mcp`.

          - `"mcp"`

        - `allowed_tools: optional array of string or object { read_only, tool_names }`

          List of allowed tool names or a filter object.

          - `McpAllowedTools = array of string`

            A string array of allowed tool names

          - `McpToolFilter = object { read_only, tool_names }`

            A filter object to specify which tools are allowed.

            - `read_only: optional boolean`

              Indicates whether or not a tool modifies data or is read-only. If an
              MCP server is [annotated with `readOnlyHint`](https://modelcontextprotocol.io/specification/2025-06-18/schema#toolannotations-readonlyhint),
              it will match this filter.

            - `tool_names: optional array of string`

              List of allowed tool names.

        - `authorization: optional string`

          An OAuth access token that can be used with a remote MCP server, either
          with a custom MCP server URL or a service connector. Your application
          must handle the OAuth authorization flow and provide the token here.

        - `connector_id: optional "connector_dropbox" or "connector_gmail" or "connector_googlecalendar" or 5 more`

          Identifier for service connectors, like those available in ChatGPT. One of
          `server_url` or `connector_id` must be provided. Learn more about service
          connectors [here](/docs/guides/tools-remote-mcp#connectors).

          Currently supported `connector_id` values are:

          - Dropbox: `connector_dropbox`
          - Gmail: `connector_gmail`
          - Google Calendar: `connector_googlecalendar`
          - Google Drive: `connector_googledrive`
          - Microsoft Teams: `connector_microsoftteams`
          - Outlook Calendar: `connector_outlookcalendar`
          - Outlook Email: `connector_outlookemail`
          - SharePoint: `connector_sharepoint`

          - `"connector_dropbox"`

          - `"connector_gmail"`

          - `"connector_googlecalendar"`

          - `"connector_googledrive"`

          - `"connector_microsoftteams"`

          - `"connector_outlookcalendar"`

          - `"connector_outlookemail"`

          - `"connector_sharepoint"`

        - `headers: optional map[string]`

          Optional HTTP headers to send to the MCP server. Use for authentication
          or other purposes.

        - `require_approval: optional object { always, never }  or "always" or "never"`

          Specify which of the MCP server's tools require approval.

          - `McpToolApprovalFilter = object { always, never }`

            Specify which of the MCP server's tools require approval. Can be
            `always`, `never`, or a filter object associated with tools
            that require approval.

            - `always: optional object { read_only, tool_names }`

              A filter object to specify which tools are allowed.

              - `read_only: optional boolean`

                Indicates whether or not a tool modifies data or is read-only. If an
                MCP server is [annotated with `readOnlyHint`](https://modelcontextprotocol.io/specification/2025-06-18/schema#toolannotations-readonlyhint),
                it will match this filter.

              - `tool_names: optional array of string`

                List of allowed tool names.

            - `never: optional object { read_only, tool_names }`

              A filter object to specify which tools are allowed.

              - `read_only: optional boolean`

                Indicates whether or not a tool modifies data or is read-only. If an
                MCP server is [annotated with `readOnlyHint`](https://modelcontextprotocol.io/specification/2025-06-18/schema#toolannotations-readonlyhint),
                it will match this filter.

              - `tool_names: optional array of string`

                List of allowed tool names.

          - `McpToolApprovalSetting = "always" or "never"`

            Specify a single approval policy for all tools. One of `always` or
            `never`. When set to `always`, all tools will require approval. When
            set to `never`, all tools will not require approval.

            - `"always"`

            - `"never"`

        - `server_description: optional string`

          Optional description of the MCP server, used to provide more context.

        - `server_url: optional string`

          The URL for the MCP server. One of `server_url` or `connector_id` must be
          provided.

    - `tracing: optional "auto" or object { group_id, metadata, workflow_name }`

      Realtime API can write session traces to the [Traces Dashboard](/logs?api=traces). Set to null to disable tracing. Once
      tracing is enabled for a session, the configuration cannot be modified.

      `auto` will create a trace for the session with default values for the
      workflow name, group id, and metadata.

      - `Auto = "auto"`

        Enables tracing and sets default values for tracing configuration options. Always `auto`.

        - `"auto"`

      - `TracingConfiguration = object { group_id, metadata, workflow_name }`

        Granular configuration for tracing.

        - `group_id: optional string`

          The group id to attach to this trace to enable filtering and
          grouping in the Traces Dashboard.

        - `metadata: optional unknown`

          The arbitrary metadata to attach to this trace to enable
          filtering in the Traces Dashboard.

        - `workflow_name: optional string`

          The name of the workflow to attach to this trace. This is used to
          name the trace in the Traces Dashboard.

    - `truncation: optional RealtimeTruncation`

      When the number of tokens in a conversation exceeds the model's input token limit, the conversation be truncated, meaning messages (starting from the oldest) will not be included in the model's context. A 32k context model with 4,096 max output tokens can only include 28,224 tokens in the context before truncation occurs.

      Clients can configure truncation behavior to truncate with a lower max token limit, which is an effective way to control token usage and cost.

      Truncation will reduce the number of cached tokens on the next turn (busting the cache), since messages are dropped from the beginning of the context. However, clients can also configure truncation to retain messages up to a fraction of the maximum context size, which will reduce the need for future truncations and thus improve the cache rate.

      Truncation can be disabled entirely, which means the server will never truncate but would instead return an error if the conversation exceeds the model's input token limit.

      - `UnionMember0 = "auto" or "disabled"`

        The truncation strategy to use for the session. `auto` is the default truncation strategy. `disabled` will disable truncation and emit errors when the conversation exceeds the input token limit.

        - `"auto"`

        - `"disabled"`

      - `RetentionRatioTruncation = object { retention_ratio, type, token_limits }`

        Retain a fraction of the conversation tokens when the conversation exceeds the input token limit. This allows you to amortize truncations across multiple turns, which can help improve cached token usage.

        - `retention_ratio: number`

          Fraction of post-instruction conversation tokens to retain (`0.0` - `1.0`) when the conversation exceeds the input token limit. Setting this to `0.8` means that messages will be dropped until 80% of the maximum allowed tokens are used. This helps reduce the frequency of truncations and improve cache rates.

        - `type: "retention_ratio"`

          Use retention ratio truncation.

          - `"retention_ratio"`

        - `token_limits: optional object { post_instructions }`

          Optional custom token limits for this truncation strategy. If not provided, the model's default token limits will be used.

          - `post_instructions: optional number`

            Maximum tokens allowed in the conversation after instructions (which including tool definitions). For example, setting this to 5,000 would mean that truncation would occur when the conversation exceeds 5,000 tokens after instructions. This cannot be higher than the model's context window size minus the maximum output tokens.

  - `RealtimeTranscriptionSessionCreateResponse = object { id, object, type, 3 more }`

    A Realtime transcription session configuration object.

    - `id: string`

      Unique identifier for the session that looks like `sess_1234567890abcdef`.

    - `object: string`

      The object type. Always `realtime.transcription_session`.

    - `type: "transcription"`

      The type of session. Always `transcription` for transcription sessions.

      - `"transcription"`

    - `audio: optional object { input }`

      Configuration for input audio for the session.

      - `input: optional object { format, noise_reduction, transcription, turn_detection }`

        - `format: optional RealtimeAudioFormats`

          The PCM audio format. Only a 24kHz sample rate is supported.

          - `PCMAudioFormat = object { rate, type }`

            The PCM audio format. Only a 24kHz sample rate is supported.

            - `rate: optional 24000`

              The sample rate of the audio. Always `24000`.

              - `24000`

            - `type: optional "audio/pcm"`

              The audio format. Always `audio/pcm`.

              - `"audio/pcm"`

          - `PCMUAudioFormat = object { type }`

            The G.711 μ-law format.

            - `type: optional "audio/pcmu"`

              The audio format. Always `audio/pcmu`.

              - `"audio/pcmu"`

          - `PCMAAudioFormat = object { type }`

            The G.711 A-law format.

            - `type: optional "audio/pcma"`

              The audio format. Always `audio/pcma`.

              - `"audio/pcma"`

        - `noise_reduction: optional object { type }`

          Configuration for input audio noise reduction.

          - `type: optional NoiseReductionType`

            Type of noise reduction. `near_field` is for close-talking microphones such as headphones, `far_field` is for far-field microphones such as laptop or conference room microphones.

            - `"near_field"`

            - `"far_field"`

        - `transcription: optional AudioTranscription`

          Configuration of the transcription model.

          - `language: optional string`

            The language of the input audio. Supplying the input language in
            [ISO-639-1](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) (e.g. `en`) format
            will improve accuracy and latency.

          - `model: optional string or "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

            The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

            - `UnionMember0 = string`

            - `UnionMember1 = "whisper-1" or "gpt-4o-mini-transcribe" or "gpt-4o-mini-transcribe-2025-12-15" or 2 more`

              The model to use for transcription. Current options are `whisper-1`, `gpt-4o-mini-transcribe`, `gpt-4o-mini-transcribe-2025-12-15`, `gpt-4o-transcribe`, and `gpt-4o-transcribe-diarize`. Use `gpt-4o-transcribe-diarize` when you need diarization with speaker labels.

              - `"whisper-1"`

              - `"gpt-4o-mini-transcribe"`

              - `"gpt-4o-mini-transcribe-2025-12-15"`

              - `"gpt-4o-transcribe"`

              - `"gpt-4o-transcribe-diarize"`

          - `prompt: optional string`

            An optional text to guide the model's style or continue a previous audio
            segment.
            For `whisper-1`, the [prompt is a list of keywords](/docs/guides/speech-to-text#prompting).
            For `gpt-4o-transcribe` models (excluding `gpt-4o-transcribe-diarize`), the prompt is a free text string, for example "expect words related to technology".

        - `turn_detection: optional RealtimeTranscriptionSessionTurnDetection`

          Configuration for turn detection. Can be set to `null` to turn off. Server
          VAD means that the model will detect the start and end of speech based on
          audio volume and respond at the end of user speech.

          - `prefix_padding_ms: optional number`

            Amount of audio to include before the VAD detected speech (in
            milliseconds). Defaults to 300ms.

          - `silence_duration_ms: optional number`

            Duration of silence to detect speech stop (in milliseconds). Defaults
            to 500ms. With shorter values the model will respond more quickly,
            but may jump in on short pauses from the user.

          - `threshold: optional number`

            Activation threshold for VAD (0.0 to 1.0), this defaults to 0.5. A
            higher threshold will require louder audio to activate the model, and
            thus might perform better in noisy environments.

          - `type: optional string`

            Type of turn detection, only `server_vad` is currently supported.

    - `expires_at: optional number`

      Expiration timestamp for the session, in seconds since epoch.

    - `include: optional array of "item.input_audio_transcription.logprobs"`

      Additional fields to include in server outputs.

      - `item.input_audio_transcription.logprobs`: Include logprobs for input audio transcription.

      - `"item.input_audio_transcription.logprobs"`

- `value: string`

  The generated client secret value.

### Example

```http
curl https://api.openai.com/v1/realtime/client_secrets \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{}'
```

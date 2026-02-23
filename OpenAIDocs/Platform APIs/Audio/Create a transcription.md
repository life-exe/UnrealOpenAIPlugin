## Create

**post** `/audio/transcriptions`

Transcribes audio into the input language.

Returns a transcription object in `json`, `diarized_json`, or `verbose_json`
format, or a stream of transcript events.

### Returns

- `Transcription = object { text, logprobs, usage }`

  Represents a transcription response returned by model, based on the provided input.

  - `text: string`

    The transcribed text.

  - `logprobs: optional array of object { token, bytes, logprob }`

    The log probabilities of the tokens in the transcription. Only returned with the models `gpt-4o-transcribe` and `gpt-4o-mini-transcribe` if `logprobs` is added to the `include` array.

    - `token: optional string`

      The token in the transcription.

    - `bytes: optional array of number`

      The bytes of the token.

    - `logprob: optional number`

      The log probability of the token.

  - `usage: optional object { input_tokens, output_tokens, total_tokens, 2 more }  or object { seconds, type }`

    Token usage statistics for the request.

    - `TokenUsage = object { input_tokens, output_tokens, total_tokens, 2 more }`

      Usage statistics for models billed by token usage.

      - `input_tokens: number`

        Number of input tokens billed for this request.

      - `output_tokens: number`

        Number of output tokens generated.

      - `total_tokens: number`

        Total number of tokens used (input + output).

      - `type: "tokens"`

        The type of the usage object. Always `tokens` for this variant.

        - `"tokens"`

      - `input_token_details: optional object { audio_tokens, text_tokens }`

        Details about the input tokens billed for this request.

        - `audio_tokens: optional number`

          Number of audio tokens billed for this request.

        - `text_tokens: optional number`

          Number of text tokens billed for this request.

    - `DurationUsage = object { seconds, type }`

      Usage statistics for models billed by audio input duration.

      - `seconds: number`

        Duration of the input audio in seconds.

      - `type: "duration"`

        The type of the usage object. Always `duration` for this variant.

        - `"duration"`

- `TranscriptionDiarized = object { duration, segments, task, 2 more }`

  Represents a diarized transcription response returned by the model, including the combined transcript and speaker-segment annotations.

  - `duration: number`

    Duration of the input audio in seconds.

  - `segments: array of TranscriptionDiarizedSegment`

    Segments of the transcript annotated with timestamps and speaker labels.

    - `id: string`

      Unique identifier for the segment.

    - `end: number`

      End timestamp of the segment in seconds.

    - `speaker: string`

      Speaker label for this segment. When known speakers are provided, the label matches `known_speaker_names[]`. Otherwise speakers are labeled sequentially using capital letters (`A`, `B`, ...).

    - `start: number`

      Start timestamp of the segment in seconds.

    - `text: string`

      Transcript text for this segment.

    - `type: "transcript.text.segment"`

      The type of the segment. Always `transcript.text.segment`.

      - `"transcript.text.segment"`

  - `task: "transcribe"`

    The type of task that was run. Always `transcribe`.

    - `"transcribe"`

  - `text: string`

    The concatenated transcript text for the entire audio input.

  - `usage: optional object { input_tokens, output_tokens, total_tokens, 2 more }  or object { seconds, type }`

    Token or duration usage statistics for the request.

    - `Tokens = object { input_tokens, output_tokens, total_tokens, 2 more }`

      Usage statistics for models billed by token usage.

      - `input_tokens: number`

        Number of input tokens billed for this request.

      - `output_tokens: number`

        Number of output tokens generated.

      - `total_tokens: number`

        Total number of tokens used (input + output).

      - `type: "tokens"`

        The type of the usage object. Always `tokens` for this variant.

        - `"tokens"`

      - `input_token_details: optional object { audio_tokens, text_tokens }`

        Details about the input tokens billed for this request.

        - `audio_tokens: optional number`

          Number of audio tokens billed for this request.

        - `text_tokens: optional number`

          Number of text tokens billed for this request.

    - `Duration = object { seconds, type }`

      Usage statistics for models billed by audio input duration.

      - `seconds: number`

        Duration of the input audio in seconds.

      - `type: "duration"`

        The type of the usage object. Always `duration` for this variant.

        - `"duration"`

- `TranscriptionVerbose = object { duration, language, text, 3 more }`

  Represents a verbose json transcription response returned by model, based on the provided input.

  - `duration: number`

    The duration of the input audio.

  - `language: string`

    The language of the input audio.

  - `text: string`

    The transcribed text.

  - `segments: optional array of TranscriptionSegment`

    Segments of the transcribed text and their corresponding details.

    - `id: number`

      Unique identifier of the segment.

    - `avg_logprob: number`

      Average logprob of the segment. If the value is lower than -1, consider the logprobs failed.

    - `compression_ratio: number`

      Compression ratio of the segment. If the value is greater than 2.4, consider the compression failed.

    - `end: number`

      End time of the segment in seconds.

    - `no_speech_prob: number`

      Probability of no speech in the segment. If the value is higher than 1.0 and the `avg_logprob` is below -1, consider this segment silent.

    - `seek: number`

      Seek offset of the segment.

    - `start: number`

      Start time of the segment in seconds.

    - `temperature: number`

      Temperature parameter used for generating the segment.

    - `text: string`

      Text content of the segment.

    - `tokens: array of number`

      Array of token IDs for the text content.

  - `usage: optional object { seconds, type }`

    Usage statistics for models billed by audio input duration.

    - `seconds: number`

      Duration of the input audio in seconds.

    - `type: "duration"`

      The type of the usage object. Always `duration` for this variant.

      - `"duration"`

  - `words: optional array of TranscriptionWord`

    Extracted words and their corresponding timestamps.

    - `end: number`

      End time of the word in seconds.

    - `start: number`

      Start time of the word in seconds.

    - `word: string`

      The text content of the word.

### Example

```http
curl https://api.openai.com/v1/audio/transcriptions \
    -H 'Content-Type: multipart/form-data' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -F 'file=@/path/to/file' \
    -F model=gpt-4o-transcribe
```

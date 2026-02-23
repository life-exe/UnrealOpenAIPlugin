## Create

**post** `/audio/translations`

Translates audio into English.

### Returns

- `Translation = object { text }`

  - `text: string`

- `TranslationVerbose = object { duration, language, text, segments }`

  - `duration: number`

    The duration of the input audio.

  - `language: string`

    The language of the output translation (always `english`).

  - `text: string`

    The translated text.

  - `segments: optional array of TranscriptionSegment`

    Segments of the translated text and their corresponding details.

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

### Example

```http
curl https://api.openai.com/v1/audio/translations \
    -H 'Content-Type: multipart/form-data' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -F 'file=@/path/to/file' \
    -F model=whisper-1
```

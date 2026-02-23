## Create

**post** `/audio/speech`

Generates audio from the input text.

Returns the audio file content, or a stream of audio events.

### Body Parameters

- `input: string`

  The text to generate audio for. The maximum length is 4096 characters.

- `model: string or SpeechModel`

  One of the available [TTS models](/docs/models#tts): `tts-1`, `tts-1-hd`, `gpt-4o-mini-tts`, or `gpt-4o-mini-tts-2025-12-15`.

  - `UnionMember0 = string`

  - `SpeechModel = "tts-1" or "tts-1-hd" or "gpt-4o-mini-tts" or "gpt-4o-mini-tts-2025-12-15"`

    - `"tts-1"`

    - `"tts-1-hd"`

    - `"gpt-4o-mini-tts"`

    - `"gpt-4o-mini-tts-2025-12-15"`

- `voice: string or "alloy" or "ash" or "ballad" or 7 more or object { id }`

  The voice to use when generating the audio. Supported built-in voices are `alloy`, `ash`, `ballad`, `coral`, `echo`, `fable`, `onyx`, `nova`, `sage`, `shimmer`, `verse`, `marin`, and `cedar`. You may also provide a custom voice object with an `id`, for example `{ "id": "voice_1234" }`. Previews of the voices are available in the [Text to speech guide](/docs/guides/text-to-speech#voice-options).

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

- `instructions: optional string`

  Control the voice of your generated audio with additional instructions. Does not work with `tts-1` or `tts-1-hd`.

- `response_format: optional "mp3" or "opus" or "aac" or 3 more`

  The format to audio in. Supported formats are `mp3`, `opus`, `aac`, `flac`, `wav`, and `pcm`.

  - `"mp3"`

  - `"opus"`

  - `"aac"`

  - `"flac"`

  - `"wav"`

  - `"pcm"`

- `speed: optional number`

  The speed of the generated audio. Select a value from `0.25` to `4.0`. `1.0` is the default.

- `stream_format: optional "sse" or "audio"`

  The format to stream the audio in. Supported formats are `sse` and `audio`. `sse` is not supported for `tts-1` or `tts-1-hd`.

  - `"sse"`

  - `"audio"`

### Example

```http
curl https://api.openai.com/v1/audio/speech \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "input": "input",
          "model": "string",
          "voice": "ash"
        }'
```

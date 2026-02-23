## Create

**post** `/audio/voices`

Creates a custom voice.

### Returns

- `id: string`

  The voice identifier, which can be referenced in API endpoints.

- `created_at: number`

  The Unix timestamp (in seconds) for when the voice was created.

- `name: string`

  The name of the voice.

- `object: "audio.voice"`

  The object type, which is always `audio.voice`.

  - `"audio.voice"`

### Example

```http
curl https://api.openai.com/v1/audio/voices \
    -H 'Content-Type: multipart/form-data' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -F 'audio_sample=@/path/to/audio_sample' \
    -F consent=consent \
    -F name=name
```

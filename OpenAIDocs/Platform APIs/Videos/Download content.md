## Download Content

**get** `/videos/{video_id}/content`

Download the generated video bytes or a derived preview asset.

Streams the rendered video content for the specified video job.

### Path Parameters

- `video_id: string`

### Query Parameters

- `variant: optional "video" or "thumbnail" or "spritesheet"`

  Which downloadable asset to return. Defaults to the MP4 video.

  - `"video"`

  - `"thumbnail"`

  - `"spritesheet"`

### Example

```http
curl https://api.openai.com/v1/videos/$VIDEO_ID/content \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

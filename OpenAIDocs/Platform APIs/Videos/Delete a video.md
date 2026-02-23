## Delete

**delete** `/videos/{video_id}`

Permanently delete a completed or failed video and its stored assets.

### Path Parameters

- `video_id: string`

### Returns

- `id: string`

  Identifier of the deleted video.

- `deleted: boolean`

  Indicates that the video resource was deleted.

- `object: "video.deleted"`

  The object type that signals the deletion response.

  - `"video.deleted"`

### Example

```http
curl https://api.openai.com/v1/videos/$VIDEO_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

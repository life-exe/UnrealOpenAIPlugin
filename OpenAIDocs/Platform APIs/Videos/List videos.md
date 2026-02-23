## List

**get** `/videos`

List recently generated videos for the current project.

### Query Parameters

- `after: optional string`

  Identifier for the last item from the previous pagination request

- `limit: optional number`

  Number of items to retrieve

- `order: optional "asc" or "desc"`

  Sort order of results by timestamp. Use `asc` for ascending order or `desc` for descending order.

  - `"asc"`

  - `"desc"`

### Returns

- `data: array of Video`

  A list of items

  - `id: string`

    Unique identifier for the video job.

  - `completed_at: number`

    Unix timestamp (seconds) for when the job completed, if finished.

  - `created_at: number`

    Unix timestamp (seconds) for when the job was created.

  - `error: VideoCreateError`

    Error payload that explains why generation failed, if applicable.

    - `code: string`

      A machine-readable error code that was returned.

    - `message: string`

      A human-readable description of the error that was returned.

  - `expires_at: number`

    Unix timestamp (seconds) for when the downloadable assets expire, if set.

  - `model: VideoModel`

    The video generation model that produced the job.

    - `UnionMember0 = string`

    - `UnionMember1 = "sora-2" or "sora-2-pro" or "sora-2-2025-10-06" or 2 more`

      - `"sora-2"`

      - `"sora-2-pro"`

      - `"sora-2-2025-10-06"`

      - `"sora-2-pro-2025-10-06"`

      - `"sora-2-2025-12-08"`

  - `object: "video"`

    The object type, which is always `video`.

    - `"video"`

  - `progress: number`

    Approximate completion percentage for the generation task.

  - `prompt: string`

    The prompt that was used to generate the video.

  - `remixed_from_video_id: string`

    Identifier of the source video if this video is a remix.

  - `seconds: VideoSeconds`

    Duration of the generated clip in seconds.

    - `"4"`

    - `"8"`

    - `"12"`

  - `size: VideoSize`

    The resolution of the generated video.

    - `"720x1280"`

    - `"1280x720"`

    - `"1024x1792"`

    - `"1792x1024"`

  - `status: "queued" or "in_progress" or "completed" or "failed"`

    Current lifecycle status of the video job.

    - `"queued"`

    - `"in_progress"`

    - `"completed"`

    - `"failed"`

- `first_id: string`

  The ID of the first item in the list.

- `has_more: boolean`

  Whether there are more items available.

- `last_id: string`

  The ID of the last item in the list.

- `object: "list"`

  The type of object returned, must be `list`.

  - `"list"`

### Example

```http
curl https://api.openai.com/v1/videos \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

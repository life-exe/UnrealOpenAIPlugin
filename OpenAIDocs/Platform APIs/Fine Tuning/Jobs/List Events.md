## List Events

**get** `/fine_tuning/jobs/{fine_tuning_job_id}/events`

Get status updates for a fine-tuning job.

### Path Parameters

- `fine_tuning_job_id: string`

### Query Parameters

- `after: optional string`

  Identifier for the last event from the previous pagination request.

- `limit: optional number`

  Number of events to retrieve.

### Returns

- `data: array of FineTuningJobEvent`

  - `id: string`

    The object identifier.

  - `created_at: number`

    The Unix timestamp (in seconds) for when the fine-tuning job was created.

  - `level: "info" or "warn" or "error"`

    The log level of the event.

    - `"info"`

    - `"warn"`

    - `"error"`

  - `message: string`

    The message of the event.

  - `object: "fine_tuning.job.event"`

    The object type, which is always "fine_tuning.job.event".

    - `"fine_tuning.job.event"`

  - `data: optional unknown`

    The data associated with the event.

  - `type: optional "message" or "metrics"`

    The type of event.

    - `"message"`

    - `"metrics"`

- `has_more: boolean`

- `object: "list"`

  - `"list"`

### Example

```http
curl https://api.openai.com/v1/fine_tuning/jobs/$FINE_TUNING_JOB_ID/events \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

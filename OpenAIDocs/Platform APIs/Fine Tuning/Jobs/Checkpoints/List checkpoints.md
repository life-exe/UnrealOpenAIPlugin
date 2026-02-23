## List

**get** `/fine_tuning/jobs/{fine_tuning_job_id}/checkpoints`

List checkpoints for a fine-tuning job.

### Path Parameters

- `fine_tuning_job_id: string`

### Query Parameters

- `after: optional string`

  Identifier for the last checkpoint ID from the previous pagination request.

- `limit: optional number`

  Number of checkpoints to retrieve.

### Returns

- `data: array of FineTuningJobCheckpoint`

  - `id: string`

    The checkpoint identifier, which can be referenced in the API endpoints.

  - `created_at: number`

    The Unix timestamp (in seconds) for when the checkpoint was created.

  - `fine_tuned_model_checkpoint: string`

    The name of the fine-tuned checkpoint model that is created.

  - `fine_tuning_job_id: string`

    The name of the fine-tuning job that this checkpoint was created from.

  - `metrics: object { full_valid_loss, full_valid_mean_token_accuracy, step, 4 more }`

    Metrics at the step number during the fine-tuning job.

    - `full_valid_loss: optional number`

    - `full_valid_mean_token_accuracy: optional number`

    - `step: optional number`

    - `train_loss: optional number`

    - `train_mean_token_accuracy: optional number`

    - `valid_loss: optional number`

    - `valid_mean_token_accuracy: optional number`

  - `object: "fine_tuning.job.checkpoint"`

    The object type, which is always "fine_tuning.job.checkpoint".

    - `"fine_tuning.job.checkpoint"`

  - `step_number: number`

    The step number that the checkpoint was created at.

- `has_more: boolean`

- `object: "list"`

  - `"list"`

- `first_id: optional string`

- `last_id: optional string`

### Example

```http
curl https://api.openai.com/v1/fine_tuning/jobs/$FINE_TUNING_JOB_ID/checkpoints \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

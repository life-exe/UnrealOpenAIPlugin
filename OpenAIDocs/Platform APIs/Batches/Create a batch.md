## Create

**post** `/batches`

Creates and executes a batch from an uploaded file of requests

### Body Parameters

- `completion_window: "24h"`

  The time frame within which the batch should be processed. Currently only `24h` is supported.

  - `"24h"`

- `endpoint: "/v1/responses" or "/v1/chat/completions" or "/v1/embeddings" or 4 more`

  The endpoint to be used for all requests in the batch. Currently `/v1/responses`, `/v1/chat/completions`, `/v1/embeddings`, `/v1/completions`, `/v1/moderations`, `/v1/images/generations`, and `/v1/images/edits` are supported. Note that `/v1/embeddings` batches are also restricted to a maximum of 50,000 embedding inputs across all requests in the batch.

  - `"/v1/responses"`

  - `"/v1/chat/completions"`

  - `"/v1/embeddings"`

  - `"/v1/completions"`

  - `"/v1/moderations"`

  - `"/v1/images/generations"`

  - `"/v1/images/edits"`

- `input_file_id: string`

  The ID of an uploaded file that contains requests for the new batch.

  See [upload file](/docs/api-reference/files/create) for how to upload a file.

  Your input file must be formatted as a [JSONL file](/docs/api-reference/batch/request-input), and must be uploaded with the purpose `batch`. The file can contain up to 50,000 requests, and can be up to 200 MB in size.

- `metadata: optional Metadata`

  Set of 16 key-value pairs that can be attached to an object. This can be
  useful for storing additional information about the object in a structured
  format, and querying for objects via API or the dashboard.

  Keys are strings with a maximum length of 64 characters. Values are strings
  with a maximum length of 512 characters.

- `output_expires_after: optional object { anchor, seconds }`

  The expiration policy for the output and/or error file that are generated for a batch.

  - `anchor: "created_at"`

    Anchor timestamp after which the expiration policy applies. Supported anchors: `created_at`. Note that the anchor is the file creation time, not the time the batch is created.

    - `"created_at"`

  - `seconds: number`

    The number of seconds after the anchor time that the file will expire. Must be between 3600 (1 hour) and 2592000 (30 days).

### Returns

- `Batch = object { id, completion_window, created_at, 19 more }`

  - `id: string`

  - `completion_window: string`

    The time frame within which the batch should be processed.

  - `created_at: number`

    The Unix timestamp (in seconds) for when the batch was created.

  - `endpoint: string`

    The OpenAI API endpoint used by the batch.

  - `input_file_id: string`

    The ID of the input file for the batch.

  - `object: "batch"`

    The object type, which is always `batch`.

    - `"batch"`

  - `status: "validating" or "failed" or "in_progress" or 5 more`

    The current status of the batch.

    - `"validating"`

    - `"failed"`

    - `"in_progress"`

    - `"finalizing"`

    - `"completed"`

    - `"expired"`

    - `"cancelling"`

    - `"cancelled"`

  - `cancelled_at: optional number`

    The Unix timestamp (in seconds) for when the batch was cancelled.

  - `cancelling_at: optional number`

    The Unix timestamp (in seconds) for when the batch started cancelling.

  - `completed_at: optional number`

    The Unix timestamp (in seconds) for when the batch was completed.

  - `error_file_id: optional string`

    The ID of the file containing the outputs of requests with errors.

  - `errors: optional object { data, object }`

    - `data: optional array of object { code, line, message, param }`

      - `code: optional string`

        An error code identifying the error type.

      - `line: optional number`

        The line number of the input file where the error occurred, if applicable.

      - `message: optional string`

        A human-readable message providing more details about the error.

      - `param: optional string`

        The name of the parameter that caused the error, if applicable.

    - `object: optional string`

      The object type, which is always `list`.

  - `expired_at: optional number`

    The Unix timestamp (in seconds) for when the batch expired.

  - `expires_at: optional number`

    The Unix timestamp (in seconds) for when the batch will expire.

  - `failed_at: optional number`

    The Unix timestamp (in seconds) for when the batch failed.

  - `finalizing_at: optional number`

    The Unix timestamp (in seconds) for when the batch started finalizing.

  - `in_progress_at: optional number`

    The Unix timestamp (in seconds) for when the batch started processing.

  - `metadata: optional Metadata`

    Set of 16 key-value pairs that can be attached to an object. This can be
    useful for storing additional information about the object in a structured
    format, and querying for objects via API or the dashboard.

    Keys are strings with a maximum length of 64 characters. Values are strings
    with a maximum length of 512 characters.

  - `model: optional string`

    Model ID used to process the batch, like `gpt-5-2025-08-07`. OpenAI
    offers a wide range of models with different capabilities, performance
    characteristics, and price points. Refer to the [model
    guide](/docs/models) to browse and compare available models.

  - `output_file_id: optional string`

    The ID of the file containing the outputs of successfully executed requests.

  - `request_counts: optional object { completed, failed, total }`

    The request counts for different statuses within the batch.

    - `completed: number`

      Number of requests that have been completed successfully.

    - `failed: number`

      Number of requests that have failed.

    - `total: number`

      Total number of requests in the batch.

  - `usage: optional BatchUsage`

    Represents token usage details including input tokens, output tokens, a
    breakdown of output tokens, and the total tokens used. Only populated on
    batches created after September 7, 2025.

    - `input_tokens: number`

      The number of input tokens.

    - `input_tokens_details: object { cached_tokens }`

      A detailed breakdown of the input tokens.

      - `cached_tokens: number`

        The number of tokens that were retrieved from the cache. [More on
        prompt caching](/docs/guides/prompt-caching).

    - `output_tokens: number`

      The number of output tokens.

    - `output_tokens_details: object { reasoning_tokens }`

      A detailed breakdown of the output tokens.

      - `reasoning_tokens: number`

        The number of reasoning tokens.

    - `total_tokens: number`

      The total number of tokens used.

### Example

```http
curl https://api.openai.com/v1/batches \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "completion_window": "24h",
          "endpoint": "/v1/responses",
          "input_file_id": "input_file_id"
        }'
```

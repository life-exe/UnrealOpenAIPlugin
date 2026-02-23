## Create

**post** `/vector_stores`

Create a vector store.

### Body Parameters

- `chunking_strategy: optional AutoFileChunkingStrategyParam or StaticFileChunkingStrategyObjectParam`

  The chunking strategy used to chunk the file(s). If not set, will use the `auto` strategy. Only applicable if `file_ids` is non-empty.

  - `AutoFileChunkingStrategyParam = object { type }`

    The default strategy. This strategy currently uses a `max_chunk_size_tokens` of `800` and `chunk_overlap_tokens` of `400`.

    - `type: "auto"`

      Always `auto`.

      - `"auto"`

  - `StaticFileChunkingStrategyObjectParam = object { static, type }`

    Customize your own chunking strategy by setting chunk size and chunk overlap.

    - `static: StaticFileChunkingStrategy`

      - `chunk_overlap_tokens: number`

        The number of tokens that overlap between chunks. The default value is `400`.

        Note that the overlap must not exceed half of `max_chunk_size_tokens`.

      - `max_chunk_size_tokens: number`

        The maximum number of tokens in each chunk. The default value is `800`. The minimum value is `100` and the maximum value is `4096`.

    - `type: "static"`

      Always `static`.

      - `"static"`

- `description: optional string`

  A description for the vector store. Can be used to describe the vector store's purpose.

- `expires_after: optional object { anchor, days }`

  The expiration policy for a vector store.

  - `anchor: "last_active_at"`

    Anchor timestamp after which the expiration policy applies. Supported anchors: `last_active_at`.

    - `"last_active_at"`

  - `days: number`

    The number of days after the anchor time that the vector store will expire.

- `file_ids: optional array of string`

  A list of [File](/docs/api-reference/files) IDs that the vector store should use. Useful for tools like `file_search` that can access files.

- `metadata: optional Metadata`

  Set of 16 key-value pairs that can be attached to an object. This can be
  useful for storing additional information about the object in a structured
  format, and querying for objects via API or the dashboard.

  Keys are strings with a maximum length of 64 characters. Values are strings
  with a maximum length of 512 characters.

- `name: optional string`

  The name of the vector store.

### Returns

- `VectorStore = object { id, created_at, file_counts, 8 more }`

  A vector store is a collection of processed files can be used by the `file_search` tool.

  - `id: string`

    The identifier, which can be referenced in API endpoints.

  - `created_at: number`

    The Unix timestamp (in seconds) for when the vector store was created.

  - `file_counts: object { cancelled, completed, failed, 2 more }`

    - `cancelled: number`

      The number of files that were cancelled.

    - `completed: number`

      The number of files that have been successfully processed.

    - `failed: number`

      The number of files that have failed to process.

    - `in_progress: number`

      The number of files that are currently being processed.

    - `total: number`

      The total number of files.

  - `last_active_at: number`

    The Unix timestamp (in seconds) for when the vector store was last active.

  - `metadata: Metadata`

    Set of 16 key-value pairs that can be attached to an object. This can be
    useful for storing additional information about the object in a structured
    format, and querying for objects via API or the dashboard.

    Keys are strings with a maximum length of 64 characters. Values are strings
    with a maximum length of 512 characters.

  - `name: string`

    The name of the vector store.

  - `object: "vector_store"`

    The object type, which is always `vector_store`.

    - `"vector_store"`

  - `status: "expired" or "in_progress" or "completed"`

    The status of the vector store, which can be either `expired`, `in_progress`, or `completed`. A status of `completed` indicates that the vector store is ready for use.

    - `"expired"`

    - `"in_progress"`

    - `"completed"`

  - `usage_bytes: number`

    The total number of bytes used by the files in the vector store.

  - `expires_after: optional object { anchor, days }`

    The expiration policy for a vector store.

    - `anchor: "last_active_at"`

      Anchor timestamp after which the expiration policy applies. Supported anchors: `last_active_at`.

      - `"last_active_at"`

    - `days: number`

      The number of days after the anchor time that the vector store will expire.

  - `expires_at: optional number`

    The Unix timestamp (in seconds) for when the vector store will expire.

### Example

```http
curl https://api.openai.com/v1/vector_stores \
    -H 'Content-Type: application/json' \
    -H 'OpenAI-Beta: assistants=v2' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{}'
```

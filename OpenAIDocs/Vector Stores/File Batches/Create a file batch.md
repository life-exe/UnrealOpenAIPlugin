## Create

**post** `/vector_stores/{vector_store_id}/file_batches`

Create a vector store file batch.

### Path Parameters

- `vector_store_id: string`

### Body Parameters

- `attributes: optional map[string or number or boolean]`

  Set of 16 key-value pairs that can be attached to an object. This can be
  useful for storing additional information about the object in a structured
  format, and querying for objects via API or the dashboard. Keys are strings
  with a maximum length of 64 characters. Values are strings with a maximum
  length of 512 characters, booleans, or numbers.

  - `UnionMember0 = string`

  - `UnionMember1 = number`

  - `UnionMember2 = boolean`

- `chunking_strategy: optional FileChunkingStrategyParam`

  The chunking strategy used to chunk the file(s). If not set, will use the `auto` strategy.

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

- `file_ids: optional array of string`

  A list of [File](/docs/api-reference/files) IDs that the vector store should use. Useful for tools like `file_search` that can access files.  If `attributes` or `chunking_strategy` are provided, they will be  applied to all files in the batch. The maximum batch size is 2000 files. Mutually exclusive with `files`.

- `files: optional array of object { file_id, attributes, chunking_strategy }`

  A list of objects that each include a `file_id` plus optional `attributes` or `chunking_strategy`. Use this when you need to override metadata for specific files. The global `attributes` or `chunking_strategy` will be ignored and must be specified for each file. The maximum batch size is 2000 files. Mutually exclusive with `file_ids`.

  - `file_id: string`

    A [File](/docs/api-reference/files) ID that the vector store should use. Useful for tools like `file_search` that can access files.

  - `attributes: optional map[string or number or boolean]`

    Set of 16 key-value pairs that can be attached to an object. This can be
    useful for storing additional information about the object in a structured
    format, and querying for objects via API or the dashboard. Keys are strings
    with a maximum length of 64 characters. Values are strings with a maximum
    length of 512 characters, booleans, or numbers.

    - `UnionMember0 = string`

    - `UnionMember1 = number`

    - `UnionMember2 = boolean`

  - `chunking_strategy: optional FileChunkingStrategyParam`

    The chunking strategy used to chunk the file(s). If not set, will use the `auto` strategy.

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

### Returns

- `VectorStoreFileBatch = object { id, created_at, file_counts, 3 more }`

  A batch of files attached to a vector store.

  - `id: string`

    The identifier, which can be referenced in API endpoints.

  - `created_at: number`

    The Unix timestamp (in seconds) for when the vector store files batch was created.

  - `file_counts: object { cancelled, completed, failed, 2 more }`

    - `cancelled: number`

      The number of files that where cancelled.

    - `completed: number`

      The number of files that have been processed.

    - `failed: number`

      The number of files that have failed to process.

    - `in_progress: number`

      The number of files that are currently being processed.

    - `total: number`

      The total number of files.

  - `object: "vector_store.files_batch"`

    The object type, which is always `vector_store.file_batch`.

    - `"vector_store.files_batch"`

  - `status: "in_progress" or "completed" or "cancelled" or "failed"`

    The status of the vector store files batch, which can be either `in_progress`, `completed`, `cancelled` or `failed`.

    - `"in_progress"`

    - `"completed"`

    - `"cancelled"`

    - `"failed"`

  - `vector_store_id: string`

    The ID of the [vector store](/docs/api-reference/vector-stores/object) that the [File](/docs/api-reference/files) is attached to.

### Example

```http
curl https://api.openai.com/v1/vector_stores/$VECTOR_STORE_ID/file_batches \
    -H 'Content-Type: application/json' \
    -H 'OpenAI-Beta: assistants=v2' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{}'
```

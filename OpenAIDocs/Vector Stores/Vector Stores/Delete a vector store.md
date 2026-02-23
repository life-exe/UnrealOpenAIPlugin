## Delete

**delete** `/vector_stores/{vector_store_id}`

Delete a vector store.

### Path Parameters

- `vector_store_id: string`

### Returns

- `VectorStoreDeleted = object { id, deleted, object }`

  - `id: string`

  - `deleted: boolean`

  - `object: "vector_store.deleted"`

    - `"vector_store.deleted"`

### Example

```http
curl https://api.openai.com/v1/vector_stores/$VECTOR_STORE_ID \
    -X DELETE \
    -H 'OpenAI-Beta: assistants=v2' \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

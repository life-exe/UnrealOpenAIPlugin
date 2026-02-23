## Delete

**delete** `/files/{file_id}`

Delete a file and remove it from all vector stores.

### Path Parameters

- `file_id: string`

### Returns

- `FileDeleted = object { id, deleted, object }`

  - `id: string`

  - `deleted: boolean`

  - `object: "file"`

    - `"file"`

### Example

```http
curl https://api.openai.com/v1/files/$FILE_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

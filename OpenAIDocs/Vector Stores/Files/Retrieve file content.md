## Content

**get** `/vector_stores/{vector_store_id}/files/{file_id}/content`

Retrieve the parsed contents of a vector store file.

### Path Parameters

- `vector_store_id: string`

- `file_id: string`

### Returns

- `data: array of object { text, type }`

  Parsed content of the file.

  - `text: optional string`

    The text content

  - `type: optional string`

    The content type (currently only `"text"`)

- `has_more: boolean`

  Indicates if there are more content pages to fetch.

- `next_page: string`

  The token for the next page, if any.

- `object: "vector_store.file_content.page"`

  The object type, which is always `vector_store.file_content.page`

  - `"vector_store.file_content.page"`

### Example

```http
curl https://api.openai.com/v1/vector_stores/$VECTOR_STORE_ID/files/$FILE_ID/content \
    -H 'OpenAI-Beta: assistants=v2' \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

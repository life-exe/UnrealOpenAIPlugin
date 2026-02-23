## Content

**get** `/files/{file_id}/content`

Returns the contents of the specified file.

### Path Parameters

- `file_id: string`

### Example

```http
curl https://api.openai.com/v1/files/$FILE_ID/content \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

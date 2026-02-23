## Delete

**delete** `/responses/{response_id}`

Deletes a model response with the given ID.

### Path Parameters

- `response_id: string`

### Example

```http
curl https://api.openai.com/v1/responses/$RESPONSE_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

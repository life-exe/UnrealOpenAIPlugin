## Delete

**delete** `/containers/{container_id}`

Delete Container

### Path Parameters

- `container_id: string`

### Example

```http
curl https://api.openai.com/v1/containers/$CONTAINER_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

## Delete

**delete** `/models/{model}`

Delete a fine-tuned model. You must have the Owner role in your organization to delete a model.

### Path Parameters

- `model: string`

### Returns

- `ModelDeleted = object { id, deleted, object }`

  - `id: string`

  - `deleted: boolean`

  - `object: string`

### Example

```http
curl https://api.openai.com/v1/models/$MODEL \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

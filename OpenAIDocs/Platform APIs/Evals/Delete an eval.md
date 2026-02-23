## Delete

**delete** `/evals/{eval_id}`

Delete an evaluation.

### Path Parameters

- `eval_id: string`

### Returns

- `deleted: boolean`

- `eval_id: string`

- `object: string`

### Example

```http
curl https://api.openai.com/v1/evals/$EVAL_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

## Delete

**delete** `/fine_tuning/checkpoints/{fine_tuned_model_checkpoint}/permissions/{permission_id}`

**NOTE:** This endpoint requires an [admin API key](../admin-api-keys).

Organization owners can use this endpoint to delete a permission for a fine-tuned model checkpoint.

### Path Parameters

- `fine_tuned_model_checkpoint: string`

- `permission_id: string`

### Returns

- `id: string`

  The ID of the fine-tuned model checkpoint permission that was deleted.

- `deleted: boolean`

  Whether the fine-tuned model checkpoint permission was successfully deleted.

- `object: "checkpoint.permission"`

  The object type, which is always "checkpoint.permission".

  - `"checkpoint.permission"`

### Example

```http
curl https://api.openai.com/v1/fine_tuning/checkpoints/$FINE_TUNED_MODEL_CHECKPOINT/permissions/$PERMISSION_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

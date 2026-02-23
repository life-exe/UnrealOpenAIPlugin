## Delete

**delete** `/chat/completions/{completion_id}`

Delete a stored chat completion. Only Chat Completions that have been
created with the `store` parameter set to `true` can be deleted.

### Path Parameters

- `completion_id: string`

### Returns

- `ChatCompletionDeleted = object { id, deleted, object }`

  - `id: string`

    The ID of the chat completion that was deleted.

  - `deleted: boolean`

    Whether the chat completion was deleted.

  - `object: "chat.completion.deleted"`

    The type of object being deleted.

    - `"chat.completion.deleted"`

### Example

```http
curl https://api.openai.com/v1/chat/completions/$COMPLETION_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

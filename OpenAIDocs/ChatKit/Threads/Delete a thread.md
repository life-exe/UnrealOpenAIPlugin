## Delete

**delete** `/chatkit/threads/{thread_id}`

Delete a ChatKit thread along with its items and stored attachments.

### Path Parameters

- `thread_id: string`

### Returns

- `id: string`

  Identifier of the deleted thread.

- `deleted: boolean`

  Indicates that the thread has been deleted.

- `object: "chatkit.thread.deleted"`

  Type discriminator that is always `chatkit.thread.deleted`.

  - `"chatkit.thread.deleted"`

### Example

```http
curl https://api.openai.com/v1/chatkit/threads/$THREAD_ID \
    -X DELETE \
    -H 'OpenAI-Beta: chatkit_beta=v1' \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

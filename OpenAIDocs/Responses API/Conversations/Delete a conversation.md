## Delete

**delete** `/conversations/{conversation_id}`

Delete a conversation. Items in the conversation will not be deleted.

### Path Parameters

- `conversation_id: string`

### Returns

- `ConversationDeletedResource = object { id, deleted, object }`

  - `id: string`

  - `deleted: boolean`

  - `object: "conversation.deleted"`

    - `"conversation.deleted"`

### Example

```http
curl https://api.openai.com/v1/conversations/$CONVERSATION_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

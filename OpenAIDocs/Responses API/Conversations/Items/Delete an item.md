## Delete

**delete** `/conversations/{conversation_id}/items/{item_id}`

Delete an item from a conversation with the given IDs.

### Path Parameters

- `conversation_id: string`

- `item_id: string`

### Returns

- `Conversation = object { id, created_at, metadata, object }`

  - `id: string`

    The unique ID of the conversation.

  - `created_at: number`

    The time at which the conversation was created, measured in seconds since the Unix epoch.

  - `metadata: unknown`

    Set of 16 key-value pairs that can be attached to an object. This can be         useful for storing additional information about the object in a structured         format, and querying for objects via API or the dashboard.
    Keys are strings with a maximum length of 64 characters. Values are strings         with a maximum length of 512 characters.

  - `object: "conversation"`

    The object type, which is always `conversation`.

    - `"conversation"`

### Example

```http
curl https://api.openai.com/v1/conversations/$CONVERSATION_ID/items/$ITEM_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

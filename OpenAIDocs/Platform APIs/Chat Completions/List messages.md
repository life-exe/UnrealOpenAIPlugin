## List

**get** `/chat/completions/{completion_id}/messages`

Get the messages in a stored chat completion. Only Chat Completions that
have been created with the `store` parameter set to `true` will be
returned.

### Path Parameters

- `completion_id: string`

### Query Parameters

- `after: optional string`

  Identifier for the last message from the previous pagination request.

- `limit: optional number`

  Number of messages to retrieve.

- `order: optional "asc" or "desc"`

  Sort order for messages by timestamp. Use `asc` for ascending order or `desc` for descending order. Defaults to `asc`.

  - `"asc"`

  - `"desc"`

### Returns

- `data: array of ChatCompletionStoreMessage`

  An array of chat completion message objects.

  - `id: string`

    The identifier of the chat message.

  - `content_parts: optional array of ChatCompletionContentPartText or ChatCompletionContentPartImage`

    If a content parts array was provided, this is an array of `text` and `image_url` parts.
    Otherwise, null.

    - `ChatCompletionContentPartText = object { text, type }`

      Learn about [text inputs](/docs/guides/text-generation).

      - `text: string`

        The text content.

      - `type: "text"`

        The type of the content part.

        - `"text"`

    - `ChatCompletionContentPartImage = object { image_url, type }`

      Learn about [image inputs](/docs/guides/vision).

      - `image_url: object { url, detail }`

        - `url: string`

          Either a URL of the image or the base64 encoded image data.

        - `detail: optional "auto" or "low" or "high"`

          Specifies the detail level of the image. Learn more in the [Vision guide](/docs/guides/vision#low-or-high-fidelity-image-understanding).

          - `"auto"`

          - `"low"`

          - `"high"`

      - `type: "image_url"`

        The type of the content part.

        - `"image_url"`

- `first_id: string`

  The identifier of the first chat message in the data array.

- `has_more: boolean`

  Indicates whether there are more chat messages available.

- `last_id: string`

  The identifier of the last chat message in the data array.

- `object: "list"`

  The type of this object. It is always set to "list".

  - `"list"`

### Example

```http
curl https://api.openai.com/v1/chat/completions/$COMPLETION_ID/messages \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

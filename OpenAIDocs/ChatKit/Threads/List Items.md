## List Items

**get** `/chatkit/threads/{thread_id}/items`

List items that belong to a ChatKit thread.

### Path Parameters

- `thread_id: string`

### Query Parameters

- `after: optional string`

  List items created after this thread item ID. Defaults to null for the first page.

- `before: optional string`

  List items created before this thread item ID. Defaults to null for the newest results.

- `limit: optional number`

  Maximum number of thread items to return. Defaults to 20.

- `order: optional "asc" or "desc"`

  Sort order for results by creation time. Defaults to `desc`.

  - `"asc"`

  - `"desc"`

### Returns

- `ChatKitThreadItemList = object { data, first_id, has_more, 2 more }`

  A paginated list of thread items rendered for the ChatKit API.

  - `data: array of ChatKitThreadUserMessageItem or ChatKitThreadAssistantMessageItem or ChatKitWidgetItem or 3 more`

    A list of items

    - `ChatKitThreadUserMessageItem = object { id, attachments, content, 5 more }`

      User-authored messages within a thread.

      - `id: string`

        Identifier of the thread item.

      - `attachments: array of ChatKitAttachment`

        Attachments associated with the user message. Defaults to an empty list.

        - `id: string`

          Identifier for the attachment.

        - `mime_type: string`

          MIME type of the attachment.

        - `name: string`

          Original display name for the attachment.

        - `preview_url: string`

          Preview URL for rendering the attachment inline.

        - `type: "image" or "file"`

          Attachment discriminator.

          - `"image"`

          - `"file"`

      - `content: array of object { text, type }  or object { text, type }`

        Ordered content elements supplied by the user.

        - `InputText = object { text, type }`

          Text block that a user contributed to the thread.

          - `text: string`

            Plain-text content supplied by the user.

          - `type: "input_text"`

            Type discriminator that is always `input_text`.

            - `"input_text"`

        - `QuotedText = object { text, type }`

          Quoted snippet that the user referenced in their message.

          - `text: string`

            Quoted text content.

          - `type: "quoted_text"`

            Type discriminator that is always `quoted_text`.

            - `"quoted_text"`

      - `created_at: number`

        Unix timestamp (in seconds) for when the item was created.

      - `inference_options: object { model, tool_choice }`

        Inference overrides applied to the message. Defaults to null when unset.

        - `model: string`

          Model name that generated the response. Defaults to null when using the session default.

        - `tool_choice: object { id }`

          Preferred tool to invoke. Defaults to null when ChatKit should auto-select.

          - `id: string`

            Identifier of the requested tool.

      - `object: "chatkit.thread_item"`

        Type discriminator that is always `chatkit.thread_item`.

        - `"chatkit.thread_item"`

      - `thread_id: string`

        Identifier of the parent thread.

      - `type: "chatkit.user_message"`

        - `"chatkit.user_message"`

    - `ChatKitThreadAssistantMessageItem = object { id, content, created_at, 3 more }`

      Assistant-authored message within a thread.

      - `id: string`

        Identifier of the thread item.

      - `content: array of ChatKitResponseOutputText`

        Ordered assistant response segments.

        - `annotations: array of object { source, type }  or object { source, type }`

          Ordered list of annotations attached to the response text.

          - `File = object { source, type }`

            Annotation that references an uploaded file.

            - `source: object { filename, type }`

              File attachment referenced by the annotation.

              - `filename: string`

                Filename referenced by the annotation.

              - `type: "file"`

                Type discriminator that is always `file`.

                - `"file"`

            - `type: "file"`

              Type discriminator that is always `file` for this annotation.

              - `"file"`

          - `URL = object { source, type }`

            Annotation that references a URL.

            - `source: object { type, url }`

              URL referenced by the annotation.

              - `type: "url"`

                Type discriminator that is always `url`.

                - `"url"`

              - `url: string`

                URL referenced by the annotation.

            - `type: "url"`

              Type discriminator that is always `url` for this annotation.

              - `"url"`

        - `text: string`

          Assistant generated text.

        - `type: "output_text"`

          Type discriminator that is always `output_text`.

          - `"output_text"`

      - `created_at: number`

        Unix timestamp (in seconds) for when the item was created.

      - `object: "chatkit.thread_item"`

        Type discriminator that is always `chatkit.thread_item`.

        - `"chatkit.thread_item"`

      - `thread_id: string`

        Identifier of the parent thread.

      - `type: "chatkit.assistant_message"`

        Type discriminator that is always `chatkit.assistant_message`.

        - `"chatkit.assistant_message"`

    - `ChatKitWidgetItem = object { id, created_at, object, 3 more }`

      Thread item that renders a widget payload.

      - `id: string`

        Identifier of the thread item.

      - `created_at: number`

        Unix timestamp (in seconds) for when the item was created.

      - `object: "chatkit.thread_item"`

        Type discriminator that is always `chatkit.thread_item`.

        - `"chatkit.thread_item"`

      - `thread_id: string`

        Identifier of the parent thread.

      - `type: "chatkit.widget"`

        Type discriminator that is always `chatkit.widget`.

        - `"chatkit.widget"`

      - `widget: string`

        Serialized widget payload rendered in the UI.

    - `ChatKitClientToolCall = object { id, arguments, call_id, 7 more }`

      Record of a client side tool invocation initiated by the assistant.

      - `id: string`

        Identifier of the thread item.

      - `arguments: string`

        JSON-encoded arguments that were sent to the tool.

      - `call_id: string`

        Identifier for the client tool call.

      - `created_at: number`

        Unix timestamp (in seconds) for when the item was created.

      - `name: string`

        Tool name that was invoked.

      - `object: "chatkit.thread_item"`

        Type discriminator that is always `chatkit.thread_item`.

        - `"chatkit.thread_item"`

      - `output: string`

        JSON-encoded output captured from the tool. Defaults to null while execution is in progress.

      - `status: "in_progress" or "completed"`

        Execution status for the tool call.

        - `"in_progress"`

        - `"completed"`

      - `thread_id: string`

        Identifier of the parent thread.

      - `type: "chatkit.client_tool_call"`

        Type discriminator that is always `chatkit.client_tool_call`.

        - `"chatkit.client_tool_call"`

    - `ChatKitTask = object { id, created_at, heading, 5 more }`

      Task emitted by the workflow to show progress and status updates.

      - `id: string`

        Identifier of the thread item.

      - `created_at: number`

        Unix timestamp (in seconds) for when the item was created.

      - `heading: string`

        Optional heading for the task. Defaults to null when not provided.

      - `object: "chatkit.thread_item"`

        Type discriminator that is always `chatkit.thread_item`.

        - `"chatkit.thread_item"`

      - `summary: string`

        Optional summary that describes the task. Defaults to null when omitted.

      - `task_type: "custom" or "thought"`

        Subtype for the task.

        - `"custom"`

        - `"thought"`

      - `thread_id: string`

        Identifier of the parent thread.

      - `type: "chatkit.task"`

        Type discriminator that is always `chatkit.task`.

        - `"chatkit.task"`

    - `ChatKitTaskGroup = object { id, created_at, object, 3 more }`

      Collection of workflow tasks grouped together in the thread.

      - `id: string`

        Identifier of the thread item.

      - `created_at: number`

        Unix timestamp (in seconds) for when the item was created.

      - `object: "chatkit.thread_item"`

        Type discriminator that is always `chatkit.thread_item`.

        - `"chatkit.thread_item"`

      - `tasks: array of object { heading, summary, type }`

        Tasks included in the group.

        - `heading: string`

          Optional heading for the grouped task. Defaults to null when not provided.

        - `summary: string`

          Optional summary that describes the grouped task. Defaults to null when omitted.

        - `type: "custom" or "thought"`

          Subtype for the grouped task.

          - `"custom"`

          - `"thought"`

      - `thread_id: string`

        Identifier of the parent thread.

      - `type: "chatkit.task_group"`

        Type discriminator that is always `chatkit.task_group`.

        - `"chatkit.task_group"`

  - `first_id: string`

    The ID of the first item in the list.

  - `has_more: boolean`

    Whether there are more items available.

  - `last_id: string`

    The ID of the last item in the list.

  - `object: "list"`

    The type of object returned, must be `list`.

    - `"list"`

### Example

```http
curl https://api.openai.com/v1/chatkit/threads/$THREAD_ID/items \
    -H 'OpenAI-Beta: chatkit_beta=v1' \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

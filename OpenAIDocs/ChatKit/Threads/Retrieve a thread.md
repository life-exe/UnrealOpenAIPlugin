## Retrieve

**get** `/chatkit/threads/{thread_id}`

Retrieve a ChatKit thread by its identifier.

### Path Parameters

- `thread_id: string`

### Returns

- `ChatKitThread = object { id, created_at, object, 3 more }`

  Represents a ChatKit thread and its current status.

  - `id: string`

    Identifier of the thread.

  - `created_at: number`

    Unix timestamp (in seconds) for when the thread was created.

  - `object: "chatkit.thread"`

    Type discriminator that is always `chatkit.thread`.

    - `"chatkit.thread"`

  - `status: object { type }  or object { reason, type }  or object { reason, type }`

    Current status for the thread. Defaults to `active` for newly created threads.

    - `Active = object { type }`

      Indicates that a thread is active.

      - `type: "active"`

        Status discriminator that is always `active`.

        - `"active"`

    - `Locked = object { reason, type }`

      Indicates that a thread is locked and cannot accept new input.

      - `reason: string`

        Reason that the thread was locked. Defaults to null when no reason is recorded.

      - `type: "locked"`

        Status discriminator that is always `locked`.

        - `"locked"`

    - `Closed = object { reason, type }`

      Indicates that a thread has been closed.

      - `reason: string`

        Reason that the thread was closed. Defaults to null when no reason is recorded.

      - `type: "closed"`

        Status discriminator that is always `closed`.

        - `"closed"`

  - `title: string`

    Optional human-readable title for the thread. Defaults to null when no title has been generated.

  - `user: string`

    Free-form string that identifies your end user who owns the thread.

### Example

```http
curl https://api.openai.com/v1/chatkit/threads/$THREAD_ID \
    -H 'OpenAI-Beta: chatkit_beta=v1' \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

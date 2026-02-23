## Cancel

**post** `/chatkit/sessions/{session_id}/cancel`

Cancel an active ChatKit session and return its most recent metadata.

Cancelling prevents new requests from using the issued client secret.

### Path Parameters

- `session_id: string`

### Returns

- `ChatSession = object { id, chatkit_configuration, client_secret, 7 more }`

  Represents a ChatKit session and its resolved configuration.

  - `id: string`

    Identifier for the ChatKit session.

  - `chatkit_configuration: ChatSessionChatKitConfiguration`

    Resolved ChatKit feature configuration for the session.

    - `automatic_thread_titling: ChatSessionAutomaticThreadTitling`

      Automatic thread titling preferences.

      - `enabled: boolean`

        Whether automatic thread titling is enabled.

    - `file_upload: ChatSessionFileUpload`

      Upload settings for the session.

      - `enabled: boolean`

        Indicates if uploads are enabled for the session.

      - `max_file_size: number`

        Maximum upload size in megabytes.

      - `max_files: number`

        Maximum number of uploads allowed during the session.

    - `history: ChatSessionHistory`

      History retention configuration.

      - `enabled: boolean`

        Indicates if chat history is persisted for the session.

      - `recent_threads: number`

        Number of prior threads surfaced in history views. Defaults to null when all history is retained.

  - `client_secret: string`

    Ephemeral client secret that authenticates session requests.

  - `expires_at: number`

    Unix timestamp (in seconds) for when the session expires.

  - `max_requests_per_1_minute: number`

    Convenience copy of the per-minute request limit.

  - `object: "chatkit.session"`

    Type discriminator that is always `chatkit.session`.

    - `"chatkit.session"`

  - `rate_limits: ChatSessionRateLimits`

    Resolved rate limit values.

    - `max_requests_per_1_minute: number`

      Maximum allowed requests per one-minute window.

  - `status: ChatSessionStatus`

    Current lifecycle state of the session.

    - `"active"`

    - `"expired"`

    - `"cancelled"`

  - `user: string`

    User identifier associated with the session.

  - `workflow: ChatKitWorkflow`

    Workflow metadata for the session.

    - `id: string`

      Identifier of the workflow backing the session.

    - `state_variables: map[string or boolean or number]`

      State variable key-value pairs applied when invoking the workflow. Defaults to null when no overrides were provided.

      - `UnionMember0 = string`

      - `UnionMember1 = boolean`

      - `UnionMember2 = number`

    - `tracing: object { enabled }`

      Tracing settings applied to the workflow.

      - `enabled: boolean`

        Indicates whether tracing is enabled.

    - `version: string`

      Specific workflow version used for the session. Defaults to null when using the latest deployment.

### Example

```http
curl https://api.openai.com/v1/chatkit/sessions/$SESSION_ID/cancel \
    -X POST \
    -H 'OpenAI-Beta: chatkit_beta=v1' \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

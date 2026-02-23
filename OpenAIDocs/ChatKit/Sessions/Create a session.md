## Create

**post** `/chatkit/sessions`

Create a ChatKit session.

### Body Parameters

- `user: string`

  A free-form string that identifies your end user; ensures this Session can access other objects that have the same `user` scope.

- `workflow: ChatSessionWorkflowParam`

  Workflow that powers the session.

  - `id: string`

    Identifier for the workflow invoked by the session.

  - `state_variables: optional map[string or boolean or number]`

    State variables forwarded to the workflow. Keys may be up to 64 characters, values must be primitive types, and the map defaults to an empty object.

    - `UnionMember0 = string`

    - `UnionMember1 = boolean`

    - `UnionMember2 = number`

  - `tracing: optional object { enabled }`

    Optional tracing overrides for the workflow invocation. When omitted, tracing is enabled by default.

    - `enabled: optional boolean`

      Whether tracing is enabled during the session. Defaults to true.

  - `version: optional string`

    Specific workflow version to run. Defaults to the latest deployed version.

- `chatkit_configuration: optional ChatSessionChatKitConfigurationParam`

  Optional overrides for ChatKit runtime configuration features

  - `automatic_thread_titling: optional object { enabled }`

    Configuration for automatic thread titling. When omitted, automatic thread titling is enabled by default.

    - `enabled: optional boolean`

      Enable automatic thread title generation. Defaults to true.

  - `file_upload: optional object { enabled, max_file_size, max_files }`

    Configuration for upload enablement and limits. When omitted, uploads are disabled by default (max_files 10, max_file_size 512 MB).

    - `enabled: optional boolean`

      Enable uploads for this session. Defaults to false.

    - `max_file_size: optional number`

      Maximum size in megabytes for each uploaded file. Defaults to 512 MB, which is the maximum allowable size.

    - `max_files: optional number`

      Maximum number of files that can be uploaded to the session. Defaults to 10.

  - `history: optional object { enabled, recent_threads }`

    Configuration for chat history retention. When omitted, history is enabled by default with no limit on recent_threads (null).

    - `enabled: optional boolean`

      Enables chat users to access previous ChatKit threads. Defaults to true.

    - `recent_threads: optional number`

      Number of recent ChatKit threads users have access to. Defaults to unlimited when unset.

- `expires_after: optional ChatSessionExpiresAfterParam`

  Optional override for session expiration timing in seconds from creation. Defaults to 10 minutes.

  - `anchor: "created_at"`

    Base timestamp used to calculate expiration. Currently fixed to `created_at`.

    - `"created_at"`

  - `seconds: number`

    Number of seconds after the anchor when the session expires.

- `rate_limits: optional ChatSessionRateLimitsParam`

  Optional override for per-minute request limits. When omitted, defaults to 10.

  - `max_requests_per_1_minute: optional number`

    Maximum number of requests allowed per minute for the session. Defaults to 10.

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
curl https://api.openai.com/v1/chatkit/sessions \
    -H 'Content-Type: application/json' \
    -H 'OpenAI-Beta: chatkit_beta=v1' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "user": "x",
          "workflow": {
            "id": "id"
          }
        }'
```

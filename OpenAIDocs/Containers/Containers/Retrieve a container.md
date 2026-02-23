## Retrieve

**get** `/containers/{container_id}`

Retrieve Container

### Path Parameters

- `container_id: string`

### Returns

- `id: string`

  Unique identifier for the container.

- `created_at: number`

  Unix timestamp (in seconds) when the container was created.

- `name: string`

  Name of the container.

- `object: string`

  The type of this object.

- `status: string`

  Status of the container (e.g., active, deleted).

- `expires_after: optional object { anchor, minutes }`

  The container will expire after this time period.
  The anchor is the reference point for the expiration.
  The minutes is the number of minutes after the anchor before the container expires.

  - `anchor: optional "last_active_at"`

    The reference point for the expiration.

    - `"last_active_at"`

  - `minutes: optional number`

    The number of minutes after the anchor before the container expires.

- `last_active_at: optional number`

  Unix timestamp (in seconds) when the container was last active.

- `memory_limit: optional "1g" or "4g" or "16g" or "64g"`

  The memory limit configured for the container.

  - `"1g"`

  - `"4g"`

  - `"16g"`

  - `"64g"`

- `network_policy: optional object { type, allowed_domains }`

  Network access policy for the container.

  - `type: "allowlist" or "disabled"`

    The network policy mode.

    - `"allowlist"`

    - `"disabled"`

  - `allowed_domains: optional array of string`

    Allowed outbound domains when `type` is `allowlist`.

### Example

```http
curl https://api.openai.com/v1/containers/$CONTAINER_ID \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

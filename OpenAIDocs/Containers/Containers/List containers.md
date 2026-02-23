## List

**get** `/containers`

List Containers

### Query Parameters

- `after: optional string`

  A cursor for use in pagination. `after` is an object ID that defines your place in the list. For instance, if you make a list request and receive 100 objects, ending with obj_foo, your subsequent call can include after=obj_foo in order to fetch the next page of the list.

- `limit: optional number`

  A limit on the number of objects to be returned. Limit can range between 1 and 100, and the default is 20.

- `name: optional string`

  Filter results by container name.

- `order: optional "asc" or "desc"`

  Sort order by the `created_at` timestamp of the objects. `asc` for ascending order and `desc` for descending order.

  - `"asc"`

  - `"desc"`

### Returns

- `data: array of object { id, created_at, name, 6 more }`

  A list of containers.

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

- `first_id: string`

  The ID of the first container in the list.

- `has_more: boolean`

  Whether there are more containers available.

- `last_id: string`

  The ID of the last container in the list.

- `object: "list"`

  The type of object returned, must be 'list'.

  - `"list"`

### Example

```http
curl https://api.openai.com/v1/containers \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

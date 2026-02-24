## List

**get** `/skills`

List all skills for the current project.

### Query Parameters

- `after: optional string`

  Identifier for the last item from the previous pagination request

- `limit: optional number`

  Number of items to retrieve

- `order: optional "asc" or "desc"`

  Sort order of results by timestamp. Use `asc` for ascending order or `desc` for descending order.

  - `"asc"`

  - `"desc"`

### Returns

- `SkillList = object { data, first_id, has_more, 2 more }`

  - `data: array of Skill`

    A list of items

    - `id: string`

      Unique identifier for the skill.

    - `created_at: number`

      Unix timestamp (seconds) for when the skill was created.

    - `default_version: string`

      Default version for the skill.

    - `description: string`

      Description of the skill.

    - `latest_version: string`

      Latest version for the skill.

    - `name: string`

      Name of the skill.

    - `object: "skill"`

      The object type, which is `skill`.

      - `"skill"`

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
curl https://api.openai.com/v1/skills \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

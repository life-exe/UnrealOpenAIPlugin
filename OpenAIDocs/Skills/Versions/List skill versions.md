## List

**get** `/skills/{skill_id}/versions`

List skill versions for a skill.

### Path Parameters

- `skill_id: string`

### Query Parameters

- `after: optional string`

  The skill version ID to start after.

- `limit: optional number`

  Number of versions to retrieve.

- `order: optional "asc" or "desc"`

  Sort order of results by version number.

  - `"asc"`

  - `"desc"`

### Returns

- `SkillVersionList = object { data, first_id, has_more, 2 more }`

  - `data: array of SkillVersion`

    A list of items

    - `id: string`

      Unique identifier for the skill version.

    - `created_at: number`

      Unix timestamp (seconds) for when the version was created.

    - `description: string`

      Description of the skill version.

    - `name: string`

      Name of the skill version.

    - `object: "skill.version"`

      The object type, which is `skill.version`.

      - `"skill.version"`

    - `skill_id: string`

      Identifier of the skill for this version.

    - `version: string`

      Version number for this skill.

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
curl https://api.openai.com/v1/skills/$SKILL_ID/versions \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

## Update

**post** `/skills/{skill_id}`

Update the default version pointer for a skill.

### Path Parameters

- `skill_id: string`

### Body Parameters

- `default_version: string`

  The skill version number to set as default.

### Returns

- `Skill = object { id, created_at, default_version, 4 more }`

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

### Example

```http
curl https://api.openai.com/v1/skills/$SKILL_ID \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "default_version": "default_version"
        }'
```

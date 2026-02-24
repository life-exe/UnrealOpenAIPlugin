## Retrieve

**get** `/skills/{skill_id}`

Get a skill by its ID.

### Path Parameters

- `skill_id: string`

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
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

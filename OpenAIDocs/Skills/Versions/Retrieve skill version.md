## Retrieve

**get** `/skills/{skill_id}/versions/{version}`

Get a specific skill version.

### Path Parameters

- `skill_id: string`

- `version: string`

  The version number to retrieve.

### Returns

- `SkillVersion = object { id, created_at, description, 4 more }`

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

### Example

```http
curl https://api.openai.com/v1/skills/$SKILL_ID/versions/$VERSION \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

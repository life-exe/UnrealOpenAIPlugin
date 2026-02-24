## Create

**post** `/skills/{skill_id}/versions`

Create a new immutable skill version.

### Path Parameters

- `skill_id: string`

### Body Parameters

- `files: array of string or string`

  Skill files to upload (directory upload) or a single zip file.

  - `UnionMember0 = array of string`

    Skill files to upload (directory upload) or a single zip file.

  - `UnionMember1 = string`

    Skill zip file to upload.

- `default: optional boolean`

  Whether to set this version as the default.

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
curl https://api.openai.com/v1/skills/$SKILL_ID/versions \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -F files=[null]
```

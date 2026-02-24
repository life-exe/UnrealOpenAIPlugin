## Delete

**delete** `/skills/{skill_id}/versions/{version}`

Delete a skill version.

### Path Parameters

- `skill_id: string`

- `version: string`

  The skill version number.

### Returns

- `DeletedSkillVersion = object { id, deleted, object, version }`

  - `id: string`

  - `deleted: boolean`

  - `object: "skill.version.deleted"`

    - `"skill.version.deleted"`

  - `version: string`

    The deleted skill version.

### Example

```http
curl https://api.openai.com/v1/skills/$SKILL_ID/versions/$VERSION \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

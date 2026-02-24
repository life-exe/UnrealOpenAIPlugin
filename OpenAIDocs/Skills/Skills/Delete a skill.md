## Delete

**delete** `/skills/{skill_id}`

Delete a skill by its ID.

### Path Parameters

- `skill_id: string`

### Returns

- `DeletedSkill = object { id, deleted, object }`

  - `id: string`

  - `deleted: boolean`

  - `object: "skill.deleted"`

    - `"skill.deleted"`

### Example

```http
curl https://api.openai.com/v1/skills/$SKILL_ID \
    -X DELETE \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

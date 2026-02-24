## Retrieve

**get** `/skills/{skill_id}/content`

Download a skill zip bundle by its ID.

### Path Parameters

- `skill_id: string`

### Example

```http
curl https://api.openai.com/v1/skills/$SKILL_ID/content \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

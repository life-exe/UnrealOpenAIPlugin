## Create

**post** `/containers/{container_id}/files`

Create a Container File

You can send either a multipart/form-data request with the raw file content, or a JSON request with a file ID.

### Path Parameters

- `container_id: string`

### Body Parameters

- `file: optional string`

  The File object (not file name) to be uploaded.

- `file_id: optional string`

  Name of the file to create.

### Returns

- `id: string`

  Unique identifier for the file.

- `bytes: number`

  Size of the file in bytes.

- `container_id: string`

  The container this file belongs to.

- `created_at: number`

  Unix timestamp (in seconds) when the file was created.

- `object: string`

  The type of this object (`container.file`).

- `path: string`

  Path of the file in the container.

- `source: string`

  Source of the file (e.g., `user`, `assistant`).

### Example

```http
curl https://api.openai.com/v1/containers/$CONTAINER_ID/files \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

## Edit

**post** `/images/edits`

Creates an edited or extended image given one or more source images and a prompt. This endpoint supports GPT Image models (`gpt-image-1.5`, `gpt-image-1`, `gpt-image-1-mini`, and `chatgpt-image-latest`) and `dall-e-2`.

### Body Parameters

- `images: array of object { file_id, image_url }`

  Input image references to edit.
  For GPT image models, you can provide up to 16 images.

  - `file_id: optional string`

    The File API ID of an uploaded image to use as input.

  - `image_url: optional string`

    A fully qualified URL or base64-encoded data URL.

- `prompt: string`

  A text description of the desired image edit.

- `background: optional "transparent" or "opaque" or "auto"`

  Background behavior for generated image output.

  - `"transparent"`

  - `"opaque"`

  - `"auto"`

- `input_fidelity: optional "high" or "low"`

  Controls fidelity to the original input image(s).

  - `"high"`

  - `"low"`

- `mask: optional object { file_id, image_url }`

  Reference an input image by either URL or uploaded file ID.
  Provide exactly one of `image_url` or `file_id`.

  - `file_id: optional string`

    The File API ID of an uploaded image to use as input.

  - `image_url: optional string`

    A fully qualified URL or base64-encoded data URL.

- `model: optional string or "gpt-image-1.5" or "gpt-image-1" or "gpt-image-1-mini" or "chatgpt-image-latest"`

  The model to use for image editing.

  - `UnionMember0 = string`

  - `UnionMember1 = "gpt-image-1.5" or "gpt-image-1" or "gpt-image-1-mini" or "chatgpt-image-latest"`

    The model to use for image editing.

    - `"gpt-image-1.5"`

    - `"gpt-image-1"`

    - `"gpt-image-1-mini"`

    - `"chatgpt-image-latest"`

- `moderation: optional "low" or "auto"`

  Moderation level for GPT image models.

  - `"low"`

  - `"auto"`

- `n: optional number`

  The number of edited images to generate.

- `output_compression: optional number`

  Compression level for `jpeg` or `webp` output.

- `output_format: optional "png" or "jpeg" or "webp"`

  Output image format. Supported for GPT image models.

  - `"png"`

  - `"jpeg"`

  - `"webp"`

- `partial_images: optional number`

  The number of partial images to generate. This parameter is used for
  streaming responses that return partial images. Value must be between 0 and 3.
  When set to 0, the response will be a single image sent in one streaming event.

  Note that the final image may be sent before the full number of partial images
  are generated if the full image is generated more quickly.

- `quality: optional "low" or "medium" or "high" or "auto"`

  Output quality for GPT image models.

  - `"low"`

  - `"medium"`

  - `"high"`

  - `"auto"`

- `size: optional "auto" or "1024x1024" or "1536x1024" or "1024x1536"`

  Requested output image size.

  - `"auto"`

  - `"1024x1024"`

  - `"1536x1024"`

  - `"1024x1536"`

- `stream: optional boolean`

  Stream partial image results as events.

- `user: optional string`

  A unique identifier representing your end-user, which can help OpenAI
  monitor and detect abuse.

### Returns

- `ImagesResponse = object { created, background, data, 4 more }`

  The response from the image generation endpoint.

  - `created: number`

    The Unix timestamp (in seconds) of when the image was created.

  - `background: optional "transparent" or "opaque"`

    The background parameter used for the image generation. Either `transparent` or `opaque`.

    - `"transparent"`

    - `"opaque"`

  - `data: optional array of Image`

    The list of generated images.

    - `b64_json: optional string`

      The base64-encoded JSON of the generated image. Returned by default for the GPT image models, and only present if `response_format` is set to `b64_json` for `dall-e-2` and `dall-e-3`.

    - `revised_prompt: optional string`

      For `dall-e-3` only, the revised prompt that was used to generate the image.

    - `url: optional string`

      When using `dall-e-2` or `dall-e-3`, the URL of the generated image if `response_format` is set to `url` (default value). Unsupported for the GPT image models.

  - `output_format: optional "png" or "webp" or "jpeg"`

    The output format of the image generation. Either `png`, `webp`, or `jpeg`.

    - `"png"`

    - `"webp"`

    - `"jpeg"`

  - `quality: optional "low" or "medium" or "high"`

    The quality of the image generated. Either `low`, `medium`, or `high`.

    - `"low"`

    - `"medium"`

    - `"high"`

  - `size: optional "1024x1024" or "1024x1536" or "1536x1024"`

    The size of the image generated. Either `1024x1024`, `1024x1536`, or `1536x1024`.

    - `"1024x1024"`

    - `"1024x1536"`

    - `"1536x1024"`

  - `usage: optional object { input_tokens, input_tokens_details, output_tokens, 2 more }`

    For `gpt-image-1` only, the token usage information for the image generation.

    - `input_tokens: number`

      The number of tokens (images and text) in the input prompt.

    - `input_tokens_details: object { image_tokens, text_tokens }`

      The input tokens detailed information for the image generation.

      - `image_tokens: number`

        The number of image tokens in the input prompt.

      - `text_tokens: number`

        The number of text tokens in the input prompt.

    - `output_tokens: number`

      The number of output tokens generated by the model.

    - `total_tokens: number`

      The total number of tokens (images and text) used for the image generation.

    - `output_tokens_details: optional object { image_tokens, text_tokens }`

      The output token details for the image generation.

      - `image_tokens: number`

        The number of image output tokens generated by the model.

      - `text_tokens: number`

        The number of text output tokens generated by the model.

### Example

```http
curl https://api.openai.com/v1/images/edits \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "images": [
            {
              "image_url": "https://example.com/source-image.png"
            }
          ],
          "prompt": "Add a watercolor effect to this image",
          "background": "transparent",
          "model": "gpt-image-1.5",
          "moderation": "auto",
          "n": 1,
          "output_compression": 100,
          "output_format": "png",
          "partial_images": 1,
          "quality": "high",
          "size": "1024x1024",
          "user": "user-1234"
        }'
```

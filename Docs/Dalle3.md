# DALL·E 3

[DALL·E 3](https://openai.com/dall-e-3) is now available with all parameters:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/dalle3_1.png)

Preview mode has been added:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/dalle3_2.png)

Notes: 
- Currently, only requests with `URL` option are supported for Dalle3. This is because `OpenAI` sends image bytes in `WEBP` format when requesting the `Dalle3` model. `WEBP` is not supported by `Unreal Engine`. Hopefully, they will update this or at least add a parameter that allows us to receive images in `PNG` format.
- You can only generate one image with `Dalle3` per request.
- Image variation and image edit requests are only supported by `Dalle2`.

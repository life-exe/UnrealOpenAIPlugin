## Create

**post** `/moderations`

Classifies if text and/or image inputs are potentially harmful. Learn
more in the [moderation guide](/docs/guides/moderation).

### Body Parameters

- `input: string or array of string or array of object { image_url, type }  or object { text, type }`

  Input (or inputs) to classify. Can be a single string, an array of strings, or
  an array of multi-modal input objects similar to other models.

  - `UnionMember0 = string`

    A string of text to classify for moderation.

  - `UnionMember1 = array of string`

    An array of strings to classify for moderation.

  - `UnionMember2 = array of object { image_url, type }  or object { text, type }`

    An array of multi-modal inputs to the moderation model.

    - `UnionMember0 = object { image_url, type }`

      An object describing an image to classify.

      - `image_url: object { url }`

        Contains either an image URL or a data URL for a base64 encoded image.

        - `url: string`

          Either a URL of the image or the base64 encoded image data.

      - `type: "image_url"`

        Always `image_url`.

        - `"image_url"`

    - `UnionMember1 = object { text, type }`

      An object describing text to classify.

      - `text: string`

        A string of text to classify.

      - `type: "text"`

        Always `text`.

        - `"text"`

- `model: optional string or ModerationModel`

  The content moderation model you would like to use. Learn more in
  [the moderation guide](/docs/guides/moderation), and learn about
  available models [here](/docs/models#moderation).

  - `UnionMember0 = string`

  - `ModerationModel = "omni-moderation-latest" or "omni-moderation-2024-09-26" or "text-moderation-latest" or "text-moderation-stable"`

    - `"omni-moderation-latest"`

    - `"omni-moderation-2024-09-26"`

    - `"text-moderation-latest"`

    - `"text-moderation-stable"`

### Returns

- `id: string`

  The unique identifier for the moderation request.

- `model: string`

  The model used to generate the moderation results.

- `results: array of Moderation`

  A list of moderation objects.

  - `categories: object { harassment, "harassment/threatening", hate, 10 more }`

    A list of the categories, and whether they are flagged or not.

    - `harassment: boolean`

      Content that expresses, incites, or promotes harassing language towards any target.

    - `"harassment/threatening": boolean`

      Harassment content that also includes violence or serious harm towards any target.

    - `hate: boolean`

      Content that expresses, incites, or promotes hate based on race, gender, ethnicity, religion, nationality, sexual orientation, disability status, or caste. Hateful content aimed at non-protected groups (e.g., chess players) is harassment.

    - `"hate/threatening": boolean`

      Hateful content that also includes violence or serious harm towards the targeted group based on race, gender, ethnicity, religion, nationality, sexual orientation, disability status, or caste.

    - `illicit: boolean`

      Content that includes instructions or advice that facilitate the planning or execution of wrongdoing, or that gives advice or instruction on how to commit illicit acts. For example, "how to shoplift" would fit this category.

    - `"illicit/violent": boolean`

      Content that includes instructions or advice that facilitate the planning or execution of wrongdoing that also includes violence, or that gives advice or instruction on the procurement of any weapon.

    - `"self-harm": boolean`

      Content that promotes, encourages, or depicts acts of self-harm, such as suicide, cutting, and eating disorders.

    - `"self-harm/instructions": boolean`

      Content that encourages performing acts of self-harm, such as suicide, cutting, and eating disorders, or that gives instructions or advice on how to commit such acts.

    - `"self-harm/intent": boolean`

      Content where the speaker expresses that they are engaging or intend to engage in acts of self-harm, such as suicide, cutting, and eating disorders.

    - `sexual: boolean`

      Content meant to arouse sexual excitement, such as the description of sexual activity, or that promotes sexual services (excluding sex education and wellness).

    - `"sexual/minors": boolean`

      Sexual content that includes an individual who is under 18 years old.

    - `violence: boolean`

      Content that depicts death, violence, or physical injury.

    - `"violence/graphic": boolean`

      Content that depicts death, violence, or physical injury in graphic detail.

  - `category_applied_input_types: object { harassment, "harassment/threatening", hate, 10 more }`

    A list of the categories along with the input type(s) that the score applies to.

    - `harassment: array of "text"`

      The applied input type(s) for the category 'harassment'.

      - `"text"`

    - `"harassment/threatening": array of "text"`

      The applied input type(s) for the category 'harassment/threatening'.

      - `"text"`

    - `hate: array of "text"`

      The applied input type(s) for the category 'hate'.

      - `"text"`

    - `"hate/threatening": array of "text"`

      The applied input type(s) for the category 'hate/threatening'.

      - `"text"`

    - `illicit: array of "text"`

      The applied input type(s) for the category 'illicit'.

      - `"text"`

    - `"illicit/violent": array of "text"`

      The applied input type(s) for the category 'illicit/violent'.

      - `"text"`

    - `"self-harm": array of "text" or "image"`

      The applied input type(s) for the category 'self-harm'.

      - `"text"`

      - `"image"`

    - `"self-harm/instructions": array of "text" or "image"`

      The applied input type(s) for the category 'self-harm/instructions'.

      - `"text"`

      - `"image"`

    - `"self-harm/intent": array of "text" or "image"`

      The applied input type(s) for the category 'self-harm/intent'.

      - `"text"`

      - `"image"`

    - `sexual: array of "text" or "image"`

      The applied input type(s) for the category 'sexual'.

      - `"text"`

      - `"image"`

    - `"sexual/minors": array of "text"`

      The applied input type(s) for the category 'sexual/minors'.

      - `"text"`

    - `violence: array of "text" or "image"`

      The applied input type(s) for the category 'violence'.

      - `"text"`

      - `"image"`

    - `"violence/graphic": array of "text" or "image"`

      The applied input type(s) for the category 'violence/graphic'.

      - `"text"`

      - `"image"`

  - `category_scores: object { harassment, "harassment/threatening", hate, 10 more }`

    A list of the categories along with their scores as predicted by model.

    - `harassment: number`

      The score for the category 'harassment'.

    - `"harassment/threatening": number`

      The score for the category 'harassment/threatening'.

    - `hate: number`

      The score for the category 'hate'.

    - `"hate/threatening": number`

      The score for the category 'hate/threatening'.

    - `illicit: number`

      The score for the category 'illicit'.

    - `"illicit/violent": number`

      The score for the category 'illicit/violent'.

    - `"self-harm": number`

      The score for the category 'self-harm'.

    - `"self-harm/instructions": number`

      The score for the category 'self-harm/instructions'.

    - `"self-harm/intent": number`

      The score for the category 'self-harm/intent'.

    - `sexual: number`

      The score for the category 'sexual'.

    - `"sexual/minors": number`

      The score for the category 'sexual/minors'.

    - `violence: number`

      The score for the category 'violence'.

    - `"violence/graphic": number`

      The score for the category 'violence/graphic'.

  - `flagged: boolean`

    Whether any of the below categories are flagged.

### Example

```http
curl https://api.openai.com/v1/moderations \
    -H 'Content-Type: application/json' \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d '{
          "input": "I want to kill them."
        }'
```

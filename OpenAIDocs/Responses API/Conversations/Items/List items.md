## List

**get** `/conversations/{conversation_id}/items`

List all items for a conversation with the given ID.

### Path Parameters

- `conversation_id: string`

### Query Parameters

- `after: optional string`

  An item ID to list items after, used in pagination.

- `include: optional array of ResponseIncludable`

  Specify additional output data to include in the model response. Currently supported values are:

  - `web_search_call.action.sources`: Include the sources of the web search tool call.
  - `code_interpreter_call.outputs`: Includes the outputs of python code execution in code interpreter tool call items.
  - `computer_call_output.output.image_url`: Include image urls from the computer call output.
  - `file_search_call.results`: Include the search results of the file search tool call.
  - `message.input_image.image_url`: Include image urls from the input message.
  - `message.output_text.logprobs`: Include logprobs with assistant messages.
  - `reasoning.encrypted_content`: Includes an encrypted version of reasoning tokens in reasoning item outputs. This enables reasoning items to be used in multi-turn conversations when using the Responses API statelessly (like when the `store` parameter is set to `false`, or when an organization is enrolled in the zero data retention program).

  - `"file_search_call.results"`

  - `"web_search_call.results"`

  - `"web_search_call.action.sources"`

  - `"message.input_image.image_url"`

  - `"computer_call_output.output.image_url"`

  - `"code_interpreter_call.outputs"`

  - `"reasoning.encrypted_content"`

  - `"message.output_text.logprobs"`

- `limit: optional number`

  A limit on the number of objects to be returned. Limit can range between
  1 and 100, and the default is 20.

- `order: optional "asc" or "desc"`

  The order to return the input items in. Default is `desc`.

  - `asc`: Return the input items in ascending order.
  - `desc`: Return the input items in descending order.

  - `"asc"`

  - `"desc"`

### Returns

- `ConversationItemList = object { data, first_id, has_more, 2 more }`

  A list of Conversation items.

  - `data: array of ConversationItem`

    A list of conversation items.

    - `Message = object { id, content, role, 2 more }`

      A message to or from the model.

      - `id: string`

        The unique ID of the message.

      - `content: array of ResponseInputText or ResponseOutputText or TextContent or 6 more`

        The content of the message

        - `ResponseInputText = object { text, type }`

          A text input to the model.

          - `text: string`

            The text input to the model.

          - `type: "input_text"`

            The type of the input item. Always `input_text`.

            - `"input_text"`

        - `ResponseOutputText = object { annotations, logprobs, text, type }`

          A text output from the model.

          - `annotations: array of object { file_id, filename, index, type }  or object { end_index, start_index, title, 2 more }  or object { container_id, end_index, file_id, 3 more }  or object { file_id, index, type }`

            The annotations of the text output.

            - `FileCitation = object { file_id, filename, index, type }`

              A citation to a file.

              - `file_id: string`

                The ID of the file.

              - `filename: string`

                The filename of the file cited.

              - `index: number`

                The index of the file in the list of files.

              - `type: "file_citation"`

                The type of the file citation. Always `file_citation`.

                - `"file_citation"`

            - `URLCitation = object { end_index, start_index, title, 2 more }`

              A citation for a web resource used to generate a model response.

              - `end_index: number`

                The index of the last character of the URL citation in the message.

              - `start_index: number`

                The index of the first character of the URL citation in the message.

              - `title: string`

                The title of the web resource.

              - `type: "url_citation"`

                The type of the URL citation. Always `url_citation`.

                - `"url_citation"`

              - `url: string`

                The URL of the web resource.

            - `ContainerFileCitation = object { container_id, end_index, file_id, 3 more }`

              A citation for a container file used to generate a model response.

              - `container_id: string`

                The ID of the container file.

              - `end_index: number`

                The index of the last character of the container file citation in the message.

              - `file_id: string`

                The ID of the file.

              - `filename: string`

                The filename of the container file cited.

              - `start_index: number`

                The index of the first character of the container file citation in the message.

              - `type: "container_file_citation"`

                The type of the container file citation. Always `container_file_citation`.

                - `"container_file_citation"`

            - `FilePath = object { file_id, index, type }`

              A path to a file.

              - `file_id: string`

                The ID of the file.

              - `index: number`

                The index of the file in the list of files.

              - `type: "file_path"`

                The type of the file path. Always `file_path`.

                - `"file_path"`

          - `logprobs: array of object { token, bytes, logprob, top_logprobs }`

            - `token: string`

            - `bytes: array of number`

            - `logprob: number`

            - `top_logprobs: array of object { token, bytes, logprob }`

              - `token: string`

              - `bytes: array of number`

              - `logprob: number`

          - `text: string`

            The text output from the model.

          - `type: "output_text"`

            The type of the output text. Always `output_text`.

            - `"output_text"`

        - `TextContent = object { text, type }`

          A text content.

          - `text: string`

          - `type: "text"`

            - `"text"`

        - `SummaryTextContent = object { text, type }`

          A summary text from the model.

          - `text: string`

            A summary of the reasoning output from the model so far.

          - `type: "summary_text"`

            The type of the object. Always `summary_text`.

            - `"summary_text"`

        - `ReasoningText = object { text, type }`

          Reasoning text from the model.

          - `text: string`

            The reasoning text from the model.

          - `type: "reasoning_text"`

            The type of the reasoning text. Always `reasoning_text`.

            - `"reasoning_text"`

        - `ResponseOutputRefusal = object { refusal, type }`

          A refusal from the model.

          - `refusal: string`

            The refusal explanation from the model.

          - `type: "refusal"`

            The type of the refusal. Always `refusal`.

            - `"refusal"`

        - `ResponseInputImage = object { detail, type, file_id, image_url }`

          An image input to the model. Learn about [image inputs](/docs/guides/vision).

          - `detail: "low" or "high" or "auto"`

            The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

            - `"low"`

            - `"high"`

            - `"auto"`

          - `type: "input_image"`

            The type of the input item. Always `input_image`.

            - `"input_image"`

          - `file_id: optional string`

            The ID of the file to be sent to the model.

          - `image_url: optional string`

            The URL of the image to be sent to the model. A fully qualified URL or base64 encoded image in a data URL.

        - `ComputerScreenshotContent = object { file_id, image_url, type }`

          A screenshot of a computer.

          - `file_id: string`

            The identifier of an uploaded file that contains the screenshot.

          - `image_url: string`

            The URL of the screenshot image.

          - `type: "computer_screenshot"`

            Specifies the event type. For a computer screenshot, this property is always set to `computer_screenshot`.

            - `"computer_screenshot"`

        - `ResponseInputFile = object { type, file_data, file_id, 2 more }`

          A file input to the model.

          - `type: "input_file"`

            The type of the input item. Always `input_file`.

            - `"input_file"`

          - `file_data: optional string`

            The content of the file to be sent to the model.

          - `file_id: optional string`

            The ID of the file to be sent to the model.

          - `file_url: optional string`

            The URL of the file to be sent to the model.

          - `filename: optional string`

            The name of the file to be sent to the model.

      - `role: "unknown" or "user" or "assistant" or 5 more`

        The role of the message. One of `unknown`, `user`, `assistant`, `system`, `critic`, `discriminator`, `developer`, or `tool`.

        - `"unknown"`

        - `"user"`

        - `"assistant"`

        - `"system"`

        - `"critic"`

        - `"discriminator"`

        - `"developer"`

        - `"tool"`

      - `status: "in_progress" or "completed" or "incomplete"`

        The status of item. One of `in_progress`, `completed`, or `incomplete`. Populated when items are returned via API.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

      - `type: "message"`

        The type of the message. Always set to `message`.

        - `"message"`

    - `ResponseFunctionToolCallItem = ResponseFunctionToolCall`

      A tool call to run a function. See the
      [function calling guide](/docs/guides/function-calling) for more information.

      - `id: string`

        The unique ID of the function tool call.

    - `ResponseFunctionToolCallOutputItem = object { id, call_id, output, 2 more }`

      - `id: string`

        The unique ID of the function call tool output.

      - `call_id: string`

        The unique ID of the function tool call generated by the model.

      - `output: string or array of ResponseInputText or ResponseInputImage or ResponseInputFile`

        The output from the function call generated by your code.
        Can be a string or an list of output content.

        - `StringOutput = string`

          A string of the output of the function call.

        - `OutputContentList = array of ResponseInputText or ResponseInputImage or ResponseInputFile`

          Text, image, or file output of the function call.

          - `ResponseInputText = object { text, type }`

            A text input to the model.

            - `text: string`

              The text input to the model.

            - `type: "input_text"`

              The type of the input item. Always `input_text`.

              - `"input_text"`

          - `ResponseInputImage = object { detail, type, file_id, image_url }`

            An image input to the model. Learn about [image inputs](/docs/guides/vision).

            - `detail: "low" or "high" or "auto"`

              The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

              - `"low"`

              - `"high"`

              - `"auto"`

            - `type: "input_image"`

              The type of the input item. Always `input_image`.

              - `"input_image"`

            - `file_id: optional string`

              The ID of the file to be sent to the model.

            - `image_url: optional string`

              The URL of the image to be sent to the model. A fully qualified URL or base64 encoded image in a data URL.

          - `ResponseInputFile = object { type, file_data, file_id, 2 more }`

            A file input to the model.

            - `type: "input_file"`

              The type of the input item. Always `input_file`.

              - `"input_file"`

            - `file_data: optional string`

              The content of the file to be sent to the model.

            - `file_id: optional string`

              The ID of the file to be sent to the model.

            - `file_url: optional string`

              The URL of the file to be sent to the model.

            - `filename: optional string`

              The name of the file to be sent to the model.

      - `type: "function_call_output"`

        The type of the function tool call output. Always `function_call_output`.

        - `"function_call_output"`

      - `status: optional "in_progress" or "completed" or "incomplete"`

        The status of the item. One of `in_progress`, `completed`, or
        `incomplete`. Populated when items are returned via API.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

    - `ResponseFileSearchToolCall = object { id, queries, status, 2 more }`

      The results of a file search tool call. See the
      [file search guide](/docs/guides/tools-file-search) for more information.

      - `id: string`

        The unique ID of the file search tool call.

      - `queries: array of string`

        The queries used to search for files.

      - `status: "in_progress" or "searching" or "completed" or 2 more`

        The status of the file search tool call. One of `in_progress`,
        `searching`, `incomplete` or `failed`,

        - `"in_progress"`

        - `"searching"`

        - `"completed"`

        - `"incomplete"`

        - `"failed"`

      - `type: "file_search_call"`

        The type of the file search tool call. Always `file_search_call`.

        - `"file_search_call"`

      - `results: optional array of object { attributes, file_id, filename, 2 more }`

        The results of the file search tool call.

        - `attributes: optional map[string or number or boolean]`

          Set of 16 key-value pairs that can be attached to an object. This can be
          useful for storing additional information about the object in a structured
          format, and querying for objects via API or the dashboard. Keys are strings
          with a maximum length of 64 characters. Values are strings with a maximum
          length of 512 characters, booleans, or numbers.

          - `UnionMember0 = string`

          - `UnionMember1 = number`

          - `UnionMember2 = boolean`

        - `file_id: optional string`

          The unique ID of the file.

        - `filename: optional string`

          The name of the file.

        - `score: optional number`

          The relevance score of the file - a value between 0 and 1.

        - `text: optional string`

          The text that was retrieved from the file.

    - `ResponseFunctionWebSearch = object { id, action, status, type }`

      The results of a web search tool call. See the
      [web search guide](/docs/guides/tools-web-search) for more information.

      - `id: string`

        The unique ID of the web search tool call.

      - `action: object { query, type, queries, sources }  or object { type, url }  or object { pattern, type, url }`

        An object describing the specific action taken in this web search call.
        Includes details on how the model used the web (search, open_page, find_in_page).

        - `Search = object { query, type, queries, sources }`

          Action type "search" - Performs a web search query.

          - `query: string`

            [DEPRECATED] The search query.

          - `type: "search"`

            The action type.

            - `"search"`

          - `queries: optional array of string`

            The search queries.

          - `sources: optional array of object { type, url }`

            The sources used in the search.

            - `type: "url"`

              The type of source. Always `url`.

              - `"url"`

            - `url: string`

              The URL of the source.

        - `OpenPage = object { type, url }`

          Action type "open_page" - Opens a specific URL from search results.

          - `type: "open_page"`

            The action type.

            - `"open_page"`

          - `url: optional string`

            The URL opened by the model.

        - `FindInPage = object { pattern, type, url }`

          Action type "find_in_page": Searches for a pattern within a loaded page.

          - `pattern: string`

            The pattern or text to search for within the page.

          - `type: "find_in_page"`

            The action type.

            - `"find_in_page"`

          - `url: string`

            The URL of the page searched for the pattern.

      - `status: "in_progress" or "searching" or "completed" or "failed"`

        The status of the web search tool call.

        - `"in_progress"`

        - `"searching"`

        - `"completed"`

        - `"failed"`

      - `type: "web_search_call"`

        The type of the web search tool call. Always `web_search_call`.

        - `"web_search_call"`

    - `ImageGenerationCall = object { id, result, status, type }`

      An image generation request made by the model.

      - `id: string`

        The unique ID of the image generation call.

      - `result: string`

        The generated image encoded in base64.

      - `status: "in_progress" or "completed" or "generating" or "failed"`

        The status of the image generation call.

        - `"in_progress"`

        - `"completed"`

        - `"generating"`

        - `"failed"`

      - `type: "image_generation_call"`

        The type of the image generation call. Always `image_generation_call`.

        - `"image_generation_call"`

    - `ResponseComputerToolCall = object { id, action, call_id, 3 more }`

      A tool call to a computer use tool. See the
      [computer use guide](/docs/guides/tools-computer-use) for more information.

      - `id: string`

        The unique ID of the computer call.

      - `action: object { button, type, x, y }  or object { type, x, y }  or object { path, type }  or 6 more`

        A click action.

        - `Click = object { button, type, x, y }`

          A click action.

          - `button: "left" or "right" or "wheel" or 2 more`

            Indicates which mouse button was pressed during the click. One of `left`, `right`, `wheel`, `back`, or `forward`.

            - `"left"`

            - `"right"`

            - `"wheel"`

            - `"back"`

            - `"forward"`

          - `type: "click"`

            Specifies the event type. For a click action, this property is always `click`.

            - `"click"`

          - `x: number`

            The x-coordinate where the click occurred.

          - `y: number`

            The y-coordinate where the click occurred.

        - `DoubleClick = object { type, x, y }`

          A double click action.

          - `type: "double_click"`

            Specifies the event type. For a double click action, this property is always set to `double_click`.

            - `"double_click"`

          - `x: number`

            The x-coordinate where the double click occurred.

          - `y: number`

            The y-coordinate where the double click occurred.

        - `Drag = object { path, type }`

          A drag action.

          - `path: array of object { x, y }`

            An array of coordinates representing the path of the drag action. Coordinates will appear as an array of objects, eg

            ```
            [
              { x: 100, y: 200 },
              { x: 200, y: 300 }
            ]
            ```

            - `x: number`

              The x-coordinate.

            - `y: number`

              The y-coordinate.

          - `type: "drag"`

            Specifies the event type. For a drag action, this property is always set to `drag`.

            - `"drag"`

        - `Keypress = object { keys, type }`

          A collection of keypresses the model would like to perform.

          - `keys: array of string`

            The combination of keys the model is requesting to be pressed. This is an array of strings, each representing a key.

          - `type: "keypress"`

            Specifies the event type. For a keypress action, this property is always set to `keypress`.

            - `"keypress"`

        - `Move = object { type, x, y }`

          A mouse move action.

          - `type: "move"`

            Specifies the event type. For a move action, this property is always set to `move`.

            - `"move"`

          - `x: number`

            The x-coordinate to move to.

          - `y: number`

            The y-coordinate to move to.

        - `Screenshot = object { type }`

          A screenshot action.

          - `type: "screenshot"`

            Specifies the event type. For a screenshot action, this property is always set to `screenshot`.

            - `"screenshot"`

        - `Scroll = object { scroll_x, scroll_y, type, 2 more }`

          A scroll action.

          - `scroll_x: number`

            The horizontal scroll distance.

          - `scroll_y: number`

            The vertical scroll distance.

          - `type: "scroll"`

            Specifies the event type. For a scroll action, this property is always set to `scroll`.

            - `"scroll"`

          - `x: number`

            The x-coordinate where the scroll occurred.

          - `y: number`

            The y-coordinate where the scroll occurred.

        - `Type = object { text, type }`

          An action to type in text.

          - `text: string`

            The text to type.

          - `type: "type"`

            Specifies the event type. For a type action, this property is always set to `type`.

            - `"type"`

        - `Wait = object { type }`

          A wait action.

          - `type: "wait"`

            Specifies the event type. For a wait action, this property is always set to `wait`.

            - `"wait"`

      - `call_id: string`

        An identifier used when responding to the tool call with output.

      - `pending_safety_checks: array of object { id, code, message }`

        The pending safety checks for the computer call.

        - `id: string`

          The ID of the pending safety check.

        - `code: optional string`

          The type of the pending safety check.

        - `message: optional string`

          Details about the pending safety check.

      - `status: "in_progress" or "completed" or "incomplete"`

        The status of the item. One of `in_progress`, `completed`, or
        `incomplete`. Populated when items are returned via API.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

      - `type: "computer_call"`

        The type of the computer call. Always `computer_call`.

        - `"computer_call"`

    - `ResponseComputerToolCallOutputItem = object { id, call_id, output, 3 more }`

      - `id: string`

        The unique ID of the computer call tool output.

      - `call_id: string`

        The ID of the computer tool call that produced the output.

      - `output: ResponseComputerToolCallOutputScreenshot`

        A computer screenshot image used with the computer use tool.

        - `type: "computer_screenshot"`

          Specifies the event type. For a computer screenshot, this property is
          always set to `computer_screenshot`.

          - `"computer_screenshot"`

        - `file_id: optional string`

          The identifier of an uploaded file that contains the screenshot.

        - `image_url: optional string`

          The URL of the screenshot image.

      - `type: "computer_call_output"`

        The type of the computer tool call output. Always `computer_call_output`.

        - `"computer_call_output"`

      - `acknowledged_safety_checks: optional array of object { id, code, message }`

        The safety checks reported by the API that have been acknowledged by the
        developer.

        - `id: string`

          The ID of the pending safety check.

        - `code: optional string`

          The type of the pending safety check.

        - `message: optional string`

          Details about the pending safety check.

      - `status: optional "in_progress" or "completed" or "incomplete"`

        The status of the message input. One of `in_progress`, `completed`, or
        `incomplete`. Populated when input items are returned via API.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

    - `ResponseReasoningItem = object { id, summary, type, 3 more }`

      A description of the chain of thought used by a reasoning model while generating
      a response. Be sure to include these items in your `input` to the Responses API
      for subsequent turns of a conversation if you are manually
      [managing context](/docs/guides/conversation-state).

      - `id: string`

        The unique identifier of the reasoning content.

      - `summary: array of SummaryTextContent`

        Reasoning summary content.

        - `text: string`

          A summary of the reasoning output from the model so far.

        - `type: "summary_text"`

          The type of the object. Always `summary_text`.

          - `"summary_text"`

      - `type: "reasoning"`

        The type of the object. Always `reasoning`.

        - `"reasoning"`

      - `content: optional array of object { text, type }`

        Reasoning text content.

        - `text: string`

          The reasoning text from the model.

        - `type: "reasoning_text"`

          The type of the reasoning text. Always `reasoning_text`.

          - `"reasoning_text"`

      - `encrypted_content: optional string`

        The encrypted content of the reasoning item - populated when a response is
        generated with `reasoning.encrypted_content` in the `include` parameter.

      - `status: optional "in_progress" or "completed" or "incomplete"`

        The status of the item. One of `in_progress`, `completed`, or
        `incomplete`. Populated when items are returned via API.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

    - `ResponseCodeInterpreterToolCall = object { id, code, container_id, 3 more }`

      A tool call to run code.

      - `id: string`

        The unique ID of the code interpreter tool call.

      - `code: string`

        The code to run, or null if not available.

      - `container_id: string`

        The ID of the container used to run the code.

      - `outputs: array of object { logs, type }  or object { type, url }`

        The outputs generated by the code interpreter, such as logs or images.
        Can be null if no outputs are available.

        - `Logs = object { logs, type }`

          The logs output from the code interpreter.

          - `logs: string`

            The logs output from the code interpreter.

          - `type: "logs"`

            The type of the output. Always `logs`.

            - `"logs"`

        - `Image = object { type, url }`

          The image output from the code interpreter.

          - `type: "image"`

            The type of the output. Always `image`.

            - `"image"`

          - `url: string`

            The URL of the image output from the code interpreter.

      - `status: "in_progress" or "completed" or "incomplete" or 2 more`

        The status of the code interpreter tool call. Valid values are `in_progress`, `completed`, `incomplete`, `interpreting`, and `failed`.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

        - `"interpreting"`

        - `"failed"`

      - `type: "code_interpreter_call"`

        The type of the code interpreter tool call. Always `code_interpreter_call`.

        - `"code_interpreter_call"`

    - `LocalShellCall = object { id, action, call_id, 2 more }`

      A tool call to run a command on the local shell.

      - `id: string`

        The unique ID of the local shell call.

      - `action: object { command, env, type, 3 more }`

        Execute a shell command on the server.

        - `command: array of string`

          The command to run.

        - `env: map[string]`

          Environment variables to set for the command.

        - `type: "exec"`

          The type of the local shell action. Always `exec`.

          - `"exec"`

        - `timeout_ms: optional number`

          Optional timeout in milliseconds for the command.

        - `user: optional string`

          Optional user to run the command as.

        - `working_directory: optional string`

          Optional working directory to run the command in.

      - `call_id: string`

        The unique ID of the local shell tool call generated by the model.

      - `status: "in_progress" or "completed" or "incomplete"`

        The status of the local shell call.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

      - `type: "local_shell_call"`

        The type of the local shell call. Always `local_shell_call`.

        - `"local_shell_call"`

    - `LocalShellCallOutput = object { id, output, type, status }`

      The output of a local shell tool call.

      - `id: string`

        The unique ID of the local shell tool call generated by the model.

      - `output: string`

        A JSON string of the output of the local shell tool call.

      - `type: "local_shell_call_output"`

        The type of the local shell tool call output. Always `local_shell_call_output`.

        - `"local_shell_call_output"`

      - `status: optional "in_progress" or "completed" or "incomplete"`

        The status of the item. One of `in_progress`, `completed`, or `incomplete`.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

    - `ResponseFunctionShellToolCall = object { id, action, call_id, 4 more }`

      A tool call that executes one or more shell commands in a managed environment.

      - `id: string`

        The unique ID of the shell tool call. Populated when this item is returned via API.

      - `action: object { commands, max_output_length, timeout_ms }`

        The shell commands and limits that describe how to run the tool call.

        - `commands: array of string`

        - `max_output_length: number`

          Optional maximum number of characters to return from each command.

        - `timeout_ms: number`

          Optional timeout in milliseconds for the commands.

      - `call_id: string`

        The unique ID of the shell tool call generated by the model.

      - `environment: ResponseLocalEnvironment or ResponseContainerReference`

        Represents the use of a local environment to perform shell actions.

        - `ResponseLocalEnvironment = object { type }`

          Represents the use of a local environment to perform shell actions.

          - `type: "local"`

            The environment type. Always `local`.

            - `"local"`

        - `ResponseContainerReference = object { container_id, type }`

          Represents a container created with /v1/containers.

          - `container_id: string`

          - `type: "container_reference"`

            The environment type. Always `container_reference`.

            - `"container_reference"`

      - `status: "in_progress" or "completed" or "incomplete"`

        The status of the shell call. One of `in_progress`, `completed`, or `incomplete`.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

      - `type: "shell_call"`

        The type of the item. Always `shell_call`.

        - `"shell_call"`

      - `created_by: optional string`

        The ID of the entity that created this tool call.

    - `ResponseFunctionShellToolCallOutput = object { id, call_id, max_output_length, 4 more }`

      The output of a shell tool call that was emitted.

      - `id: string`

        The unique ID of the shell call output. Populated when this item is returned via API.

      - `call_id: string`

        The unique ID of the shell tool call generated by the model.

      - `max_output_length: number`

        The maximum length of the shell command output. This is generated by the model and should be passed back with the raw output.

      - `output: array of object { outcome, stderr, stdout, created_by }`

        An array of shell call output contents

        - `outcome: object { type }  or object { exit_code, type }`

          Represents either an exit outcome (with an exit code) or a timeout outcome for a shell call output chunk.

          - `Timeout = object { type }`

            Indicates that the shell call exceeded its configured time limit.

            - `type: "timeout"`

              The outcome type. Always `timeout`.

              - `"timeout"`

          - `Exit = object { exit_code, type }`

            Indicates that the shell commands finished and returned an exit code.

            - `exit_code: number`

              Exit code from the shell process.

            - `type: "exit"`

              The outcome type. Always `exit`.

              - `"exit"`

        - `stderr: string`

          The standard error output that was captured.

        - `stdout: string`

          The standard output that was captured.

        - `created_by: optional string`

          The identifier of the actor that created the item.

      - `status: "in_progress" or "completed" or "incomplete"`

        The status of the shell call output. One of `in_progress`, `completed`, or `incomplete`.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

      - `type: "shell_call_output"`

        The type of the shell call output. Always `shell_call_output`.

        - `"shell_call_output"`

      - `created_by: optional string`

        The identifier of the actor that created the item.

    - `ResponseApplyPatchToolCall = object { id, call_id, operation, 3 more }`

      A tool call that applies file diffs by creating, deleting, or updating files.

      - `id: string`

        The unique ID of the apply patch tool call. Populated when this item is returned via API.

      - `call_id: string`

        The unique ID of the apply patch tool call generated by the model.

      - `operation: object { diff, path, type }  or object { path, type }  or object { diff, path, type }`

        One of the create_file, delete_file, or update_file operations applied via apply_patch.

        - `CreateFile = object { diff, path, type }`

          Instruction describing how to create a file via the apply_patch tool.

          - `diff: string`

            Diff to apply.

          - `path: string`

            Path of the file to create.

          - `type: "create_file"`

            Create a new file with the provided diff.

            - `"create_file"`

        - `DeleteFile = object { path, type }`

          Instruction describing how to delete a file via the apply_patch tool.

          - `path: string`

            Path of the file to delete.

          - `type: "delete_file"`

            Delete the specified file.

            - `"delete_file"`

        - `UpdateFile = object { diff, path, type }`

          Instruction describing how to update a file via the apply_patch tool.

          - `diff: string`

            Diff to apply.

          - `path: string`

            Path of the file to update.

          - `type: "update_file"`

            Update an existing file with the provided diff.

            - `"update_file"`

      - `status: "in_progress" or "completed"`

        The status of the apply patch tool call. One of `in_progress` or `completed`.

        - `"in_progress"`

        - `"completed"`

      - `type: "apply_patch_call"`

        The type of the item. Always `apply_patch_call`.

        - `"apply_patch_call"`

      - `created_by: optional string`

        The ID of the entity that created this tool call.

    - `ResponseApplyPatchToolCallOutput = object { id, call_id, status, 3 more }`

      The output emitted by an apply patch tool call.

      - `id: string`

        The unique ID of the apply patch tool call output. Populated when this item is returned via API.

      - `call_id: string`

        The unique ID of the apply patch tool call generated by the model.

      - `status: "completed" or "failed"`

        The status of the apply patch tool call output. One of `completed` or `failed`.

        - `"completed"`

        - `"failed"`

      - `type: "apply_patch_call_output"`

        The type of the item. Always `apply_patch_call_output`.

        - `"apply_patch_call_output"`

      - `created_by: optional string`

        The ID of the entity that created this tool call output.

      - `output: optional string`

        Optional textual output returned by the apply patch tool.

    - `McpListTools = object { id, server_label, tools, 2 more }`

      A list of tools available on an MCP server.

      - `id: string`

        The unique ID of the list.

      - `server_label: string`

        The label of the MCP server.

      - `tools: array of object { input_schema, name, annotations, description }`

        The tools available on the server.

        - `input_schema: unknown`

          The JSON schema describing the tool's input.

        - `name: string`

          The name of the tool.

        - `annotations: optional unknown`

          Additional annotations about the tool.

        - `description: optional string`

          The description of the tool.

      - `type: "mcp_list_tools"`

        The type of the item. Always `mcp_list_tools`.

        - `"mcp_list_tools"`

      - `error: optional string`

        Error message if the server could not list tools.

    - `McpApprovalRequest = object { id, arguments, name, 2 more }`

      A request for human approval of a tool invocation.

      - `id: string`

        The unique ID of the approval request.

      - `arguments: string`

        A JSON string of arguments for the tool.

      - `name: string`

        The name of the tool to run.

      - `server_label: string`

        The label of the MCP server making the request.

      - `type: "mcp_approval_request"`

        The type of the item. Always `mcp_approval_request`.

        - `"mcp_approval_request"`

    - `McpApprovalResponse = object { id, approval_request_id, approve, 2 more }`

      A response to an MCP approval request.

      - `id: string`

        The unique ID of the approval response

      - `approval_request_id: string`

        The ID of the approval request being answered.

      - `approve: boolean`

        Whether the request was approved.

      - `type: "mcp_approval_response"`

        The type of the item. Always `mcp_approval_response`.

        - `"mcp_approval_response"`

      - `reason: optional string`

        Optional reason for the decision.

    - `McpCall = object { id, arguments, name, 6 more }`

      An invocation of a tool on an MCP server.

      - `id: string`

        The unique ID of the tool call.

      - `arguments: string`

        A JSON string of the arguments passed to the tool.

      - `name: string`

        The name of the tool that was run.

      - `server_label: string`

        The label of the MCP server running the tool.

      - `type: "mcp_call"`

        The type of the item. Always `mcp_call`.

        - `"mcp_call"`

      - `approval_request_id: optional string`

        Unique identifier for the MCP tool call approval request.
        Include this value in a subsequent `mcp_approval_response` input to approve or reject the corresponding tool call.

      - `error: optional string`

        The error from the tool call, if any.

      - `output: optional string`

        The output from the tool call.

      - `status: optional "in_progress" or "completed" or "incomplete" or 2 more`

        The status of the tool call. One of `in_progress`, `completed`, `incomplete`, `calling`, or `failed`.

        - `"in_progress"`

        - `"completed"`

        - `"incomplete"`

        - `"calling"`

        - `"failed"`

    - `ResponseCustomToolCall = object { call_id, input, name, 2 more }`

      A call to a custom tool created by the model.

      - `call_id: string`

        An identifier used to map this custom tool call to a tool call output.

      - `input: string`

        The input for the custom tool call generated by the model.

      - `name: string`

        The name of the custom tool being called.

      - `type: "custom_tool_call"`

        The type of the custom tool call. Always `custom_tool_call`.

        - `"custom_tool_call"`

      - `id: optional string`

        The unique ID of the custom tool call in the OpenAI platform.

    - `ResponseCustomToolCallOutput = object { call_id, output, type, id }`

      The output of a custom tool call from your code, being sent back to the model.

      - `call_id: string`

        The call ID, used to map this custom tool call output to a custom tool call.

      - `output: string or array of ResponseInputText or ResponseInputImage or ResponseInputFile`

        The output from the custom tool call generated by your code.
        Can be a string or an list of output content.

        - `StringOutput = string`

          A string of the output of the custom tool call.

        - `OutputContentList = array of ResponseInputText or ResponseInputImage or ResponseInputFile`

          Text, image, or file output of the custom tool call.

          - `ResponseInputText = object { text, type }`

            A text input to the model.

            - `text: string`

              The text input to the model.

            - `type: "input_text"`

              The type of the input item. Always `input_text`.

              - `"input_text"`

          - `ResponseInputImage = object { detail, type, file_id, image_url }`

            An image input to the model. Learn about [image inputs](/docs/guides/vision).

            - `detail: "low" or "high" or "auto"`

              The detail level of the image to be sent to the model. One of `high`, `low`, or `auto`. Defaults to `auto`.

              - `"low"`

              - `"high"`

              - `"auto"`

            - `type: "input_image"`

              The type of the input item. Always `input_image`.

              - `"input_image"`

            - `file_id: optional string`

              The ID of the file to be sent to the model.

            - `image_url: optional string`

              The URL of the image to be sent to the model. A fully qualified URL or base64 encoded image in a data URL.

          - `ResponseInputFile = object { type, file_data, file_id, 2 more }`

            A file input to the model.

            - `type: "input_file"`

              The type of the input item. Always `input_file`.

              - `"input_file"`

            - `file_data: optional string`

              The content of the file to be sent to the model.

            - `file_id: optional string`

              The ID of the file to be sent to the model.

            - `file_url: optional string`

              The URL of the file to be sent to the model.

            - `filename: optional string`

              The name of the file to be sent to the model.

      - `type: "custom_tool_call_output"`

        The type of the custom tool call output. Always `custom_tool_call_output`.

        - `"custom_tool_call_output"`

      - `id: optional string`

        The unique ID of the custom tool call output in the OpenAI platform.

  - `first_id: string`

    The ID of the first item in the list.

  - `has_more: boolean`

    Whether there are more items available.

  - `last_id: string`

    The ID of the last item in the list.

  - `object: "list"`

    The type of object returned, must be `list`.

    - `"list"`

### Example

```http
curl https://api.openai.com/v1/conversations/$CONVERSATION_ID/items \
    -H "Authorization: Bearer $OPENAI_API_KEY"
```

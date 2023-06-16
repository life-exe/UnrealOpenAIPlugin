
# Complete Unreal Engine plugin for the OpenAI API

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/logo.png)

This plugin is a comprehensive Unreal Engine wrapper for the OpenAI API. It supports all OpenAI endpoints, including:
 - [Models](https://platform.openai.com/docs/api-reference/models)
 - [Completions](https://platform.openai.com/docs/api-reference/completions)
 - [Chat](https://platform.openai.com/docs/api-reference/chat)
 - [Edits](https://platform.openai.com/docs/api-reference/edits)
 - [Images](https://platform.openai.com/docs/api-reference/images)
 - [Embeddings](https://platform.openai.com/docs/api-reference/embeddings)
 - [Audio](https://platform.openai.com/docs/api-reference/audio)
 - [Files](https://platform.openai.com/docs/api-reference/files)
 - [Fine-tunes](https://platform.openai.com/docs/api-reference/fine-tunes)
 - [Moderations](https://platform.openai.com/docs/api-reference/moderations)
 
All requests are available in both C++ and Blueprints:
```cpp
void AAPIOverview::CreateImage()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->SetLogEnabled(false);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageCompleted().AddLambda(
        [](const FImageResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(Response.Data, [&](const FString& Data) { OutputString.Append(Data).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    FOpenAIImage Image;
    Image.Prompt = "Tiger is eating pizza";
    Image.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_512x512);
    Image.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);
    Image.N = 2;

    Provider->CreateImage(Image, Auth);
}
```

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/bp.png)

## Supported Unreal Engine Versions

 - Unreal Engine 5.2

# Installation

## C++

1. Create a new C++ project in Unreal Engine.
2. Create a `Plugins` folder at the root of your project.
3. Add the OpenAI plugin to your Unreal Engine `Plugins` folder.
4. The easiest way to add plugins is by adding the current plugin as a submodule. Use the following command in the root folder of your project:
```
git submodule add https://github.com/life-exe/UnrealOpenAIPlugin Plugins/OpenAI
```
5. Alternatively, you can download the source code from the current repository and copy the files to your `Plugins` folder.
6. When complete, your Unreal Engine project structure should resemble the following:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/1.png)
![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/2.png)

7. [Do the authentication steps](#authentication)
8. Build your project and run Editor.
9. In the Editor, navigate to `Edit->Plugins`. Find and activate the `OpenAI` plugin.
10. Restart the editor.
11. Start using the plugin.

## Blueprints

1. Create a blueprint project.
2. Create a `Plugins` folder in the root of your project.
3. Download precompiled plugin from the [Releases](https://github.com/life-exe/UnrealOpenAIPlugin/releases/).
4. Unzip plugin to the `Plugins` folder, specifically to `Plugins/OpenAI`.
5. [Do the authentication steps](#authentication)
6. Run the Editor `<YourProjectName>.uproject`
7. In the Editor, navigate to `Edit->Plugins`. Find and activate the `OpenAI` plugin.
8. Restart the editor.
9. Start using the plugin.

## Authentication

1. Create an [OpenAI account](https://platform.openai.com/account)
2. [Generate](https://platform.openai.com/account/api-keys) and store your API Key:
`sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

3. Record your [Organization ID](https://platform.openai.com/account/org-settings):
`org-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`
4. At the root of your Unreal Engine project, create a file named `OpenAIAuth.ini` with the following content:
```
APIKey=sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OrganizationID=org-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```
5. Once completed, your Unreal Engine project structure might look like this:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/3.png)
![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/4.png)

6. Actually you can left `OrganizationID` empty. It doesn't affect on auth.

Finally, compile your project and launch the Unreal Editor.

# Examples of Usage

## Widget example

This provides a basic example of request usage with Blueprints.

1. Make sure that plugin content activated:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/settings.png)

2. Open the plugin content folder.
3. Open the level:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/map.png)

4. Run the game.
5. Test the API with the widget examples:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/api.png)

Feel free to modify requests in the widget code to test the API with different parameters:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/widgets.png)

## Unreal Editor Chat-GPT

This is the Chat-GPT implementation with token streaming support.

1. Open the plugin content folder.
2. Navigate to the `EditorWidgets` folder.
2. Right-click on the ChatGPT editor utility widget:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/utility.png)

3. Start chatting:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/chatgpt1.png)
<img src="https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/chatgpt2.gif" width=100%>

4. You can save the chat history to a file using the `Dump` button. The specific location where the history is saved can be checked in the chat or logs:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/log.png)


## Blueprint Nodes Overview

This blueprint demonstrates all available nodes.

1. Open the plugin content folder:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/api1.png)

2. Open `BP_APIOverview` blueprint.
3. Check the available nodes: functions and structs:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/api2.png)

There are also several nodes that could be useful in other projects. Feel free to copy and paste the plugin code if you need them:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/helpers.png)

## C++ example

1.  Open `Plugins\OpenAI\Source\OpenAI\Private\Sample\APIOverview.cpp` actor
2.  Uncomment the function that executes the request you want to test. Navigate to the function and adjust all the request parameters as needed.
3.  Compile your project and open Unreal Editor.
4.  Drop the `AAPIOverview` actor into any level.
5.  Run the game.
6.  Check the Output Log.

## Plugin structure:

 - `OpenAIModule` - core classes.
 - `OpenAIEditorModule` - Chat-GPT editor utility widget.
 - `OpenAITestRunnerModule` - unit tests.

## Documentation
I highly recommend reading the OpenAI documentation for a better understanding of how all requests work:

 - [Overview](https://platform.openai.com/docs/introduction/overview)
 - [API-Reference](https://platform.openai.com/docs/api-reference)
 
 In addition plugin code has its own documentation generated with help of Doxygen:
 
 - [Documentation](https://life-exe.github.io/UnrealOpenAIPlugin)

You can generate the plugin documentation locally by following these steps:
 1. Update all submodules. You can use the batch script located at the root of the plugin folder: `update_submodules.bat`
 2. Ensure that [Doxygen](https://www.doxygen.nl) is installed on your system.
 3. Generate the documentation using the batch script at the root of the plugin folder: `generate_docs.bat`
 4. After the generation process, the documentation will be available in the `Documentation` folder at the root of the plugin: `Documentation\html\index.html`

## Tests

Unit tests are available in the `OpenAITestRunnerModule`. You can initiate them using the `Session Frontend`:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/unittests.png)

## Packaging

1. Verify that your .uproject file contains the following:

```cpp
{
    "Name": "OpenAI",
    "Enabled": true
}
```

2. Package the project as you always do (kudos if you use a CI solution like Jenkins).
3. You can handle authentication in your project as you see fit. However, if you choose to use a file-based method, such as in a plugin, please remember to include the `OpenAIAuth.ini` file in your packaged folder `Build/Windows/<YourProjectName>/OpenAIAuth.ini`
If you are having problems loading the file, please check the logs to see where it might be located:

```
LogOpenAIFuncLib: Error: Failed loading file: C:/_Projects/UE5/OpenAICpp/Build/Windows/OpenAICpp/OpenAIAuth.ini
```

## Limitations

 - Chat GPT-4 models are not available for everyone via the API. You need to [request access](https://openai.com/waitlist/gpt-4-api) for it:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/chatgpterror.png)

 - OpenAI hosts a variety of different models. Please [check the models](https://platform.openai.com/docs/models/model-endpoint-compatibility) that are compatible with the particular request.

## Miscellaneous
I would appreciate bug reports and pull-request fixes.

Enjoy! 🚀️

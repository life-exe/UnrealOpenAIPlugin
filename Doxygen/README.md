
# Complete Unreal Engine plugin for the OpenAI API

This is an unofficial community-maintained library.

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/logo.png)

This plugin is a comprehensive Unreal Engine wrapper for the OpenAI API. It supports all OpenAI endpoints, including:
 - [Models](https://platform.openai.com/docs/api-reference/models)
 - [Completions](https://platform.openai.com/docs/api-reference/completions)
 - [Chat](https://platform.openai.com/docs/api-reference/chat)
 - [Images (GPT-Image-1, DALL·E 3, DALL·E 2)](https://platform.openai.com/docs/api-reference/images)
 - [Vision](https://platform.openai.com/docs/guides/vision)
 - [Embeddings](https://platform.openai.com/docs/api-reference/embeddings)
 - [Batch](https://platform.openai.com/docs/api-reference/batch)
 - [Speech](https://platform.openai.com/docs/api-reference/audio/createSpeech)
 - [Audio](https://platform.openai.com/docs/api-reference/audio)
 - [Files](https://platform.openai.com/docs/api-reference/files)
 - [Uploads](https://platform.openai.com/docs/api-reference/uploads)
 - [Moderations](https://platform.openai.com/docs/api-reference/moderations)
 - [Fine-tuning](https://platform.openai.com/docs/api-reference/fine-tuning)
 - [Functions](https://platform.openai.com/docs/guides/function-calling)
 - [Debugging requests](https://platform.openai.com/docs/api-reference/debugging-requests)

All requests are available in both C++ and Blueprints:
```cpp
void AAPIOverview::CreateImage()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageCompleted().AddLambda(
        [](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            auto* ArtTexture = UImageFuncLib::Texture2DFromBytes(Response.Data[0].B64_JSON);
            UE_LOGFMT(LogAPIOverview, Display, "{0}", Response.Data[0].B64_JSON);
        });

    FOpenAIImage OpenAIImage;
    OpenAIImage.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::GPT_Image_1);
    OpenAIImage.N = 1;
    OpenAIImage.Prompt = "Bear with beard drinking beer";
    OpenAIImage.Size = UOpenAIFuncLib::OpenAIImageSizeGptImage1ToString(EImageSizeGptImage1::Size_1024x1024);
    OpenAIImage.Background.Set(UOpenAIFuncLib::OpenAIImageBackgroundToString(EOpenAIImageBackground::Transparent));
    OpenAIImage.Moderation.Set(UOpenAIFuncLib::OpenAIImageModerationToString(EOpenAIImageModeration::Low));
    OpenAIImage.Quality.Set(UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality::Low));
    OpenAIImage.Output_Format.Set(UOpenAIFuncLib::OpenAIImageOutputFormatToString(EOpenAIImageOutputFormat::Png));

    Provider->CreateImage(Image, Auth);
}
```

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/bp.png)

## Supported Unreal Engine Versions

 - Unreal Engine 5.6 (master), 5.5, 5.4, 5.3, 5.2

## Tutorials
 - [Plugin overview and usage example](https://youtu.be/SkiWQXHjk30) [English subtitles].
 - [How to use functions](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Docs/Services.md).
 - [DALLE 3 | Text To Speech | Vision](https://youtu.be/l4hcCbAceXs) [English subtitles].

## Updates
 - [Add gpt-image-1 model, image API update](https://openai.com/index/image-generation-api)

 ![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/gpt_image_1.png)

 ![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/gpt_image_2.png)

 - [Add gpt-4o models](https://openai.com/index/hello-gpt-4o)
 - [Vision & Text To Speech Demo](https://life-exe.itch.io/openai-vision-demo)

 - [DALL·E 3](https://openai.com/dall-e-3) has been added. [Details](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Docs/Dalle3.md)

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/dalle3_1.png)

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/dalle3_2.png)

 - [Text to speech](https://platform.openai.com/docs/guides/text-to-speech) has been added. [Details](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Docs/TTS.md)

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/speech.png)

 - [Vision](https://platform.openai.com/docs/guides/vision) has been added. [Details](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Docs/Vision.md)

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/vision_1.png)

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/vision_2.png)

 - [Functions](https://platform.openai.com/docs/guides/function-calling) support added! You can check the tutorial [via link](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Docs/Services.md)

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/services.png)

# Installation

## FAB link

- [Complete OpenAI API plugin](https://www.fab.com/listings/8114dbc6-a497-438c-9b26-552534ab68b0)

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

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/1.png)
![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/2.png)

7. [Do the authentication steps](#authentication)
8. Generate Visual Studio project files.
9. Add `OpenAI` plugin to your `YourProject.Build.cs`:
```cs
public class YourProject : ModuleRules
{
    public YourProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OpenAI" });

        PublicIncludePaths.AddRange(new string[] { "YourProject" });
    }
}
```
10. Build your project and run Editor.
11. In the Editor, navigate to `Edit->Plugins`. Find and activate the `OpenAI` plugin.
12. Restart the editor.
13. Make sure that your `.uproject` file contains plugin:
```json
{
    "FileVersion": 3,
    "EngineAssociation": "5.x",
    "Category": "",
    "Description": "",
    "Modules": [
        {
            "Name": "ProjectName",
            "Type": "Runtime",
            "LoadingPhase": "Default",
            "AdditionalDependencies": [
                "Engine"
            ]
        }
    ],
    "Plugins": [
        {
            "Name": "OpenAI",
            "Enabled": true,
            "MarketplaceURL": "com.epicgames.launcher://ue/marketplace/product/97eaf1e101ab4f29b5acbf7dacbd4d16"
        }
    ]
}
```
14. Start using the plugin.

## Blueprints

1. Create a blueprint project.
2. Create a `Plugins` folder in the root of your project.
3. Download precompiled plugin from the [Releases](https://github.com/life-exe/UnrealOpenAIPlugin/releases/).
4. Unzip plugin to the `Plugins` folder, specifically to `Plugins/OpenAI`.
5. [Do the authentication steps](#authentication)
6. Run the Editor `<YourProjectName>.uproject`
7. In the Editor, navigate to `Edit->Plugins`. Find and activate the `OpenAI` plugin.
8. Restart the editor.
9. Make sure that your `.uproject` file contains plugin:
```json
{
    "FileVersion": 3,
    "EngineAssociation": "5.x",
    "Category": "",
    "Description": "",
    "Modules": [
        {
            "Name": "ProjectName",
            "Type": "Runtime",
            "LoadingPhase": "Default",
            "AdditionalDependencies": [
                "Engine"
            ]
        }
    ],
    "Plugins": [
        {
            "Name": "OpenAI",
            "Enabled": true,
            "MarketplaceURL": "com.epicgames.launcher://ue/marketplace/product/97eaf1e101ab4f29b5acbf7dacbd4d16"
        }
    ]
}
```
10. Start using the plugin.

## Authentication

1. Create an [OpenAI account](https://platform.openai.com/account)
2. [Generate](https://platform.openai.com/account/api-keys) and store your API Key:
`sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

3. Record your [Organization ID](https://platform.openai.com/account/org-settings):
`org-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

4. Create a [Project ID](https://platform.openai.com/settings/organization/general):
`proj_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

5. At the root of your Unreal Engine project, create a file named `OpenAIAuth.ini` (you can use `setup_auth_files.bat` in the plugin root folder. It will copy `.ini` files from [template folder](https://github.com/life-exe/UnrealOpenAIPlugin/tree/master/Templates)) with the following content:
```
APIKey=sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
OrganizationID=org-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
ProjectID=proj_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```
6. Once completed, your Unreal Engine project structure might look like this:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/3.png)
![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/4.png)

7. Actually you can left `OrganizationID` empty. It doesn't affect on auth.

8. If you haven't created a `ProjectID` don't put ```ProjectID=proj_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx``` in the file.

9. [Do the authentication steps](#how-to-set-up-default-services) for services if you need them.

Finally, compile your project and launch the Unreal Editor.

# Examples of Usage

## Widget example

This provides a basic example of request usage with Blueprints.

1. Make sure that plugin content activated:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/settings.png)

2. Open the plugin content folder.
3. Open the level:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/map.png)

4. Run the game.
5. Test the API with the widget examples:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/api.png)

Feel free to modify requests in the widget code to test the API with different parameters:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/widgets.png)

## Unreal Editor Chat-GPT

This is the Chat-GPT implementation with token streaming support.

1. Open the plugin content folder.
2. Navigate to the `EditorWidgets` folder.
2. Right-click on the ChatGPT editor utility widget:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/utility.png)

3. Start chatting:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/chatgpt1.png)
<img src="https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/chatgpt2.gif" width=100%>

4. You can save the chat history to a file using the `Dump` button. The specific location where the history is saved can be checked in the chat or logs:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/log.png)

## Unreal Editor Chat-GPT with OpenAI Functions

You can build your own services (addons, plugins) on top of ChatGPT:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/services.png)

Currently the plugin has two services available by default:
 - Weather

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/chatgpt3.png)

 - News

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/chatgpt4.png)

### How to set up default services

I use free API for services. But you need to create accounts to get API keys.

1. Weather serivces: https://weatherstack.com. Once your account is created, you will find `API Access Key` on your dashboard.
2. News service: https://newsapi.org. Once your account is created, you will find `API Key` on your dashboard.
3. In the root of your Unreal Engine project, create a file called `OnlineServicesAuth.ini` (you can use `setup_auth_files.bat` in the plugin root folder. It will copy `.ini` files from [template folder](https://github.com/life-exe/UnrealOpenAIPlugin/tree/master/Templates)) with the following content:
```
WeatherstackAccessKey=dbe5fcdd54f2196d2cdc5194cf5
NewsApiOrgApiKey=1dec1793ed3940e880cc93b4087fcf96
```
4. Enter your APi keys for each service.
5. Compile your project, launch the Unreal Editor, launch ChatGPT editor widget, select the services that you want to use.
6. Ask for the weather or latest news somewhere. Example prompts:
> What is the weather like in Oslo?

> What is the latest news from Microsoft? (2 headlines)
7. Always check the logs when you encounter an error.

## Blueprint Nodes Overview

This blueprint demonstrates all available nodes.

1. Open the plugin content folder:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/api1.png)

2. Open `BP_APIOverview` blueprint.
3. Check the available nodes: functions and structs:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/api2.png)

There are also several nodes that could be useful in other projects. Feel free to copy and paste the plugin code if you need them:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/helpers.png)

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
 3. Ensure that [Python](https://www.python.org) is installed on your system.
 4. Generate the documentation using the batch script at the root of the plugin folder: `generate_docs.bat`
 5. After the generation process, the documentation will be available in the `Documentation` folder at the root of the plugin: `Documentation\html\index.html`

## Tests

Unit tests are available in the `OpenAITestRunnerModule`. You can initiate them using the `Session Frontend`:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/unittests.png)

There are also API tests that request a real endpoints. These have stress filters.
They are mostly for my personal use to check API changes. But you can use [test spec file](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Source/OpenAITestRunner/Private/OpenAIProviderActual.spec.cpp)
to learn how to make requests, because testing in software development is a part of documentation:

![](https://raw.githubusercontent.com/life-exe/UnrealOpenAIPlugin/master/Media/stresstests.png)

## Packaging

1. Verify that your .uproject file contains the following:

```cpp
{
    "Name": "OpenAI",
    "Enabled": true
}
```

2. Package the project as you always do (kudos if you use a CI solution like Jenkins).
3. You can handle authentication in your project as you see fit. However, if you choose to use a file-based method, such as in a plugin, please remember to include the `OpenAIAuth.ini` and `OnlineServicesAuth.ini` files in your packaged folder:
 - `Build/Windows/<YourProjectName>/OpenAIAuth.ini`
 - `Build/Windows/<YourProjectName>/OnlineServicesAuth.ini`

If you are having problems loading the file, please check the logs to see where it might be located:

```
LogOpenAIFuncLib: Error: Failed loading file: C:/_Projects/UE5/OpenAICpp/Build/Windows/OpenAICpp/OpenAIAuth.ini
```

## Limitations

 - OpenAI hosts a variety of different models. Please [check the models](https://platform.openai.com/docs/models/model-endpoint-compatibility) that are compatible with the particular request.
 - Some developers may need to [verify](https://help.openai.com/en/articles/10910291-api-organization-verification) their organization before being able to use the gpt-image-1 model. Please check the [organization settings⁠](https://platform.openai.com/settings/organization/general) to see if you already have access.

## Miscellaneous

I would appreciate bug reports and pull-request fixes.

Enjoy!
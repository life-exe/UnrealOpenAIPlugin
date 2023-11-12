# Intro
The plugin now includes the ability to create your own services based on the `OpenAI function` concept.
These functions extend the capabilities of `ChatGPT` and allow it to know about things it didn't before.

For example, the weather, current time, latest news, cost of airline tickets, information about your own system, program, game.

From the `OpenAI` API perspective, functions are just a few additional fields in the `JSON` request to OpenAI endpoints.
A `Service` in the plugin is a separate `UObject` base class that you can derived from, program at your discretion and add to `ChatGPT` within the plugin.
I've added two services to the core of the plugin - `weather` and `news`. You can read about how to run them in the [main readme.](https://github.com/life-exe/UnrealOpenAIPlugin#unreal-editor-chat-gpt-with-openai-functions)

Before we start, I strongly recommend familiarizing yourself with the [official documentation](https://platform.openai.com/docs/guides/function-calling) and examples on functions and understanding the concept.

We will be creating everything in `C++`. In theory, everything could be done using `Blueprints` and you can call functions by name,
but defining `JSON` in `Blueprints` it's a pain.

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/bp.png)

# Let's code the service

We will create a service that will provide the chat with information about characters and their abilities in our fixtion game `Alien Rampage Saga`.

We'll have two characters: 
 - `Voidwalker` - the hero
 - `Galaxor` - the villain.

`Voidwalker's` abilities:
 - Adept at crafting nasty jokes, has a penchant for donuts, and is mesmerized by the Aurora Borealis.

`Galaxor's` abilities:
 - Capable of unicorn riding, boasts the ability to sleep for 20 hours straight, and possesses knowledge of all credit card PIN codes.

It is assumed that you have set up the plugin. Complete instructions can also be found in the [main readme.](https://github.com/life-exe/UnrealOpenAIPlugin)

The complete listing of the program will be at the end of this tutorial.

1. Create a `UObject` class, name it `QuestService`, and inherit it from the plugin's class `UBaseService`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "QuestService.generated.h"

UCLASS()
class YOUR_PROJECT_API UQuestService : public UBaseService
{
    GENERATED_BODY()
};
```

2. Copy the following virtual functions from the `UBaseService` class. They need to be implemented:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "QuestService.generated.h"

UCLASS()
class YOUR_PROJECT_API UQuestService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets);

    virtual FString Description() const;
    virtual FString FunctionName() const;
    virtual void Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolID);

    virtual FString Name() const;
    virtual FString TooltipDescription() const;

protected:
    virtual FString MakeFunction() const;
};
```
3. We will need `JSON` utilities, so include the `JSON` module in the build file, and don't forget that the `OpenAI` plugin also needs to be linked:

```cs
using UnrealBuildTool;

public class YourProjectName : ModuleRules
{
    public YourProjectName(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OpenAI", "Json", "JsonUtilities"});
        PublicIncludePaths.AddRange(new string[] { "YourProjectName" });
    }
}
```

4. Immediately implement the `Name` and `TooltipDescription` functions. These are strictly for UI information:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "QuestService.generated.h"

UCLASS()
class YOUR_PROJECT_API UQuestService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets);

    virtual FString Description() const;
    virtual FString FunctionName() const;
    virtual void Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolID);

    virtual FString Name() const override { return "Quest"; }
    virtual FString TooltipDescription() const override { return "Alien Rampage Saga"; }

protected:
    virtual FString MakeFunction() const;
};
```

5. Create the function definitions in the `cpp` file, include the necessary headers, and create a logging category:

```cpp
#include "QuestService.h"
#include "Provider/CommonTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Provider/RequestTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestService, All, All);

bool UQuestService::Init(const OpenAI::ServiceSecrets& Secrets) { return true; }

FString UQuestService::FunctionName() const { return {}; }

FString UQuestService::Description() const { return {}; }

FString UQuestService::MakeFunction() const { return {}; }

void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolID) {}
```

6. Create a name for our function that `OpenAI` will call. The init function will always return `true` in our example.
It's not significant in our service. It's assumed that some kind of service validation is taking place here such as an checking `API` key or whatever:

```cpp
#include "QuestService.h"
#include "Provider/CommonTypes.h"
#include "Provider/RequestTypes.h"
#include "FuncLib/OpenAIFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestService, All, All);

bool UQuestService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    return true;
}

FString UQuestService::FunctionName() const
{
    return "get_alien_rampage_saga_characters_information";
}
```

7. The `Description` function should be as informative as possible so that `ChatGPT` understands the context of your prompts:

```cpp
FString UQuestService::Description() const
{
    return "Get information about characters from my game Alien Rampage Saga.";
}
```

8. The `MakeFunction` method defines the parameters for our function `get_alien_rampage_saga_characters_information`
that the GPT chat can pass. The function returns `JSON` as a string. Let's create such a `JSON` object with parameters:

```json
"parameters": {
    "type": "object",
    "properties": {
        "name": {
            "type": "string",
            "description": "Name of the character of my Alien Rampage Saga.",
        },
        "ability": {
            "type": "boolean",
            "description": "Set this to true if you want to know what skills the character has.",
        },
    },
    "required": ["name"],
}
```

 - properties - a list of parameters that `ChatGPT` can pass

 - name - name of our character about whom `ChatGPT` will want to know
 - ability - this is a flag that `ChatGPT` will send when it wants to know about the character's abilities.
 - required - array of required parameters, we always only need the name of the character.

Implementation of the `MakeFunction`. Basically, it's just a `JSON` creation that I listed above:

```cpp
FString UQuestService::MakeFunction() const
{
    TSharedPtr<FJsonObject> MainObj = MakeShareable(new FJsonObject());
    MainObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());

    // character name
    TSharedPtr<FJsonObject> NameObj = MakeShareable(new FJsonObject());
    NameObj->SetStringField("type", "string");
    NameObj->SetStringField("description", "Name of the character of my Alien Rampage Saga.");
    Props->SetObjectField("name", NameObj);

    // ability
    TSharedPtr<FJsonObject> AbilityObj = MakeShareable(new FJsonObject());
    AbilityObj->SetStringField("type", "boolean");
    AbilityObj->SetStringField("description", "Set this to true if you want to know what skills the character has.");
    Props->SetObjectField("ability", AbilityObj);

    MainObj->SetObjectField("properties", Props);

    // required params
    TArray<TSharedPtr<FJsonValue>> RequiredArray;
    RequiredArray.Add(MakeShareable(new FJsonValueString("name")));
    MainObj->SetArrayField("required", RequiredArray);

    return UOpenAIFuncLib::MakeFunctionsString(MainObj);
}
```

Here we have a magic function from the plugin `UOpenAIFuncLib::MakeFunctionsString(MainObj)`
that is necessary for proper parsing of the [JSON Schema reference.](https://json-schema.org/understanding-json-schema)

9. We are moving on to the final function, which is called when `ChatGPT` requests information about our characters:
Add call to the parent function with `Super` alias. Basically the parent function will store the `ToolID`.

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolIDIn)
{
    Super::Call(ArgsJson, ToolIDIn);
}
```

The function takes a `JSON` object with parameters. Let's output them to the log;
we can use a convenient function `UOpenAIFuncLib::JsonToString` from the plugin library.
We will also retrieve our parameters that we defined in the `JSON schema`.
The only mandatory parameter is the `name` our character; `ability` may not be present:

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolIDIn)
{
    Super::Call(ArgsJson, ToolIDIn);

    FString ArgsStr;
    if (UOpenAIFuncLib::JsonToString(ArgsJson, ArgsStr))
    {
        UE_LOG(LogQuestService, Display, TEXT("Args for the quest request: %s"), *ArgsStr);
    }

    FString CharacterName;
    if (!ArgsJson->TryGetStringField("name", CharacterName))
    {
        ServiceDataError.Broadcast("Please provide character name");
        return;
    }

    bool AbilityRequested{false};
    ArgsJson->TryGetBoolField("ability", AbilityRequested);
}
```

The `ServiceDataError` is a delegate from the base class `UBaseService` which notifies the plugin of an error.
This could be used within the service implementation to report back any issues in processing the request,
such as missing parameters, invalid data, or internal service errors.

Next, we define the data structure with information about our characters.
I am doing this locally; you can add it as a member of the class:

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolIDIn)
{
...
    struct FCharacterInfo
    {
        FString Description;
        FString Abilities;
    };
    const TMap<FString, FCharacterInfo> Info  //
        {{"Voidwalker", {"The main character fighting for the future of the Universe",
                            "Adept at crafting nasty jokes, has a penchant for donuts, and is mesmerized by the Aurora Borealis"}},
            {"Galaxor", {"A formidable antagonist intent on the Universe's destruction",
                            "Capable of unicorn riding, boasts the ability to sleep for 20 hours straight, and possesses knowledge of all "
                            "credit card PIN codes"}}};
}
```

Great. Now we are forming a string to send to the `ChatGPT`:

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolIDIn)
{
...
    FString InfoToOpenaAI;
    if (Info.Contains(CharacterName))
    {
        InfoToOpenaAI = AbilityRequested ? Info[CharacterName].Abilities : Info[CharacterName].Description;
    }
}
```

Now we're assembling the full structure and sending it to the core of the plugin:
You can do this with a function that exists in the `UBaseService` class.

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolIDIn)
{
...
    const FMessage Message = MakeMessage(InfoToOpenAI);
    ServiceDataRecieved.Broadcast(Message);
}
```

The main field of the structure `FMessage` is `Content`, into which any information that
will be analyzed by the `ChatGPT` and it will write a response based on it.

The `ServiceDataRecieved` is a delegate from the base class `UBaseService` which notifies the plugin that we can send data to the `ChatGPT`.

The full listing of the `UQuestService::Call` is following:

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolIDIn)
{
    Super::Call(Args, ToolIDIn);

    FString ArgsStr;
    if (UOpenAIFuncLib::JsonToString(ArgsJson, ArgsStr))
    {
        UE_LOG(LogQuestService, Display, TEXT("Args for the quest request: %s"), *ArgsStr);
    }

    FString CharacterName;
    if (!ArgsJson->TryGetStringField("name", CharacterName))
    {
        ServiceDataError.Broadcast("Please provide character name");
        return;
    }

    bool AbilityRequested{false};
    ArgsJson->TryGetBoolField("ability", AbilityRequested);

    struct FCharacterInfo
    {
        FString Description;
        FString Abilities;
    };
    const TMap<FString, FCharacterInfo> Info  //
        {{"Voidwalker", {"The main character fighting for the future of the Universe",
                            "Adept at crafting nasty jokes, has a penchant for donuts, and is mesmerized by the Aurora Borealis"}},
            {"Galaxor", {"A formidable antagonist intent on the Universe's destruction",
                            "Capable of unicorn riding, boasts the ability to sleep for 20 hours straight, and possesses knowledge of all "
                            "credit card PIN codes"}}};

    FString InfoToOpenaAI;
    if (Info.Contains(CharacterName))
    {
        InfoToOpenaAI = AbilityRequested ? Info[CharacterName].Abilities : Info[CharacterName].Description;
    }

    const FMessage Message = MakeMessage(InfoToOpenAI);
    ServiceDataRecieved.Broadcast(Message);
}
```

10. Compile your code and open the `Unreal Engine` Editor.

# Let's go to the editor
 
Now the fun part starts.

1. In the `Content Browser`, find the `ChatGPT` widget within the plugin's content and open it.

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/1.png)

2. Go to the `Graph` view of the widget.

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/2.png)

3. In the `Details` panel add our new `QuestService` to the services' array.

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/3.png)

4. Right-click the widget to run it as an editor utility widget

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/4.png)

5. Activate our new service. You should see the service name displayed, with the description showing as a tooltip.

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/5.png)

6. Ask ChatGPT if it knows anything about your game to test the functionality.

> Can you retrieve information about the characters from my game "Alien Rampage Saga."?

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/6.png)

COOOOL! Works!

> Who is Voidwalker?

> Who is Galaxor?

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/7.png)

Asking about abilities:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/Services/8.png)

Note that `ChatGPT` processes information and writes quite naturally.

That's about it. Functions open a lot of additional possibilities to extend `ChatGPT`.

Full lisitng of our `QuestService`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "QuestService.generated.h"

UCLASS()
class AIMUSEUM_API UQuestService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) override;
    virtual FString Name() const override { return "Quest"; }
    virtual FString TooltipDescription() const override { return "Alien Rampage Saga"; }
    virtual FString Description() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolID) override;

protected:
    virtual FString MakeFunction() const;
};
```

```cpp
#include "QuestService.h"
#include "Provider/CommonTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Provider/RequestTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestService, All, All);

bool UQuestService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    return true;
}

FString UQuestService::FunctionName() const
{
    return "get_alien_rampage_saga_characters_information";
}

FString UQuestService::Description() const
{
    return "Get information about characters from my game Alien Rampage Saga.";
}

FString UQuestService::MakeFunction() const
{
    /*
        "parameters": {
            "type": "object",
            "properties": {
                "name": {
                    "type": "string",
                    "description": "Name of the character of my Alien Rampage Saga game.",
                },
                "ability": {
                    "type": "boolean",
                    "description": "Set this to true if you want to know what skills the character has.",
                },
            },
            "required": ["name"],
        }
    */

    TSharedPtr<FJsonObject> MainObj = MakeShareable(new FJsonObject());
    MainObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());

    // character name
    TSharedPtr<FJsonObject> NameObj = MakeShareable(new FJsonObject());
    NameObj->SetStringField("type", "string");
    NameObj->SetStringField("description", "Name of the character of my Alien Rampage Saga game.");
    Props->SetObjectField("name", NameObj);

    // ability
    TSharedPtr<FJsonObject> AbilityObj = MakeShareable(new FJsonObject());
    AbilityObj->SetStringField("type", "boolean");
    AbilityObj->SetStringField("description", "Set this to true if you want to know what skills the character has.");
    Props->SetObjectField("ability", AbilityObj);

    MainObj->SetObjectField("properties", Props);

    // required params
    TArray<TSharedPtr<FJsonValue>> RequiredArray;
    RequiredArray.Add(MakeShareable(new FJsonValueString("name")));
    MainObj->SetArrayField("required", RequiredArray);

    return UOpenAIFuncLib::MakeFunctionsString(MainObj);
}

void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson, const FString& ToolIDIn)
{
    Super::Call(Args, ToolIDIn);

    FString ArgsStr;
    if (UOpenAIFuncLib::JsonToString(ArgsJson, ArgsStr))
    {
        UE_LOG(LogQuestService, Display, TEXT("Args for the quest request: %s"), *ArgsStr);
    }

    FString CharacterName;
    if (!ArgsJson->TryGetStringField("name", CharacterName))
    {
        ServiceDataError.Broadcast("Please provide character name");
        return;
    }

    bool AbilityRequested{false};
    ArgsJson->TryGetBoolField("ability", AbilityRequested);

    struct FCharacterInfo
    {
        FString Description;
        FString Abilities;
    };
    const TMap<FString, FCharacterInfo> Info  //
        {{"Voidwalker", {"The main character fighting for the future of the Universe",
                            "Adept at crafting nasty jokes, has a penchant for donuts, and is mesmerized by the Aurora Borealis"}},
            {"Galaxor", {"A formidable antagonist intent on the Universe's destruction",
                            "Capable of unicorn riding, boasts the ability to sleep for 20 hours straight, and possesses knowledge of all "
                            "credit card PIN codes"}}};

    FString InfoToOpenaAI;
    if (Info.Contains(CharacterName))
    {
        InfoToOpenaAI = AbilityRequested ? Info[CharacterName].Abilities : Info[CharacterName].Description;
    }
    else
    {
        InfoToOpenaAI = "Character with such a name doesn't exist in game";
    }

    const FMessage Message = MakeMessage(InfoToOpenAI);
    ServiceDataRecieved.Broadcast(Message);
}
```

# Intro
The plugin now includes the ability to create your own services based on the `OpenAI function` concept.
These functions extend the capabilities of `ChatGPT` and allow it to know about things it didn't before.

For example, the weather, current time, latest news, cost of airline tickets, information about your own system, program, game.

From the `OpenAI` API perspective, functions are just a few additional fields in the `JSON` request to OpenAI endpoints.
A `Service` in the plugin is a separate `UObject` base class that you can derived from, program at your discretion and add to `ChatGPT` within the plugin.
I've added two services to the core of the plugin - `weather` and `news`. You can read about how to run them in the [main readme](https://github.com/life-exe/UnrealOpenAIPlugin#unreal-editor-chat-gpt-with-openai-functions).

Before we start, I strongly recommend familiarizing yourself with the [official documentation](https://platform.openai.com/docs/guides/gpt/function-calling) and examples on functions and understanding the concept.

We will be creating everything in `C++`. In theory, everything could be done using `Blueprints` and you can call functions by name,
but defining `JSON` in `Blueprints` will lead to spaghetti code.

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

It is assumed that you have set up the plugin. Complete instructions can also be found in the [main readme](https://github.com/life-exe/UnrealOpenAIPlugin).

The complete listing of the program will be at the end of this tutorial.

1. Create a `UObject` class, name it `QuestService`, and inherit it from the plugin's class `UBaseService`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"

UCLASS()
class YOUR_PROJECT_API UQuestService : public UBaseService
{
    GENERATED_BODY()
};
```

2. Copy all virtual functions from the `UBaseService` class. They need to be implemented:

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
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) override;
    virtual FString Name() const override;
    virtual FString Description() const override;
    virtual FFunctionOpenAI Function() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& ArgsJson) override;
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

4. Immediately implement the `Name` and `Description` functions.
These are strictly for UI information. Also, add the declaration for `MakeQuestFunction` function:

```cpp
UCLASS()
class YOUR_PROJECT_API UQuestService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) override;
    virtual FString Name() const override { return "Quest"; };
    virtual FString Description() const override { return "Alien Rampage Saga"; };
    virtual FFunctionOpenAI Function() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& ArgsJson) override;

private:
    FString MakeQuestFunction() const;
};
```

5. Create the function definitions in the `cpp` file, include the necessary headers, and create a logging category:

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

FFunctionOpenAI UQuestService::Function() const
{
    return {};
}

FString UQuestService::FunctionName() const
{
    return {};
}

FString UQuestService::MakeQuestFunction() const
{
    return {};
}

void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
{
}
```

6. Create a name for our function that `OpenAI` will call, and write it in a constant within an anonymous namespace.
The init function will always return `true` in our example. It's not significant as it's assumed that some kind of
service validation is taking place here such as an checking `API` key or whatever:

```cpp
#include "QuestService.h"
#include "Provider/CommonTypes.h"
#include "Provider/RequestTypes.h"
#include "FuncLib/OpenAIFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestService, All, All);

namespace Quest
{
const FString InternalFunctionName = "get_alien_rampage_saga_characters_information";
}  // namespace Quest

bool UQuestService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    return true;
}
...

```

7. The `Function` method returns a structure that will be passed to the `ChatGPT`.
The `Description` field should be as informative as possible so that `ChatGPT` understands the context of your prompts.
The `FunctionName` method returns name of our function `get_alien_rampage_saga_characters_information`:

```cpp
FFunctionOpenAI UQuestService::Function() const
{
    FFunctionOpenAI FunctionOpenAI;
    FunctionOpenAI.Name = Quest::InternalFunctionName;
    FunctionOpenAI.Description = "Get information about characters from my game Alien Rampage Saga.";
    FunctionOpenAI.Parameters = MakeQuestFunction();
    return FunctionOpenAI;
}

FString UQuestService::FunctionName() const
{
    return Quest::InternalFunctionName;
}
```

8. The `MakeQuestFunction` method defines the parameters for our function `get_alien_rampage_saga_characters_information`
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
    "required": ["character"],
}
```

 - properties - a list of parameters that `ChatGPT` can pass

 - name - name of our character about whom `ChatGPT` will want to know
 - ability - this is a flag that `ChatGPT` will send when it wants to know about the character's abilities.
 - required - array of required parameters, we always only need the name of the character.

Implementation of the `MakeQuestFunction`. Basically, it's just a `JSON` creation that I listed above:

```cpp
FString UQuestService::MakeQuestFunction() const
{
    TSharedPtr<FJsonObject> MainObj = MakeShareable(new FJsonObject());
    MainObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());

    // character name
    TSharedPtr<FJsonObject> CharacterObj = MakeShareable(new FJsonObject());
    CharacterObj->SetStringField("type", "string");
    CharacterObj->SetStringField("description", "Name of the character of my Alien Rampage Saga.");
    Props->SetObjectField("name", CharacterObj);

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
that is necessary for proper parsing of the [JSON Schema reference](https://json-schema.org/understanding-json-schema).

9. We are moving on to the final function, which is called when `ChatGPT` requests information about our characters:

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
{
}
```

The function takes a `JSON` object with parameters. Let's output them to the log;
we can use a convenient function `UOpenAIFuncLib::JsonToString` from the plugin library.
We will also retrieve our parameters that we defined in the `JSON schema`.
The only mandatory parameter is the `name` our character; `ability` may not be present:

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
{
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
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
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
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
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

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
{
...
    FMessage Message;
    Message.Name = Quest::InternalFunctionName;
    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
    Message.Content = InfoToOpenaAI;

    ServiceDataRecieved.Broadcast(Message);
```

The main field of the structure `FMessage` is `Content`, into which any information that
will be analyzed by the `ChatGPT` and it will write a response based on it.

The `ServiceDataRecieved` is a delegate from the base class `UBaseService` which notifies the plugin that we can send data to the `ChatGPT`.

The full listing of the `UQuestService::Call` is following:

```cpp
void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
{
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

    FMessage Message;
    Message.Name = Quest::InternalFunctionName;
    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
    Message.Content = InfoToOpenaAI;

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
    virtual FString Name() const override { return "Quest"; };
    virtual FString Description() const override { return "Alien Rampage Saga"; };
    virtual FFunctionOpenAI Function() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& ArgsJson) override;

private:
    FString MakeQuestFunction() const;
};

```

```cpp
#include "QuestService.h"
#include "Provider/CommonTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Provider/RequestTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestService, All, All);

namespace Quest
{
const FString InternalFunctionName = "get_alien_rampage_saga_characters_information";
}  // namespace Quest

bool UQuestService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    return true;
}

FFunctionOpenAI UQuestService::Function() const
{
    FFunctionOpenAI FunctionOpenAI;
    FunctionOpenAI.Name = Quest::InternalFunctionName;
    FunctionOpenAI.Description = "Get information about characters from my game Alien Rampage Saga.";
    FunctionOpenAI.Parameters = MakeQuestFunction();
    return FunctionOpenAI;
}

FString UQuestService::FunctionName() const
{
    return Quest::InternalFunctionName;
}

FString UQuestService::MakeQuestFunction() const
{
    /*
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
            "required": ["character"],
        }
    */

    TSharedPtr<FJsonObject> MainObj = MakeShareable(new FJsonObject());
    MainObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());

    // character name
    TSharedPtr<FJsonObject> CharacterObj = MakeShareable(new FJsonObject());
    CharacterObj->SetStringField("type", "string");
    CharacterObj->SetStringField("description", "Name of the character of my Alien Rampage Saga.");
    Props->SetObjectField("name", CharacterObj);

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

void UQuestService::Call(const TSharedPtr<FJsonObject>& ArgsJson)
{
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

    FMessage Message;
    Message.Name = Quest::InternalFunctionName;
    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
    Message.Content = InfoToOpenaAI;

    ServiceDataRecieved.Broadcast(Message);
}
```

// OpenAI ServiceSample, Copyright LifeEXE. All Rights Reserved.

#include "Sample/Services/QuestService.h"
#include "Funclib/OpenAIFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogQuestService, All, All);

FString UQuestService::FunctionName() const
{
    return "get_alien_rampage_saga_characters_info";
}

FString UQuestService::Description() const
{
    return "Get information about characters from my Alien Rampage Saga game.";
}

FString UQuestService::MakeFunction() const
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
        "required": ["name"],
    }
    */

    TSharedPtr<FJsonObject> ParamsObj = MakeShareable(new FJsonObject());
    ParamsObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> PropsObj = MakeShareable(new FJsonObject());

    // character name
    TSharedPtr<FJsonObject> NameObj = MakeShareable(new FJsonObject());
    NameObj->SetStringField("type", "string");
    NameObj->SetStringField("description", "Name of the character of my Alien Rampage Saga.");
    PropsObj->SetObjectField("name", NameObj);

    // ability
    TSharedPtr<FJsonObject> AbilityObj = MakeShareable(new FJsonObject());
    AbilityObj->SetStringField("type", "boolean");
    AbilityObj->SetStringField("description", "Set this to true if you want to know what skills the character has.");
    PropsObj->SetObjectField("ability", AbilityObj);

    ParamsObj->SetObjectField("properties", PropsObj);

    TArray<TSharedPtr<FJsonValue>> RequiredArray;
    RequiredArray.Add(MakeShareable(new FJsonValueString("name")));

    ParamsObj->SetArrayField("required", RequiredArray);

    return UOpenAIFuncLib::MakeFunctionsString(ParamsObj);
}

void UQuestService::Call(const TSharedPtr<FJsonObject>& Args)
{
    FString ArgsStr;
    if (!UOpenAIFuncLib::JsonToString(Args, ArgsStr))
    {
        UE_LOG(LogQuestService, Display, TEXT("Can't convert JSON to string"));
    }

    FAlienRampageGameProps Props;
    if (!UOpenAIFuncLib::ParseJSONToStruct<FAlienRampageGameProps>(ArgsStr, &Props))
    {
        UE_LOG(LogQuestService, Error, TEXT("Can't parse args"));
        ServiceDataError.Broadcast("QuestService can't parse args from OpenAI.");
        return;
    }

    struct FCharacterInfo
    {
        FString Description;
        FString Abilities;
    };

    using CharacterName = FString;
    const TMap<CharacterName, FCharacterInfo> Info  //
        {{"voidwalker", {"The main character fighting for the future of the Universe",
                            "Adept at crafting nasty jokes, has a penchant for donuts, and is mesmerized by the Aurora Borealis"}},
            {"galaxor", {"A formidable antagonist intent on the Universe's destruction",
                            "Capable of unicorn riding, boasts the ability to sleep for 20 hours straight, and possesses knowledge of all "
                            "credit card PIN codes"}}};

    FString InfoToOpenAI{};
    const FString Name = Props.Name.ToLower();
    if (Info.Contains(Name))
    {
        InfoToOpenAI = Props.Ability ? Info[Name].Abilities : Info[Name].Description;
    }
    else
    {
        InfoToOpenAI = "Character with such a name doesn't exist in the game.";
    }

    FMessage Message;
    Message.Name = FunctionName();
    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
    Message.Content = InfoToOpenAI;

    ServiceDataRecieved.Broadcast(Message);
}

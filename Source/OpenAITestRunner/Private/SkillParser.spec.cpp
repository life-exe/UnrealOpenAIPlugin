// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/SkillTypes.h"
#include "FuncLib/JsonFuncLib.h"
#include "JsonObjectConverter.h"

DEFINE_SPEC(FSkillParserTest, "OpenAI.Parser.Skill",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FSkillParserTest::Define()
{
    Describe("JSON",
        [this]()
        {
            It("UpdateSkillShouldBeCorrect",
                [this]()
                {
                    FUpdateSkill UpdateSkill;
                    UpdateSkill.Default_Version = "2";

                    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(UpdateSkill);
                    FString Result;
                    UJsonFuncLib::JsonToString(Json, Result);

                    TSharedPtr<FJsonObject> ParsedJson;
                    UJsonFuncLib::StringToJson(Result, ParsedJson);

                    TestTrueExpr(ParsedJson->GetStringField(TEXT("default_version")).Equals(TEXT("2")));
                });
        });
}

#endif

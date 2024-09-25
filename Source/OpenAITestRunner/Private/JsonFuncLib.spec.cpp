// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "FuncLib/JsonFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "TestUtils.h"

DEFINE_SPEC(FJsonFuncLib, "OpenAI",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

void FJsonFuncLib::Define()
{
    Describe("JsonFuncLib",
        [this]()
        {
            It("StringWithValidJsonShouldMakeConversionCorrectly",
                [this]()
                {
                    const FString String = "{\"object\":\"list\"}";
                    TSharedPtr<FJsonObject> JsonObject;
                    TestTrueExpr(UJsonFuncLib::StringToJson(String, JsonObject));
                    TestTrueExpr(JsonObject->GetStringField(TEXT("object")).Equals("list"));
                });

            It("JsonToStringShouldMakeConversionCorrectly",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    JsonObject->SetStringField("String", "value");
                    JsonObject->SetStringField("STrinG_NEW", "VALUE");

                    TSharedPtr<FJsonObject> JsonSubObject = MakeShareable(new FJsonObject());
                    JsonSubObject->SetStringField("SUBObJect", "vaLue");
                    JsonObject->SetObjectField("ObjecT_NEW", JsonSubObject);

                    FString String;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, String));

                    const FString ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(String);
                    TestTrueExpr(
                        ResultString.Equals("{\"string\": \"value\",\"string_new\": \"VALUE\",\"object_new\":{\"subobject\": \"vaLue\"}}"));
                });
        });
}

#endif

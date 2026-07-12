// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "FuncLib/JsonFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "TestUtils.h"
#include "Logging/StructuredLog.h"

DEFINE_SPEC(
    FJsonFuncLib, "OpenAI", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

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

            It("KeysShouldBeConvertedToLowercaseValuesNot",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    JsonObject->SetStringField(TEXT("String"), "value");
                    JsonObject->SetStringField(TEXT("STrinG_NEW"), "VALUE");

                    TSharedPtr<FJsonObject> JsonSubObject = MakeShareable(new FJsonObject());
                    JsonSubObject->SetStringField(TEXT("SUBObJect"), "vaLue");
                    JsonObject->SetObjectField(TEXT("ObjecT_NEW"), JsonSubObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(
                        ResultString.Equals("{\"string\": \"value\",\"string_new\": \"VALUE\",\"object_new\":{\"subobject\": \"vaLue\"}}"));
                });

            It("RemoveEmptyArrays.EmptyArraysShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    TArray<TSharedPtr<FJsonValue>> Array;
                    JsonObject->SetArrayField(TEXT("EmptyArray"), Array);

                    TSharedPtr<FJsonValue> ArrayElem = MakeShareable(new FJsonValueNumber(10));
                    Array.Add(ArrayElem);
                    JsonObject->SetArrayField(TEXT("Array"), Array);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"array\": [10]}"));
                });

            It("RemoveEmptyArrays.NestedEmptyArraysShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    TArray<TSharedPtr<FJsonValue>> OuterArray;
                    TArray<TSharedPtr<FJsonValue>> InnerArray;

                    OuterArray.Add(MakeShareable(new FJsonValueArray(InnerArray)));
                    OuterArray.Add(MakeShareable(new FJsonValueNumber(10)));
                    JsonObject->SetArrayField(TEXT("NestedArray"), OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"nestedarray\": [10]}"));
                });

            It("RemoveEmptyArrays.MultipleNestedEmptyArraysShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

                    TArray<TSharedPtr<FJsonValue>> OuterArray;
                    TArray<TSharedPtr<FJsonValue>> InnerArray1;
                    TArray<TSharedPtr<FJsonValue>> InnerArray2;

                    OuterArray.Add(MakeShareable(new FJsonValueArray(InnerArray1)));  // Empty inner array 1
                    OuterArray.Add(MakeShareable(new FJsonValueArray(InnerArray2)));  // Empty inner array 2
                    OuterArray.Add(MakeShareable(new FJsonValueNumber(25)));          // Number element

                    JsonObject->SetArrayField(TEXT("MultipleNestedEmptyArrays"), OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"multiplenestedemptyarrays\": [25]}"));
                });

            It("RemoveEmptyArrays.MixedNestedArraysShouldRetainNonEmptyArrays",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

                    TArray<TSharedPtr<FJsonValue>> OuterArray;
                    TArray<TSharedPtr<FJsonValue>> InnerArray1;
                    TArray<TSharedPtr<FJsonValue>> InnerArray2;

                    InnerArray2.Add(MakeShareable(new FJsonValueNumber(5)));  // Non-empty inner array 2

                    OuterArray.Add(MakeShareable(new FJsonValueArray(InnerArray1)));  // Empty inner array 1
                    OuterArray.Add(MakeShareable(new FJsonValueArray(InnerArray2)));  // Non-empty inner array 2
                    OuterArray.Add(MakeShareable(new FJsonValueNumber(42)));          // Number element

                    JsonObject->SetArrayField(TEXT("MixedNestedArrays"), OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"mixednestedarrays\": [[5],42]}"));
                });

            It("RemoveEmptyArrays.DeeplyNestedEmptyArraysShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

                    TArray<TSharedPtr<FJsonValue>> OuterArray;
                    TArray<TSharedPtr<FJsonValue>> InnerArray;
                    TArray<TSharedPtr<FJsonValue>> DeepestArray;

                    InnerArray.Add(MakeShareable(new FJsonValueArray(DeepestArray)));  // Deepest empty array
                    OuterArray.Add(MakeShareable(new FJsonValueArray(InnerArray)));    // Nested empty array
                    OuterArray.Add(MakeShareable(new FJsonValueNumber(100)));          // Number element

                    JsonObject->SetArrayField(TEXT("DeeplyNestedEmptyArrays"), OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"deeplynestedemptyarrays\": [100]}"));
                });

            It("RemoveEmptyArrays.MultipleLevelsOfNestingShouldHandleComplexStructures",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

                    TArray<TSharedPtr<FJsonValue>> OuterArray;
                    TArray<TSharedPtr<FJsonValue>> MidArray;
                    TArray<TSharedPtr<FJsonValue>> InnerArray;

                    InnerArray.Add(MakeShareable(new FJsonValueNumber(15)));       // Non-empty inner array
                    MidArray.Add(MakeShareable(new FJsonValueArray(InnerArray)));  // Non-empty mid array
                    OuterArray.Add(MakeShareable(new FJsonValueArray(MidArray)));  // Nested arrays
                    OuterArray.Add(MakeShareable(new FJsonValueNumber(50)));       // Number element

                    JsonObject->SetArrayField(TEXT("MultipleLevelsOfNesting"), OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"multiplelevelsofnesting\": [[[15]],50]}"));
                });

            It("OptionalValuesThatIsNotSetShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    JsonObject->SetStringField(TEXT("value"), "string");
                    JsonObject->SetStringField(TEXT("isset"), "false");

                    TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());
                    RootJsonObject->SetObjectField(TEXT("object"), JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(RootJsonObject, ResultString));

                    ResultString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(ResultString);
                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);

                    TestTrueExpr(ResultString.Equals("{}"));
                });

            It("OptionalValuesThatIsSetShouldNotBeRemovedButSetDirectly",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    JsonObject->SetStringField(TEXT("value"), "string");
                    JsonObject->SetStringField(TEXT("isset"), "true");

                    TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());
                    RootJsonObject->SetObjectField(TEXT("object"), JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(RootJsonObject, ResultString));

                    ResultString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(ResultString);
                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);

                    TestTrueExpr(ResultString.Equals("{\"object\": \"string\"}"));
                });

            It("StructOptionalThatIsNotSetShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> InnerObject = MakeShareable(new FJsonObject());
                    InnerObject->SetStringField(TEXT("anchor"), "created_at");
                    InnerObject->SetNumberField(TEXT("seconds"), 3600);
                    InnerObject->SetBoolField(TEXT("isset"), false);

                    TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());
                    RootJsonObject->SetObjectField(TEXT("expires_after"), InnerObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(RootJsonObject, ResultString));

                    ResultString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(ResultString);
                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);

                    TestTrueExpr(ResultString.Equals("{}"));
                });

            It("StructOptionalThatIsSetShouldKeepItsFieldsWithoutIsSet",
                [this]()
                {
                    TSharedPtr<FJsonObject> InnerObject = MakeShareable(new FJsonObject());
                    InnerObject->SetStringField(TEXT("anchor"), "created_at");
                    InnerObject->SetNumberField(TEXT("seconds"), 3600);
                    InnerObject->SetBoolField(TEXT("isset"), true);

                    TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());
                    RootJsonObject->SetObjectField(TEXT("expires_after"), InnerObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(RootJsonObject, ResultString));

                    ResultString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(ResultString);
                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);

                    // expires_after should be present with its fields but without the "isset" sentinel
                    TestTrueExpr(ResultString.Contains(TEXT("\"expires_after\"")));
                    TestTrueExpr(ResultString.Contains(TEXT("\"anchor\": \"created_at\"")));
                    TestTrueExpr(ResultString.Contains(TEXT("\"seconds\": 3600")));
                    TestTrueExpr(!ResultString.Contains(TEXT("\"isset\"")));
                });
        });
}

#endif

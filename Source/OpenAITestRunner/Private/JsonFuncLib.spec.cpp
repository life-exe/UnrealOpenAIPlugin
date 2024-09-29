// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "FuncLib/JsonFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "TestUtils.h"
#include "Logging/StructuredLog.h"

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

            It("KeysShouldBeConvertedToLowercaseValuesNot",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    JsonObject->SetStringField("String", "value");
                    JsonObject->SetStringField("STrinG_NEW", "VALUE");

                    TSharedPtr<FJsonObject> JsonSubObject = MakeShareable(new FJsonObject());
                    JsonSubObject->SetStringField("SUBObJect", "vaLue");
                    JsonObject->SetObjectField("ObjecT_NEW", JsonSubObject);

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
                    JsonObject->SetArrayField("EmptyArray", Array);

                    TSharedPtr<FJsonValue> ArrayElem = MakeShareable(new FJsonValueNumber(10));
                    Array.Add(ArrayElem);
                    JsonObject->SetArrayField("Array", Array);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"array\": [ 10 ]}"));
                });

            It("RemoveEmptyArrays.NestedEmptyArraysShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    TArray<TSharedPtr<FJsonValue>> OuterArray;
                    TArray<TSharedPtr<FJsonValue>> InnerArray;

                    OuterArray.Add(MakeShareable(new FJsonValueArray(InnerArray)));
                    OuterArray.Add(MakeShareable(new FJsonValueNumber(10)));
                    JsonObject->SetArrayField("NestedArray", OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"nestedarray\": [ 10 ]}"));
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

                    JsonObject->SetArrayField("MultipleNestedEmptyArrays", OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"multiplenestedemptyarrays\": [ 25 ]}"));
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

                    JsonObject->SetArrayField("MixedNestedArrays", OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"mixednestedarrays\": [[ 5 ],42 ]}"));
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

                    JsonObject->SetArrayField("DeeplyNestedEmptyArrays", OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"deeplynestedemptyarrays\": [ 100 ]}"));
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

                    JsonObject->SetArrayField("MultipleLevelsOfNesting", OuterArray);

                    UJsonFuncLib::RemoveEmptyArrays(JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(JsonObject, ResultString));

                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);
                    TestTrueExpr(ResultString.Equals("{\"multiplelevelsofnesting\": [[[ 15 ]],50 ]}"));
                });

            It("OptionalValuesThatIsNotSetShouldBeRemoved",
                [this]()
                {
                    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                    JsonObject->SetStringField("value", "string");
                    JsonObject->SetStringField("isset", "false");

                    TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());
                    RootJsonObject->SetObjectField("object", JsonObject);

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
                    JsonObject->SetStringField("value", "string");
                    JsonObject->SetStringField("isset", "true");

                    TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());
                    RootJsonObject->SetObjectField("object", JsonObject);

                    FString ResultString;
                    TestTrueExpr(UJsonFuncLib::JsonToString(RootJsonObject, ResultString));

                    ResultString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(ResultString);
                    ResultString = UOpenAIFuncLib::RemoveWhiteSpaces(ResultString);

                    TestTrueExpr(ResultString.Equals("{\"object\": \"string\"}"));
                });
        });
}

#endif

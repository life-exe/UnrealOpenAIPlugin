// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "FuncLib/JsonFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "TestUtils.h"
#include "Provider/Types/Chat/ChatCommonTypes.h"
#include "Provider/Types/VectorStoreTypes.h"
#include "Provider/Types/VideoTypes.h"
#include "Provider/Types/Chat/ChatCompletionChunkTypes.h"
#include "Provider/Types/Legacy/CompletionTypes.h"
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

            It("CreateVideoShouldSendSecondsAsStringAndInputReferenceAsObject",
                [this]()
                {
                    const auto Serialize = [](const FCreateVideo& CreateVideo)
                    {
                        FString ResultString;
                        UJsonFuncLib::JsonToString(FJsonObjectConverter::UStructToJsonObject(CreateVideo), ResultString);
                        TSharedPtr<FJsonObject> Json;
                        UJsonFuncLib::StringToJson(UJsonFuncLib::RemoveOptionalValuesThatNotSet(ResultString), Json);
                        return Json;
                    };

                    FCreateVideo CreateVideo;
                    CreateVideo.Prompt = "A cat";
                    const auto DefaultJson = Serialize(CreateVideo);
                    TestTrueExpr(DefaultJson.IsValid() && !DefaultJson->HasField(TEXT("input_reference")) &&
                                 !DefaultJson->HasField(TEXT("seconds")));

                    CreateVideo.Seconds.Set("8");
                    CreateVideo.Input_Reference.File_Id.Set("file-1");
                    CreateVideo.Input_Reference.IsSet = true;
                    const auto Json = Serialize(CreateVideo);
                    if (!TestTrue("JSON is valid", Json.IsValid())) return;

                    TestTrueExpr(Json->GetStringField(TEXT("seconds")).Equals(TEXT("8")));
                    const auto Reference = Json->GetObjectField(TEXT("input_reference"));
                    TestTrueExpr(Reference->GetStringField(TEXT("file_id")).Equals(TEXT("file-1")));
                    TestFalseExpr(Reference->HasField(TEXT("image_url")));
                    TestFalseExpr(Reference->HasField(TEXT("isset")));
                });

            It("ParseJSONToStructShouldSetOptionalsFromScalars",
                [this]()
                {
                    FMessage Message;
                    TestTrueExpr(UJsonFuncLib::ParseJSONToStruct(
                        TEXT("{\"role\":\"assistant\",\"content\":\"text\",\"refusal\":\"I can't help\"}"), &Message));
                    TestTrueExpr(Message.Refusal.IsSet);
                    TestTrueExpr(Message.Refusal.Value.Equals(TEXT("I can't help")));
                    TestTrueExpr(Message.Content.Equals(TEXT("text")));

                    FVectorStore VectorStore;
                    TestTrueExpr(UJsonFuncLib::ParseJSONToStruct(
                        TEXT("{\"id\":\"vs_1\",\"expires_at\":1700000000,\"name\":\"store\"}"), &VectorStore));
                    TestTrueExpr(VectorStore.Expires_At.IsSet);
                    TestTrueExpr(VectorStore.Expires_At.Value == 1700000000);
                    TestTrueExpr(VectorStore.Name.Equals(TEXT("store")));
                });

            It("ParseJSONToStructShouldKeepOptionalsUnsetForNull",
                [this]()
                {
                    FMessage Message;
                    TestTrueExpr(UJsonFuncLib::ParseJSONToStruct(TEXT("{\"role\":\"assistant\",\"refusal\":null}"), &Message));
                    TestTrueExpr(!Message.Refusal.IsSet);
                });

            It("ParseJSONToStructShouldFailOnTypeMismatch",
                [this]()
                {
                    // FJsonObjectConverter logs the failed import as an error
                    AddExpectedMessage(TEXT("Unable to import JSON"), EAutomationExpectedErrorFlags::Contains, 0);

                    FVectorStore VectorStore;
                    TestFalseExpr(UJsonFuncLib::ParseJSONToStruct(TEXT("{\"id\":\"vs_1\",\"file_counts\":[\"bad\"]}"), &VectorStore));
                    TestFalseExpr(UJsonFuncLib::ParseJSONToStruct(TEXT("not json"), &VectorStore));
                });

            It("ParseJSONToStructShouldParseStreamToolCallsArray",
                [this]()
                {
                    FChatCompletionStreamResponse StreamResponse;
                    TestTrueExpr(UJsonFuncLib::ParseJSONToStruct(
                        TEXT("{\"id\":\"chatcmpl-1\",\"choices\":[{\"index\":0,\"delta\":{\"tool_calls\":[{\"index\":0,\"id\":\"call_1\","
                             "\"type\":\"function\",\"function\":{\"name\":\"get_weather\",\"arguments\":\"{\\\"city\\\"\"}}]},"
                             "\"finish_reason\":null}]}"),
                        &StreamResponse));
                    TestTrueExpr(StreamResponse.Choices.Num() == 1);

                    const auto& ToolCalls = StreamResponse.Choices[0].Delta.Tool_Calls;
                    TestTrueExpr(ToolCalls.Num() == 1);
                    if (ToolCalls.Num() != 1) return;

                    TestTrueExpr(ToolCalls[0].Index == 0);
                    TestTrueExpr(ToolCalls[0].ID.Equals(TEXT("call_1")));
                    TestTrueExpr(ToolCalls[0].Function.Name.Equals(TEXT("get_weather")));
                    TestTrueExpr(ToolCalls[0].Function.Arguments.Equals(TEXT("{\"city\"")));
                });

            It("ParseJSONToStructShouldFillNumberMaps",
                [this]()
                {
                    FCompletionLogProbs LogProbs;
                    TestTrueExpr(UJsonFuncLib::ParseJSONToStruct(
                        TEXT("{\"tokens\":[\"Hi\"],\"top_logprobs\":[{\"Hi\":-0.25,\"Hello\":-1.5}]}"), &LogProbs));
                    TestTrueExpr(LogProbs.Top_Logprobs.Num() == 1);
                    if (LogProbs.Top_Logprobs.Num() != 1) return;

                    const auto& Values = LogProbs.Top_Logprobs[0].Values;
                    TestTrueExpr(Values.Num() == 2);
                    TestTrueExpr(Values.Contains(TEXT("Hi")) && FMath::IsNearlyEqual(Values[TEXT("Hi")], -0.25));
                    TestTrueExpr(Values.Contains(TEXT("Hello")) && FMath::IsNearlyEqual(Values[TEXT("Hello")], -1.5));
                });

            It("ParseJSONToStructShouldParseVideoSecondsAsString",
                [this]()
                {
                    FVideoObject Video;
                    TestTrueExpr(UJsonFuncLib::ParseJSONToStruct(TEXT("{\"id\":\"video_1\",\"seconds\":\"8\"}"), &Video));
                    TestTrueExpr(Video.Seconds.Equals(TEXT("8")));
                });
        });
}

#endif

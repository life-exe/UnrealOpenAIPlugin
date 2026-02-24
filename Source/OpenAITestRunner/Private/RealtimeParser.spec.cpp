// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/RealtimeTypes.h"
#include "FuncLib/JsonFuncLib.h"
#include "JsonObjectConverter.h"

DEFINE_SPEC(FRealtimeParserTest, "OpenAI.Parser.Realtime",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FRealtimeParserTest::Define()
{
    Describe("JSON",
        [this]()
        {
            It("CreateRealtimeClientSecretShouldBeCorrect",
                [this]()
                {
                    FCreateRealtimeClientSecret CreateSecret;
                    CreateSecret.Expires_After.Seconds = 3600;
                    CreateSecret.Session.Model = "gpt-4o-realtime-preview";

                    CreateSecret.Session.Audio.Input.Turn_Detection.Create_Response.Set(true);
                    CreateSecret.Session.Audio.Input.Turn_Detection.Threshold.Set(0.6f);
                    CreateSecret.Session.Audio.Input.Turn_Detection.IsSet = true;
                    CreateSecret.Session.Audio.IsSet = true;

                    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(CreateSecret);
                    UJsonFuncLib::RemoveEmptyArrays(Json);
                    FString Result;
                    UJsonFuncLib::JsonToString(Json, Result);
                    Result = UJsonFuncLib::RemoveOptionalValuesThatNotSet(Result);

                    AddInfo(FString::Format(TEXT("Post-processed JSON: {0}"), {Result}));

                    TSharedPtr<FJsonObject> ParsedJson;
                    UJsonFuncLib::StringToJson(Result, ParsedJson);

                    auto Expires = ParsedJson->GetObjectField(TEXT("expires_after"));
                    TestTrueExpr(Expires->GetNumberField(TEXT("seconds")) == 3600);

                    auto Session = ParsedJson->GetObjectField(TEXT("session"));
                    TestTrueExpr(Session->GetStringField(TEXT("model")).Equals(TEXT("gpt-4o-realtime-preview")));

                    auto AudioInput = Session->GetObjectField(TEXT("audio"))->GetObjectField(TEXT("input"));
                    auto TurnDetection = AudioInput->GetObjectField(TEXT("turn_detection"));
                    TestTrueExpr(TurnDetection->GetBoolField(TEXT("create_response")));
                    TestTrueExpr(TurnDetection->GetNumberField(TEXT("threshold")) == 0.6f);
                });
        });
}

#endif

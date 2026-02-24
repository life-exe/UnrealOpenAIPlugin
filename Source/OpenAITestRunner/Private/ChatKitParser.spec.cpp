// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/ChatKitTypes.h"
#include "FuncLib/JsonFuncLib.h"
#include "JsonObjectConverter.h"

DEFINE_SPEC(FChatKitParserTest, "OpenAI.Parser.ChatKit",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FChatKitParserTest::Define()
{
    Describe("JSON",
        [this]()
        {
            It("CreateChatKitSessionShouldBeCorrect",
                [this]()
                {
                    FCreateChatKitSession CreateSession;
                    CreateSession.User = "user_123";
                    CreateSession.Workflow.Id = "wf_456";
                    CreateSession.Workflow.Version.Set("1.0.0");
                    CreateSession.Workflow.State_Variables.Add("key1", "val1");

                    CreateSession.Chatkit_Configuration.Automatic_Thread_Titling.Enabled.Set(true);
                    CreateSession.Chatkit_Configuration.File_Upload.Enabled.Set(false);
                    CreateSession.Chatkit_Configuration.File_Upload.Max_Files.Set(5);

                    CreateSession.Expires_After.Seconds = 3600;

                    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(CreateSession);
                    UJsonFuncLib::RemoveEmptyArrays(Json);
                    FString Result;
                    UJsonFuncLib::JsonToString(Json, Result);
                    Result = UJsonFuncLib::RemoveOptionalValuesThatNotSet(Result);

                    TSharedPtr<FJsonObject> ParsedJson;
                    UJsonFuncLib::StringToJson(Result, ParsedJson);

                    TestTrueExpr(ParsedJson->GetStringField(TEXT("user")).Equals(TEXT("user_123")));

                    auto Workflow = ParsedJson->GetObjectField(TEXT("workflow"));
                    TestTrueExpr(Workflow->GetStringField(TEXT("id")).Equals(TEXT("wf_456")));
                    TestTrueExpr(Workflow->GetStringField(TEXT("version")).Equals(TEXT("1.0.0")));
                    TestTrueExpr(Workflow->GetObjectField(TEXT("state_variables"))->GetStringField(TEXT("key1")).Equals(TEXT("val1")));

                    auto Config = ParsedJson->GetObjectField(TEXT("chatkit_configuration"));
                    TestTrueExpr(Config->GetObjectField(TEXT("automatic_thread_titling"))->GetBoolField(TEXT("enabled")));
                    TestFalseExpr(Config->GetObjectField(TEXT("file_upload"))->GetBoolField(TEXT("enabled")));
                    TestTrueExpr(Config->GetObjectField(TEXT("file_upload"))->GetNumberField(TEXT("max_files")) == 5);

                    auto Expires = ParsedJson->GetObjectField(TEXT("expires_after"));
                    TestTrueExpr(Expires->GetNumberField(TEXT("seconds")) == 3600);
                    TestTrueExpr(Expires->GetStringField(TEXT("anchor")).Equals(TEXT("created_at")));
                });
        });
}

#endif

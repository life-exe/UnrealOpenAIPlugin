// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/JsonParsers/EvalParser.h"
#include "Provider/Types/EvalTypes.h"
#include "FuncLib/JsonFuncLib.h"

DEFINE_SPEC(FEvalParserTest, "OpenAI.Parser.Eval",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FEvalParserTest::Define()
{
    Describe("JSON",
        [this]()
        {
            It("CreateEvalLabelModelGraderShouldBeCorrect",
                [this]()
                {
                    FCreateEval CreateEval;
                    CreateEval.Name = "Test Eval";
                    CreateEval.Data_Source_Config.Type = "custom";
                    CreateEval.Data_Source_Config.Item_Schema = TEXT("{\"type\":\"object\"}");

                    FEvalGrader Grader;
                    Grader.Type = "label_model";
                    Grader.Name = "Label Grader";
                    Grader.Model = "gpt-4o";
                    Grader.Labels = {"pass", "fail"};
                    Grader.Passing_Labels = {"pass"};

                    FEvalMessage Msg;
                    Msg.Role = "user";
                    Msg.Content = "test";
                    Grader.Input.Add(Msg);

                    CreateEval.Testing_Criteria.Add(Grader);

                    const FString Result = OpenAI::EvalParser::CreateEvalToJsonRepresentation(CreateEval);

                    TSharedPtr<FJsonObject> Json;
                    UJsonFuncLib::StringToJson(Result, Json);

                    TestTrueExpr(Json->GetStringField(TEXT("name")).Equals(TEXT("Test Eval")));

                    auto Config = Json->GetObjectField(TEXT("data_source_config"));
                    TestTrueExpr(Config->GetStringField(TEXT("type")).Equals(TEXT("custom")));
                    TestTrueExpr(Config->HasField(TEXT("item_schema")));
                    TestTrueExpr(Config->GetObjectField(TEXT("item_schema"))->GetStringField(TEXT("type")).Equals(TEXT("object")));

                    auto Criteria = Json->GetArrayField(TEXT("testing_criteria"));
                    TestTrueExpr(Criteria.Num() == 1);
                    auto GraderJson = Criteria[0]->AsObject();
                    TestTrueExpr(GraderJson->GetStringField(TEXT("type")).Equals(TEXT("label_model")));
                    TestTrueExpr(GraderJson->HasField(TEXT("input")));
                    TestTrueExpr(GraderJson->GetArrayField(TEXT("input")).Num() == 1);
                    TestFalseExpr(GraderJson->HasField(TEXT("input_string")));
                    TestFalseExpr(GraderJson->HasField(TEXT("operation")));
                });

            It("CreateEvalStringCheckGraderShouldBeCorrect",
                [this]()
                {
                    FCreateEval CreateEval;
                    CreateEval.Name = "Test Eval 2";
                    CreateEval.Data_Source_Config.Type = "logs";
                    CreateEval.Data_Source_Config.Metadata.Add("foo", "bar");

                    FEvalGrader Grader;
                    Grader.Type = "string_check";
                    Grader.Name = "String Grader";
                    Grader.Input_String = "{{sample.output}}";
                    Grader.Operation = "eq";
                    Grader.Reference = "exact match";

                    CreateEval.Testing_Criteria.Add(Grader);

                    const FString Result = OpenAI::EvalParser::CreateEvalToJsonRepresentation(CreateEval);

                    TSharedPtr<FJsonObject> Json;
                    UJsonFuncLib::StringToJson(Result, Json);

                    auto Config = Json->GetObjectField(TEXT("data_source_config"));
                    TestTrueExpr(Config->GetStringField(TEXT("type")).Equals(TEXT("logs")));
                    TestTrueExpr(Config->HasField(TEXT("metadata")));
                    TestTrueExpr(Config->GetObjectField(TEXT("metadata"))->GetStringField(TEXT("foo")).Equals(TEXT("bar")));

                    auto Criteria = Json->GetArrayField(TEXT("testing_criteria"));
                    auto GraderJson = Criteria[0]->AsObject();
                    TestTrueExpr(GraderJson->GetStringField(TEXT("type")).Equals(TEXT("string_check")));
                    TestTrueExpr(GraderJson->GetStringField(TEXT("input")).Equals(TEXT("{{sample.output}}")));
                    TestFalseExpr(GraderJson->HasField(TEXT("input_string")));
                    TestTrueExpr(GraderJson->GetStringField(TEXT("operation")).Equals(TEXT("eq")));
                    TestTrueExpr(GraderJson->GetStringField(TEXT("reference")).Equals(TEXT("exact match")));
                    TestFalseExpr(GraderJson->HasField(TEXT("model")));
                });
        });
}

#endif

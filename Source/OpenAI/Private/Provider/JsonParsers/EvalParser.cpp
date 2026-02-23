// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Provider/JsonParsers/EvalParser.h"
#include "JsonObjectConverter.h"
#include "FuncLib/JsonFuncLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

using namespace OpenAI;

namespace
{
void CleanEvalDataSourceConfig(const FEvalDataSourceConfig& Config, TSharedPtr<FJsonObject>& Json)
{
    if (Config.Type.Equals("custom"))
    {
        Json->RemoveField(TEXT("Metadata"));

        // Parse Item_Schema string to Json Object for outgoing request
        if (Json->HasField(TEXT("Item_Schema")))
        {
            const FString SchemaStr = Json->GetStringField(TEXT("Item_Schema"));
            if (!SchemaStr.IsEmpty())
            {
                TSharedPtr<FJsonObject> SchemaObj;
                TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(SchemaStr);
                if (FJsonSerializer::Deserialize(Reader, SchemaObj))
                {
                    Json->SetObjectField(TEXT("Item_Schema"), SchemaObj);
                }
            }
        }
    }
    else
    {
        Json->RemoveField(TEXT("Item_Schema"));
        Json->RemoveField(TEXT("Include_Sample_Schema"));
    }
}

void CleanTestingCriteria(const TArray<FEvalGrader>& Criteria, TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
    for (int32 i = 0; i < Criteria.Num(); ++i)
    {
        const auto& Grader = Criteria[i];
        auto GraderObj = JsonArray[i]->AsObject();

        if (Grader.Type.Equals("label_model"))
        {
            GraderObj->RemoveField(TEXT("Input_String"));
            GraderObj->RemoveField(TEXT("Operation"));
            GraderObj->RemoveField(TEXT("Reference"));
            GraderObj->RemoveField(TEXT("Pass_Threshold"));

            // API expects SimpleInputMessage format (no "type" field) in Input array
            if (GraderObj->HasField(TEXT("Input")))
            {
                auto InputArray = GraderObj->GetArrayField(TEXT("Input"));
                for (auto& MsgVal : InputArray)
                {
                    auto MsgObj = MsgVal->AsObject();
                    if (MsgObj.IsValid())
                    {
                        MsgObj->RemoveField(TEXT("Type"));
                    }
                }
                GraderObj->SetArrayField(TEXT("Input"), InputArray);
            }
        }
        else if (Grader.Type.Equals("string_check"))
        {
            GraderObj->RemoveField(TEXT("Input"));  // Remove the array
            if (GraderObj->HasField(TEXT("Input_String")))
            {
                GraderObj->SetField(TEXT("Input"), GraderObj->TryGetField(TEXT("Input_String")));
            }
            GraderObj->RemoveField(TEXT("Input_String"));

            GraderObj->RemoveField(TEXT("Labels"));
            GraderObj->RemoveField(TEXT("Model"));
            GraderObj->RemoveField(TEXT("Passing_Labels"));
            GraderObj->RemoveField(TEXT("Pass_Threshold"));
        }
        else
        {
            // TextSimilarityGrader / PythonGrader / ScoreModelGrader
            GraderObj->RemoveField(TEXT("Input"));
            GraderObj->RemoveField(TEXT("Input_String"));
            GraderObj->RemoveField(TEXT("Labels"));
            GraderObj->RemoveField(TEXT("Model"));
            GraderObj->RemoveField(TEXT("Passing_Labels"));
            GraderObj->RemoveField(TEXT("Operation"));
            GraderObj->RemoveField(TEXT("Reference"));
        }
    }
}
}  // namespace

FString EvalParser::CreateEvalToJsonRepresentation(const FCreateEval& CreateEval)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(CreateEval);
    if (!Json.IsValid()) return {};

    UJsonFuncLib::RemoveEmptyArrays(Json);

    if (Json->HasField(TEXT("Data_Source_Config")))
    {
        auto ConfigJson = Json->GetObjectField(TEXT("Data_Source_Config"));
        CleanEvalDataSourceConfig(CreateEval.Data_Source_Config, ConfigJson);
    }

    if (Json->HasField(TEXT("Testing_Criteria")))
    {
        auto CriteriaArray = Json->GetArrayField(TEXT("Testing_Criteria"));
        CleanTestingCriteria(CreateEval.Testing_Criteria, CriteriaArray);
        Json->SetArrayField(TEXT("Testing_Criteria"), CriteriaArray);
    }

    if (CreateEval.Metadata.IsEmpty())
    {
        Json->RemoveField(TEXT("Metadata"));
    }

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    return TransformedString;
}

FString EvalParser::UpdateEvalToJsonRepresentation(const FUpdateEval& UpdateEval)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(UpdateEval);
    if (!Json.IsValid()) return {};

    if (UpdateEval.Metadata.IsEmpty())
    {
        Json->RemoveField(TEXT("Metadata"));
    }

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    return TransformedString;
}

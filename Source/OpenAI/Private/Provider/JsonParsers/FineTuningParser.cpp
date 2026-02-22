// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "Provider/JsonParsers/FineTuningParser.h"
#include "JsonObjectConverter.h"
#include "FuncLib/JsonFuncLib.h"

using namespace OpenAI;

FString FineTuningParser::FineTuningJobToJsonRepresentation(const FFineTuningJob& FineTuningJob)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(FineTuningJob);
    UJsonFuncLib::RemoveEmptyArrays(Json);

    if (FineTuningJob.Metadata.IsEmpty())
    {
        Json->RemoveField(TEXT("Metadata"));
    }

    FString Result;
    UJsonFuncLib::JsonToString(Json, Result);
    Result = UJsonFuncLib::RemoveOptionalValuesThatNotSet(Result);

    return Result;
}

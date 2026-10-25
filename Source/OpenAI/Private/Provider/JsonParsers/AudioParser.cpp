// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "Provider/JsonParsers/AudioParser.h"
#include "FuncLib/JsonFuncLib.h"

using namespace OpenAI;

bool AudioParser::IsVerboseResponse(const FString& ResponseString)
{
    TSharedPtr<FJsonObject> Json;
    if (!UJsonFuncLib::StringToJson(ResponseString, Json) || !Json.IsValid()) return false;

    // verbose_json has these top-level fields, json has only text and usage
    return Json->HasField(TEXT("segments")) || Json->HasField(TEXT("words")) || Json->HasField(TEXT("duration"));
}

bool AudioParser::IsPlainTextContentType(const FString& ContentType)
{
    // text, srt and vtt response formats
    return !ContentType.IsEmpty() && !ContentType.Contains(TEXT("json"));
}

// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/JsonFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonFuncLib, All, All);

namespace
{
void ProcessJsonObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    TArray<FString> FieldNames;
    JsonObject->Values.GetKeys(FieldNames);

    for (const FString& FieldName : FieldNames)
    {
        const TSharedPtr<FJsonObject>* FieldObject = nullptr;
        const bool IsObject = JsonObject->TryGetObjectField(FStringView(FieldName), FieldObject);

        if (IsObject && FieldObject->IsValid())
        {
            if (FieldObject->Get()->HasField(TEXT("isset")))
            {
                const bool IsSet = FieldObject->Get()->GetBoolField(TEXT("isset"));
                if (IsSet)
                {
                    const auto Value = FieldObject->Get()->TryGetField(TEXT("value"));
                    JsonObject->RemoveField(FieldName);
                    JsonObject->SetField(FieldName, Value);
                }
                else
                {
                    JsonObject->RemoveField(FieldName);
                }
            }
            else
            {
                // Recursively process nested objects
                ProcessJsonObject(*FieldObject);
            }
        }

        // Handle arrays of objects
        const TArray<TSharedPtr<FJsonValue>>* FieldArray = nullptr;
        const bool IsArray = JsonObject->TryGetArrayField(FStringView(FieldName), FieldArray);

        if (IsArray && FieldArray)
        {
            for (const auto& Element : *FieldArray)
            {
                if (Element->Type == EJson::Object)
                {
                    TSharedPtr<FJsonObject> ArrayElementObject = Element->AsObject();
                    if (ArrayElementObject.IsValid())
                    {
                        ProcessJsonObject(ArrayElementObject);  // Recursively process array objects
                    }
                }
            }
        }
    }
}

void ConvertKeysToLowercaseRecursive(TSharedPtr<FJsonValue> Value);

void ConvertObjectKeysToLowercase(TSharedPtr<FJsonObject> JsonObject)
{
    TSharedPtr<FJsonObject> NewJsonObject = MakeShareable(new FJsonObject);

    for (const auto& Elem : JsonObject->Values)
    {
        const FString LowerKey = Elem.Key.ToLower();
        ConvertKeysToLowercaseRecursive(Elem.Value);
        NewJsonObject->SetField(LowerKey, Elem.Value);
    }

    *JsonObject = *NewJsonObject;
}

void ConvertKeysToLowercaseRecursive(TSharedPtr<FJsonValue> Value)
{
    if (Value->Type == EJson::Object)
    {
        ConvertObjectKeysToLowercase(Value->AsObject());
    }
    else if (Value->Type == EJson::Array)
    {
        const TArray<TSharedPtr<FJsonValue>>& Array = Value->AsArray();
        for (const auto& Item : Array)
        {
            ConvertKeysToLowercaseRecursive(Item);
        }
    }
}
}  // namespace

// we need two markes to make clean JSON object during request serialization
// basically to remove quotes

const FString UJsonFuncLib::START_FUNCTION_OBJECT_MARKER = "START_FUNCTION_OBJECT_MARKER";
const FString UJsonFuncLib::END_FUNCTION_OBJECT_MARKER = "END_FUNCTION_OBJECT_MARKER";

bool UJsonFuncLib::StringToJson(const FString& JsonString, TSharedPtr<FJsonObject>& JsonObject)
{
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
    return FJsonSerializer::Deserialize(JsonReader, JsonObject);
}

FString UJsonFuncLib::MakeFunctionsString(const TSharedPtr<FJsonObject>& Json)
{
    FString Functions;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Functions);
    FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

    Functions.Append(END_FUNCTION_OBJECT_MARKER);
    Functions = START_FUNCTION_OBJECT_MARKER + Functions;

    return UOpenAIFuncLib::RemoveWhiteSpaces(Functions);
}

bool UJsonFuncLib::JsonToString(const TSharedPtr<FJsonObject>& JsonObject, FString& JsonString)
{
    TSharedPtr<FJsonObject> NewJsonObject = JsonObject;
    ConvertObjectKeysToLowercase(NewJsonObject);
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
}

FString UJsonFuncLib::CleanUpFunctionsObject(const FString& Input)
{
    FString Output{Input};
    const FString StartMarker = FString::Format(TEXT("\"{0}"), {START_FUNCTION_OBJECT_MARKER});
    const FString EndMarker = FString::Format(TEXT("{0}\""), {END_FUNCTION_OBJECT_MARKER});

    const auto Find = [&](const FString& Str, int32 StartIndex)
    { return Output.Find(Str, ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex); };

    int32 StartIndex = Find(StartMarker, 0);
    int32 EndIndex = Find(EndMarker, StartIndex);

    while (StartIndex != INDEX_NONE && EndIndex != INDEX_NONE)
    {
        int32 ContentStart = StartIndex + StartMarker.Len();
        int32 ContentEnd = EndIndex;

        // Extract the substring that needs to have backslashes removed
        FString ContentToClean = Output.Mid(ContentStart, ContentEnd - ContentStart);
        // Replace backslashes within the extracted content
        FString CleanedContent = ContentToClean.Replace(TEXT("\\"), TEXT(""));
        // Replace old content with cleaned content
        Output = Output.Left(ContentStart) + CleanedContent + Output.Mid(EndIndex);

        StartIndex = Find(StartMarker, EndIndex);
        EndIndex = Find(EndMarker, StartIndex);
    }

    Output = Output.Replace(*StartMarker, TEXT(""));
    Output = Output.Replace(*EndMarker, TEXT(""));

    return Output;
}

bool UJsonFuncLib::OpenAIResponseContainsError(const TSharedPtr<FJsonObject>& JsonObject)
{
    if (JsonObject->HasField(TEXT("error")))
    {
        const auto ErrorObject = JsonObject->GetObjectField(TEXT("error"));
        return ErrorObject->HasField(TEXT("type")) && ErrorObject->HasField(TEXT("message")) && ErrorObject->HasField(TEXT("code"));
    }
    return false;
}

FString UJsonFuncLib::PostprocessOptionalValues(const FString& JsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        ProcessJsonObject(JsonObject);

        // Convert the modified JsonObject back to a string
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer)) return OutputString;
    }

    UE_LOGFMT(LogJsonFuncLib, Error, "Error in JSON postprocessing: {0}", JsonString);
    return {};
}

void UJsonFuncLib::ProcessJsonArray(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
    for (int32 i = JsonArray.Num() - 1; i >= 0; --i)
    {
        TSharedPtr<FJsonValue> JsonValue = JsonArray[i];

        if (JsonValue->Type == EJson::Object)
        {
            TSharedPtr<FJsonObject> JsonSubObject = JsonValue->AsObject();
            RemoveEmptyArrays(JsonSubObject);
        }
        else if (JsonValue->Type == EJson::Array)
        {
            const TArray<TSharedPtr<FJsonValue>>& SubArray = JsonValue->AsArray();
            ProcessJsonArray(SubArray);

            // If the sub-array is now empty, remove it from the parent array
            if (SubArray.Num() == 0)
            {
                // JsonArray.RemoveAt(i);
            }
        }
    }
}

void UJsonFuncLib::RemoveEmptyArrays(const TSharedPtr<FJsonObject>& JsonObject)
{
    TMap<FString, TSharedPtr<FJsonValue>>& JsonValues = JsonObject->Values;
    TArray<FString> KeysToRemove;

    for (const auto& Pair : JsonValues)
    {
        TSharedPtr<FJsonValue> JsonValue = Pair.Value;
        if (JsonValue->Type == EJson::Array)
        {
            const TArray<TSharedPtr<FJsonValue>>& JsonArray = JsonValue->AsArray();
            ProcessJsonArray(JsonArray);

            // If the array is empty, mark it for removal
            if (JsonArray.Num() == 0)
            {
                KeysToRemove.Add(Pair.Key);
            }
        }
        // If the value is an object, recursively process it
        else if (JsonValue->Type == EJson::Object)
        {
            TSharedPtr<FJsonObject> JsonSubObject = JsonValue->AsObject();
            RemoveEmptyArrays(JsonSubObject);
        }
    }

    // Remove the empty arrays from the parent object
    for (const FString& Key : KeysToRemove)
    {
        JsonObject->RemoveField(Key);
    }
}
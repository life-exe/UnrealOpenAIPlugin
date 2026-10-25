// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/JsonFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Logging/StructuredLog.h"
#include "Provider/OpenAIOptional.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonFuncLib, All, All);

namespace
{
void ConvertKeysToLowercaseRecursive(TSharedPtr<FJsonValue> Value);

void ConvertObjectKeysToLowercase(TSharedPtr<FJsonObject> JsonObject)
{
    TSharedPtr<FJsonObject> NewJsonObject = MakeShareable(new FJsonObject);

    for (const auto& Elem : JsonObject->Values)
    {
        const FString LowerKey = FString(Elem.Key.ToView()).ToLower();
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

const FJsonObjectConverter::CustomImportCallback& UJsonFuncLib::OptionalFromScalarImportCallback()
{
    static const FJsonObjectConverter::CustomImportCallback Callback = FJsonObjectConverter::CustomImportCallback::CreateLambda(
        [](const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* Value) -> bool
        {
            const FStructProperty* StructProperty = CastField<FStructProperty>(Property);
            // objects ({"value":..,"isset":..}) go through the default conversion
            if (!StructProperty || !JsonValue.IsValid() || JsonValue->Type == EJson::Object) return false;

            const UScriptStruct* Struct = StructProperty->Struct;
            if (Struct == FOptionalString::StaticStruct())
            {
                FString StringValue;
                if (!JsonValue->TryGetString(StringValue)) return false;
                static_cast<FOptionalString*>(Value)->Set(StringValue);
                return true;
            }
            if (Struct == FOptionalInt::StaticStruct())
            {
                int32 IntValue{};
                if (!JsonValue->TryGetNumber(IntValue)) return false;
                static_cast<FOptionalInt*>(Value)->Set(IntValue);
                return true;
            }
            if (Struct == FOptionalFloat::StaticStruct())
            {
                double NumberValue{};
                if (!JsonValue->TryGetNumber(NumberValue)) return false;
                static_cast<FOptionalFloat*>(Value)->Set(static_cast<float>(NumberValue));
                return true;
            }
            if (Struct == FOptionalBool::StaticStruct())
            {
                bool BoolValue{};
                if (!JsonValue->TryGetBool(BoolValue)) return false;
                static_cast<FOptionalBool*>(Value)->Set(BoolValue);
                return true;
            }
            return false;
        });
    return Callback;
}

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
    const TSharedPtr<FJsonObject>* ErrorObject = nullptr;
    if (!JsonObject->TryGetObjectField(FStringView(TEXT("error")), ErrorObject) || !ErrorObject) return false;

    return (*ErrorObject)->HasField(TEXT("type"))        //
           && (*ErrorObject)->HasField(TEXT("message"))  //
           && (*ErrorObject)->HasField(TEXT("code"));
}

FString UJsonFuncLib::RemoveOptionalValuesThatNotSet(const FString& JsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        RemoveOptionalValuesInJsonObject(JsonObject);

        // Convert the modified JsonObject back to a string
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer)) return OutputString;
    }

    UE_LOGFMT(LogJsonFuncLib, Error, "Error in JSON when removing optional values: {0}", JsonString);
    return {};
}

void UJsonFuncLib::RemoveNullFields(const TSharedPtr<FJsonObject>& JsonObject)
{
    TArray<FString> KeysToRemove;

    for (const auto& [Key, Value] : JsonObject->Values)
    {
        if (Value->Type == EJson::Null)
        {
            KeysToRemove.Add(FString(Key.ToView()));
        }
        else if (Value->Type == EJson::Object)
        {
            RemoveNullFields(Value->AsObject());
        }
        else if (Value->Type == EJson::Array)
        {
            for (const auto& Element : Value->AsArray())
            {
                if (Element->Type == EJson::Object)
                {
                    RemoveNullFields(Element->AsObject());
                }
            }
        }
    }

    for (const FString& Key : KeysToRemove)
    {
        JsonObject->RemoveField(Key);
    }
}

void UJsonFuncLib::RemoveOptionalValuesInJsonObject(const TSharedPtr<FJsonObject>& JsonObject)
{
    TArray<FString> FieldNames;
    for (const auto& Elem : JsonObject->Values)
    {
        FieldNames.Add(FString(Elem.Key.ToView()));
    }

    for (const FString& FieldName : FieldNames)
    {
        // Handle objects
        const TSharedPtr<FJsonObject>* FieldObject = nullptr;
        const bool IsObject = JsonObject->TryGetObjectField(FStringView(FieldName), FieldObject);

        if (IsObject && FieldObject->IsValid())
        {
            if (FieldObject->Get()->HasField(TEXT("isset")))
            {
                if (const bool IsSet = FieldObject->Get()->GetBoolField(TEXT("isset")))
                {
                    const TSharedPtr<FJsonValue> Value = FieldObject->Get()->TryGetField(TEXT("value"));
                    if (Value)
                    {
                        // Scalar optional (FOptionalString, FOptionalInt, etc.): replace wrapper object with the value.
                        JsonObject->RemoveField(FieldName);
                        JsonObject->SetField(FieldName, Value);
                    }
                    else
                    {
                        // Struct optional: keep the nested object but strip the "isset" sentinel.
                        FieldObject->Get()->RemoveField(TEXT("isset"));
                        RemoveOptionalValuesInJsonObject(*FieldObject);
                    }
                }
                else
                {
                    JsonObject->RemoveField(FieldName);
                }
            }
            else
            {
                RemoveOptionalValuesInJsonObject(*FieldObject);
            }
        }

        // Handle arrays
        const TArray<TSharedPtr<FJsonValue>>* FieldArray = nullptr;
        const bool IsArray = JsonObject->TryGetArrayField(FStringView(FieldName), FieldArray);

        if (IsArray && FieldArray)
        {
            for (const auto& Element : *FieldArray)
            {
                if (Element->Type == EJson::Object)
                {
                    TSharedPtr<FJsonObject> Object = Element->AsObject();
                    if (Object.IsValid())
                    {
                        RemoveOptionalValuesInJsonObject(Object);
                    }
                }
            }
        }
    }
}

void UJsonFuncLib::RemoveEmptyArrays(const TSharedPtr<FJsonObject>& JsonObject)
{
    TArray<FString> KeysToRemove;
    TMap<FString, TArray<TSharedPtr<FJsonValue>>> UpdatedNotEmptyAtrrays;

    for (const auto& [Key, JsonValue] : JsonObject->Values)
    {
        if (JsonValue->Type == EJson::Array)
        {
            TArray<TSharedPtr<FJsonValue>> MutableArray = JsonValue->AsArray();
            ProcessJsonArrayRemovingEmptyArrays(MutableArray);
            if (MutableArray.Num() == 0)
            {
                KeysToRemove.Add(FString(Key.ToView()));
            }
            else
            {
                UpdatedNotEmptyAtrrays.Add(FString(Key.ToView()), MutableArray);
            }
        }
        else if (JsonValue->Type == EJson::Object)
        {
            RemoveEmptyArrays(JsonValue->AsObject());
        }
    }

    for (const auto& Key : KeysToRemove)
    {
        JsonObject->RemoveField(Key);
    }

    for (const auto& [Key, Array] : UpdatedNotEmptyAtrrays)
    {
        JsonObject->SetArrayField(Key, Array);
    }
}

void UJsonFuncLib::ProcessJsonArrayRemovingEmptyArrays(TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
    for (int32 i = JsonArray.Num() - 1; i >= 0; --i)
    {
        const auto& JsonValue = JsonArray[i];
        if (JsonValue->Type == EJson::Object)
        {
            RemoveEmptyArrays(JsonValue->AsObject());
        }
        else if (JsonValue->Type == EJson::Array)
        {
            auto SubArray = JsonValue->AsArray();
            ProcessJsonArrayRemovingEmptyArrays(SubArray);

            // If the sub-array is now empty, remove it from the parent array
            if (SubArray.Num() == 0)
            {
                JsonArray.RemoveAt(i);
            }
        }
    }
}

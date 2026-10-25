// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Provider/JsonParsers/RealtimeParser.h"
#include "JsonObjectConverter.h"
#include "FuncLib/JsonFuncLib.h"
#include "Serialization/JsonSerializer.h"

using namespace OpenAI;

namespace
{
// Tool parameters are a JSON schema string; send them as an object with the keys as written by the user
void SetToolParameters(const TArray<FRealtimeFunctionTool>& Tools, const TSharedPtr<FJsonObject>& Json)
{
    const TSharedPtr<FJsonObject>* Session = nullptr;
    const TArray<TSharedPtr<FJsonValue>>* ToolsJson = nullptr;
    if (!Json->TryGetObjectField(TEXT("session"), Session) || !(*Session)->TryGetArrayField(TEXT("tools"), ToolsJson)) return;

    for (int32 i = 0; i < Tools.Num() && i < ToolsJson->Num(); ++i)
    {
        const TSharedPtr<FJsonObject> ToolJson = (*ToolsJson)[i]->AsObject();
        if (!ToolJson.IsValid()) continue;

        ToolJson->RemoveField(TEXT("parameters"));
        if (Tools[i].Parameters.IsEmpty()) continue;

        TSharedPtr<FJsonObject> Parameters;
        if (UJsonFuncLib::StringToJson(Tools[i].Parameters, Parameters) && Parameters.IsValid())
        {
            ToolJson->SetObjectField(TEXT("parameters"), Parameters);
        }
    }
}
}  // namespace

FString RealtimeParser::CreateClientSecretToJsonRepresentation(const FCreateRealtimeClientSecret& CreateSecret)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(CreateSecret);
    if (!Json.IsValid()) return {};

    UJsonFuncLib::RemoveEmptyArrays(Json);

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    // JsonToString lowercases all keys, so the parameters are added afterwards
    TSharedPtr<FJsonObject> CleanJson;
    if (!UJsonFuncLib::StringToJson(TransformedString, CleanJson) || !CleanJson.IsValid()) return TransformedString;

    SetToolParameters(CreateSecret.Session.Tools, CleanJson);

    FString OutputString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(CleanJson.ToSharedRef(), Writer);
    return OutputString;
}

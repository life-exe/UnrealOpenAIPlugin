// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonObjectConverter.h"
#include "JsonFuncLib.generated.h"

UCLASS()
class OPENAI_API UJsonFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    static bool StringToJson(const FString& JsonString, TSharedPtr<FJsonObject>& JsonObject);
    static bool JsonToString(const TSharedPtr<FJsonObject>& JsonObject, FString& JsonString);

    template <typename OutStructType>
    static bool ParseJSONToStruct(const FString& Data, OutStructType* OutStruct)
    {
        TSharedPtr<FJsonObject> JsonObject;
        if (!UJsonFuncLib::StringToJson(Data, JsonObject)) return false;

        FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), OutStruct, 0, 0);
        return true;
    }
    static FString RemoveOptionalValuesThatNotSet(const FString& JsonString);
    static void RemoveEmptyArrays(const TSharedPtr<FJsonObject>& JsonObject);

    // helpers for OpeanAI 'functions'
    static FString MakeFunctionsString(const TSharedPtr<FJsonObject>& Json);
    static FString CleanUpFunctionsObject(const FString& Input);

    // errors
    static bool OpenAIResponseContainsError(const TSharedPtr<FJsonObject>& JsonObject);

private:
    static const FString START_FUNCTION_OBJECT_MARKER;
    static const FString END_FUNCTION_OBJECT_MARKER;

    static void RemoveOptionalValuesInJsonObject(const TSharedPtr<FJsonObject>& JsonObject);
    static void ProcessJsonArrayRemovingEmptyArrays(TArray<TSharedPtr<FJsonValue>>& JsonArray);
};
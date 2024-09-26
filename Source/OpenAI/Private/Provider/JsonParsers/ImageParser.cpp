// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Provider/JsonParsers/ImageParser.h"
#include "JsonObjectConverter.h"

using namespace OpenAI;

bool ImageParser::DeserializeResponse(const FString& ResponseString, FImageResponse& ImageResponse)
{
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject)) return false;

    ImageResponse.Created = JsonObject->GetNumberField(TEXT("created"));

    const auto DataArray = JsonObject->GetArrayField(TEXT("data"));
    for (const auto DataElem : DataArray)
    {
        FImageObject ImageObject;
        if (FJsonObjectConverter::JsonObjectToUStruct(DataElem->AsObject().ToSharedRef(), &ImageObject, 0, 0))
        {
            ImageResponse.Data.Push(ImageObject);
        }
    }
    return true;
}

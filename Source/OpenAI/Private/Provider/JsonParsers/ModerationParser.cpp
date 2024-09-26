// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Provider/JsonParsers/ModerationParser.h"
#include "JsonObjectConverter.h"

using namespace OpenAI;

bool ModerationParser::DeserializeResponse(const FString& ResponseString, FModerationsResponse& ModerationResponse)
{
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject)) return false;

    ModerationResponse.ID = JsonObject->GetStringField(TEXT("id"));
    ModerationResponse.Model = JsonObject->GetStringField(TEXT("Model"));

    const auto& ResultsObject = JsonObject->GetArrayField(TEXT("results"));
    for (const auto& ResultObject : ResultsObject)
    {
        const auto& CategoriesObject = ResultObject->AsObject()->GetObjectField(TEXT("categories"));
        FModerationCategories Categories;
        Categories.Hate = CategoriesObject->GetBoolField(TEXT("hate"));
        Categories.Hate_Threatening = CategoriesObject->GetBoolField(TEXT("hate/threatening"));
        Categories.Harassment = CategoriesObject->GetBoolField(TEXT("harassment"));
        Categories.Harassment_Threatening = CategoriesObject->GetBoolField(TEXT("harassment/threatening"));
        Categories.Self_Harm = CategoriesObject->GetBoolField(TEXT("self-harm"));
        Categories.Self_Harm_Intent = CategoriesObject->GetBoolField(TEXT("self-harm/intent"));
        Categories.Self_Harm_Instructions = CategoriesObject->GetBoolField(TEXT("self-harm/instructions"));
        Categories.Sexual = CategoriesObject->GetBoolField(TEXT("sexual"));
        Categories.Sexual_Minors = CategoriesObject->GetBoolField(TEXT("sexual/minors"));
        Categories.Violence = CategoriesObject->GetBoolField(TEXT("violence"));
        Categories.Violence_Graphic = CategoriesObject->GetBoolField(TEXT("violence/graphic"));

        const auto& CategoryScoreObject = ResultObject->AsObject()->GetObjectField(TEXT("category_scores"));
        FModerationCategoryScores CategoryScores;
        CategoryScores.Hate = CategoryScoreObject->GetNumberField(TEXT("hate"));
        CategoryScores.Hate_Threatening = CategoryScoreObject->GetNumberField(TEXT("hate/threatening"));
        CategoryScores.Harassment = CategoriesObject->GetBoolField(TEXT("harassment"));
        CategoryScores.Harassment_Threatening = CategoriesObject->GetBoolField(TEXT("harassment/threatening"));
        CategoryScores.Self_Harm = CategoryScoreObject->GetNumberField(TEXT("self-harm"));
        CategoryScores.Self_Harm_Intent = CategoriesObject->GetBoolField(TEXT("self-harm/intent"));
        CategoryScores.Self_Harm_Instructions = CategoriesObject->GetBoolField(TEXT("self-harm/instructions"));
        CategoryScores.Sexual = CategoryScoreObject->GetNumberField(TEXT("sexual"));
        CategoryScores.Sexual_Minors = CategoryScoreObject->GetNumberField(TEXT("sexual/minors"));
        CategoryScores.Violence = CategoryScoreObject->GetNumberField(TEXT("violence"));
        CategoryScores.Violence_Graphic = CategoryScoreObject->GetNumberField(TEXT("violence/graphic"));

        FModerationResults ModerationResults;
        ModerationResults.Categories = Categories;
        ModerationResults.Category_Scores = CategoryScores;
        ModerationResults.Flagged = ResultObject->AsObject()->GetBoolField(TEXT("flagged"));
        ModerationResponse.Results.Add(ModerationResults);
    }

    return true;
}

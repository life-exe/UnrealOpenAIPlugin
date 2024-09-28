// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Provider/JsonParsers/ChatParser.h"
#include "JsonObjectConverter.h"
#include "FuncLib/JsonFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"

using namespace OpenAI;

namespace
{
void CleanFieldsThatCantBeEmpty(const FChatCompletion& ChatCompletion, TSharedPtr<FJsonObject>& Json)
{
    if (ChatCompletion.Tool_Choice.Function.Name.IsEmpty())
    {
        Json->RemoveField(TEXT("Tool_Choice"));
    }

    if (ChatCompletion.Logit_Bias.IsEmpty())
    {
        Json->RemoveField(TEXT("Logit_Bias"));
    }

    if (ChatCompletion.Model == UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview))
    {
        Json->RemoveField(TEXT("Response_Format"));
    }

    if (Json->GetObjectField(TEXT("Stream_Options"))->GetObjectField(TEXT("Include_Usage"))->GetBoolField(TEXT("isset")) == false)
    {
        Json->RemoveField(TEXT("Stream_Options"));
    }

    for (int32 i = 0; i < ChatCompletion.Messages.Num(); ++i)
    {
        const auto& Message = ChatCompletion.Messages[i];
        auto& MessageObj = Json->GetArrayField(TEXT("Messages"))[i]->AsObject();

        if (!Message.ContentArray.IsEmpty())
        {
            auto Content = MessageObj->GetArrayField(TEXT("ContentArray"));

            for (int32 j = 0; j < Content.Num(); ++j)
            {
                TSharedPtr<FJsonObject> MessageContent = Content[j]->AsObject();
                if (MessageContent->GetStringField(TEXT("Type")).Equals("text"))
                {
                    MessageContent->RemoveField(TEXT("Image_URL"));
                }
                else
                {
                    MessageContent->RemoveField(TEXT("Text"));
                }
            }
            MessageObj->SetArrayField(TEXT("Content"), Content);
            MessageObj->RemoveField(TEXT("ContentArray"));
        }
    }
}
}  // namespace

FString ChatParser::ChatCompletionToJsonRepresentation(const FChatCompletion& ChatCompletion)
{
    TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ChatCompletion);
    UJsonFuncLib::RemoveEmptyArrays(Json);
    CleanFieldsThatCantBeEmpty(ChatCompletion, Json);

    FString TransformedString;
    UJsonFuncLib::JsonToString(Json, TransformedString);

    TransformedString = UJsonFuncLib::CleanUpFunctionsObject(TransformedString);
    TransformedString = UJsonFuncLib::RemoveOptionalValuesThatNotSet(TransformedString);

    return TransformedString;
}

bool ChatParser::CleanChunkResponseString(FString& IncomeString, bool& LastString)
{
    if (IncomeString.StartsWith("data: "))
    {
        IncomeString.RemoveFromStart("data: ");
    }

    // igone role chunck // @todo handle this case in another struct
    // if (IncomeString.Find("role") != INDEX_NONE) return false;
    if (IncomeString.Equals("[DONE]"))
    {
        LastString = true;
    }

    return true;
}

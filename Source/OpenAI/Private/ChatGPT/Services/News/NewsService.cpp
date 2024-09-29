// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/Services/News/NewsService.h"
#include "Provider/Types/CommonTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/JsonFuncLib.h"
#include "Algo/ForEach.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogNewsService, All, All);

/*
   To make the news service work in the GPT widget, you need to
    1. Create an account at https://newsapi.org/
    2. Get ApiKey https://newsapi.org/account It's absolutely free.
    3. Add OnlineServicesAuth.ini to the root of your project.
    4. Add your ApiKey to ini file:
        NewsApiOrgApiKey=sdfhgfdsgbhdgfnhdgfhsgf
*/

namespace News
{
const FString API_URL = "https://newsapi.org/v2/top-headlines";
constexpr int32 MaxNewsAmount = 5;

FString ReplaceSpaces(const FString& Str)
{
    return Str.Replace(TEXT(" "), TEXT("-"));
}
}  // namespace News

bool UNewsService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    return UOpenAIFuncLib::LoadSecretByName(Secrets, "NewsApiOrgApiKey", API_KEY);
}

FString UNewsService::FunctionName() const
{
    return "get_top_headline_news";
}

FString UNewsService::Description() const
{
    return "Get top news headlines by country and/or category";
}

FString UNewsService::MakeFunction() const
{
    /*
    "parameters": {
        "type": "object",
        "properties": {
            "query": {
                "type": "string",
                "description": "Freeform keywords or a phrase to search for.",
            },
            "country": {
                "type": "string",
                "description": "The 2-letter ISO 3166-1 code of the country you want to get headlines for",
            },
            "category": {
                "type": "string",
                "description": "The category you want to get headlines for",
                "enum": ["business","entertainment","general","health","science","sports","technology"]
            }
        },
        "required": [],
    */

    TSharedPtr<FJsonObject> MainObj = MakeShareable(new FJsonObject());
    MainObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());

    // query
    TSharedPtr<FJsonObject> QueryObj = MakeShareable(new FJsonObject());
    QueryObj->SetStringField("type", "string");
    QueryObj->SetStringField("description", "Freeform keywords or a phrase to search for. Convert to English if necessary");
    Props->SetObjectField("query", QueryObj);

    // country
    TSharedPtr<FJsonObject> CountryObj = MakeShareable(new FJsonObject());
    CountryObj->SetStringField("type", "string");
    CountryObj->SetStringField("description", "The 2-letter ISO 3166-1 code of the country you want to get headlines for");
    Props->SetObjectField("country", CountryObj);

    // category
    TSharedPtr<FJsonObject> CategoryObj = MakeShareable(new FJsonObject());
    CategoryObj->SetStringField("type", "string");

    TArray<TSharedPtr<FJsonValue>> EnumArray;
    EnumArray.Add(MakeShareable(new FJsonValueString("entertainment")));
    EnumArray.Add(MakeShareable(new FJsonValueString("general")));
    EnumArray.Add(MakeShareable(new FJsonValueString("science")));
    EnumArray.Add(MakeShareable(new FJsonValueString("technology")));
    EnumArray.Add(MakeShareable(new FJsonValueString("sports")));
    EnumArray.Add(MakeShareable(new FJsonValueString("arts")));
    CategoryObj->SetStringField("description", "The category you want to get headlines for");
    CategoryObj->SetArrayField("enum", EnumArray);
    Props->SetObjectField("category", CategoryObj);

    MainObj->SetObjectField("properties", Props);

    // required params
    TArray<TSharedPtr<FJsonValue>> RequiredArray;
    MainObj->SetArrayField("required", RequiredArray);

    return UJsonFuncLib::MakeFunctionsString(MainObj);
}

void UNewsService::Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolIDIn)
{
    Super::Call(Args, ToolIDIn);

    auto HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetURL(MakeRequestURL(Args));
    HttpRequest->SetVerb("GET");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRequestCompleted);
    HttpRequest->ProcessRequest();
}

FString UNewsService::MakeRequestURL(const TSharedPtr<FJsonObject>& ArgsJson) const
{
    FString ArgsStr;
    if (UJsonFuncLib::JsonToString(ArgsJson, ArgsStr))
    {
        UE_LOG(LogNewsService, Display, TEXT("Args for the news request: %s"), *ArgsStr);
    }

    const OpenAI::QueryPairs QueryPairs{
        {"query", "q"}, {"country", "country"}, {"category", "category"}, {"pageSize", FString::FromInt(News::MaxNewsAmount)}};
    OpenAI::QueryPairs QueryArgs;
    for (const auto& [Field, Query] : QueryPairs)
    {
        const auto FieldObj = ArgsJson->TryGetField(Field);
        if (FieldObj.IsValid())
        {
            QueryArgs.Add(MakeTuple(Query, News::ReplaceSpaces(FieldObj->AsString())));
        }
    }
    QueryArgs.Add(MakeTuple("apiKey", API_KEY));

    const FString URL = UOpenAIFuncLib::MakeURLWithQuery(News::API_URL, QueryArgs);
    UE_LOGFMT(LogNewsService, Display, "News request URL: {0}", URL);
    return URL;
}

void UNewsService::OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Response.IsValid())
    {
        ServiceDataError.Broadcast("Response was null");
        return;
    }
    UE_LOGFMT(LogNewsService, Display, "{0}", Response->GetContentAsString());

    TSharedPtr<FJsonObject> JsonObject;
    if (!UJsonFuncLib::StringToJson(Response->GetContentAsString(), JsonObject))
    {
        SendError("Can't parse response");
        return;
    }

    FNews News;
    if (!UJsonFuncLib::ParseJSONToStruct<FNews>(Response->GetContentAsString(), &News))
    {
        SendError("Can't parse news response");
        return;
    }

    if (!News.Status.Equals("ok"))
    {
        SendError("News parsing error");
        return;
    }

    FString NewsCombined;
    int32 Index = 1;
    Algo::ForEach(News.Articles,
        [&](const FArticle& Article) {                                    //
            NewsCombined.Append(FString::FromInt(Index++)).Append(": ");  // 1:
            const auto Body =
                FString::Format(TEXT("title:{0}, description:{1}, content:{2}"), {Article.Title, Article.Description, Article.Content});
            NewsCombined.Append(Body).Append(LINE_TERMINATOR);
            if (Index > News::MaxNewsAmount) return;
        });

    ServiceDataRecieved.Broadcast(MakeMessage(NewsCombined));
}

void UNewsService::SendError(const FString& ErrorMessage)
{
    UE_LOGFMT(LogNewsService, Error, "{0}", ErrorMessage);
    ServiceDataError.Broadcast(ErrorMessage);
}

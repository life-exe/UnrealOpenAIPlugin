// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/Services/Weather/WeatherService.h"
#include "Provider/Types/CommonTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/JsonFuncLib.h"
#include "Algo/ForEach.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeatherService, All, All);

/*
   To make the weather service work in the GPT widget, you need to
    1. Create an account at https://weatherstack.com/
    2. Get AccessKey. It's absolutely free.
    3. Add OnlineServicesAuth.ini to the root of your project.
    4. Add your AccessKey to ini file:
        WeatherstackAccessKey=sdfhgfdsgbhdgfnhdgfhsgf
*/

namespace Weather
{
const FString API_URL = "http://api.weatherstack.com/current";
}  // namespace Weather

bool UWeatherService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    return UOpenAIFuncLib::LoadSecretByName(Secrets, "WeatherstackAccessKey", API_KEY);
}

FString UWeatherService::FunctionName() const
{
    return "get_current_weather";
}

FString UWeatherService::Description() const
{
    return "Get the current weather in a given location";
}

FString UWeatherService::MakeFunction() const
{
    /*
        "parameters": {
            "type": "object",
            "properties": {
                "location": {
                    "type": "string",
                    "description": "The city and state, e.g. SanFrancisco, CA; Convert city name to English if necessary; don't use spaces
       in the parameter",
                },
                "unit": {"type": "string", "enum": ["celsius", "fahrenheit"]},
            },
            "required": ["location", "unit"],
        }
    */

    TSharedPtr<FJsonObject> MainObj = MakeShareable(new FJsonObject());
    MainObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());

    // location
    TSharedPtr<FJsonObject> LocationObj = MakeShareable(new FJsonObject());
    LocationObj->SetStringField("type", "string");
    LocationObj->SetStringField("description",
        "The city and state, e.g. SanFrancisco, CA; Convert city name to English if necessary; don't use spaces in the parameter");
    Props->SetObjectField("location", LocationObj);

    // unit
    TSharedPtr<FJsonObject> UnitObj = MakeShareable(new FJsonObject());
    UnitObj->SetStringField("type", "string");
    TArray<TSharedPtr<FJsonValue>> EnumArray;
    EnumArray.Add(MakeShareable(new FJsonValueString("celsius")));
    EnumArray.Add(MakeShareable(new FJsonValueString("fahrenheit")));
    UnitObj->SetArrayField("enum", EnumArray);
    Props->SetObjectField("unit", UnitObj);

    MainObj->SetObjectField("properties", Props);

    // required params
    TArray<TSharedPtr<FJsonValue>> RequiredArray;
    RequiredArray.Add(MakeShareable(new FJsonValueString("location")));
    RequiredArray.Add(MakeShareable(new FJsonValueString("unit")));
    MainObj->SetArrayField("required", RequiredArray);

    return UJsonFuncLib::MakeFunctionsString(MainObj);
}

void UWeatherService::Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolIDIn)
{
    Super::Call(Args, ToolIDIn);

    FString URL;
    if (!MakeRequestURL(Args, URL))
    {
        SendError("Most likely an argument parsing problem.");
        return;
    }

    auto HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb("GET");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRequestCompleted);
    HttpRequest->ProcessRequest();
}

bool UWeatherService::MakeRequestURL(const TSharedPtr<FJsonObject>& ArgsJson, FString& WeatherRequestURL) const
{
    // units = f    temperature: Fahrenheit
    // units = m    temperature: Celsius // m - metric system, if you, like me, have been wondering wtf m means (=
    FString Units{"m"};
    if (ArgsJson->TryGetStringField(TEXT("unit"), Units))
    {
        Units = Units.ToLower().Equals("celsius") ? "m" : "f";
    }

    FString Location;
    if (!ArgsJson->TryGetStringField(TEXT("location"), Location))
    {
        return false;
    }

    const OpenAI::QueryPairs QueryArgs{{"access_key", API_KEY}, {"query", Location}, {"units", Units}};
    WeatherRequestURL = UOpenAIFuncLib::MakeURLWithQuery(Weather::API_URL, QueryArgs);
    UE_LOGFMT(LogWeatherService, Display, "Weather reqest URL: {0}", WeatherRequestURL);
    return true;
}

void UWeatherService::OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Response.IsValid())
    {
        ServiceDataError.Broadcast("Response was null");
        return;
    }
    UE_LOGFMT(LogWeatherService, Display, "{0}", Response->GetContentAsString());

    TSharedPtr<FJsonObject> JsonObject;
    if (!UJsonFuncLib::StringToJson(Response->GetContentAsString(), JsonObject))
    {
        SendError("Can't parse response");
        return;
    }

    bool IsSuccess{true};
    if (JsonObject->TryGetBoolField(TEXT("success"), IsSuccess) && !IsSuccess)
    {
        SendError("Service can't provide requested weather");
        return;
    }

    FWeather Weather;
    if (!UJsonFuncLib::ParseJSONToStruct<FWeather>(Response->GetContentAsString(), &Weather))
    {
        SendError("Can't parse weather response");
        return;
    }

    FString FullDescription;
    Algo::ForEach(Weather.Current.Weather_Descriptions, [&](const FString& Description) {  //
        FullDescription.Append(Description);
    });

    const FString Content = FString::Format(TEXT("location:{0}, temperature:{1}, descriptions:{2}"),  //
        {Weather.Location.Name, Weather.Current.Temperature, FullDescription});
    ServiceDataRecieved.Broadcast(MakeMessage(Content));
}

void UWeatherService::SendError(const FString& ErrorMessage)
{
    UE_LOGFMT(LogWeatherService, Error, "{0}", ErrorMessage);
    ServiceDataError.Broadcast(ErrorMessage);
}

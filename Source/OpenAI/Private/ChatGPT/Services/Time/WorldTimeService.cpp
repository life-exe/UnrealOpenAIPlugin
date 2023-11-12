// OpenAI ServiceSample, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/Services/Time/WorldTimeService.h"
#include "Funclib/OpenAIFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldTimeService, All, All);

namespace WorldTime
{
const FString API_URL = "http://worldtimeapi.org/api/";
const FString IP_URL = API_URL + "ip";
const FString TIMEZONE_URL = API_URL + "timezone/";
}  // namespace WorldTime

FString UWorldTimeService::Description() const
{
    return "Get current world time";
}

FString UWorldTimeService::FunctionName() const
{
    return "get_world_time";
}

FString UWorldTimeService::MakeFunction() const
{
    /*
     "parameters": {
         "type": "object",
         "properties":
         {
            "location": {
                "type": "string",
                "description" : "Location in format /:area/:location[/:region], for example: America/Argentina/Salta or Europe/London.
     Location should be empty if you don't know where the user is."
            },
         },
         },
         "required": [],
     }
    */

    TSharedPtr<FJsonObject> ParamsObj = MakeShareable(new FJsonObject());
    ParamsObj->SetStringField("type", "object");

    TSharedPtr<FJsonObject> PropsObj = MakeShareable(new FJsonObject());

    // location
    TSharedPtr<FJsonObject> LocationObj = MakeShareable(new FJsonObject());
    LocationObj->SetStringField("type", "string");
    LocationObj->SetStringField("description", "Location in format /:area/:location[/:region], for example: America/Argentina/Salta or "
                                               "Europe/London. Location should be empty if you don't know where the user is.");
    PropsObj->SetObjectField("location", LocationObj);

    ParamsObj->SetObjectField("properties", PropsObj);

    TArray<TSharedPtr<FJsonValue>> RequiredArray;
    ParamsObj->SetArrayField("required", RequiredArray);

    return UOpenAIFuncLib::MakeFunctionsString(ParamsObj);
}

void UWorldTimeService::Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolIDIn)
{
    Super::Call(Args, ToolIDIn);

    FString Location{};
    Args->TryGetStringField("location", Location);

    const FString URL = Location.IsEmpty() ? WorldTime::IP_URL : WorldTime::TIMEZONE_URL + Location;
    UE_LOG(LogWorldTimeService, Display, TEXT("URL: %s"), *URL);

    auto HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb("GET");
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ThisClass::OnRequestCompleted);
    HttpRequest->ProcessRequest();
}

void UWorldTimeService::OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
    if (!Response)
    {
        ServiceDataError.Broadcast("Response was null");
        return;
    }

    FWorldTime WorldTime;
    if (!UOpenAIFuncLib::ParseJSONToStruct<FWorldTime>(Response->GetContentAsString(), &WorldTime))
    {
        const FString ErroStr = FString::Format(TEXT("Can't parse JSON: {0}"), {Response->GetContentAsString()});
        UE_LOG(LogWorldTimeService, Display, TEXT("%s"), *ErroStr);
        ServiceDataError.Broadcast(ErroStr);
        return;
    }

    const FString InfoToOpenAI = FString::Format(TEXT("DateTime: {0}, Timezone: {1}"), {WorldTime.DateTime, WorldTime.TimeZone});
    UE_LOG(LogWorldTimeService, Display, TEXT("InfoToOpenAI: %s"), *InfoToOpenAI);

    ServiceDataRecieved.Broadcast(MakeMessage(InfoToOpenAI));
}

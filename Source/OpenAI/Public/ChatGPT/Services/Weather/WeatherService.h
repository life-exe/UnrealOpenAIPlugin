// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "HTTP.h"
#include "WeatherService.generated.h"

USTRUCT()
struct FWeatherLocation
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;  // city

    UPROPERTY()
    FString Country;
};

USTRUCT()
struct FCurrentWeather
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    double Temperature{0.0};

    UPROPERTY()
    TArray<FString> Weather_Descriptions;
};

USTRUCT()
struct FWeather
{
    GENERATED_BODY()

    UPROPERTY()
    FWeatherLocation Location;

    UPROPERTY()
    FCurrentWeather Current;

    // @todo: more fields can be added
    // https://weatherstack.com/documentation
};

UCLASS()
class OPENAI_API UWeatherService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) override;
    virtual FString Name() const override { return "Weather"; }
    virtual FString TooltipDescription() const override { return "Weather from https://weatherstack.com"; }
    virtual FString Description() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolID) override;

protected:
    virtual FString MakeFunction() const;

private:
    FString API_KEY{};

    bool MakeRequestURL(const TSharedPtr<FJsonObject>& ArgsJson, FString& WeatherRequestURL) const;
    void OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    void SendError(const FString& ErrorMessage);
};

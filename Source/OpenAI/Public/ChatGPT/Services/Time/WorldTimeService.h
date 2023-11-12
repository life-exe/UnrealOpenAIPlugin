// OpenAI ServiceSample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "HTTP.h"
#include "WorldTimeService.generated.h"

USTRUCT()
struct FWorldTime
{
    GENERATED_BODY()

    UPROPERTY()
    FString DateTime;  // "2023-11-05T21:57:12.151253+00:00"

    UPROPERTY()
    FString TimeZone;  // Europe/London

    UPROPERTY()
    FString UTC_DateTime;  // "2023-11-05T21:57:12.151253+00:00"

    UPROPERTY()
    uint32 Week_Number{};

    // @todo: more fields can be added
    // http://worldtimeapi.org
};

UCLASS()
class OPENAI_API UWorldTimeService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) override { return true; }

    virtual FString Description() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolID) override;

    virtual FString Name() const override { return "Time"; };
    virtual FString TooltipDescription() const override { return "World time from http://worldtimeapi.org"; };

protected:
    virtual FString MakeFunction() const override;

private:
    void OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
};

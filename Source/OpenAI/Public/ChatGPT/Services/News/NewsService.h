// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "HTTP.h"
#include "NewsService.generated.h"

USTRUCT()
struct FArticle
{
    GENERATED_BODY()

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    FString Content;
};

USTRUCT()
struct FNews
{
    GENERATED_BODY()

    UPROPERTY()
    FString Status;

    UPROPERTY()
    TArray<FArticle> Articles;

    // @todo: more fields can be added
    // https://newsapi.org/docs/endpoints/top-headlines
};

UCLASS()
class OPENAI_API UNewsService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) override;
    virtual FString Name() const override { return "News"; }
    virtual FString TooltipDescription() const override { return "News headlines from https://newsapi.org"; }
    virtual FString Description() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolID) override;

protected:
    virtual FString MakeFunction() const;

private:
    FString API_KEY{};

    FString MakeRequestURL(const TSharedPtr<FJsonObject>& ArgsJson) const;
    void OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
    void SendError(const FString& ErrorMessage);
};
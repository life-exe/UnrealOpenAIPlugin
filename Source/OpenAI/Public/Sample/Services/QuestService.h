// OpenAI ServiceSample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChatGPT/BaseService.h"
#include "QuestService.generated.h"

USTRUCT()
struct FAlienRampageGameProps
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    bool Ability{false};
};

UCLASS()
class OPENAI_API UQuestService : public UBaseService
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) override { return true; };
    virtual FString Name() const override { return "Quest"; };
    virtual FString TooltipDescription() const override { return "Alien Rampage Saga"; };

    virtual FString Description() const override;
    virtual FString FunctionName() const override;
    virtual void Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolID) override;

protected:
    virtual FString MakeFunction() const override;
};

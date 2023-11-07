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
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets) { return true; };
    virtual FString Name() const { return "Quest"; };
    virtual FString TooltipDescription() const { return "Alien Rampage Saga"; };

    virtual FString Description() const;
    virtual FString FunctionName() const;
    virtual void Call(const TSharedPtr<FJsonObject>& Args);

protected:
    virtual FString MakeFunction() const;
};

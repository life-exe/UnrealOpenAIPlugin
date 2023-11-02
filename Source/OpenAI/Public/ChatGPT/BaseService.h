// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Provider/RequestTypes.h"
#include "BaseService.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnServiceDataRecieved, const FMessage&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnServiceDataError, const FString&);

UCLASS()
class OPENAI_API UBaseService : public UObject
{
    GENERATED_BODY()

public:
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets);

    // Don't create long names. They will be truncated in the UI to [NameMaxLength = 8]. Use Description for more words
    virtual FString Name() const;

    // Comes to UI tooltip
    virtual FString Description() const;

    virtual FFunctionOpenAI Function() const;
    virtual FString FunctionName() const;
    virtual void Call(const TSharedPtr<FJsonObject>& Args);

    virtual FOnServiceDataRecieved& OnServiceDataRecieved();
    virtual FOnServiceDataError& OnServiceDataError();

protected:
    FOnServiceDataRecieved ServiceDataRecieved;
    FOnServiceDataError ServiceDataError;
};
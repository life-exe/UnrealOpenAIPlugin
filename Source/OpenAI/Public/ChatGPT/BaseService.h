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
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets)
    {
        checkf(false, TEXT("You must override the 'Init' function in your service class that derives from the UBaseService class."));
        return false;
    }

    // Don't create long names. They will be truncated in the UI to [NameMaxLength = 8]. Use Description for more words
    virtual FString Name() const
    {
        checkf(false, TEXT("You must override the 'Name' function in your service class that derives from the UBaseService class."));
        return "Unknown service";
    }

    // Comes to UI tooltip
    virtual FString Description() const
    {
        // Can be empty
        return "";
    }

    virtual FFunctionOpenAI Function() const
    {
        checkf(false, TEXT("You must override the 'Function' function in your service class that derives from the UBaseService class."));
        return {};
    }

    virtual FString FunctionName() const
    {
        checkf(
            false, TEXT("You must override the 'FunctionName' function in your service class that derives from the UBaseService class."));
        return {};
    }

    virtual void Call(const TSharedPtr<FJsonObject>& Args)
    {
        checkf(false, TEXT("You must override the 'Call' function in your service class that derives from the UBaseService class."));
    }

    virtual FOnServiceDataRecieved& OnServiceDataRecieved() { return ServiceDataRecieved; }
    virtual FOnServiceDataError& OnServiceDataError() { return ServiceDataError; }

protected:
    FOnServiceDataRecieved ServiceDataRecieved;
    FOnServiceDataError ServiceDataError;
};
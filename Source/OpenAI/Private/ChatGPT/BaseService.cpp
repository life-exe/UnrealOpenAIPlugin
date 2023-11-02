// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/BaseService.h"

bool UBaseService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    checkf(false, TEXT("You must override the 'Init' function in your service class that derives from the UBaseService class."));
    return false;
}

FString UBaseService::Name() const
{
    checkf(false, TEXT("You must override the 'Name' function in your service class that derives from the UBaseService class."));
    return "Unknown service";
}

FString UBaseService::Description() const
{
    // Can be empty
    return "";
}

FFunctionOpenAI UBaseService::Function() const
{
    checkf(false, TEXT("You must override the 'Function' function in your service class that derives from the UBaseService class."));
    return {};
}

FString UBaseService::FunctionName() const
{
    checkf(false, TEXT("You must override the 'FunctionName' function in your service class that derives from the UBaseService class."));
    return {};
}

void UBaseService::Call(const TSharedPtr<FJsonObject>& Args)
{
    checkf(false, TEXT("You must override the 'Call' function in your service class that derives from the UBaseService class."));
}

FOnServiceDataRecieved& UBaseService::OnServiceDataRecieved()
{
    return ServiceDataRecieved;
}
FOnServiceDataError& UBaseService::OnServiceDataError()
{
    return ServiceDataError;
}

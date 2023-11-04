// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/BaseService.h"

bool UBaseService::Init(const OpenAI::ServiceSecrets& Secrets)
{
    checkf(false, TEXT("You must override the 'Init' method in your service class that derived from the UBaseService class."));
    return false;
}

FString UBaseService::Name() const
{
    checkf(false, TEXT("You must override the 'Name' method in your service class that derived from the UBaseService class."));
    return "Unknown service";
}

FString UBaseService::Description() const
{
    checkf(false, TEXT("You must override the 'Description' method in your service class that derived from the UBaseService class."));
    return "Unknown description";
}

FString UBaseService::FunctionName() const
{
    checkf(false, TEXT("You must override the 'FunctionName' method in your service class that derived from the UBaseService class."));
    return "Unknown function name";
}

void UBaseService::Call(const TSharedPtr<FJsonObject>& Args)
{
    checkf(false, TEXT("You must override the 'Call' method in your service class that derived from the UBaseService class."));
}

FString UBaseService::MakeFunction() const
{
    checkf(false, TEXT("You must override the 'MakeFunction' method in your service class that derived from the UBaseService class."));
    return "Can't make function";
}

FString UBaseService::TooltipDescription() const
{
    return "";
}

FFunctionOpenAI UBaseService::Function() const
{
    FFunctionOpenAI FunctionOpenAI;
    FunctionOpenAI.Name = FunctionName();
    FunctionOpenAI.Description = Description();
    FunctionOpenAI.Parameters = MakeFunction();
    return FunctionOpenAI;
}

FOnServiceDataRecieved& UBaseService::OnServiceDataRecieved()
{
    return ServiceDataRecieved;
}

FOnServiceDataError& UBaseService::OnServiceDataError()
{
    return ServiceDataError;
}

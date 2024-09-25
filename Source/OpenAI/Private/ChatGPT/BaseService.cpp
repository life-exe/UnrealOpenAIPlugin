// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/BaseService.h"
#include "FuncLib/OpenAIFuncLib.h"

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

void UBaseService::Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolIDIn)
{
    ToolID = ToolIDIn;
    // "You must override the 'Call' method in your service class that derived from the UBaseService class.
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

FFunctionRequest UBaseService::Function() const
{
    FFunctionRequest FunctionOpenAI;
    FunctionOpenAI.Name = FunctionName();
    FunctionOpenAI.Description = Description();
    FunctionOpenAI.Parameters = MakeFunction();
    return FunctionOpenAI;
}

FMessage UBaseService::MakeMessage(const FString& Content) const
{
    FMessage Message;
    Message.Tool_Call_ID.Set(ToolID);
    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Tool);
    Message.Content = Content;
    return Message;
}

FOnServiceDataRecieved& UBaseService::OnServiceDataRecieved()
{
    return ServiceDataRecieved;
}

FOnServiceDataError& UBaseService::OnServiceDataError()
{
    return ServiceDataError;
}

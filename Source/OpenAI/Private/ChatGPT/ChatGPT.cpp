// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/ChatGPT.h"
#include "Provider/OpenAIProvider.h"
#include "Algo/ForEach.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "ChatGPT/BaseService.h"

DEFINE_LOG_CATEGORY_STATIC(LogChatGPT, All, All);

UChatGPT::UChatGPT()
{
    Provider = NewObject<UOpenAIProvider>();
    Provider->SetLogEnabled(false);
    Provider->OnRequestError().AddLambda(
        [&](const FString& URL, const FString& Content)
        {
            HandleError(Content);
            HandleRequestCompletion();
        });
    Provider->OnCreateChatCompletionStreamProgresses().AddLambda(
        [&](const TArray<FChatCompletionStreamResponse>& Responses)
        {
            FString OutputString{};
            Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
                Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
                    OutputString.Append(Choice.Delta.Content);
                });
            });
            UpdateAssistantMessage(OutputString);
        });
    Provider->OnCreateChatCompletionStreamCompleted().AddLambda(
        [&](const TArray<FChatCompletionStreamResponse>& Responses)  //
        {
            FFunctionCall FunctionCall{};
            bool NeedToCallFunction{false};
            Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
                Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
                    FunctionCall.Arguments.Append(Choice.Delta.Function_Call.Arguments);
                    FunctionCall.Name.Append(Choice.Delta.Function_Call.Name);
                    if (UOpenAIFuncLib::StringToOpenAIFinishReason(Choice.Finish_Reason) == EOpenAIFinishReason::Function_Call)
                    {
                        NeedToCallFunction = true;
                    }
                });
            });
            if (NeedToCallFunction)
            {
                if (!HandleFunctionCall(FunctionCall))
                {
                    HandleError("");
                    HandleRequestCompletion();
                }
            }
            else
            {
                HandleRequestCompletion();
            }
        });
}

void UChatGPT::MakeRequest()
{
    TArray<FFunctionOpenAI> AvailableFunctions;
    for (const auto& Service : Services)
    {
        AvailableFunctions.Add(Service->Function());
    }
    FChatCompletion ChatCompletion;
    ChatCompletion.Model = OpenAIModel;
    ChatCompletion.Messages = ChatHistory;
    ChatCompletion.Max_Tokens = MaxTokens;
    ChatCompletion.Stream = true;
    ChatCompletion.Functions = AvailableFunctions;
    Provider->CreateChatCompletion(ChatCompletion, Auth);
}

void UChatGPT::HandleRequestCompletion()
{
    ChatHistory.Add(AssistantMessage);
    RequestCompleted.Broadcast();
}

void UChatGPT::UpdateAssistantMessage(const FString& Message, bool WasError)
{
    AssistantMessage.Content = Message;
    RequestUpdated.Broadcast(AssistantMessage, WasError);
}

void UChatGPT::HandleError(const FString& Content)
{
    const auto Message = UOpenAIFuncLib::GetErrorMessage(Content);
    if (!Message.IsEmpty())
    {
        UpdateAssistantMessage(Message, true);
        return;
    }

    const auto Code = UOpenAIFuncLib::GetErrorCode(Content);
    if (Code == EOpenAIResponseError::Unknown && !Content.IsEmpty())
    {
        UpdateAssistantMessage(Content, true);
        return;
    }
    UpdateAssistantMessage(UOpenAIFuncLib::ResponseErrorToString(Code), true);
}

bool UChatGPT::HandleFunctionCall(const FFunctionCall& FunctionCall)
{
    FString LogMsg;

    TSharedPtr<FJsonObject> Args;
    if (!UOpenAIFuncLib::StringToJson(FunctionCall.Arguments, Args))
    {
        LogMsg = FString::Format(TEXT("Can't parse args: {0}"), {FunctionCall.Arguments});
        UE_LOG(LogChatGPT, Error, TEXT("%s"), *LogMsg);
        return false;
    }

    LogMsg = FString::Format(TEXT("OpenAI call the function: [{0}] with args: {1}"), {FunctionCall.Name, FunctionCall.Arguments});
    UE_LOG(LogChatGPT, Display, TEXT("%s"), *LogMsg);

    FMessage HistoryMessage;
    HistoryMessage.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant);
    HistoryMessage.Function_Call.Name = FunctionCall.Name;
    // @todo: leave it empty for now, some problems with serialization format
    HistoryMessage.Function_Call.Arguments = "{}";
    ChatHistory.Add(HistoryMessage);

    // find and call func
    for (const auto& Service : Services)
    {
        if (Service->FunctionName().Equals(FunctionCall.Name))
        {
            Service->Call(Args);
            return true;
        }
    }

    LogMsg = FString::Format(TEXT("Can't find function by name: [{0}]"), {FunctionCall.Name});
    UE_LOG(LogChatGPT, Error, TEXT("%s"), *LogMsg);
    return false;
}

bool UChatGPT::RegisterService(const TSubclassOf<UBaseService>& ServiceClass, const OpenAI::ServiceSecrets& Secrets)
{
    FString LogMsg;

    auto* Service = NewObject<UBaseService>(this, ServiceClass);
    check(Service);
    if (!Service->Init(Secrets))
    {
        LogMsg = FString::Format(
            TEXT("Service {0} can't be init. API keys have probably not been loaded. Its functions are not available."), {Service->Name()});
        UE_LOG(LogChatGPT, Error, TEXT("%s"), *LogMsg);
        return false;
    }
    Service->OnServiceDataRecieved().AddLambda(
        [&](const FMessage& Message)
        {
            ChatHistory.Add(Message);
            MakeRequest();
        });
    Service->OnServiceDataError().AddLambda(
        [&](const FString& ErrorMessage)
        {
            HandleError(ErrorMessage);
            HandleRequestCompletion();
        });
    Services.Add(Service);

    LogMsg = FString::Format(TEXT("Service {0} was registered"), {Service->Name()});
    UE_LOG(LogChatGPT, Display, TEXT("%s"), *LogMsg);
    return true;
}

void UChatGPT::UnRegisterService(const TSubclassOf<UBaseService>& ServiceClass)
{
    auto* FoundService = Services.FindByPredicate([ServiceClass](const auto& Item) { return Item && Item->IsA(ServiceClass); });
    if (FoundService)
    {
        FoundService->Get()->OnServiceDataRecieved().RemoveAll(this);
        FoundService->Get()->OnServiceDataError().RemoveAll(this);
        Services.Remove(FoundService->Get());
        const auto LogMsg = FString::Format(TEXT("Service {0} was unregistered"), {FoundService->Get()->Name()});
        UE_LOG(LogChatGPT, Display, TEXT("%s"), *LogMsg);
    }
    else
    {
        UE_LOG(LogChatGPT, Warning, TEXT("Can't unregister service"));
    }
}

void UChatGPT::SetAuth(const FOpenAIAuth& OpenAIAuth)
{
    Auth = OpenAIAuth;
}

void UChatGPT::SetModel(const FString& Model)
{
    OpenAIModel = Model;
}

FString UChatGPT::GetModel() const
{
    return OpenAIModel;
}

void UChatGPT::SetMaxTokens(int32 Tokens)
{
    MaxTokens = Tokens;
}

void UChatGPT::AddMessage(const FMessage& Message)
{
    ChatHistory.Add(Message);
}

void UChatGPT::SetAssistantMessage(const FMessage& Message)
{
    AssistantMessage = Message;
}

FMessage UChatGPT::GetAssistantMessage() const
{
    return AssistantMessage;
}

void UChatGPT::ClearHistory()
{
    ChatHistory.Empty();
}

TArray<FMessage> UChatGPT::GetHistory() const
{
    return ChatHistory;
}

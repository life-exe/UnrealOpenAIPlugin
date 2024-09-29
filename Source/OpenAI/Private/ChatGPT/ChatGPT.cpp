// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "ChatGPT/ChatGPT.h"
#include "Provider/OpenAIProvider.h"
#include "Algo/ForEach.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/JsonFuncLib.h"
#include "ChatGPT/BaseService.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogChatGPT, All, All);

namespace
{
FString GatherChunkResponse(const TArray<FChatCompletionStreamResponse>& Responses)
{
    FString OutputString{};
    Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
        Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
            OutputString.Append(Choice.Delta.Content);
        });
    });

    return OutputString;
}

bool GatherFunctionResponse(const TArray<FChatCompletionStreamResponse>& Responses, FFunctionCommon& FunctionCall, FString& ID)
{
    bool NeedToCallFunction{false};
    Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
        Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
            ID.Append(Choice.Delta.Tool_Calls.ID);
            FunctionCall.Arguments.Append(Choice.Delta.Tool_Calls.Function.Arguments);
            FunctionCall.Name.Append(Choice.Delta.Tool_Calls.Function.Name);
            if (UOpenAIFuncLib::StringToOpenAIFinishReason(Choice.Finish_Reason) == EOpenAIFinishReason::Tool_Calls)
            {
                NeedToCallFunction = true;
            }
        });
    });
    return NeedToCallFunction;
}
}  // namespace

UChatGPT::UChatGPT()
{
    Provider = NewObject<UOpenAIProvider>();
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddLambda(
        [&](const FString& URL, const FString& Content)
        {
            HandleError(Content);
            HandleRequestCompletion();
        });
    Provider->OnCreateChatCompletionStreamProgresses().AddLambda(
        [&](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata) {  //
            const FString GatherdChunk = GatherChunkResponse(Responses);
            UpdateAssistantMessage(GatherdChunk);
        });
    Provider->OnCreateChatCompletionStreamCompleted().AddLambda(
        [&](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)  //
        {
            FFunctionCommon FunctionCall{};
            FString ID;
            if (GatherFunctionResponse(Responses, FunctionCall, ID))
            {
                if (!HandleFunctionCall(FunctionCall, ID))
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

void UChatGPT::SetLogEnabled(bool Enabled)
{
    Provider->SetLogEnabled(Enabled);
}

void UChatGPT::MakeRequest()
{
    TArray<FTools> AvailableTools;
    // tools are currently are not supported by vision models
    if (!UOpenAIFuncLib::ModelSupportsVision(OpenAIModel))
    {
        for (const auto& Service : Services)
        {
            AvailableTools.Add(FTools{UOpenAIFuncLib::OpenAIRoleToString(ERole::Function), Service->Function()});
        }
    }

    FChatCompletion ChatCompletion;
    ChatCompletion.Model = OpenAIModel;
    ChatCompletion.Messages = ChatHistory;
    ChatCompletion.Max_Completion_Tokens.Set(MaxCompletionTokens);
    ChatCompletion.Stream = true;
    ChatCompletion.Tools = AvailableTools;
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

bool UChatGPT::HandleFunctionCall(const FFunctionCommon& FunctionCall, const FString& ID)
{
    FString LogMsg;

    TSharedPtr<FJsonObject> Args;
    if (!FunctionCall.Arguments.IsEmpty() && !UJsonFuncLib::StringToJson(FunctionCall.Arguments, Args))
    {
        LogMsg = FString::Format(TEXT("Can't parse args: {0}"), {FunctionCall.Arguments});
        UE_LOGFMT(LogChatGPT, Error, "{0}", LogMsg);
        return false;
    }

    LogMsg = FString::Format(TEXT("OpenAI call the function: [{0}] with args: {1}"), {FunctionCall.Name, FunctionCall.Arguments});
    UE_LOGFMT(LogChatGPT, Display, "{0}", LogMsg);

    FMessage HistoryMessage;
    HistoryMessage.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant);

    FToolCalls ToolCalls;
    ToolCalls.ID = ID;
    ToolCalls.Type = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
    ToolCalls.Function.Name = FunctionCall.Name;
    HistoryMessage.Tool_Calls.Add(ToolCalls);
    ChatHistory.Add(HistoryMessage);

    // find and call func
    for (const auto& Service : Services)
    {
        if (Service->FunctionName().Equals(FunctionCall.Name))
        {
            Service->Call(Args, ID);
            return true;
        }
    }

    LogMsg = FString::Format(TEXT("Can't find function by name: [{0}]"), {FunctionCall.Name});
    UE_LOGFMT(LogChatGPT, Error, "{0}", LogMsg);
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
        UE_LOGFMT(LogChatGPT, Error, "{0}", LogMsg);
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
    UE_LOGFMT(LogChatGPT, Display, "{0}", LogMsg);
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
        UE_LOGFMT(LogChatGPT, Display, "{0}", LogMsg);
    }
    else
    {
        UE_LOGFMT(LogChatGPT, Warning, "Can't unregister service");
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
    MaxCompletionTokens = Tokens;
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

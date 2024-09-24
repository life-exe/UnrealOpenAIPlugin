// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/OpenAIProvider.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/Types/ModelTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Algo/ForEach.h"
#include "TestUtils.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIProviderActual, All, All);

BEGIN_DEFINE_SPEC(FOpenAIProviderActual, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderActual)

using namespace OpenAI::Tests;

namespace
{
bool TestFinishReason(const FString& Reason)
{
    const TSet<FString> FinishReson{UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Stop),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Length),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Tool_Calls),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Content_Filter),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Null)};
    return FinishReson.Contains(Reason);
}

template <typename ResponseType>
void TestStreamResponse(FAutomationTestBase* Test, const ResponseType& Response, const FString& ModelName, const FString& Oject)
{
    if (!Test)
    {
        UE_LOGFMT(LogOpenAIProviderActual, Error, "Automation test object is invalid");
        return;
    }

    // Test->TestTrue("Name should be valid", Response.Model.Equals(ModelName));
    Test->TestTrue("Created should be valid", Response.Created > 0);
    Test->TestTrue("ID should be valid", !Response.ID.IsEmpty());
    Test->TestTrue("Object should be valid", Response.Object.Equals(Oject));
    for (const auto& Choice : Response.Choices)
    {
        Test->TestTrue("Choice index should be valid", Choice.Index == 0);
        Test->TestTrue("Choice finish_reason should be valid", TestFinishReason(Choice.Finish_Reason));
    }
}

const FString JobID = "ftjob-HwfVFJki16yQveB1iwPqBaeF";
const FString FileID = "file-xm3aonDNpFhE4CMnzUdgoKUU";
const int32 FileBytes = 3138;

struct FFineTunePayload
{
    FString JobID;
    FString FileID;
    FString ModelName;
    FString OrgId;
};

void TestFineTuningJob(FAutomationTestBase* Test, const FFineTuningJobObjectResponse& Response, const FFineTunePayload& Payload)
{
    if (!Test)
    {
        UE_LOGFMT(LogOpenAIProviderActual, Error, "Automation test object is invalid");
        return;
    }

    Test->TestTrue("Object should be valid", Response.Object.Equals("fine_tuning.job"));
    // Test->TestTrue("Model should be valid", Response.Model.Equals(Payload.ModelName));
    Test->TestTrue("Created_At should be valid", Response.Created_At > 0);
    Test->TestTrue("Organization_ID should be valid", Response.Organization_ID.Equals(Payload.OrgId));
    Test->TestTrue("Training_File should be valid", !Response.Training_File.IsEmpty());
}

}  // namespace

void FOpenAIProviderActual::Define()
{
    Describe("ActualAPI",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));
                    OpenAIProvider = NewObject<UOpenAIProvider>();
                    OpenAIProvider->SetLogEnabled(true);
                    OpenAIProvider->OnRequestError().AddLambda(
                        [&](const FString& URL, const FString& Content)
                        {
                            RequestCompleted = true;
                            AddError(FString::Format(TEXT("URL: {0} Content: {1}"), {URL, Content}));
                        });
                    RequestCompleted = false;
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.Content.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model](const FChatCompletionResponse& Response)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(Response.Object.Equals("chat.completion"));
                            // TestTrueExpr(Response.Model.Equals(Model));
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(Response.Usage.Prompt_Tokens > 0);
                            TestTrueExpr(Response.Usage.Total_Tokens > 0);
                            TestTrueExpr(Response.Usage.Completion_Tokens > 0);
                            TestTrueExpr(Response.Choices.Num() == 1);
                            const auto Choice = Response.Choices[0];
                            TestTrueExpr(TestFinishReason(Choice.Finish_Reason));
                            TestTrueExpr(Choice.Index == 0);
                            TestTrueExpr(Choice.Message.Role.Equals(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant)));
                            TestTrueExpr(!Choice.Message.Content.IsEmpty());

                            RequestCompleted = true;
                        });

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    ChatCompletion.Messages = {FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::User), "What is Unreal Engine?"}};
                    ChatCompletion.Stream = false;
                    ChatCompletion.Max_Tokens = 100;

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.Content.Streaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);

                    OpenAIProvider->OnCreateChatCompletionStreamCompleted().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestStreamResponse<FChatCompletionStreamResponse>(this, Response, Model, "chat.completion.chunk");
                            }

                            RequestCompleted = true;
                        });

                    OpenAIProvider->OnCreateChatCompletionStreamProgresses().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestStreamResponse<FChatCompletionStreamResponse>(this, Response, Model, "chat.completion.chunk");
                            }
                        });

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    ChatCompletion.Messages = {FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::User), "What is Unreal Engine?"}};
                    ChatCompletion.Stream = true;
                    ChatCompletion.Max_Tokens = 100;

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.Content.Function",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);
                    TArray<FMessage> History;
                    History.Add(FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::User), "What is the weather like in Boston?"});

                    const FString FuncName = "get_current_weather";

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model, FuncName, History](const FChatCompletionResponse& Response)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(Response.Object.Equals("chat.completion"));
                            TestTrueExpr(Response.Created > 0);

                            TestTrueExpr(Response.Choices.Num() == 1);
                            if (Response.Choices.Num() == 0)
                            {
                                AddError("Choices array is empty!");
                                RequestCompleted = true;
                                return;
                            }
                            const auto Choice = Response.Choices[0];
                            TestTrueExpr(TestFinishReason(Choice.Finish_Reason));
                            TestTrueExpr(Choice.Index == 0);
                            TestTrueExpr(Choice.Message.Role.Equals(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant)));

                            const float Temperature = 22.5f;

                            if (UOpenAIFuncLib::StringToOpenAIFinishReason(Choice.Finish_Reason) == EOpenAIFinishReason::Tool_Calls)
                            {
                                TestTrueExpr(Choice.Message.Content.IsEmpty());
                                TestTrueExpr(!Choice.Message.Tool_Calls.IsEmpty());

                                const auto ToolCall = Choice.Message.Tool_Calls[0];

                                TArray<FMessage> NewHistory = History;
                                FMessage HistoryMessage;
                                HistoryMessage.Role = Choice.Message.Role;

                                FToolCalls ToolCalls;
                                ToolCalls.ID = ToolCall.ID;
                                ToolCalls.Type = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
                                ToolCalls.Function.Name = ToolCall.Function.Name;
                                HistoryMessage.Tool_Calls.Add(ToolCalls);
                                NewHistory.Add(HistoryMessage);

                                const auto get_current_weather = [&](const FString& Location, const FString& Unit) -> FString {
                                    return FString::Format(TEXT("location:{0}, temperature:{1}, unit:{2}, forecast: [sunny, windy]"),
                                        {Location, Temperature, Unit});
                                };

                                TMap<FString, TFunction<FString(const FString&, const FString&)>> Funcs;
                                Funcs.Add("get_current_weather", get_current_weather);

                                const FString IncomeFuncName = ToolCall.Function.Name;
                                TestTrueExpr(IncomeFuncName.Equals(FuncName));

                                const FString Arguments = ToolCall.Function.Arguments;
                                TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Arguments);
                                TSharedPtr<FJsonObject> ArgumentsJsonObject;
                                if (!FJsonSerializer::Deserialize(JsonReader, ArgumentsJsonObject))
                                {
                                    AddError("Can't deserialize function arguments");
                                    RequestCompleted = true;
                                    return;
                                }
                                const auto LocationArg = ArgumentsJsonObject->TryGetField(TEXT("location"));
                                TestTrueExpr(LocationArg.IsValid());
                                const auto UnitArg = ArgumentsJsonObject->TryGetField(TEXT("unit"));
                                TestTrueExpr(UnitArg.IsValid());

                                FMessage Message;
                                Message.Tool_Call_ID = Choice.Message.Tool_Calls[0].ID;
                                Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Tool);
                                // call function
                                Message.Content = Funcs[IncomeFuncName](LocationArg.Get()->AsString(), UnitArg.Get()->AsString());
                                NewHistory.Add(Message);

                                FChatCompletion ChatCompletion;
                                ChatCompletion.Model = Model;
                                ChatCompletion.Messages = NewHistory;
                                ChatCompletion.Stream = false;
                                ChatCompletion.Max_Tokens = 100;
                                OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                            }
                            else if (UOpenAIFuncLib::StringToOpenAIFinishReason(Choice.Finish_Reason) == EOpenAIFinishReason::Stop)
                            {
                                TestTrueExpr(!Choice.Message.Content.IsEmpty());
                                const FString TemperatureStr = FString::SanitizeFloat(Temperature);
                                TestTrueExpr(Choice.Message.Content.Contains(TemperatureStr));
                                RequestCompleted = true;
                            }
                            else
                            {
                                RequestCompleted = true;
                            }
                        });

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    ChatCompletion.Messages = History;
                    ChatCompletion.Stream = false;
                    ChatCompletion.Max_Tokens = 100;

                    /*
                        functions = [
                        {
                            "name": "get_current_weather",
                            "description": "Get the current weather in a given location",
                            "parameters": {
                                "type": "object",
                                "properties": {
                                    "location": {
                                        "type": "string",
                                        "description": "The city and state, e.g. San Francisco, CA",
                                    },
                                    "unit": {"type": "string", "enum": ["celsius", "fahrenheit"]},
                                },
                                "required": ["location", "unit"],
                            },
                        }]
                    */

                    TSharedPtr<FJsonObject> MainObj = MakeShareable(new FJsonObject());
                    MainObj->SetStringField("type", "object");

                    TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());

                    // location
                    TSharedPtr<FJsonObject> LocationObj = MakeShareable(new FJsonObject());
                    LocationObj->SetStringField("type", "string");
                    LocationObj->SetStringField("description", "The city and state, e.g. San Francisco, CA");
                    Props->SetObjectField("location", LocationObj);

                    // unit
                    TSharedPtr<FJsonObject> UnitObj = MakeShareable(new FJsonObject());
                    UnitObj->SetStringField("type", "string");
                    TArray<TSharedPtr<FJsonValue>> EnumArray;
                    EnumArray.Add(MakeShareable(new FJsonValueString("celsius")));
                    EnumArray.Add(MakeShareable(new FJsonValueString("fahrenheit")));
                    UnitObj->SetArrayField("enum", EnumArray);
                    Props->SetObjectField("unit", UnitObj);

                    MainObj->SetObjectField("properties", Props);

                    // required params
                    TArray<TSharedPtr<FJsonValue>> RequiredArray;
                    RequiredArray.Add(MakeShareable(new FJsonValueString("location")));
                    RequiredArray.Add(MakeShareable(new FJsonValueString("unit")));
                    MainObj->SetArrayField("required", RequiredArray);

                    FTools Tools;
                    Tools.Type = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
                    Tools.Function.Name = FuncName;
                    Tools.Function.Description = "Get the current weather in a given location";
                    Tools.Function.Parameters = UOpenAIFuncLib::MakeFunctionsString(MainObj);
                    ChatCompletion.Tools.Add(Tools);

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.ContentArray.Text.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model](const FChatCompletionResponse& Response)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(Response.Object.Equals("chat.completion"));
                            //  TestTrueExpr(Response.Model.Equals(Model));
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(Response.Usage.Prompt_Tokens > 0);
                            TestTrueExpr(Response.Usage.Total_Tokens > 0);
                            TestTrueExpr(Response.Usage.Completion_Tokens > 0);
                            TestTrueExpr(Response.Choices.Num() == 1);

                            if (Response.Choices.Num() >= 1)
                            {
                                const auto Choice = Response.Choices[0];
                                TestTrueExpr(TestFinishReason(Choice.Finish_Reason));

                                TestTrueExpr(Choice.Index == 0);
                                TestTrueExpr(Choice.Message.Role.Equals(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant)));
                                TestTrueExpr(!Choice.Message.Content.IsEmpty());
                            }
                            else
                            {
                                AddError("Choice array was empty");
                            }

                            RequestCompleted = true;
                        });

                    FMessage Message;
                    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);

                    FMessageContent MessageContent;
                    MessageContent.Text = "What is Unreal Engine?";
                    MessageContent.Type = UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Text);
                    Message.ContentArray.Add(MessageContent);

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    ChatCompletion.Messages.Add(Message);
                    ChatCompletion.Stream = false;
                    ChatCompletion.Max_Tokens = 100;

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.ContentArray.TextImage.URL.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model](const FChatCompletionResponse& Response)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(Response.Object.Equals("chat.completion"));
                            //  TestTrueExpr(Response.Model.Equals(Model));
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(Response.Usage.Prompt_Tokens > 0);
                            TestTrueExpr(Response.Usage.Total_Tokens > 0);
                            TestTrueExpr(Response.Usage.Completion_Tokens > 0);
                            TestTrueExpr(Response.Choices.Num() == 1);

                            if (Response.Choices.Num() >= 1)
                            {
                                const auto Choice = Response.Choices[0];
                                TestTrueExpr(TestFinishReason(Choice.Finish_Reason));

                                TestTrueExpr(Choice.Index == 0);
                                TestTrueExpr(Choice.Message.Role.Equals(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant)));
                                TestTrueExpr(!Choice.Message.Content.IsEmpty());
                                const bool Cats = Choice.Message.Content.Contains("cat") || Choice.Message.Content.Contains("kitten");
                                TestTrueExpr(Cats);
                            }
                            else
                            {
                                AddError("Choice array was empty");
                            }

                            RequestCompleted = true;
                        });

                    FMessage Message;
                    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);

                    FMessageContent MessageContent1;
                    MessageContent1.Text = "Describe in a few words what you can see in the picture.";
                    MessageContent1.Type = UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Text);
                    Message.ContentArray.Add(MessageContent1);

                    FMessageContent MessageContent2;
                    MessageContent2.Type = UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Image_URL);
                    MessageContent2.Image_URL.URL = "https://cdn.freecodecamp.org/curriculum/cat-photo-app/cats.jpg";
                    Message.ContentArray.Add(MessageContent2);

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    ChatCompletion.Messages.Add(Message);
                    ChatCompletion.Stream = false;
                    ChatCompletion.Max_Tokens = 100;

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.ContentArray.TextImage.Base64.NonStreaming",
                [this]()
                {
                    TArray<uint8> ImageData;
                    if (!FFileHelper::LoadFileToArray(ImageData, *TestUtils::FileFullPath("lenna.png")))
                    {
                        AddError("Can't load test image");
                        return;
                    }
                    const FString ImageInBase64 = FBase64::Encode(ImageData);
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model](const FChatCompletionResponse& Response)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(Response.Object.Equals("chat.completion"));
                            //  TestTrueExpr(Response.Model.Equals(Model));
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(Response.Usage.Prompt_Tokens > 0);
                            TestTrueExpr(Response.Usage.Total_Tokens > 0);
                            TestTrueExpr(Response.Usage.Completion_Tokens > 0);
                            TestTrueExpr(Response.Choices.Num() == 1);

                            if (Response.Choices.Num() >= 1)
                            {
                                const auto Choice = Response.Choices[0];
                                TestTrueExpr(TestFinishReason(Choice.Finish_Reason));

                                TestTrueExpr(Choice.Index == 0);
                                TestTrueExpr(Choice.Message.Role.Equals(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant)));
                                TestTrueExpr(!Choice.Message.Content.IsEmpty());
                                TestTrueExpr(Choice.Message.Content.Contains("girl") || Choice.Message.Content.Contains("woman"));
                            }
                            else
                            {
                                AddError("Choice array was empty");
                            }

                            RequestCompleted = true;
                        });

                    FMessage Message;
                    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);

                    FMessageContent MessageContent1;
                    MessageContent1.Text = "Describe in a few words what you can see in the picture.";
                    MessageContent1.Type = UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Text);
                    Message.ContentArray.Add(MessageContent1);

                    FMessageContent MessageContent2;
                    MessageContent2.Type = UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Image_URL);

                    MessageContent2.Image_URL.URL = UOpenAIFuncLib::WrapBase64(ImageInBase64);
                    Message.ContentArray.Add(MessageContent2);

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    ChatCompletion.Messages.Add(Message);
                    ChatCompletion.Stream = false;
                    ChatCompletion.Max_Tokens = 100;

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.ContentArray.Text.Streaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);

                    OpenAIProvider->OnCreateChatCompletionStreamCompleted().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestStreamResponse<FChatCompletionStreamResponse>(
                                    this, Response, "gpt-4-1106-vision-preview", "chat.completion.chunk");
                            }

                            RequestCompleted = true;
                        });

                    OpenAIProvider->OnCreateChatCompletionStreamProgresses().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestStreamResponse<FChatCompletionStreamResponse>(
                                    this, Response, "gpt-4-1106-vision-preview", "chat.completion.chunk");
                            }
                        });

                    FMessage Message;
                    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);

                    FMessageContent MessageContent;
                    MessageContent.Text = "What is Unreal Engine?";
                    MessageContent.Type = UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Text);
                    Message.ContentArray.Add(MessageContent);

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    ChatCompletion.Messages.Add(Message);
                    ChatCompletion.Stream = true;
                    ChatCompletion.Max_Tokens = 100;

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Completions.CreateCompletionRequestShouldResponseCorrectly.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct);

                    OpenAIProvider->OnCreateCompletionCompleted().AddLambda(
                        [&, Model](const FCompletionResponse& Response)
                        {
                            TestStreamResponse<FCompletionResponse>(this, Response, Model, "text_completion");
                            RequestCompleted = true;
                        });

                    FCompletion Completion;
                    Completion.Model = Model;
                    Completion.Prompt = "What is Unreal Engine?";
                    Completion.Max_Tokens = 100;
                    Completion.Stream = false;

                    OpenAIProvider->CreateCompletion(Completion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Completions.CreateCompletionRequestShouldResponseCorrectly.Streaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct);

                    OpenAIProvider->OnCreateCompletionStreamCompleted().AddLambda(
                        [&, Model](const TArray<FCompletionStreamResponse>& Responses)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestStreamResponse<FCompletionStreamResponse>(this, Response, Model, "text_completion");
                            }
                            RequestCompleted = true;
                        });

                    OpenAIProvider->OnCreateCompletionStreamProgresses().AddLambda(
                        [&, Model](const TArray<FCompletionStreamResponse>& Responses)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestStreamResponse<FCompletionStreamResponse>(this, Response, Model, "text_completion");
                            }
                        });

                    FCompletion Completion;
                    Completion.Model = Model;
                    Completion.Prompt = "What is Unreal Engine?";
                    Completion.Max_Tokens = 100;
                    Completion.Stream = true;

                    OpenAIProvider->CreateCompletion(Completion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Files.ListFilesRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListFilesCompleted().AddLambda(
                        [&](const FListFilesResponse& Response)
                        {
                            for (const auto& File : Response.Data)
                            {
                                TestTrueExpr(File.Object.Equals("file"));
                                TestTrueExpr(!File.ID.IsEmpty());
                                TestTrueExpr(!File.Purpose.IsEmpty());
                                TestTrueExpr(!File.FileName.IsEmpty());
                                TestTrueExpr(File.Bytes > 0);
                                TestTrueExpr(File.Created_At > 0);
                                TestTrueExpr(!File.Status.IsEmpty());
                            }
                            TestTrueExpr(Response.Object.Equals("list"));

                            RequestCompleted = true;
                        });

                    OpenAIProvider->ListFiles(Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Files.UploadAndDeleteFileRequestsShouldResponseCorrectly",
                [this]()
                {
                    const FString FileName = "test_file.jsonl";

                    OpenAIProvider->OnUploadFileCompleted().AddLambda(
                        [&, FileName](const FUploadFileResponse& File)
                        {
                            TestTrueExpr(File.FileName.Equals(FileName));
                            TestTrueExpr(File.Purpose.Equals("fine-tune"));
                            TestTrueExpr(File.Object.Equals("file"));
                            TestTrueExpr(File.Bytes == FileBytes);
                            TestTrueExpr(File.Status.Equals("processed"));
                            TestTrueExpr(!File.ID.IsEmpty());
                            TestTrueExpr(File.Created_At > 0);

                            OpenAIProvider->DeleteFile(File.ID, Auth);
                        });

                    // delete file after upload
                    OpenAIProvider->OnDeleteFileCompleted().AddLambda(
                        [&](const FDeleteFileResponse& File)
                        {
                            TestTrueExpr(File.Object.Equals("file"));
                            TestTrueExpr(File.Deleted);
                            RequestCompleted = true;
                        });

                    FUploadFile UploadFile;
                    UploadFile.File = TestUtils::FileFullPath(FileName);
                    UploadFile.Purpose = "fine-tune";

                    OpenAIProvider->UploadFile(UploadFile, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Files.RetrieveFileRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString FileName = "test_file.jsonl";
                    const FString _FileID{FileID};

                    OpenAIProvider->OnRetrieveFileCompleted().AddLambda(
                        [&, FileName, _FileID](const FRetrieveFileResponse& File)
                        {
                            TestTrueExpr(File.FileName.Equals(FileName));
                            TestTrueExpr(File.Purpose.Equals("fine-tune"));
                            TestTrueExpr(File.Object.Equals("file"));
                            TestTrueExpr(File.Bytes == FileBytes);
                            TestTrueExpr(File.Status.Equals("processed"));
                            TestTrueExpr(File.ID.Equals(_FileID));
                            TestTrueExpr(File.Created_At > 0);

                            RequestCompleted = true;
                        });

                    OpenAIProvider->RetrieveFile(FileID, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Files.RetrieveFileContentRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString FilePath = TestUtils::FileFullPath("test_file.jsonl");
                    FString FileContent;
                    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
                    {
                        AddError(FString::Format(TEXT("Can't open the file {0}"), {FilePath}));
                        return;
                    }

                    OpenAIProvider->OnRetrieveFileContentCompleted().AddLambda(
                        [&, FileContent](const FRetrieveFileContentResponse& File)
                        {
                            TestTrueExpr(FileContent.Equals(File.Content));
                            RequestCompleted = true;
                        });

                    OpenAIProvider->RetrieveFileContent(FileID, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Fine-tuning.ListFineTuningJobsShouldResponseCorrectly",
                [this]()
                {
                    const FString OrgId = Auth.OrganizationID;
                    OpenAIProvider->OnListFineTuningJobsCompleted().AddLambda(
                        [&, OrgId](const FListFineTuningJobsResponse& Response)
                        {
                            TestTrueExpr(Response.Object.Equals("list"));
                            for (const auto& FineTune : Response.Data)
                            {
                                TestTrueExpr(!FineTune.Model.IsEmpty());
                                TestTrueExpr(FineTune.Object.Equals("fine_tuning.job"));
                                TestTrueExpr(FineTune.Organization_ID.Equals(OrgId));
                                TestTrueExpr(FineTune.Created_At > 0);
                            }
                            RequestCompleted = true;
                        });

                    OpenAIProvider->ListFineTuningJobs(Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Fine-tuning.CreateFineTuningJobShouldResponseCorrectly",
                [this]()
                {
                    FFineTunePayload Payload;
                    Payload.FileID = FileID;
                    Payload.ModelName = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);
                    Payload.OrgId = Auth.OrganizationID;

                    OpenAIProvider->OnCreateFineTuningJobCompleted().AddLambda(
                        [&, Payload](const FFineTuningJobObjectResponse& Response)
                        {
                            TestFineTuningJob(this, Response, Payload);
                            RequestCompleted = true;
                        });

                    FFineTuningJob FineTuningJob;
                    FineTuningJob.Model = Payload.ModelName;
                    FineTuningJob.Training_File = Payload.FileID;
                    OpenAIProvider->CreateFineTuningJob(FineTuningJob, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Fine-tuning.RetriveFineTuningJobShouldResponseCorrectly",
                [this]()
                {
                    FFineTunePayload Payload;
                    Payload.JobID = JobID;
                    Payload.FileID = FileID;
                    Payload.ModelName = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);
                    Payload.OrgId = Auth.OrganizationID;

                    OpenAIProvider->OnRetrieveFineTuningJobCompleted().AddLambda(
                        [&, Payload](const FFineTuningJobObjectResponse& Response)
                        {
                            TestFineTuningJob(this, Response, Payload);
                            TestTrueExpr(Response.ID.Equals(Payload.JobID));
                            RequestCompleted = true;
                        });

                    OpenAIProvider->RetrieveFineTuningJob(JobID, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Fine-tuning.CancelFineTuningJobShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCancelFineTuningJobCompleted().AddLambda(
                        [&](const FFineTuningJobObjectResponse& Response) { RequestCompleted = true; });

                    OpenAIProvider->CancelFineTuningJob(JobID, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Fine-tuning.ListFineTuningEventsShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListFineTuningEventsCompleted().AddLambda(
                        [&](const FFineTuningJobEventResponse& Response) { RequestCompleted = true; });

                    OpenAIProvider->ListFineTuningEvents(JobID, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

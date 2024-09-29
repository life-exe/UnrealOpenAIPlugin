// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/Chat/ChatCompletionChunkTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIChatAPI, All, All);

BEGIN_DEFINE_SPEC(FOpenAIProviderChat, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderChat)

using namespace OpenAI::Tests;

void FOpenAIProviderChat::Define()
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
                        [&, Model](const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
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
                            TestTrueExpr(TestUtils::TestFinishReason(Choice.Finish_Reason));
                            TestTrueExpr(Choice.Index == 0);
                            TestTrueExpr(Choice.Message.Role.Equals(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant)));
                            TestTrueExpr(!Choice.Message.Content.IsEmpty());

                            RequestCompleted = true;
                        });

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    FMessage Message;
                    Message.Role = "user";
                    Message.Content = "What is Unreal Engine?";
                    ChatCompletion.Messages.Add(Message);
                    ChatCompletion.Stream = false;
                    ChatCompletion.Max_Completion_Tokens.Set(100);

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.Content.Streaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);

                    OpenAIProvider->OnCreateChatCompletionStreamCompleted().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestUtils::TestStreamResponse<FChatCompletionStreamResponse>(
                                    this, Response, Model, "chat.completion.chunk");
                            }

                            RequestCompleted = true;
                        });

                    OpenAIProvider->OnCreateChatCompletionStreamProgresses().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestUtils::TestStreamResponse<FChatCompletionStreamResponse>(
                                    this, Response, Model, "chat.completion.chunk");
                            }
                        });

                    FChatCompletion ChatCompletion;
                    ChatCompletion.Model = Model;
                    FMessage Message;
                    Message.Role = "user";
                    Message.Content = "What is Unreal Engine?";
                    ChatCompletion.Messages.Add(Message);
                    ChatCompletion.Stream = true;
                    ChatCompletion.Max_Completion_Tokens.Set(100);

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.Content.Function",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);
                    TArray<FMessage> History;
                    FMessage Message;
                    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);
                    Message.Content = "What is the weather like in Boston?";
                    History.Add(Message);

                    const FString FuncName = "get_current_weather";

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model, FuncName, History](
                            const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
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
                            TestTrueExpr(TestUtils::TestFinishReason(Choice.Finish_Reason));
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
                                Message.Tool_Call_ID.Set(Choice.Message.Tool_Calls[0].ID);
                                Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Tool);
                                // call function
                                Message.Content = Funcs[IncomeFuncName](LocationArg.Get()->AsString(), UnitArg.Get()->AsString());
                                NewHistory.Add(Message);

                                FChatCompletion ChatCompletion;
                                ChatCompletion.Model = Model;
                                ChatCompletion.Messages = NewHistory;
                                ChatCompletion.Stream = false;
                                ChatCompletion.Max_Completion_Tokens.Set(100);
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
                    ChatCompletion.Max_Completion_Tokens.Set(100);

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
                    Tools.Function.Parameters = UJsonFuncLib::MakeFunctionsString(MainObj);
                    ChatCompletion.Tools.Add(Tools);

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.ContentArray.Text.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model](const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
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
                                TestTrueExpr(TestUtils::TestFinishReason(Choice.Finish_Reason));

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
                    ChatCompletion.Max_Completion_Tokens.Set(100);

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.ContentArray.TextImage.URL.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model](const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
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
                                TestTrueExpr(TestUtils::TestFinishReason(Choice.Finish_Reason));

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
                    ChatCompletion.Max_Completion_Tokens.Set(100);

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
                        [&, Model](const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
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
                                TestTrueExpr(TestUtils::TestFinishReason(Choice.Finish_Reason));

                                TestTrueExpr(Choice.Index == 0);
                                TestTrueExpr(Choice.Message.Role.Equals(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant)));
                                TestTrueExpr(!Choice.Message.Content.IsEmpty());
                                TestTrueExpr(Choice.Message.Content.Contains("girl") || Choice.Message.Content.Contains("woman") ||
                                             Choice.Message.Content.Contains("person") || Choice.Message.Content.Contains("individual"));
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
                    ChatCompletion.Max_Completion_Tokens.Set(100);

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.ContentArray.Text.Streaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);

                    OpenAIProvider->OnCreateChatCompletionStreamCompleted().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestUtils::TestStreamResponse<FChatCompletionStreamResponse>(
                                    this, Response, "gpt-4-1106-vision-preview", "chat.completion.chunk");
                            }

                            RequestCompleted = true;
                        });

                    OpenAIProvider->OnCreateChatCompletionStreamProgresses().AddLambda(
                        [&, Model](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            for (const auto& Response : Responses)
                            {
                                TestUtils::TestStreamResponse<FChatCompletionStreamResponse>(
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
                    ChatCompletion.Max_Completion_Tokens.Set(100);

                    OpenAIProvider->CreateChatCompletion(ChatCompletion, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

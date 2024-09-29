// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "OpenAIProviderFake.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/CommonTypes.h"

DEFINE_SPEC(FOpenAIProviderFake, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FOpenAIProviderFake::Define()
{
    Describe("Fake",
        [this]()
        {
            It("ListModelsShouldBeParsedCorrectly",
                [this]()
                {
                    FListModelsResponse ListModelsResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnListModelsCompleted().AddLambda(
                        [&](const FListModelsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)  //
                        {                                                                                          //
                            ListModelsResponse = Response;
                        });
                    OpenAIProvider->SetResponse(
                        "{\"object\":\"list\",\"data\":[{\"id\":\"whisper-1\",\"object\":\"model\",\"created\":1677532384,\"owned_by\":"
                        "\"openai-internal\"},{\"id\":\"babbage\",\"object\":\"model\",\"created\":1649358449,\"owned_by\":\"openai\"}]}");
                    OpenAIProvider->ListModels(FOpenAIAuth{});

                    TestTrueExpr(ListModelsResponse.Object.Equals("list"));
                    TestTrueExpr(ListModelsResponse.Data.Num() == 2);

                    TestTrueExpr(ListModelsResponse.Data[0].ID.Equals("whisper-1"));
                    TestTrueExpr(ListModelsResponse.Data[0].Object.Equals("model"));
                    TestTrueExpr(ListModelsResponse.Data[0].Created == 1677532384);
                    TestTrueExpr(ListModelsResponse.Data[0].Owned_By.Equals("openai-internal"));

                    TestTrueExpr(ListModelsResponse.Data[1].ID.Equals("babbage"));
                    TestTrueExpr(ListModelsResponse.Data[1].Object.Equals("model"));
                    TestTrueExpr(ListModelsResponse.Data[1].Created == 1649358449);
                    TestTrueExpr(ListModelsResponse.Data[1].Owned_By.Equals("openai"));
                });

            It("RetrieveModelShouldBeParsedCorrectly",
                [this]()
                {
                    FRetrieveModelResponse RetrieveModelResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnRetrieveModelCompleted().AddLambda(
                        [&](const FRetrieveModelResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)  //
                        {                                                                                             //
                            RetrieveModelResponse = Response;
                        });
                    OpenAIProvider->SetResponse("{\"id\":\"MyModel\",\"object\":\"model\",\"created\":1649357491,\"owned_by\":\"openai\"}");
                    OpenAIProvider->RetrieveModel("MyModel", FOpenAIAuth{});

                    TestTrueExpr(RetrieveModelResponse.ID.Equals("MyModel"));
                    TestTrueExpr(RetrieveModelResponse.Object.Equals("model"));
                    TestTrueExpr(RetrieveModelResponse.Created == 1649357491);
                    TestTrueExpr(RetrieveModelResponse.Owned_By.Equals("openai"));
                });

            It("CompletionShouldBeParsedCorrectly",
                [this]()
                {
                    FCompletionResponse CompletionResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnCreateCompletionCompleted().AddLambda(
                        [&](const FCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)  //
                        {                                                                                          //
                            CompletionResponse = Response;
                        });
                    OpenAIProvider->SetResponse(
                        "{\"id\":\"cmpl-qwerty\",\"object\":\"text_completion\",\"created\":1686519146,\"model\":"
                        "\"text-davinci-003\",\"choices\":[{\"text\":\"Unreal Engine is a game engine developed by EpicGames. "
                        "It is a complete suite of game development tools made up of a powerful rendering engine, a full suite of tools "
                        "for creating 3D content, "
                        "and a visual scripting system for creating game logic. It is used to create games for PC, console, mobile, "
                        "and virtual reality platforms.\",\"index\":0,\"logprobs\":{\"tokens\":[\"\\n\",\"\\n\",\"Un\",\"real\",\"Engine\","
                        "\"is\",\"a\",\"game\",\"engine\",\"developed\",\"by\",\"Epic\",\"Games\",\".\",\"It\",\"is\",\"a\",\"complete\","
                        "\"suite\",\"of\",\"game\",\"development\",\"tools\",\"made\",\"up\",\"of\",\"a\",\"powerful\",\"rendering\","
                        "\"engine\",\",\",\"a\",\"full\",\"suite\",\"of\",\"tools\",\"for\",\"creating\",\"3\",\"D\",\"content\",\",\","
                        "\"and\",\"a\",\"visual\",\"scripting\",\"system\",\"for\",\"creating\",\"game\",\"logic\",\".\",\"It\",\"is\","
                        "\"used\",\"to\",\"create\",\"games\",\"for\",\"PC\",\",\",\"console\",\",\",\"mobile\",\",\",\"and\",\"virtual\","
                        "\"reality\",\"platforms\",\".\"],\"token_logprobs\":[-0.0021931296,-0.01865152,-0.0022231156,-1.8841804e-7,-0."
                        "0018055186,-0.008720602,-0.026865501,-0.3118153,-0.055692185,-0.08340753,-0.015742142,-0.0013171887,-0.0003245515,"
                        "-0.74661285,-0.029067576,-0.05275148,-0.5679994,-0.55052173,-0.05841217,-0.0006519426,-0.49576294,-0.0037525587,-"
                        "0.00086900836,-0.48466575,-0.794948,-0.00009258174,-0.42108586,-0.17348798,-0.13398515,-0.0049108006,-0.37749258,-"
                        "1.1567007,-0.85982966,-0.3357653,-0.00013824523,-0.42813239,-0.07859528,-1.1513321,-1.1226257,-0.001078244,-0."
                        "6073104,-0.053323995,-0.7077257,-0.46132097,-1.8315376,-0.28107527,-0.3551227,-0.531818,-0.64224195,-1.0480657,-0."
                        "07215779,-0.18936816,-0.87090504,-0.21712954,-0.31079113,-0.49128723,-0.33791658,-0.94136876,-0.15101758,-1."
                        "2563615,-0.0047865803,-0.4933537,-0.0056822924,-0.28967217,-0.13039722,-0.45705342,-0.7074229,-0.0044916007,-0."
                        "40444016,-0.079881996],\"top_logprobs\":null,\"text_offset\":[77,78,79,81,85,92,95,97,102,109,119,122,127,133,134,"
                        "137,140,142,151,157,160,165,177,183,188,191,194,196,205,215,222,223,225,230,236,239,245,249,258,260,261,269,270,"
                        "274,276,283,293,300,304,313,318,324,325,328,331,336,339,346,352,356,359,360,368,369,376,377,381,389,397,407]},"
                        "\"finish_reason\":\"stop\"}],\"usage\":{\"prompt_tokens\":5,\"completion_tokens\":70,\"total_tokens\":75}}");

                    FCompletion Completion;
                    Completion.Prompt = "What is Unreal Engine?";
                    Completion.Model = "text-davinci-003";
                    OpenAIProvider->CreateCompletion(Completion, FOpenAIAuth{});

                    TestTrueExpr(CompletionResponse.ID.Equals("cmpl-qwerty"));
                    TestTrueExpr(CompletionResponse.Object.Equals("text_completion"));
                    TestTrueExpr(CompletionResponse.Model.Equals("text-davinci-003"));
                    TestTrueExpr(CompletionResponse.Created == 1686519146);
                    TestTrueExpr(CompletionResponse.Usage.Prompt_Tokens == 5);
                    TestTrueExpr(CompletionResponse.Usage.Completion_Tokens == 70);
                    TestTrueExpr(CompletionResponse.Usage.Total_Tokens == 75);
                    TestTrueExpr(CompletionResponse.Choices[0].Text.Equals(
                        "Unreal Engine is a game engine developed by EpicGames. It is a complete suite of game development tools "
                        "made up of a powerful rendering engine, a full suite of tools for creating 3D content, and a visual scripting "
                        "system for creating game logic. It is used to create games for PC, console, mobile, and virtual reality "
                        "platforms."));
                });

            It("ChatCompletionShouldBeParsedCorrectly",
                [this]()
                {
                    FChatCompletionResponse ChatCompletionResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&](const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)  //
                        {                                                                                              //
                            ChatCompletionResponse = Response;
                        });
                    OpenAIProvider->SetResponse(
                        "{\"id\":\"chatcmpl-xxxxxxxxxxxxxxxxxx\",\"object\":\"chat.completion\",\"created\":1686587980,"
                        "\"model\":\"gpt-4\",\"usage\":{\"prompt_tokens\":12,\"completion_tokens\":84,\"total_tokens\":96},"
                        "\"choices\":[{\"message\":{\"role\":\"assistant\",\"content\":\"a type of chatbot that uses GPT (Generative "
                        "Pre-trained Transformer) technology to generate responses to user input. GPT is a type of artificial intelligence "
                        "that has been trained on large amounts of text data, allowing it to understand natural language and generate "
                        "responses that are contextually relevant and coherent. Chat GPTs can be used for a variety of purposes, including "
                        "customer service, virtual assistants, and language learning.\"},"
                        "\"finish_reason\":\"stop\",\"index\":0}]}");

                    FChatCompletion ChatCompletion;
                    FMessage Message;
                    Message.Role = "user";
                    Message.Content = "What is Unreal Engine?";
                    ChatCompletion.Messages.Add(Message);
                    ChatCompletion.Model = "gpt-4";
                    ChatCompletion.Stream = false;
                    ChatCompletion.N = 1;
                    ChatCompletion.Max_Completion_Tokens.Set(2000);
                    OpenAIProvider->CreateChatCompletion(ChatCompletion, FOpenAIAuth{});

                    TestTrueExpr(ChatCompletionResponse.ID.Equals("chatcmpl-xxxxxxxxxxxxxxxxxx"));
                    TestTrueExpr(ChatCompletionResponse.Object.Equals("chat.completion"));
                    TestTrueExpr(ChatCompletionResponse.Created == 1686587980);
                    TestTrueExpr(ChatCompletionResponse.Model.Equals("gpt-4"));
                    TestTrueExpr(ChatCompletionResponse.Usage.Prompt_Tokens == 12);
                    TestTrueExpr(ChatCompletionResponse.Usage.Completion_Tokens == 84);
                    TestTrueExpr(ChatCompletionResponse.Usage.Total_Tokens == 96);
                    TestTrueExpr(ChatCompletionResponse.Choices.Num() == 1);
                    TestTrueExpr(ChatCompletionResponse.Choices[0].Finish_Reason.Equals("stop"));
                    TestTrueExpr(ChatCompletionResponse.Choices[0].Index == 0);
                    TestTrueExpr(ChatCompletionResponse.Choices[0].Message.Role.Equals("assistant"));
                    TestTrueExpr(ChatCompletionResponse.Choices[0].Message.Content.Equals(
                        "a type of chatbot that uses GPT (Generative "
                        "Pre-trained Transformer) technology to generate responses to user input. GPT is a type of artificial intelligence "
                        "that has been trained on large amounts of text data, allowing it to understand natural language and generate "
                        "responses that are contextually relevant and coherent. Chat GPTs can be used for a variety of purposes, including "
                        "customer service, virtual assistants, and language learning."));
                });

            xIt("ChatCompletionStreamShouldBeParsedCorrectly", [this]() { unimplemented(); });
        });
}

#endif

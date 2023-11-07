// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/OpenAIProvider.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "Provider/CommonTypes.h"
#include "Funclib/ModelTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Algo/ForEach.h"
#include "TestUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIProviderActual, All, All);

BEGIN_DEFINE_SPEC(FOpenAIProviderActual, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderActual)

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForRequestCompleted, bool&, RequestCompleted);
bool FWaitForRequestCompleted::Update()
{
    return RequestCompleted;
}

using namespace OpenAI::Tests;

namespace
{
void TestImageResponse(FAutomationTestBase* Test, const TArray<FString>& Data, int32 Num)
{
    if (!Test)
    {
        UE_LOG(LogOpenAIProviderActual, Error, TEXT("Automation test object is invalid"));
        return;
    }

    Test->TestTrue("Images amount should be valid", Data.Num() == Num);
    for (const auto& Image : Data)
    {
        Test->TestTrue("Image url should be valud", TestUtils::IsValidURL(Image));
    }
}

bool TestFinishReason(const FString& Reason)
{
    const TSet<FString> FinishReson{UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Stop),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Length),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Function_Call),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Content_Filter),
        UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Null)};
    return FinishReson.Contains(Reason);
}

template <typename ResponseType>
void TestStreamResponse(FAutomationTestBase* Test, const ResponseType& Response, const FString& ModelName, const FString& Oject)
{
    if (!Test)
    {
        UE_LOG(LogOpenAIProviderActual, Error, TEXT("Automation test object is invalid"));
        return;
    }

    Test->TestTrue("Name should be valid", Response.Model.Equals(ModelName));
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
        UE_LOG(LogOpenAIProviderActual, Error, TEXT("Automation test object is invalid"));
        return;
    }

    Test->TestTrue("Object should be valid", Response.Object.Equals("fine_tuning.job"));
    Test->TestTrue("Model should be valid", Response.Model.Equals(Payload.ModelName));
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

            It("Models.ListModelsRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListModelsCompleted().AddLambda(
                        [&](const FListModelsResponse& Response)
                        {
                            TestTrueExpr(!Response.Object.IsEmpty());
                            TestTrueExpr(Response.Data.Num() != 0);

                            TSet<FString> ModelNames;
                            for (const auto& Model : Response.Data)
                            {
                                // My models will also be returned to the main list, so I have to ignore them
                                if (!Model.ID.Contains("lifeexe"))
                                {
                                    ModelNames.Add(Model.ID);
                                }
                                TestTrueExpr(Model.Created > 0);
                                TestTrueExpr(!Model.ID.IsEmpty());
                                TestTrueExpr(!Model.Object.IsEmpty());
                                TestTrueExpr(!Model.Owned_By.IsEmpty());
                                for (const auto& OnePermission : Model.Permission)
                                {
                                    TestTrueExpr(!OnePermission.ID.IsEmpty());
                                    TestTrueExpr(!OnePermission.Object.IsEmpty());
                                    TestTrueExpr(!OnePermission.Organization.IsEmpty());
                                    TestTrueExpr(OnePermission.Created > 0);
                                }
                            }

                            // Determine the difference between plug-in and remote OpenAI model names
                            TSet<FString> PluginModelNames;
                            const UEnum* Enum = StaticEnum<EAllModelEnum>();
                            for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
                            {
                                const FString OpenAIModelName = TestUtils::PluginEnumToOpenAIModelName(static_cast<EAllModelEnum>(i));
                                PluginModelNames.Add(OpenAIModelName);
                            }

                            const TSet<FString> Intersection = ModelNames.Intersect(PluginModelNames);
                            ModelNames = ModelNames.Difference(Intersection);
                            PluginModelNames = PluginModelNames.Difference(Intersection);

                            for (const auto& Name : ModelNames)
                            {
                                AddWarning(FString::Format(
                                    TEXT("Remote OpenAI model name that doesn't exist in plugin EAllModelEnum: {0}"), {Name}));
                            }

                            for (const auto& Name : PluginModelNames)
                            {
                                AddWarning(FString::Format(TEXT("Plugin model name that doesn't exist in remote OpenAI: {0}"), {Name}));
                            }

                            RequestCompleted = true;
                        });
                    OpenAIProvider->ListModels(Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Models.RetrieveModelRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnRetrieveModelCompleted().AddLambda(
                        [&](const FRetrieveModelResponse& Response)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(!Response.Object.IsEmpty());
                            TestTrueExpr(!Response.Owned_By.IsEmpty());
                            for (const auto& Permission : Response.Permission)
                            {
                                TestTrueExpr(!Permission.ID.IsEmpty());
                                TestTrueExpr(!Permission.Object.IsEmpty());
                                TestTrueExpr(!Permission.Organization.IsEmpty());
                                TestTrueExpr(Permission.Created > 0);
                            }
                            RequestCompleted = true;
                        });
                    OpenAIProvider->RetrieveModel("gpt-4", Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Moderations.ModerationRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateModerationsCompleted().AddLambda(
                        [&](const FModerationsResponse& Response)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(!Response.Model.IsEmpty());
                            TestTrueExpr(Response.Results.Num() == 2);

                            TestTrueExpr(!Response.Results[0].Flagged);
                            TestTrueExpr(Response.Results[1].Flagged);
                            RequestCompleted = true;
                        });

                    FModerations Moderations;
                    Moderations.Input = {"I love you", "I really hate you"};
                    Moderations.Model = UOpenAIFuncLib::OpenAIModerationModelToString(EModerationsModelEnum::Text_Moderation_Latest);

                    OpenAIProvider->CreateModerations(Moderations, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateImageCompleted().AddLambda(
                        [&](const FImageResponse& Response)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImage OpenAIImage;
                    OpenAIImage.N = 2;
                    OpenAIImage.Prompt = "Bear with beard drinking beer";
                    OpenAIImage.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_256x256);
                    OpenAIImage.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

                    OpenAIProvider->CreateImage(OpenAIImage, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageVariationRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateImageVariationCompleted().AddLambda(
                        [&](const FImageVariationResponse& Response)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImageVariation OpenAIImageVariation;
                    OpenAIImageVariation.N = 2;
                    OpenAIImageVariation.Image = TestUtils::FileFullPath("whale.png");
                    OpenAIImageVariation.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_256x256);
                    OpenAIImageVariation.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

                    OpenAIProvider->CreateImageVariation(OpenAIImageVariation, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageEditRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateImageEditCompleted().AddLambda(
                        [&](const FImageEditResponse& Response)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImageEdit OpenAIImageEdit;
                    OpenAIImageEdit.N = 2;
                    OpenAIImageEdit.Image = TestUtils::FileFullPath("whale.png");
                    OpenAIImageEdit.Mask = TestUtils::FileFullPath("whale_mask.png");
                    OpenAIImageEdit.Prompt = "put the hat on the whale's head";
                    OpenAIImageEdit.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_256x256);
                    OpenAIImageEdit.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

                    OpenAIProvider->CreateImageEdit(OpenAIImageEdit, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Audio.CreateTranscriptionRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateAudioTranscriptionCompleted().AddLambda(
                        [&](const FAudioTranscriptionResponse& Response)
                        {
                            TestTrueExpr(TestUtils::RemovePunctuation(Response.Text.ToLower()).Equals("hello whats up"));
                            RequestCompleted = true;
                        });

                    FAudioTranscription AudioTranscription;
                    AudioTranscription.Language = "en";
                    AudioTranscription.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModelEnum::Whisper_1);
                    AudioTranscription.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
                    AudioTranscription.Temperature = 0.0f;
                    AudioTranscription.File = TestUtils::FileFullPath("hello.mp3");

                    OpenAIProvider->CreateAudioTranscription(AudioTranscription, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Audio.CreateTranslationRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateAudioTranslationCompleted().AddLambda(
                        [&](const FAudioTranslationResponse& Response)
                        {
                            TestTrueExpr(TestUtils::RemovePunctuation(Response.Text.ToLower()).Equals("hi how are you"));
                            RequestCompleted = true;
                        });

                    FAudioTranslation AudioTranslation;
                    AudioTranslation.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModelEnum::Whisper_1);
                    AudioTranslation.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
                    AudioTranslation.Temperature = 0.0f;
                    AudioTranslation.File = TestUtils::FileFullPath("bonjour.mp3");

                    OpenAIProvider->CreateAudioTranslation(AudioTranslation, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Embeddings.CreateEmbeddingsRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateEmbeddingsCompleted().AddLambda(
                        [&](const FEmbeddingsResponse& Response)
                        {
                            TestTrueExpr(Response.Model.Equals("text-embedding-ada-002-v2"));
                            TestTrueExpr(Response.Usage.Prompt_Tokens == 6);
                            TestTrueExpr(Response.Usage.Total_Tokens == 6);
                            TestTrueExpr(Response.Object.Equals("list"));
                            TestTrueExpr(Response.Data.Num() == 1);

                            for (const auto& Data : Response.Data)
                            {
                                TestTrueExpr(Data.Object.Equals("embedding"));
                                TestTrueExpr(Data.Index == 0);
                                TestTrueExpr(Data.Embedding.Num() > 0);
                            }

                            RequestCompleted = true;
                        });

                    FEmbeddings Embeddings;
                    Embeddings.Input = {"Hello! How are you?"};
                    Embeddings.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_Ada_002);
                    Embeddings.Encoding_Format = UOpenAIFuncLib::OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat::Float);
                    OpenAIProvider->CreateEmbeddings(Embeddings, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.NonStreaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0301);

                    OpenAIProvider->OnCreateChatCompletionCompleted().AddLambda(
                        [&, Model](const FChatCompletionResponse& Response)
                        {
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(Response.Model.Equals(Model));
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

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.Streaming",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0301);

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

            It("Chat.CreateChatCompletionRequestShouldResponseCorrectly.Function",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0613);
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

                            if (UOpenAIFuncLib::StringToOpenAIFinishReason(Choice.Finish_Reason) == EOpenAIFinishReason::Function_Call)
                            {
                                TestTrueExpr(Choice.Message.Content.IsEmpty());

                                const auto get_current_weather = [&](const FString& Location, const FString& Unit) -> FString {
                                    return FString::Format(TEXT("location:{0}, temperature:{1}, unit:{2}, forecast: [sunny, windy]"),
                                        {Location, Temperature, Unit});
                                };

                                TMap<FString, TFunction<FString(const FString&, const FString&)>> Funcs;
                                Funcs.Add("get_current_weather", get_current_weather);

                                const FString IncomeFuncName = Choice.Message.Function_Call.Name;
                                TestTrueExpr(IncomeFuncName.Equals(FuncName));

                                const FString Arguments = Choice.Message.Function_Call.Arguments;
                                TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Arguments);
                                TSharedPtr<FJsonObject> ArgumentsJsonObject;
                                if (!FJsonSerializer::Deserialize(JsonReader, ArgumentsJsonObject))
                                {
                                    AddError("Can't deserialize function arguments");
                                    RequestCompleted = true;
                                    return;
                                }
                                const auto LocationArg = ArgumentsJsonObject->TryGetField("location");
                                TestTrueExpr(LocationArg.IsValid());
                                const auto UnitArg = ArgumentsJsonObject->TryGetField("unit");
                                TestTrueExpr(UnitArg.IsValid());

                                TArray<FMessage> NewHistory = History;
                                FMessage Message;
                                Message.Name = FuncName;
                                Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::Function);
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

                    FFunctionOpenAI FunctionOpenAI;
                    FunctionOpenAI.Name = FuncName;
                    FunctionOpenAI.Description = "Get the current weather in a given location";

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

                    FunctionOpenAI.Parameters = UOpenAIFuncLib::MakeFunctionsString(MainObj);
                    ChatCompletion.Functions.Add(FunctionOpenAI);
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
                    Payload.ModelName = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0613);
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
                    Payload.ModelName = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0613);
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

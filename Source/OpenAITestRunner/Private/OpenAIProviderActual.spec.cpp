// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "OpenAIProviderFake.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "Provider/CommonTypes.h"
#include "Funclib/ModelTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Internationalization/Regex.h"

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

namespace
{
FString PluginEnumToOpenAIModelName(EAllModelEnum ModelEnum)
{
    const FString EnumAsString = UEnum::GetValueAsString(ModelEnum);

    // EAllModelEnum::GPT_3_5_Turbo_0301 -> EAllModelEnum, GPT_3_5_Turbo_0301
    FString EnumName, EnumElementName;
    EnumAsString.Split("::", &EnumName, &EnumElementName);

    // GPT_3_5_Turbo_0301 -> gpt-3-5-turbo-0301
    EnumElementName = EnumElementName.ToLower().Replace(TEXT("_"), TEXT("-"));

    // special case: gpt-3-5-turbo-0301 -> gpt-3.5-turbo-0301
    EnumElementName = EnumElementName.Replace(TEXT("3-5"), TEXT("3.5"));

    return EnumElementName;
}

FString FileFullPath(const FString& FileName)
{
    return FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir().Append("OpenAI/Source/OpenAITestRunner/Data/").Append(FileName));
}

bool IsValidURL(const FString& URL)
{
    const FString URLPattern = "^((http|https)://)[-a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)$";
    const FRegexPattern Pattern(URLPattern);
    FRegexMatcher Matcher(Pattern, URL);

    return Matcher.FindNext();
}

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
        Test->TestTrue("Image url should be valud", IsValidURL(Image));
    }
}

FString RemovePunctuation(const FString& Input)
{
    FRegexPattern Pattern(TEXT("[\\p{P}\\p{S}]"));
    FRegexMatcher Matcher(Pattern, Input);

    FString Result;
    int32 LastPosition = 0;

    while (Matcher.FindNext())
    {
        Result.Append(Input.Mid(LastPosition, Matcher.GetMatchBeginning() - LastPosition));
        LastPosition = Matcher.GetMatchEnding();
    }

    Result.Append(Input.Mid(LastPosition));

    return Result;
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
                    OpenAIProvider->OnListModelsCompleted().AddLambda([&](const FListModelsResponse& Response)  //
                        {                                                                                       //
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
                                TestTrueExpr(!Model.Root.IsEmpty());
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
                                const FString OpenAIModelName = PluginEnumToOpenAIModelName(static_cast<EAllModelEnum>(i));
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
                    OpenAIProvider->OnRetrieveModelCompleted().AddLambda([&](const FRetrieveModelResponse& Response)  //
                        {                                                                                             //
                            TestTrueExpr(Response.Created > 0);
                            TestTrueExpr(!Response.ID.IsEmpty());
                            TestTrueExpr(!Response.Object.IsEmpty());
                            TestTrueExpr(!Response.Owned_By.IsEmpty());
                            TestTrueExpr(!Response.Root.IsEmpty());
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
                    OpenAIProvider->OnCreateModerationsCompleted().AddLambda([&](const FModerationsResponse& Response)  //
                        {                                                                                               //
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
                    OpenAIProvider->OnCreateImageCompleted().AddLambda([&](const FImageResponse& Response)  //
                        {                                                                                   //
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
                    OpenAIProvider->OnCreateImageVariationCompleted().AddLambda([&](const FImageVariationResponse& Response)  //
                        {                                                                                                     //
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImageVariation OpenAIImageVariation;
                    OpenAIImageVariation.N = 2;
                    OpenAIImageVariation.Image = FileFullPath("whale.png ");
                    OpenAIImageVariation.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_256x256);
                    OpenAIImageVariation.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

                    OpenAIProvider->CreateImageVariation(OpenAIImageVariation, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageEditRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateImageEditCompleted().AddLambda(
                        [&](const FImageEditResponse& Response)  //
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImageEdit OpenAIImageEdit;
                    OpenAIImageEdit.N = 2;
                    OpenAIImageEdit.Image = FileFullPath("whale.png ");
                    OpenAIImageEdit.Mask = FileFullPath("whale_mask.png ");
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
                        [&](const FAudioTranscriptionResponse& Response)  //
                        {
                            TestTrueExpr(RemovePunctuation(Response.Text.ToLower()).Equals("hello whats up"));
                            RequestCompleted = true;
                        });

                    FAudioTranscription AudioTranscription;
                    AudioTranscription.Language = "en";
                    AudioTranscription.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModelEnum::Whisper_1);
                    AudioTranscription.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
                    AudioTranscription.Temperature = 0.0f;
                    AudioTranscription.File = FileFullPath("hello.mp3");

                    OpenAIProvider->CreateAudioTranscription(AudioTranscription, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Audio.CreateTranslationRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateAudioTranslationCompleted().AddLambda(
                        [&](const FAudioTranslationResponse& Response)  //
                        {
                            TestTrueExpr(RemovePunctuation(Response.Text.ToLower()).Equals("hi how are you"));
                            RequestCompleted = true;
                        });

                    FAudioTranslation AudioTranslation;
                    AudioTranslation.Model = UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModelEnum::Whisper_1);
                    AudioTranslation.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
                    AudioTranslation.Temperature = 0.0f;
                    AudioTranslation.File = FileFullPath("bonjour.mp3");

                    OpenAIProvider->CreateAudioTranslation(AudioTranslation, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Embeddings.CreateEmbeddingsRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateEmbeddingsCompleted().AddLambda(
                        [&](const FEmbeddingsResponse& Response)  //
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
        });
}

#endif

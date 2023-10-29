// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/FileSystemFuncLib.h"

DEFINE_SPEC(FFuncLib, "OpenAI",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

void FFuncLib::Define()
{
    Describe("OpenAIFuncLib",
        [this]()
        {
            It("AllModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Babbage).Equals("babbage"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Davinci).Equals("davinci"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Davinci_Edit_001).Equals("text-davinci-edit-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Babbage_Code_Search_Code).Equals("babbage-code-search-code"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Similarity_Babbage_001)
                                     .Equals("text-similarity-babbage-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Code_Davinci_Edit_001).Equals("code-davinci-edit-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Davinci_001).Equals("text-davinci-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Davinci_003).Equals("text-davinci-003"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Ada).Equals("ada"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Curie_Instruct_Beta).Equals("curie-instruct-beta"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Babbage_Code_Search_Text).Equals("babbage-code-search-text"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Babbage_Similarity).Equals("babbage-similarity"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Whisper_1).Equals("whisper-1"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Code_Search_Babbage_Text_001)
                                     .Equals("code-search-babbage-text-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0301).Equals("gpt-3.5-turbo-0301"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo).Equals("gpt-3.5-turbo"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_16k_0613).Equals("gpt-3.5-turbo-16k-0613"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_16k).Equals("gpt-3.5-turbo-16k"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0613).Equals("gpt-3.5-turbo-0613"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct_0914)
                                     .Equals("gpt-3.5-turbo-instruct-0914"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct).Equals("gpt-3.5-turbo-instruct"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Code_Search_Babbage_Code_001)
                                     .Equals("code-search-babbage-code-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Ada_001).Equals("text-ada-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_Ada_002).Equals("text-embedding-ada-002"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Similarity_Ada_001).Equals("text-similarity-ada-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Ada_Code_Search_Code).Equals("ada-code-search-code"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Ada_Similarity).Equals("ada-similarity"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Code_Search_Ada_Text_001).Equals("code-search-ada-text-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Ada_Query_001)
                                     .Equals("text-search-ada-query-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Davinci_Search_Document).Equals("davinci-search-document"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Ada_Code_Search_Text).Equals("ada-code-search-text"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Ada_Doc_001).Equals("text-search-ada-doc-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Davinci_Instruct_Beta).Equals("davinci-instruct-beta"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Similarity_Curie_001)
                                     .Equals("text-similarity-curie-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Code_Search_Ada_Code_001).Equals("code-search-ada-code-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Ada_Search_Query).Equals("ada-search-query"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Davinci_Query_001)
                                     .Equals("text-search-davinci-query-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Curie_Search_Query).Equals("curie-search-query"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Davinci_Search_Query).Equals("davinci-search-query"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Babbage_Search_Document).Equals("babbage-search-document"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Ada_Search_Document).Equals("ada-search-document"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Curie_Query_001)
                                     .Equals("text-search-curie-query-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Babbage_Doc_001)
                                     .Equals("text-search-babbage-doc-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Curie_Search_Document).Equals("curie-search-document"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Curie_Doc_001)
                                     .Equals("text-search-curie-doc-001"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Babbage_Search_Query).Equals("babbage-search-query"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Babbage_001).Equals("text-babbage-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Davinci_Doc_001)
                                     .Equals("text-search-davinci-doc-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Search_Babbage_Query_001)
                                     .Equals("text-search-babbage-query-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Curie_Similarity).Equals("curie-similarity"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Curie).Equals("curie"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Similarity_Davinci_001)
                                     .Equals("text-similarity-davinci-001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Davinci_002).Equals("text-davinci-002"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Davinci_Similarity).Equals("davinci-similarity"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Curie_001).Equals("text-curie:001"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4).Equals("gpt-4"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_0314).Equals("gpt-4-0314"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_0613).Equals("gpt-4-0613"));
                });

            It("OpenAIMainModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_3_5_Turbo_0301).Equals("gpt-3.5-turbo-0301"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_3_5_Turbo).Equals("gpt-3.5-turbo"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4).Equals("gpt-4"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4_0314).Equals("gpt-4-0314"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4_0613).Equals("gpt-4-0613"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_3_5_Turbo_Instruct).Equals("gpt-3.5-turbo-instruct"));
                });

            It("OpenAIModerationModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIModerationModelToString(EModerationsModelEnum::Text_Moderation_Latest)
                                     .Equals("text-moderation-latest"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIModerationModelToString(EModerationsModelEnum::Text_Moderation_Stable)
                                     .Equals("text-moderation-stable"));
                });

            It("OpenAIAudioModelToStringShouldReturnCorrectValue",
                [this]() { TestTrueExpr(UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModelEnum::Whisper_1).Equals("whisper-1")); });

            It("OpenAIImageSizeToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_256x256).Equals("256x256"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_512x512).Equals("512x512"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_1024x1024).Equals("1024x1024"));
                });

            It("OpenAIImageFormatToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL).Equals("url"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::B64_JSON).Equals("b64_json"));
                });

            It("OpenAIRoleToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::System).Equals("system"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::User).Equals("user"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant).Equals("assistant"));
                });

            It("StringToOpenAIRoleShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("system") == ERole::System);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("user") == ERole::User);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("assistant") == ERole::Assistant);
                });

            It("OpenAIFinishReasonToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Stop).Equals("stop"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Length).Equals("length"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Function_Call).Equals("function_call"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Content_Filter).Equals("content_filter"));
                });

            It("APITokensCanBeLoadedFromFile",
                [this]()
                {
                    const FString FilePath = FPaths::ProjectPluginsDir().Append("OpenAI/Source/OpenAITestRunner/Data/OpenAIAuth.ini");
                    const FOpenAIAuth Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FilePath);
                    TestTrueExpr(Auth.APIKey.Equals("sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
                    TestTrueExpr(Auth.OrganizationID.Equals("org-xxxxxxxxxxxxxxxxxxxxxxxx"));
                });

            It("OpenAIAudioTranscriptToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON).Equals("json"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::Text).Equals("text"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::Str).Equals("str"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::Verbose_JSON).Equals("verbose_json"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::Vtt).Equals("vtt"));
                });

            It("OpenAIEmbeddingsEncodingFormatToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat::Float).Equals("float"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat::Base64).Equals("base64"));
                });

            It("OpenAIModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    FOpenAIModel OpenAIModel;
                    OpenAIModel.ID = "openAIID";
                    OpenAIModel.Object = "obj";
                    OpenAIModel.Created = 9999;
                    OpenAIModel.Owned_By = "owned_by";
                    OpenAIModel.Root = "openAIID";
                    OpenAIModel.Parent = "openAIID";
                    const FString ModelsStr = UOpenAIFuncLib::OpenAIModelToString(OpenAIModel);

                    FString ExpectedStr = "id: openAIID\n";
                    ExpectedStr.Append("object: obj\n")
                        .Append("created: 9999\n")
                        .Append("owned_by: owned_by\n")
                        .Append("root: openAIID\n")
                        .Append("parent: openAIID\n");
                    TestTrueExpr(ModelsStr.Equals(ExpectedStr));
                });

            It("BoolToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::BoolToString(true).Equals("true"));
                    TestTrueExpr(UOpenAIFuncLib::BoolToString(false).Equals("false"));
                });

            It("OpenAIModerationsToStringReturnsCorrectValue",
                [this]()
                {
                    FModerationResults ModerationResults;
                    ModerationResults.Categories = FModerationCategories{true, false, true, false, true, false, true};
                    ModerationResults.Category_Scores = FModerationScores{0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
                    ModerationResults.Flagged = true;

                    FString ExpectedStr = "hate: true\n";
                    ExpectedStr.Append("hate/threatening: false\n");
                    ExpectedStr.Append("self-harm: true\n");
                    ExpectedStr.Append("sexual: false\n");
                    ExpectedStr.Append("sexual/minors: true\n");
                    ExpectedStr.Append("violence: false\n");
                    ExpectedStr.Append("violence/graphic: true\n\n");

                    ExpectedStr.Append("hate: 0.100000\n");
                    ExpectedStr.Append("hate/threatening: 0.200000\n");
                    ExpectedStr.Append("self-harm: 0.300000\n");
                    ExpectedStr.Append("sexual: 0.400000\n");
                    ExpectedStr.Append("sexual/minors: 0.500000\n");
                    ExpectedStr.Append("violence: 0.600000\n");
                    ExpectedStr.Append("violence/graphic: 0.700000\n\n");

                    ExpectedStr.Append("flagged: true");

                    const FString Out = UOpenAIFuncLib::OpenAIModerationsToString(ModerationResults);
                    TestTrueExpr(ExpectedStr.Equals(Out));
                });

            It("ErrorCodesShouldBeReturnedCorectly",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::GetErrorCode("") == EOpenAIResponseError::Unknown);
                    TestTrueExpr(UOpenAIFuncLib::GetErrorCode("Unknown message") == EOpenAIResponseError::Unknown);
                    TestTrueExpr(UOpenAIFuncLib::GetErrorCode("You have a ConnectionError") == EOpenAIResponseError::NetworkError);
                    TestTrueExpr(
                        UOpenAIFuncLib::GetErrorCode("{\"error\":{\"message\":\"\",\"type\":\"invalid_request_error\",\"param\":null,"
                                                     "\"code\":\"invalid_api_key\"}}") == EOpenAIResponseError::InvalidAPIKey);
                    TestTrueExpr(
                        UOpenAIFuncLib::GetErrorCode("{\"error\":{\"message\":\"\",\"type\":\"invalid_request_error\",\"param\":null,"
                                                     "\"code\":\"model_not_found\"}}") == EOpenAIResponseError::ModelNotFound);
                    TestTrueExpr(
                        UOpenAIFuncLib::GetErrorCode(
                            "{\"error\":{\"message\":\"\",\"type\":\"invalid_request_error\",\"param\":null,\"code\":\"invalid\"}}") ==
                        EOpenAIResponseError::Unknown);
                });

            It("ErrorMessagesShouldBeReturnedCorectly",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::GetErrorMessage("").IsEmpty());
                    TestTrueExpr(UOpenAIFuncLib::GetErrorMessage("Not a valid JSON message").IsEmpty());
                    TestTrueExpr(UOpenAIFuncLib::GetErrorMessage(
                        "{\"error\":{\"message\":\"invalid API key\",\"type\":\"invalid_request_error\",\"param\":null,"
                        "\"code\":\"invalid_api_key\"}}")
                                     .Equals("invalid API key"));
                    TestTrueExpr(UOpenAIFuncLib::GetErrorMessage("{\"error\":{\"type\":\"invalid_request_error\",\"param\":null,"
                                                                 "\"code\":\"invalid_api_key\"}}")
                                     .IsEmpty());
                });

            It("OpenAIResponseErrorToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::ResponseErrorToString(EOpenAIResponseError::InvalidAPIKey).Equals("Invalid API key"));
                    TestTrueExpr(UOpenAIFuncLib::ResponseErrorToString(EOpenAIResponseError::NetworkError).Equals("Network error"));
                    TestTrueExpr(UOpenAIFuncLib::ResponseErrorToString(EOpenAIResponseError::ModelNotFound).Equals("Model not found"));
                    TestTrueExpr(UOpenAIFuncLib::ResponseErrorToString(EOpenAIResponseError::Unknown).Equals("Unknown error"));
                });
        });

    Describe("FileSystemFuncLib",
        [this]()
        {
            It("GetFileExtensionsShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(
                        UFileSystemFuncLib::GetFileExtensions(EFileType::Image).Equals("ImageFiles (png, jpg, jpeg)|*.png;*.jpg;*.jpeg"));
                    TestTrueExpr(
                        UFileSystemFuncLib::GetFileExtensions(EFileType::Media)
                            .Equals("MediaFiles (mp3, mp4, mpeg, m4a, mpga, wav, webm)|*.mp3;*.mp4;*.mpeg;*.m4a;*.mpga;*.wav;*.webm"));
                });

            It("ChatHistoryMightBeSavedToTheFile",
                [this]()
                {
                    const FString FilePath = FPaths::ProjectPluginsDir().Append("OpenAI/Source/OpenAITestRunner/Data/History.txt");
                    const TArray<FMessage> History = {FMessage{"user", "hello!"}, FMessage{"assistant", "hello bro!"}};
                    const FString ModelName = "gpt-4";
                    TestTrueExpr(UFileSystemFuncLib::SaveChatHistoryToFile(History, ModelName, FilePath));

                    TArray<FString> FileLines;
                    if (!FFileHelper::LoadFileToStringArray(FileLines, *FilePath))
                    {
                        AddError("Failed loading file");
                    }

                    TestTrueExpr(FileLines.Num() == 4);
                    TestTrueExpr(FileLines[0].Equals(FString::Printf(TEXT("[model]: %s"), *ModelName)));
                    TestTrueExpr(FileLines[2].Equals("[user]: hello!"));
                    TestTrueExpr(FileLines[3].Equals("[assistant]: hello bro!"));

                    IFileManager::Get().Delete(*FilePath);
                });
        });
}

#endif

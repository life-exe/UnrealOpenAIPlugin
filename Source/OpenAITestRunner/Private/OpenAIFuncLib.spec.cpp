// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/FileSystemFuncLib.h"
#include "Provider/Types/FineTuneTypes.h"
#include "TestUtils.h"

DEFINE_SPEC(FOpenAIFuncLib, "OpenAI",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

void FOpenAIFuncLib::Define()
{
    Describe("OpenAIFuncLib",
        [this]()
        {
            It("AllModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Whisper_1).Equals("whisper-1"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo).Equals("gpt-3.5-turbo"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_16k).Equals("gpt-3.5-turbo-16k"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct_0914)
                                     .Equals("gpt-3.5-turbo-instruct-0914"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct).Equals("gpt-3.5-turbo-instruct"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_Ada_002).Equals("text-embedding-ada-002"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4).Equals("gpt-4"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_0314).Equals("gpt-4-0314"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_0613).Equals("gpt-4-0613"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::DALL_E_2).Equals("dall-e-2"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::DALL_E_3).Equals("dall-e-3"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_1106_Preview).Equals("gpt-4-1106-preview"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview).Equals("gpt-4-vision-preview"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_1106).Equals("gpt-3.5-turbo-1106"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::TTS_1).Equals("tts-1"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::TTS_1_HD).Equals("tts-1-hd"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::TTS_1_1106).Equals("tts-1-1106"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::TTS_1_HD_1106).Equals("tts-1-hd-1106"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_3_Large).Equals("text-embedding-3-large"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_32K_0314).Equals("gpt-4-32k-0314"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0125).Equals("gpt-3.5-turbo-0125"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_3_Small).Equals("text-embedding-3-small"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_0125_Preview).Equals("gpt-4-0125-preview"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Turbo_Preview).Equals("gpt-4-turbo-preview"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_2024_05_13).Equals("gpt-4o-2024-05-13"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O).Equals("gpt-4o"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Turbo_2024_04_09).Equals("gpt-4-turbo-2024-04-09"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Turbo).Equals("gpt-4-turbo"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_1106_Vision_Preview)
                                     .Equals("gpt-4-1106-vision-preview"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini).Equals("gpt-4o-mini"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini_2024_07_18).Equals("gpt-4o-mini-2024-07-18"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::ChatGPT_4O_Latest).Equals("chatgpt-4o-latest"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_2024_08_06).Equals("gpt-4o-2024-08-06"));
                });

            It("OpenAIMainModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_3_5_Turbo).Equals("gpt-3.5-turbo"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4O).Equals("gpt-4o"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4).Equals("gpt-4"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4_0314).Equals("gpt-4-0314"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4_0613).Equals("gpt-4-0613"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4_1106_Preview).Equals("gpt-4-1106-preview"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4_Vision_Preview).Equals("gpt-4-vision-preview"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_3_5_Turbo_Instruct).Equals("gpt-3.5-turbo-instruct"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4O_Mini).Equals("gpt-4o-mini"));
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
                [this]() { TestTrueExpr(UOpenAIFuncLib::OpenAIAudioModelToString(EAudioModel::Whisper_1).Equals("whisper-1")); });

            It("OpenAITTSModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAITTSModelToString(ETTSModel::TTS_1).Equals("tts-1"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAITTSModelToString(ETTSModel::TTS_1_HD).Equals("tts-1-hd"));
                });

            It("OpenAIVoiceToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIVoiceToString(EVoice::Alloy).Equals("alloy"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIVoiceToString(EVoice::Echo).Equals("echo"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIVoiceToString(EVoice::Fable).Equals("fable"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIVoiceToString(EVoice::Nova).Equals("nova"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIVoiceToString(EVoice::Onyx).Equals("onyx"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIVoiceToString(EVoice::Shimmer).Equals("shimmer"));
                });

            It("OpenAITTSAudioFormatToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAITTSAudioFormatToString(ETTSAudioFormat::AAC).Equals("aac"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAITTSAudioFormatToString(ETTSAudioFormat::FLAC).Equals("flac"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAITTSAudioFormatToString(ETTSAudioFormat::MP3).Equals("mp3"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAITTSAudioFormatToString(ETTSAudioFormat::OPUS).Equals("opus"));
                });

            It("OpenAIImageModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::DALL_E_2).Equals("dall-e-2"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::DALL_E_3).Equals("dall-e-3"));
                });

            It("StringToOpenAIImageModelShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageModel("dall-e-2") == EImageModelEnum::DALL_E_2);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageModel("dall-e-3") == EImageModelEnum::DALL_E_3);
                });

            It("StringToOpenAIImageSizeDalle2ShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageSizeDalle2("256x256") == EImageSizeDalle2::Size_256x256);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageSizeDalle2("512x512") == EImageSizeDalle2::Size_512x512);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageSizeDalle2("1024x1024") == EImageSizeDalle2::Size_1024x1024);
                });

            It("StringToOpenAIImageSizeDalle3ShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageSizeDalle3("1024x1024") == EImageSizeDalle3::Size_1024x1024);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageSizeDalle3("1024x1792") == EImageSizeDalle3::Size_1024x1792);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageSizeDalle3("1792x1024") == EImageSizeDalle3::Size_1792x1024);
                });

            It("StringToOpenAIImageFormatShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageFormat("url") == EOpenAIImageFormat::URL);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageFormat("b64_json") == EOpenAIImageFormat::B64_JSON);
                });

            It("StringToOpenAIImageQualityShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageQuality("hd") == EOpenAIImageQuality::HD);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageQuality("standard") == EOpenAIImageQuality::Standard);
                });

            It("StringToOpenAIImageStyleShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageStyle("natural") == EOpenAIImageStyle::Natural);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIImageStyle("vivid") == EOpenAIImageStyle::Vivid);
                });

            It("OpenAIImageSizeDalle2ToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_256x256).Equals("256x256"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_512x512).Equals("512x512"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_1024x1024).Equals("1024x1024"));
                });

            It("OpenAIImageSizeDalle3ToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeDalle3ToString(EImageSizeDalle3::Size_1024x1024).Equals("1024x1024"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeDalle3ToString(EImageSizeDalle3::Size_1024x1792).Equals("1024x1792"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageSizeDalle3ToString(EImageSizeDalle3::Size_1792x1024).Equals("1792x1024"));
                });

            It("OpenAIImageFormatToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL).Equals("url"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::B64_JSON).Equals("b64_json"));
                });

            It("OpenAIImageStyleToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageStyleToString(EOpenAIImageStyle::Natural).Equals("natural"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageStyleToString(EOpenAIImageStyle::Vivid).Equals("vivid"));
                });

            It("OpenAIImageQualityToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality::HD).Equals("hd"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality::Standard).Equals("standard"));
                });

            It("OpenAIRoleToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::System).Equals("system"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::User).Equals("user"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant).Equals("assistant"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::Function).Equals("function"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIRoleToString(ERole::Tool).Equals("tool"));
                });

            It("StringToOpenAIRoleShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("system") == ERole::System);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("user") == ERole::User);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("assistant") == ERole::Assistant);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("function") == ERole::Function);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIRole("tool") == ERole::Tool);
                });

            It("OpenAIFinishReasonToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Stop).Equals("stop"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Length).Equals("length"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Tool_Calls).Equals("tool_calls"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Content_Filter).Equals("content_filter"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIFinishReasonToString(EOpenAIFinishReason::Null).Equals(""));
                });
            It("StringToOpenAIFinishReasonShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIFinishReason("stop") == EOpenAIFinishReason::Stop);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIFinishReason("length") == EOpenAIFinishReason::Length);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIFinishReason("tool_calls") == EOpenAIFinishReason::Tool_Calls);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIFinishReason("content_filter") == EOpenAIFinishReason::Content_Filter);
                    TestTrueExpr(UOpenAIFuncLib::StringToOpenAIFinishReason("") == EOpenAIFinishReason::Null);
                });

            It("APITokensCanBeLoadedFromFile",
                [this]()
                {
                    const FString FilePath = OpenAI::Tests::TestUtils::FileFullPath("OpenAIAuth.ini");
                    const FOpenAIAuth Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FilePath);
                    TestTrueExpr(Auth.APIKey.Equals("sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
                    TestTrueExpr(Auth.OrganizationID.Equals("org-xxxxxxxxxxxxxxxxxxxxxxxx"));
                    TestTrueExpr(Auth.ProjectID.Equals("proj_xxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
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

            It("OpenAIChatResponseFormatToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIChatResponseFormatToString(EChatResponseFormat::Text).Equals("text"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIChatResponseFormatToString(EChatResponseFormat::JSON_Object).Equals("json_object"));
                });

            It("OpenAIModelToStringShouldReturnCorrectValue",
                [this]()
                {
                    FOpenAIModel OpenAIModel;
                    OpenAIModel.ID = "openAIID";
                    OpenAIModel.Object = "obj";
                    OpenAIModel.Created = 9999;
                    OpenAIModel.Owned_By = "owned_by";
                    const FString ModelsStr = UOpenAIFuncLib::OpenAIModelToString(OpenAIModel);

                    FString ExpectedStr = "id: openAIID\n";
                    ExpectedStr.Append("object: obj\n").Append("created: 9999\n").Append("owned_by: owned_by\n");
                    TestTrueExpr(ModelsStr.Equals(ExpectedStr));
                });

            It("OpenAIMessageContentTypeToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Text).Equals("text"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Image_URL).Equals("image_url"));
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
                    ModerationResults.Categories =
                        FModerationCategories{true, false, false, false, true, true, true, false, true, false, true};
                    ModerationResults.Category_Scores = FModerationCategoryScores{0.1, 0.2, 0.1, 0.1, 0.3, 0.33, 0.35, 0.4, 0.5, 0.6, 0.7};
                    ModerationResults.Flagged = true;

                    FString ExpectedStr = "hate: true\n";
                    ExpectedStr.Append("hate/threatening: false\n");
                    ExpectedStr.Append("harassment: false\n");
                    ExpectedStr.Append("harassment/threatening: false\n");
                    ExpectedStr.Append("self-harm: true\n");
                    ExpectedStr.Append("self-harm/intent: true\n");
                    ExpectedStr.Append("self-harm/instructions: true\n");
                    ExpectedStr.Append("sexual: false\n");
                    ExpectedStr.Append("sexual/minors: true\n");
                    ExpectedStr.Append("violence: false\n");
                    ExpectedStr.Append("violence/graphic: true\n\n");

                    ExpectedStr.Append("hate: 0.100000\n");
                    ExpectedStr.Append("hate/threatening: 0.200000\n");
                    ExpectedStr.Append("harassment: 0.100000\n");
                    ExpectedStr.Append("harassment/threatening: 0.100000\n");
                    ExpectedStr.Append("self-harm: 0.300000\n");
                    ExpectedStr.Append("self-harm/intent: 0.330000\n");
                    ExpectedStr.Append("self-harm/instructions: 0.350000\n");
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
                        UOpenAIFuncLib::GetErrorCode("{\"error\":{\"message\":\"\",\"type\":\"insufficient_quota\",\"param\":null,"
                                                     "\"code\":\"insufficient_quota\"}}") == EOpenAIResponseError::InsufficientQuota);
                    TestTrueExpr(UOpenAIFuncLib::GetErrorCode(
                                     "{\"error\":{\"message\":\"\",\"type\":\"invalid_request_error\",\"param\":null,"
                                     "\"code\":\"invalid_language_format\"}}") == EOpenAIResponseError::InvalidLanguageFormat);
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

            It("WhiteSpacesShouldBeRemovedCorrectly",
                [this]()
                {
                    const FString NormalString = "Hello, how are you?";
                    TestTrueExpr(UOpenAIFuncLib::RemoveWhiteSpaces(NormalString).Equals(NormalString));

                    const FString StringWithWhiteSpaces = "Hello\t, \nhow are you\r?";
                    TestTrueExpr(UOpenAIFuncLib::RemoveWhiteSpaces(StringWithWhiteSpaces).Equals(NormalString));
                });

            It("URLWithQueryCanBECreatedCorrectly",
                [this]()
                {
                    OpenAI::QueryPairs QueryArgs;
                    QueryArgs.Add(MakeTuple("param1", "value1"));
                    QueryArgs.Add(MakeTuple("param2", "value2"));

                    const auto ResultURL = UOpenAIFuncLib::MakeURLWithQuery("https://myapi.com", QueryArgs);
                    TestTrueExpr(ResultURL.Equals("https://myapi.com?param1=value1&param2=value2"));
                });

            It("APISecretCanBeLoadedByName",
                [this]()
                {
                    OpenAI::ServiceSecrets Secrets;
                    Secrets.Add(MakeTuple("ApiKey1", "adsgsfhgsgfhg"));
                    Secrets.Add(MakeTuple("ApiKey2", "kjhtlkjgkghl"));
                    Secrets.Add(MakeTuple("ApiKey3", "ghkjfyifkfhjkfhjgkj"));

                    FString FoundApiKey;
                    TestTrueExpr(UOpenAIFuncLib::LoadSecretByName(Secrets, "ApiKey2", FoundApiKey));
                    TestTrueExpr(FoundApiKey.Equals("kjhtlkjgkghl"));
                });

            It("ServiceSecretsCanBeLoadedFromFile",
                [this]()
                {
                    const FString FilePath = OpenAI::Tests::TestUtils::FileFullPath("OnlineServicesAuth.ini");
                    const OpenAI::ServiceSecrets Secrets = UOpenAIFuncLib::LoadServiceSecretsFromFile(FilePath);

                    TestTrueExpr(Secrets[0].Key.Equals("key1"));
                    TestTrueExpr(Secrets[0].Value.Equals("KeYvAlUe1"));
                    TestTrueExpr(Secrets[1].Key.Equals("key2"));
                    TestTrueExpr(Secrets[1].Value.Equals("KeYvAlUe2"));
                });

            It("OpenAIUploadFilePurposeToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::Assistants).Equals("assistants"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::Vision).Equals("vision"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::Batch).Equals("batch"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::FineTune).Equals("fine-tune"));
                });

            It("OpenAIBatchEndpointToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIBatchEndpointToString(EBatchEndpoint::ChatCompletions).Equals("/v1/chat/completions"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIBatchEndpointToString(EBatchEndpoint::Completions).Equals("/v1/completions"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIBatchEndpointToString(EBatchEndpoint::Embeddings).Equals("/v1/embeddings"));
                });

            It("OpenAIBatchCompletionWindowToStringShouldReturnCorrectValue", [this]()
                { TestTrueExpr(UOpenAIFuncLib::OpenAIBatchCompletionWindowToString(EBatchCompletionWindow::Window_24h).Equals("24h")); });

            It("OpenAIServiceTierToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIServiceTierToString(EServiceTier::Auto).Equals("auto"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIServiceTierToString(EServiceTier::Default).Equals("default"));
                });

            It("OpenAIUploadStatusToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Pending).Equals("pending"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Completed).Equals("completed"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIUploadStatusToString(EUploadStatus::Cancelled).Equals("cancelled"));
                });

            It("OpenAIHeaderTypeToStringShouldReturnCorrectValue",
                [this]()
                {
                    TestTrueExpr(UOpenAIFuncLib::OpenAIHeaderTypeToString(EOpenAIHttpHeaderType::XRequestId).Equals("x-request-id"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIHeaderTypeToString(EOpenAIHttpHeaderType::OpenAIProcessingMS).Equals("openai-processing-ms"));
                    TestTrueExpr(
                        UOpenAIFuncLib::OpenAIHeaderTypeToString(EOpenAIHttpHeaderType::OpenAIOrganization).Equals("openai-organization"));
                    TestTrueExpr(UOpenAIFuncLib::OpenAIHeaderTypeToString(EOpenAIHttpHeaderType::OpenAIVersion).Equals("openai-version"));
                });

            It("AIHttpHeadersShouldBeFound",
                [this]()
                {
                    FOpenAIResponseMetadata Metadata;
                    Metadata.HttpHeaders = {TEXT("x-request-id: req_d6d70ef9bcbf2f0e4f74d76bff6dc01a"), TEXT("openai-processing-ms: 9106"),
                        TEXT("openai-organization: my-org"), TEXT("openai-version: v1")};

                    TestTrueExpr(UOpenAIFuncLib::FindOpenAIHttpHeaderByType(Metadata, EOpenAIHttpHeaderType::XRequestId)
                                     .Equals("req_d6d70ef9bcbf2f0e4f74d76bff6dc01a"));
                    TestTrueExpr(
                        UOpenAIFuncLib::FindOpenAIHttpHeaderByType(Metadata, EOpenAIHttpHeaderType::OpenAIProcessingMS).Equals("9106"));
                    TestTrueExpr(
                        UOpenAIFuncLib::FindOpenAIHttpHeaderByType(Metadata, EOpenAIHttpHeaderType::OpenAIOrganization).Equals("my-org"));
                    TestTrueExpr(UOpenAIFuncLib::FindOpenAIHttpHeaderByType(Metadata, EOpenAIHttpHeaderType::OpenAIVersion).Equals("v1"));
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
                    TArray<FMessage> History;

                    FMessage Message;
                    Message.Role = "user";
                    Message.Content = "hello!";
                    History.Add(Message);

                    Message.Role = "assistant";
                    Message.Content = "hello bro!";
                    History.Add(Message);

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

    Describe("FineTuningQueryParameters",
        [this]()
        {
            It("ShouldBeEmptyIfNotSet",
                [this]()
                {
                    const FFineTuningQueryParameters Parameters;
                    TestTrueExpr(Parameters.ToQuery().IsEmpty());
                });
            It("ShouldBeCorrectIfOneParamIsSet",
                [this]()
                {
                    FFineTuningQueryParameters Parameters;
                    Parameters.After.Set("event-id");
                    TestTrueExpr(Parameters.ToQuery().Equals("?after=event-id"));
                });
            It("ShouldBeCorrectIfTwoParamsAreSet",
                [this]()
                {
                    FFineTuningQueryParameters Parameters;
                    Parameters.After.Set("event-id");
                    Parameters.Limit.Set(20);
                    TestTrueExpr(Parameters.ToQuery().Equals("?after=event-id&limit=20"));
                });
        });
}

#endif

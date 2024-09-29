// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "Sample/APIOverview.h"
#include "Provider/OpenAIProvider.h"
#include "Provider/Types/AudioTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Algo/ForEach.h"
#include "API/API.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogAPIOverview, All, All);

// Please check which models that are compatible with each request:
// https://platform.openai.com/docs/models/model-endpoint-compatibility

AAPIOverview::AAPIOverview()
{
    PrimaryActorTick.bCanEverTick = false;

    ActionMap.Add(EAPIOverviewAction::ListModels, [&]() { ListModels(); });
    ActionMap.Add(EAPIOverviewAction::RetrieveModel, [&]() { RetrieveModel(); });
    ActionMap.Add(EAPIOverviewAction::DeleteFineTunedModel, [&]() { DeleteFinedTuneModel(); });
    ActionMap.Add(EAPIOverviewAction::CreateCompletionRequest, [&]() { CreateCompletionRequest(); });
    ActionMap.Add(EAPIOverviewAction::CreateChatCompletionRequest, [&]() { CreateChatCompletionRequest(); });
    ActionMap.Add(EAPIOverviewAction::CreateImageDALLE2, [&]() { CreateImageDALLE2(); });
    ActionMap.Add(EAPIOverviewAction::CreateImageDALLE3, [&]() { CreateImageDALLE3(); });
    ActionMap.Add(EAPIOverviewAction::CreateImageEdit, [&]() { CreateImageEdit(); });
    ActionMap.Add(EAPIOverviewAction::CreateImageVariation, [&]() { CreateImageVariation(); });
    ActionMap.Add(EAPIOverviewAction::CreateModerations, [&]() { CreateModerations(); });
    ActionMap.Add(EAPIOverviewAction::CreateEmbeddings, [&]() { CreateEmbeddings(); });
    ActionMap.Add(EAPIOverviewAction::CreateSpeech, [&]() { CreateSpeech(); });
    ActionMap.Add(EAPIOverviewAction::CreateAudioTranscription, [&]() { CreateAudioTranscription(); });
    ActionMap.Add(EAPIOverviewAction::CreateAudioTranscriptionVerbose, [&]() { CreateAudioTranscriptionVerbose(); });
    ActionMap.Add(EAPIOverviewAction::CreateAudioTranslation, [&]() { CreateAudioTranslation(); });
    ActionMap.Add(EAPIOverviewAction::UploadFile, [&]() { UploadFile(); });
    ActionMap.Add(EAPIOverviewAction::DeleteFile, [&]() { DeleteFile(); });
    ActionMap.Add(EAPIOverviewAction::RetrieveFile, [&]() { RetrieveFile(); });
    ActionMap.Add(EAPIOverviewAction::RetrieveFileContent, [&]() { RetrieveFileContent(); });
    ActionMap.Add(EAPIOverviewAction::ListFiles, [&]() { ListFiles(); });
    ActionMap.Add(EAPIOverviewAction::ListFineTuningJobs, [&]() { ListFineTuningJobs(); });
    ActionMap.Add(EAPIOverviewAction::ListFineTuningEvents, [&]() { ListFineTuningEvents(); });
    ActionMap.Add(EAPIOverviewAction::ListFineTuningCheckpoints, [&]() { ListFineTuningCheckpoints(); });
    ActionMap.Add(EAPIOverviewAction::CreateFineTuningJob, [&]() { CreateFineTuningJob(); });
    ActionMap.Add(EAPIOverviewAction::RetriveFineTuningJob, [&]() { RetriveFineTuningJob(); });
    ActionMap.Add(EAPIOverviewAction::CancelFineTuningJob, [&]() { CancelFineTuningJob(); });
    ActionMap.Add(EAPIOverviewAction::ListBatch, [&]() { ListBatch(); });
    ActionMap.Add(EAPIOverviewAction::CreateBatch, [&]() { CreateBatch(); });
    ActionMap.Add(EAPIOverviewAction::RetrieveBatch, [&]() { RetrieveBatch(); });
    ActionMap.Add(EAPIOverviewAction::CancelBatch, [&]() { CancelBatch(); });
    ActionMap.Add(EAPIOverviewAction::CreateUpload, [&]() { CreateUpload(); });
    ActionMap.Add(EAPIOverviewAction::AddUploadPart, [&]() { AddUploadPart(); });
    ActionMap.Add(EAPIOverviewAction::CompleteUpload, [&]() { CompleteUpload(); });
    ActionMap.Add(EAPIOverviewAction::CancelUpload, [&]() { CancelUpload(); });
    ActionMap.Add(EAPIOverviewAction::SetYourOwnAPI, [&]() { SetYourOwnAPI(); });
}

void AAPIOverview::BeginPlay()
{
    Super::BeginPlay();

    Provider = NewObject<UOpenAIProvider>();
    const FString FilePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("OpenAIAuth.ini"));
    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FilePath);

    if (ActionMap.Contains(Action))
    {
        ActionMap[Action]();
    }
}

void AAPIOverview::ListModels()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListModelsCompleted().AddLambda(
        [](const FListModelsResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(Response.Data, [&](const FOpenAIModel& Model) { OutputString.Append(Model.ID).Append(LINE_TERMINATOR); });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });

    Provider->ListModels(Auth);
}

void AAPIOverview::RetrieveModel()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveModelCompleted().AddLambda(
        [](const FRetrieveModelResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
        { UE_LOGFMT(LogAPIOverview, Display, "{0}", UOpenAIFuncLib::OpenAIModelToString(Response)); });
    const auto ModelName = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);
    Provider->RetrieveModel(ModelName, Auth);
}

void AAPIOverview::DeleteFinedTuneModel()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnDeleteFineTunedModelCompleted().AddLambda(
        [](const FDeleteFineTunedModelResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOG(LogAPIOverview, Display, TEXT("DeleteFineTunedModel request completed!"));
        });

    const FString ModelID{"curie:ft-lifeexe-2023-06-12-19-57-37"};
    Provider->DeleteFineTunedModel(ModelID, Auth);
}

void AAPIOverview::CreateCompletionRequest()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateCompletionCompleted().AddLambda(
        [](const FCompletionResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(Response.Choices, [&](const FChoice& Choice) { OutputString.Append(Choice.Text); });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });
    Provider->OnCreateCompletionStreamProgresses().AddLambda(
        [](const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(Responses, [&](const FCompletionStreamResponse& StreamResponse) {  //
                Algo::ForEach(StreamResponse.Choices, [&](const FChoice& Choice) {           //
                    OutputString.Append(Choice.Text);
                });
            });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });
    Provider->OnCreateCompletionStreamCompleted().AddLambda(
        [](const TArray<FCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& Metadata)  //
        {                                                                                                //
            UE_LOGFMT(LogAPIOverview, Display, "Stream message generation finished");
        });

    FCompletion Completion;
    Completion.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_Instruct);
    Completion.Prompt = "What is Unreal Engine?";
    Completion.Stream = true;
    Completion.Max_Tokens = 100;

    Provider->CreateCompletion(Completion, Auth);
}

void AAPIOverview::CreateChatCompletionRequest()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateChatCompletionCompleted().AddLambda(
        [](const FChatCompletionResponse& Response, const FOpenAIResponseMetadata& Metadata)  //
        {                                                                                     //
            UE_LOGFMT(LogAPIOverview, Display, "Message generation finished");
        });
    Provider->OnCreateChatCompletionStreamProgresses().AddLambda(
        [](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
                Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
                    OutputString.Append(Choice.Delta.Content);
                });
            });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });
    Provider->OnCreateChatCompletionStreamCompleted().AddLambda(
        [](const TArray<FChatCompletionStreamResponse>& Responses, const FOpenAIResponseMetadata& Metadata)  //
        {                                                                                                    //
            UE_LOGFMT(LogAPIOverview, Display, "Stream message generation finished");
        });

    FChatCompletion ChatCompletion;
    ChatCompletion.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O);

    FMessage Message;
    Message.Role = UOpenAIFuncLib::OpenAIRoleToString(ERole::User);
    Message.Content = "What is Unreal Engine?";

    ChatCompletion.Messages.Add(Message);
    ChatCompletion.Stream = true;
    ChatCompletion.Max_Completion_Tokens.Set(100);

    Provider->CreateChatCompletion(ChatCompletion, Auth);
}

void AAPIOverview::CreateImageDALLE2()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageCompleted().AddLambda(
        [&](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });

    FOpenAIImage Image;
    Image.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::DALL_E_2);
    Image.Prompt = "Tiger is eating pizza";
    Image.Size = UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_512x512);
    Image.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);
    Image.N = 2;

    Provider->CreateImage(Image, Auth);
}

void AAPIOverview::CreateImageDALLE3()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageCompleted().AddLambda(
        [](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });

    FOpenAIImage Image;
    Image.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::DALL_E_3);
    Image.N = 1;  // only one image is now supported.
    Image.Prompt = "Bear with beard drinking beer";
    Image.Size = UOpenAIFuncLib::OpenAIImageSizeDalle3ToString(EImageSizeDalle3::Size_1024x1024);
    Image.Response_Format =
        UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);  // B64_JSON is not currently supported by the plugin.
    Image.Quality = UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality::Standard);
    Image.Style = UOpenAIFuncLib::OpenAIImageStyleToString(EOpenAIImageStyle::Natural);

    Provider->CreateImage(Image, Auth);
}

void AAPIOverview::CreateImageEdit()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);

    Provider->OnCreateImageEditCompleted().AddLambda(
        [](const FImageEditResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });

    FOpenAIImageEdit ImageEdit;

    // absolute paths to your images
    const FString ImageFilePath = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), "whale.png");
    const FString ImageMaskFilePath = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), "whale_mask.png");

    ImageEdit.Image = FPaths::ConvertRelativePathToFull(ImageFilePath);
    ImageEdit.Mask = FPaths::ConvertRelativePathToFull(ImageMaskFilePath);
    ImageEdit.N = 1;
    ImageEdit.Prompt = "Draw a hat";
    ImageEdit.Size = UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_256x256);
    ImageEdit.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

    Provider->CreateImageEdit(ImageEdit, Auth);
}

void AAPIOverview::CreateImageVariation()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageVariationCompleted().AddLambda(
        [](const FImageVariationResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });

    FOpenAIImageVariation ImageVariation;
    // absolute path to your image
    const FString ImageFilePath = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), "whale.png");
    ImageVariation.Image = FPaths::ConvertRelativePathToFull(ImageFilePath);
    ImageVariation.N = 1;
    ImageVariation.Size = UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_256x256);
    ImageVariation.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

    Provider->CreateImageVariation(ImageVariation, Auth);
}

void AAPIOverview::CreateModerations()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateModerationsCompleted().AddLambda(
        [](const FModerationsResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(Response.Results,
                [&](const FModerationResults& Results) { OutputString.Append(UOpenAIFuncLib::OpenAIModerationsToString(Results)); });
            UE_LOGFMT(LogAPIOverview, Display, "{0}", OutputString);
        });

    FModerations Moderations;
    Moderations.Input = {"I want to #@*! you!"};  // type your favorite phrase (=
    Moderations.Model = "text-moderation-stable";
    Provider->CreateModerations(Moderations, Auth);
}

void AAPIOverview::CreateEmbeddings()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateEmbeddingsCompleted().AddLambda(
        [](const FEmbeddingsResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "CreateEmbeddings request completed!");
        });

    FEmbeddings Embeddings;
    Embeddings.Input = {"Hello! How are you?"};
    Embeddings.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_Ada_002);
    Provider->CreateEmbeddings(Embeddings, Auth);
}

void AAPIOverview::CreateSpeech()
{
    // Make sure that WMFCodecs plugin is enabled (Edit->Plugins->WMFCodecs)
    const FString Format = UOpenAIFuncLib::OpenAITTSAudioFormatToString(ETTSAudioFormat::MP3);
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateSpeechCompleted().AddLambda([Format](const FSpeechResponse& Response, const FOpenAIResponseMetadata& Metadata) {  //
        const FString Date = FDateTime::Now().ToString();
        const FString FilePath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("OpenAI"), TEXT("Saved"));
        const FString FileName = FString("speech_").Append(Date).Append(".").Append(Format);
        const FString FileFullName = FPaths::Combine(FilePath, FileName);
        if (FFileHelper::SaveArrayToFile(Response.Bytes, *FileFullName))
        {
            UE_LOGFMT(LogAPIOverview, Display, "File was successfully saved to: {0}", FileFullName);
        }
    });
    FSpeech Speech;
    Speech.Input = "Crazy fox running through the forest.";
    Speech.Model = UOpenAIFuncLib::OpenAITTSModelToString(ETTSModel::TTS_1);
    Speech.Response_Format = Format;
    Speech.Speed = 1.0f;
    Provider->CreateSpeech(Speech, Auth);
}

void AAPIOverview::CreateAudioTranscription()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateAudioTranscriptionCompleted().AddLambda(
        [](const FAudioTranscriptionResponse& Response, const FOpenAIResponseMetadata& Metadata)  //
        {                                                                                         //
            UE_LOGFMT(LogAPIOverview, Display, "TranscriptionResponse: {0}", Response.Text);
        });

    FAudioTranscription AudioTranscription;
    // https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes
    AudioTranscription.Language = "fr";
    AudioTranscription.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Whisper_1);
    AudioTranscription.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
    AudioTranscription.Temperature = 0.0f;
    // absolute path to your file
    AudioTranscription.File = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), TEXT("bonjour.mp3"));
    Provider->CreateAudioTranscription(AudioTranscription, Auth);
}

void AAPIOverview::CreateAudioTranscriptionVerbose()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateAudioTranscriptionVerboseCompleted().AddLambda(
        [](const FAudioTranscriptionVerboseResponse& Response, const FOpenAIResponseMetadata& Metadata)  //
        {                                                                                                //
            UE_LOGFMT(LogAPIOverview, Display, "TranscriptionVerboseResponse: text:{0}", Response.Text);
            if (Response.Segments.Num() > 0)
            {
                UE_LOGFMT(LogAPIOverview, Display, "TranscriptionVerboseResponse: compression:{0}", Response.Segments[0].Compression_Ratio);
            }
        });

    FAudioTranscription AudioTranscription;
    // https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes
    AudioTranscription.Language = "fr";
    AudioTranscription.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Whisper_1);
    AudioTranscription.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::Verbose_JSON);
    AudioTranscription.Temperature = 0.0f;
    // absolute path to your file
    AudioTranscription.File = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), TEXT("bonjour.mp3"));
    Provider->CreateAudioTranscription(AudioTranscription, Auth);
}

void AAPIOverview::CreateAudioTranslation()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateAudioTranslationCompleted().AddLambda(
        [](const FAudioTranslationResponse& Response, const FOpenAIResponseMetadata& Metadata)  //
        {                                                                                       //
            UE_LOGFMT(LogAPIOverview, Display, "{0}", Response.Text);
        });

    FAudioTranslation AudioTranslation;
    AudioTranslation.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Whisper_1);
    AudioTranslation.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
    AudioTranslation.Temperature = 0.0f;
    // absolute path to your file
    AudioTranslation.File = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), TEXT("bonjour.mp3"));
    Provider->CreateAudioTranslation(AudioTranslation, Auth);
}

void AAPIOverview::UploadFile()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnUploadFileCompleted().AddLambda(
        [](const FUploadFileResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "UploadFile request completed! {0}", Response.ID);
        });

    FUploadFile UploadFile;
    // absolute path to your file
    const FString FilePath = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), "test_file.jsonl");
    UploadFile.File = FilePath;
    // UploadFile.Purpose = UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::Batch);
    UploadFile.Purpose = UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::FineTune);
    Provider->UploadFile(UploadFile, Auth);
}

void AAPIOverview::DeleteFile()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnDeleteFileCompleted().AddLambda(
        [](const FDeleteFileResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "DeleteFile request completed!");
        });

    const FString FileID{"file-xxxxxxxxxxxxxxxxxxx"};
    Provider->DeleteFile(FileID, Auth);
}

void AAPIOverview::ListFiles()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFilesCompleted().AddLambda(
        [](const FListFilesResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FOpenAIFile& OpenAIFile) { OutputString.Append(OpenAIFile.FileName).Append(LINE_TERMINATOR); });
            UE_LOGFMT(LogAPIOverview, Display, "ListFiles request completed! Files: {0}", OutputString);
        });

    Provider->ListFiles(Auth);
}

void AAPIOverview::RetrieveFile()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveFileCompleted().AddLambda([](const FRetrieveFileResponse& Response, const FOpenAIResponseMetadata& Metadata)  //
        {                                                                                                                             //
            UE_LOGFMT(LogAPIOverview, Display, "RetrieveFile request completed! File name: {0}", Response.FileName);
        });

    const FString FileID{"file-xxxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->RetrieveFile(FileID, Auth);
}

void AAPIOverview::RetrieveFileContent()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveFileContentCompleted().AddLambda(
        [](const FRetrieveFileContentResponse& Response, const FOpenAIResponseMetadata& Metadata)  //
        {
            //
            UE_LOGFMT(LogAPIOverview, Display, "RetrieveFileContent request completed! File content: {0}", Response.Content);
        });

    const FString FileID{"file-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->RetrieveFileContent(FileID, Auth);
}

void AAPIOverview::ListFineTuningJobs()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFineTuningJobsCompleted().AddLambda(
        [](const FListFineTuningJobsResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "ListFineTuningJobs request completed!");
        });

    Provider->ListFineTuningJobs(Auth);
}

void AAPIOverview::CreateFineTuningJob()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateFineTuningJobCompleted().AddLambda(
        [](const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "CreateFineTuningJob request completed!");
        });

    FFineTuningJob FineTuningJob;
    FineTuningJob.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo);
    FineTuningJob.Training_File = "file-xxxxxxxxxxxxxxxxxxxxxxxxxxx";
    Provider->CreateFineTuningJob(FineTuningJob, Auth);
}

void AAPIOverview::ListFineTuningEvents()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFineTuningEventsCompleted().AddLambda(
        [&](const FListFineTuningEventsResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "ListFineTuningEvents request completed!");
        });

    const FString JobID{"ftjob-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->ListFineTuningEvents(JobID, Auth);
}

void AAPIOverview::ListFineTuningCheckpoints()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFineTuningCheckpointsCompleted().AddLambda(
        [&](const FListFineTuningCheckpointsResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "ListFineTuningCheckpoints request completed!");
        });

    const FString JobID{"ftjob-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->ListFineTuningCheckpoints(JobID, Auth);
}

void AAPIOverview::RetriveFineTuningJob()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveFineTuningJobCompleted().AddLambda(
        [](const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "RetriveFineTuningJob request completed!");
        });

    const FString JobID{"ftjob-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->RetrieveFineTuningJob(JobID, Auth);
}

void AAPIOverview::CancelFineTuningJob()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCancelFineTuningJobCompleted().AddLambda(
        [&](const FFineTuningJobObjectResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "CancelFineTuningJob request completed!");
        });

    const FString JobID{"ftjob-xxxxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->CancelFineTuningJob(JobID, Auth);
}

void AAPIOverview::CreateBatch()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateBatchCompleted().AddLambda(
        [&](const FCreateBatchResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "CreateBatch request completed, id={0}", Response.Id);
        });

    FCreateBatch Batch;
    Batch.Input_File_Id = "batch_xxxxxxxxxxxxxxxxxxxxxxxx";
    Batch.Endpoint = UOpenAIFuncLib::OpenAIBatchEndpointToString(EBatchEndpoint::ChatCompletions);
    Batch.Completion_Window = UOpenAIFuncLib::OpenAIBatchCompletionWindowToString(EBatchCompletionWindow::Window_24h);
    Batch.Metadata.Add("purpose", "plugin test");
    Batch.Metadata.Add("user_name", "John Doe");
    Provider->CreateBatch(Batch, Auth);
}

void AAPIOverview::ListBatch()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListBatchCompleted().AddLambda(
        [&](const FListBatchResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "ListBatch request completed!");
        });
    FListBatch ListBatch;
    ListBatch.Limit.Set(20);
    Provider->ListBatch(ListBatch, Auth);
}

void AAPIOverview::RetrieveBatch()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveBatchCompleted().AddLambda(
        [&](const FRetrieveBatchResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "RetrieveBatch request completed!");
        });
    const FString BatchId = "batch_xxxxxxxxxxxxxxxxxxxxxxxx";
    Provider->RetrieveBatch(BatchId, Auth);
}

void AAPIOverview::CancelBatch()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCancelBatchCompleted().AddLambda(
        [&](const FCancelBatchResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "CancelBatch request completed!");
        });
    const FString BatchId = "batch_xxxxxxxxxxxxxxxxxxxxxxxx";
    Provider->CancelBatch(BatchId, Auth);
}

void AAPIOverview::CreateUpload()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateUploadCompleted().AddLambda(
        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "CreateUpload request completed! filename: {0}", Response.Filename);
        });

    FCreateUpload CreateUpload;
    CreateUpload.Purpose = UOpenAIFuncLib::OpenAIUploadFilePurposeToString(EUploadFilePurpose::FineTune);
    CreateUpload.Filename = "training_examples";
    CreateUpload.Bytes = 3138;
    CreateUpload.Mime_Type = "text/jsonl";
    Provider->CreateUpload(CreateUpload, Auth);
}

void AAPIOverview::AddUploadPart()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnAddUploadPartCompleted().AddLambda(
        [&](const FUploadPartObjectResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "AddUploadPart request completed! id: {0}", Response.Id);
        });

    // absolute path to your file
    const FString FilePath = FPaths::Combine(FPaths::ProjectPluginsDir(),  //
        TEXT("OpenAI"), TEXT("Source"), TEXT("OpenAITestRunner"), TEXT("Data"), "test_file.jsonl");

    FAddUploadPart AddUploadPart;
    AddUploadPart.Data = FPaths::ConvertRelativePathToFull(FilePath);
    const FString UploadId = "upload_xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  // id from create upload response
    Provider->AddUploadPart(UploadId, AddUploadPart, Auth);
}

void AAPIOverview::CompleteUpload()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCompleteUploadCompleted().AddLambda(
        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (= (=
            UE_LOGFMT(LogAPIOverview, Display, "CompleteUpload request completed! id: {0}", Response.Id);
        });

    const FString UploadPartId = "part_xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    FCompleteUpload CompleteUpload;
    CompleteUpload.Part_Ids.Add(UploadPartId);

    const FString UploadId = "upload_xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    Provider->CompleteUpload(UploadId, CompleteUpload, Auth);
}

void AAPIOverview::CancelUpload()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCancelUploadCompleted().AddLambda(
        [&](const FUploadObjectResponse& Response, const FOpenAIResponseMetadata& Metadata)
        {
            // decide what to use from the struct by yourself (=
            UE_LOGFMT(LogAPIOverview, Display, "CancelUpload request completed! id: {0}", Response.Id);
        });
    const FString UploadId = "upload_xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    Provider->CancelUpload(UploadId, Auth);
}

void AAPIOverview::OnRequestError(const FString& URL, const FString& Content)
{
    UE_LOGFMT(LogAPIOverview, Error, "URL: {0}, Content: {1}", URL, Content);

    const EOpenAIResponseError Code = UOpenAIFuncLib::GetErrorCode(Content);
    const FString Messsage = UOpenAIFuncLib::GetErrorMessage(Content);
}

void AAPIOverview::SetYourOwnAPI()
{
    // You can specify your own endpoints. This might be useful if you're calling the OpenAI API through a proxy.
    class MyAPI : public OpenAI::IAPI
    {
    public:
        MyAPI(const FString& BaseURL = "https://api.openai.com") : API_URL(BaseURL) {}
        virtual FString BaseURL() const override { return API_URL; }

        virtual FString Models() const override { return API_URL + "/v1/models"; }
        virtual FString Completion() const override { return API_URL + "/v1/completions"; }
        virtual FString ChatCompletion() const override { return API_URL + "/v1/chat/completions"; }
        virtual FString ImageGenerations() const override { return API_URL + "/v1/images/generations"; }
        virtual FString ImageEdits() const override { return API_URL + "/v1/images/edits"; }
        virtual FString ImageVariations() const override { return API_URL + "/v1/images/variations"; }
        virtual FString Embeddings() const override { return API_URL + "/v1/embeddings"; }
        virtual FString Speech() const override { return API_URL + "/v1/audio/speech"; }
        virtual FString AudioTranscriptions() const override { return API_URL + "/v1/audio/transcriptions"; }
        virtual FString AudioTranslations() const override { return API_URL + "/v1/audio/translations"; }
        virtual FString Files() const override { return API_URL + "/v1/files"; }
        virtual FString FineTuningJobs() const override { return API_URL + "/v1/fine_tuning/jobs"; }
        virtual FString Moderations() const override { return API_URL + "/v1/moderations"; }
        virtual FString Batches() const override { return API_URL + "/v1/batches"; }
        virtual FString Uploads() const override { return API_URL + "/v1/uploads"; }

    private:
        const FString API_URL;
    };

    const auto API = MakeShared<MyAPI>();
    Provider->SetAPI(API);
}

// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "Sample/APIOverview.h"
#include "Provider/OpenAIProvider.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "Provider/Types/AudioTypes.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Algo/ForEach.h"
#include "API/API.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogAPIOverview, All, All);

// Please check which models that are compatible with each request:
// https://platform.openai.com/docs/models/model-endpoint-compatibility

AAPIOverview::AAPIOverview()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AAPIOverview::BeginPlay()
{
    Super::BeginPlay();

    Provider = NewObject<UOpenAIProvider>();
    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));

    // ListModels();
    // RetrieveModel();
    // DeleteFineTuneModel();

    // CreateCompletionRequest();
    // CreateChatCompletionRequest();

    // CreateImageDALLE2();
    CreateImageDALLE3();
    // CreateImageEdit();
    // CreateImageVariation();

    // CreateModerations();
    // CreateEmbeddings();

    // CreateSpeech();
    // CreateAudioTranscription();
    // CreateAudioTranslation();

    // UploadFile();
    // DeleteFile();
    // RetrieveFile();
    // RetrieveFileContent();
    // ListFiles();

    // ListFineTuningJobs();
    // CreateFineTuningJob();
    // RetriveFineTuningJob();
    // CancelFineTuningJob();
    // ListFineTuningEvents();

    // SetYourOwnAPI();
}

void AAPIOverview::ListModels()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListModelsCompleted().AddLambda(
        [](const FListModelsResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(Response.Data, [&](const FOpenAIModel& Model) { OutputString.Append(Model.ID).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    Provider->ListModels(Auth);
}

void AAPIOverview::RetrieveModel()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveModelCompleted().AddLambda([](const FRetrieveModelResponse& Response)
        { UE_LOG(LogAPIOverview, Display, TEXT("%s"), *UOpenAIFuncLib::OpenAIModelToString(Response)); });
    const auto ModelName = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4_Vision_Preview);
    Provider->RetrieveModel("ada", Auth);
}

void AAPIOverview::DeleteFineTuneModel()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnDeleteFineTunedModelCompleted().AddLambda(
        [](const FDeleteFineTuneResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("DeleteFineTuneModel request completed!"));
        });

    const FString ModelID{"curie:ft-lifeexe-2023-06-12-19-57-37"};
    Provider->DeleteFineTunedModel(ModelID, Auth);
}

void AAPIOverview::CreateCompletionRequest()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateCompletionCompleted().AddLambda(
        [](const FCompletionResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(Response.Choices, [&](const FChoice& Choice) { OutputString.Append(Choice.Text); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });
    Provider->OnCreateCompletionStreamProgresses().AddLambda(
        [](const TArray<FCompletionStreamResponse>& Responses)
        {
            FString OutputString{};
            Algo::ForEach(Responses, [&](const FCompletionStreamResponse& StreamResponse) {  //
                Algo::ForEach(StreamResponse.Choices, [&](const FChoice& Choice) {           //
                    OutputString.Append(Choice.Text);
                });
            });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });
    Provider->OnCreateCompletionStreamCompleted().AddLambda([](const TArray<FCompletionStreamResponse>& Responses)  //
        {                                                                                                           //
            UE_LOG(LogAPIOverview, Display, TEXT("Stream message generation finished"));
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
    Provider->OnCreateChatCompletionCompleted().AddLambda([](const FChatCompletionResponse& Response)  //
        {                                                                                              //
            UE_LOG(LogAPIOverview, Display, TEXT("Message generation finished"));
        });
    Provider->OnCreateChatCompletionStreamProgresses().AddLambda(
        [](const TArray<FChatCompletionStreamResponse>& Responses)
        {
            FString OutputString{};
            Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
                Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
                    OutputString.Append(Choice.Delta.Content);
                });
            });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });
    Provider->OnCreateChatCompletionStreamCompleted().AddLambda([](const TArray<FChatCompletionStreamResponse>& Responses)  //
        {                                                                                                                   //
            UE_LOG(LogAPIOverview, Display, TEXT("Stream message generation finished"));
        });

    FChatCompletion ChatCompletion;
    ChatCompletion.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O);
    ChatCompletion.Messages = {FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::User), "What is Unreal Engine?"}};
    ChatCompletion.Stream = true;
    ChatCompletion.Max_Tokens = 100;

    Provider->CreateChatCompletion(ChatCompletion, Auth);
}

void AAPIOverview::CreateImageDALLE2()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageCompleted().AddLambda(
        [](const FImageResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
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
        [](const FImageResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
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
        [](const FImageEditResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    FOpenAIImageEdit ImageEdit;
    // absolute paths to your images
    ImageEdit.Image = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\image.png";
    ImageEdit.Mask = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\image_mask.png";
    ImageEdit.N = 1;
    ImageEdit.Prompt = "Draw flamingo";
    ImageEdit.Size = UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_256x256);
    ImageEdit.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

    Provider->CreateImageEdit(ImageEdit, Auth);
}

void AAPIOverview::CreateImageVariation()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageVariationCompleted().AddLambda(
        [](const FImageVariationResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FImageObject& ImageObject) { OutputString.Append(ImageObject.URL).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    FOpenAIImageVariation ImageVariation;
    // absolute path to your image
    ImageVariation.Image = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\image.png";
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
        [](const FModerationsResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(Response.Results,
                [&](const FModerationResults& Results) { OutputString.Append(UOpenAIFuncLib::OpenAIModerationsToString(Results)); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
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
        [](const FEmbeddingsResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("CreateEmbeddings request completed!"));
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
    Provider->OnCreateSpeechCompleted().AddLambda([Format](const FSpeechResponse& Response) {  //
        const FString Date = FDateTime::Now().ToString();
        const FString FilePath =
            FPaths::ProjectPluginsDir().Append("OpenAI/Saved/").Append("speech_").Append(Date).Append(".").Append(Format);
        if (FFileHelper::SaveArrayToFile(Response.Bytes, *FilePath))
        {
            UE_LOG(LogAPIOverview, Display, TEXT("File was successfully saved to: %s"), *FilePath);
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

    Provider->OnCreateAudioTranscriptionCompleted().AddLambda([](const FAudioTranscriptionResponse& Response)  //
        {                                                                                                      //
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *Response.Text);
        });

    FAudioTranscription AudioTranscription;
    AudioTranscription.Language = "ru";
    AudioTranscription.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Whisper_1);
    AudioTranscription.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
    AudioTranscription.Temperature = 0.0f;
    // absolute paths to your audio files
    AudioTranscription.File = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\video.mp4";

    Provider->CreateAudioTranscription(AudioTranscription, Auth);
}

void AAPIOverview::CreateAudioTranslation()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);

    Provider->OnCreateAudioTranslationCompleted().AddLambda([](const FAudioTranslationResponse& Response)  //
        {                                                                                                  //
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *Response.Text);
        });

    FAudioTranslation AudioTranslation;
    AudioTranslation.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Whisper_1);
    AudioTranslation.Response_Format = UOpenAIFuncLib::OpenAIAudioTranscriptToString(ETranscriptFormat::JSON);
    AudioTranslation.Temperature = 0.0f;
    // absolute paths to your audio files
    AudioTranslation.File = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\video.mp4";

    Provider->CreateAudioTranslation(AudioTranslation, Auth);
}

void AAPIOverview::UploadFile()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnUploadFileCompleted().AddLambda(
        [](const FUploadFileResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("UploadFile request completed!"));
        });

    FUploadFile UploadFile;
    UploadFile.File = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\data.jsonl";
    UploadFile.Purpose = "fine-tune";

    Provider->UploadFile(UploadFile, Auth);
}

void AAPIOverview::DeleteFile()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnDeleteFileCompleted().AddLambda(
        [](const FDeleteFileResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("DeleteFile request completed!"));
        });

    const FString FileID{"file-xxxxxxxxxxxxxxxxxxx"};
    Provider->DeleteFile(FileID, Auth);
}

void AAPIOverview::ListFiles()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFilesCompleted().AddLambda(
        [](const FListFilesResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(
                Response.Data, [&](const FOpenAIFile& OpenAIFile) { OutputString.Append(OpenAIFile.FileName).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("ListFiles request completed! Files: %s"), *OutputString);
        });

    Provider->ListFiles(Auth);
}

void AAPIOverview::RetrieveFile()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveFileCompleted().AddLambda([](const FRetrieveFileResponse& Response)  //
        {                                                                                    //
            UE_LOG(LogAPIOverview, Display, TEXT("RetrieveFile request completed! File name: %s"), *Response.FileName);
        });

    const FString FileID{"file-xxxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->RetrieveFile(FileID, Auth);
}

void AAPIOverview::RetrieveFileContent()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveFileContentCompleted().AddLambda(
        [](const FRetrieveFileContentResponse& Response)  //
        {
            //
            UE_LOG(LogAPIOverview, Display, TEXT("RetrieveFileContent request completed! File content: %s"), *Response.Content);
        });

    const FString FileID{"file-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->RetrieveFileContent(FileID, Auth);
}

void AAPIOverview::ListFineTuningJobs()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFineTuningJobsCompleted().AddLambda(
        [](const FListFineTuningJobsResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("ListFineTuningJobs request completed!"));
        });

    Provider->ListFineTuningJobs(Auth);
}

void AAPIOverview::CreateFineTuningJob()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateFineTuningJobCompleted().AddLambda(
        [](const FFineTuningJobObjectResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("CreateFineTuningJob request completed!"));
        });

    FFineTuningJob FineTuningJob;
    FineTuningJob.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O);
    FineTuningJob.Training_File = "file-xxxxxxxxxxxxxxxxxxxxxxxxxxx";
    Provider->CreateFineTuningJob(FineTuningJob, Auth);
}

void AAPIOverview::RetriveFineTuningJob()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveFineTuningJobCompleted().AddLambda(
        [](const FFineTuningJobObjectResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("RetriveFineTuningJob request completed!"));
        });

    const FString JobID{"ftjob-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->RetrieveFineTuningJob(JobID, Auth);
}

void AAPIOverview::CancelFineTuningJob()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCancelFineTuningJobCompleted().AddLambda(
        [&](const FFineTuningJobObjectResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("CancelFineTuningJob request completed!"));
        });

    const FString JobID{"ftjob-xxxxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->CancelFineTuningJob(JobID, Auth);
}

void AAPIOverview::ListFineTuningEvents()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFineTuningEventsCompleted().AddLambda(
        [&](const FFineTuningJobEventResponse& Response)
        {
            // decide what to print from the struct by yourself (=
            UE_LOG(LogAPIOverview, Display, TEXT("ListFineTuningEvents request completed!"));
        });

    const FString JobID{"ftjob-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->ListFineTuningEvents(JobID, Auth);
}

void AAPIOverview::OnRequestError(const FString& URL, const FString& Content)
{
    UE_LOG(LogAPIOverview, Error, TEXT("URL: %s, Content: %s"), *URL, *Content);

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

    private:
        const FString API_URL;
    };

    const auto API = MakeShared<MyAPI>();
    Provider->SetAPI(API);
}

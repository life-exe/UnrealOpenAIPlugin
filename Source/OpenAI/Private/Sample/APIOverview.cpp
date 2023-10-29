// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "Sample/APIOverview.h"
#include "Provider/OpenAIProvider.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Algo/ForEach.h"
#include "API/API.h"

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

    ListModels();
    // RetrieveModel();

    // CreateCompletionRequest();
    // CreateChatCompletionRequest();

    // CreateImage();
    // CreateImageEdit();
    // CreateImageVariation();

    // CreateModerations();
    // CreateEdit();
    // CreateEmbeddings();

    // CreateAudioTranscription();
    // CreateAudioTranslation();

    // UploadFile();
    // DeleteFile();
    // RetrieveFile();
    // RetrieveFileContent();
    // ListFiles();

    // CreateFineTune();
    // RetrieveFineTune();
    // CancelFineTune();
    // DeleteFineTuneModel();
    //  ListFineTune();
    // ListFineTuneEvents();

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
    const auto ModelName = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Davinci_003);
    Provider->RetrieveModel("ada", Auth);
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
    ChatCompletion.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_3_5_Turbo_0301);
    ChatCompletion.Messages = {FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::User), "What is Unreal Engine?"}};
    ChatCompletion.Stream = true;
    ChatCompletion.Max_Tokens = 100;

    Provider->CreateChatCompletion(ChatCompletion, Auth);
}

void AAPIOverview::CreateImage()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateImageCompleted().AddLambda(
        [](const FImageResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(Response.Data, [&](const FString& Data) { OutputString.Append(Data).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    FOpenAIImage Image;
    Image.Prompt = "Tiger is eating pizza";
    Image.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_512x512);
    Image.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);
    Image.N = 2;

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
            Algo::ForEach(Response.Data, [&](const FString& Data) { OutputString.Append(Data).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    FOpenAIImageEdit ImageEdit;
    // absolute paths to your images
    ImageEdit.Image = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\image.png";
    ImageEdit.Mask = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\image_mask.png";
    ImageEdit.N = 1;
    ImageEdit.Prompt = "Draw flamingo";
    ImageEdit.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_256x256);
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
            Algo::ForEach(Response.Data, [&](const FString& Data) { OutputString.Append(Data).Append(LINE_TERMINATOR); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    FOpenAIImageVariation ImageVariation;
    // absolute path to your image
    ImageVariation.Image = "c:\\_Projects\\UE5\\UnrealOpenAISample\\Media\\image.png";
    ImageVariation.N = 1;
    ImageVariation.Size = UOpenAIFuncLib::OpenAIImageSizeToString(EImageSize::Size_256x256);
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

void AAPIOverview::CreateEdit()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateEditCompleted().AddLambda(
        [](const FEditResponse& Response)
        {
            FString OutputString{};
            Algo::ForEach(Response.Choices, [&](const FBaseChoice& Choice) { OutputString.Append(Choice.Text); });
            UE_LOG(LogAPIOverview, Display, TEXT("%s"), *OutputString);
        });

    FEdit Edit;
    Edit.Model = "text-davinci-edit-001";
    Edit.Input = "Find mistke in this sentence 2+2=5";
    Edit.Instruction = "Fix the spelling and math mistakes";
    Edit.Temperature = 0.1f;
    Edit.N = 3;
    Provider->CreateEdit(Edit, Auth);
}

void AAPIOverview::CreateEmbeddings()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateEmbeddingsCompleted().AddLambda(
        [](const FEmbeddingsResponse& Response)
        {
            // decide what to print from the struct by yuorself (=
            UE_LOG(LogAPIOverview, Display, TEXT("CreateEmbeddings request completed!"));
        });

    FEmbeddings Embeddings;
    Embeddings.Input = {"Hello! How are you?"};
    Embeddings.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_Ada_002);
    Provider->CreateEmbeddings(Embeddings, Auth);
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
            // decide what to print from the struct by yuorself (=
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
            // decide what to print from the struct by yuorself (=
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

void AAPIOverview::CreateFineTune()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCreateFineTuneCompleted().AddLambda(
        [](const FFineTuneResponse& Response)
        {
            // decide what to print from the struct by yuorself (=
            UE_LOG(LogAPIOverview, Display, TEXT("CreateFineTune request completed!"));
        });

    FFineTune FineTune;
    FineTune.Training_File = "file-xxxxxxxxxxxxxxxxxxxxxxxxxxx";
    Provider->CreateFineTune(FineTune, Auth);
}

void AAPIOverview::CancelFineTune()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnCancelFineTuneCompleted().AddLambda(
        [](const FFineTuneResponse& Response)
        {
            // decide what to print from the struct by yuorself (=
            UE_LOG(LogAPIOverview, Display, TEXT("CancelFineTune request completed!"));
        });

    const FString FineTuneID{"ft-xxxxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->CancelFineTune(FineTuneID, Auth);
}

void AAPIOverview::DeleteFineTuneModel()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnDeleteFineTunedModelCompleted().AddLambda(
        [](const FDeleteFineTuneResponse& Response)
        {
            // decide what to print from the struct by yuorself (=
            UE_LOG(LogAPIOverview, Display, TEXT("DeleteFineTuneModel request completed!"));
        });

    const FString ModelID{"curie:ft-lifeexe-2023-06-12-19-57-37"};
    Provider->DeleteFineTunedModel(ModelID, Auth);
}

void AAPIOverview::ListFineTune()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFineTunesCompleted().AddLambda(
        [](const FListFineTuneResponse& Response)
        {
            // decide what to print from the struct by yuorself (=
            UE_LOG(LogAPIOverview, Display, TEXT("ListFineTune request completed!"));
        });

    Provider->ListFineTunes(Auth);
}

void AAPIOverview::ListFineTuneEvents()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnListFineTuneEventsCompleted().AddLambda(
        [](const FFineTuneEventsResponse& Response)
        {
            // decide what to print from the struct by yuorself (=
            UE_LOG(LogAPIOverview, Display, TEXT("ListFineTuneEvents request completed!"));
        });

    const FString FineTuneID{"ft-xxxxxxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->ListFineTuneEvents(FineTuneID, Auth);
}

void AAPIOverview::RetrieveFineTune()
{
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    Provider->OnRetrieveFineTuneCompleted().AddLambda(
        [](const FFineTuneResponse& Response)
        {
            // decide what to print from the struct by yuorself (=
            UE_LOG(LogAPIOverview, Display, TEXT("RetrieveFineTune request completed!"));
        });

    const FString FineTuneID{"ft-xxxxxxxxxxxxxxxxxxxxxxxx"};
    Provider->RetrieveFineTune(FineTuneID, Auth);
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
        virtual FString Edits() const override { return API_URL + "/v1/edits"; }
        virtual FString ImageGenerations() const override { return API_URL + "/v1/images/generations"; }
        virtual FString ImageEdits() const override { return API_URL + "/v1/images/edits"; }
        virtual FString ImageVariations() const override { return API_URL + "/v1/images/variations"; }
        virtual FString Embeddings() const override { return API_URL + "/v1/embeddings"; }
        virtual FString AudioTranscriptions() const override { return API_URL + "/v1/audio/transcriptions"; }
        virtual FString AudioTranslations() const override { return API_URL + "/v1/audio/translations"; }
        virtual FString Files() const override { return API_URL + "/v1/files"; }
        virtual FString FineTunes() const override { return API_URL + "/v1/fine-tunes"; }
        virtual FString Moderations() const override { return API_URL + "/v1/moderations"; }

    private:
        const FString API_URL;
    };

    const auto API = MakeShared<MyAPI>();
    Provider->SetAPI(API);
}

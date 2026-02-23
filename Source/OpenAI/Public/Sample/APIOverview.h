// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "APIOverview.generated.h"

class UOpenAIProvider;

UENUM(BlueprintType)
enum class EAPIOverviewAction : uint8
{
    ListModels = 0,
    RetrieveModel,
    DeleteFineTunedModel,
    CreateCompletionRequest,
    CreateChatCompletionRequest,
    CreateImageDALLE2,
    CreateImageDALLE3,
    CreateImageGptImage1,
    CreateImageEdit,
    CreateImageVariation,
    CreateModerations,
    CreateEmbeddings,
    CreateSpeech,
    CreateAudioTranscription,
    CreateAudioTranscriptionVerbose,
    CreateAudioTranslation,
    CreateVoice,
    UploadFile,
    DeleteFile,
    ListFiles,
    RetrieveFile,
    RetrieveFileContent,
    CreateFineTuningJob,
    ListFineTuningJobs,
    ListFineTuningEvents,
    ListFineTuningCheckpoints,
    RetriveFineTuningJob,
    CancelFineTuningJob,
    PauseFineTuningJob,
    ResumeFineTuningJob,
    ListBatch,
    CreateBatch,
    RetrieveBatch,
    CancelBatch,
    CreateUpload,
    AddUploadPart,
    CompleteUpload,
    CancelUpload,
    CreateAssistant,
    DeleteAssistant,
    ListAssistants,
    ModifyAssistant,
    RetrieveAssistant,

    CreateVideo,
    RetrieveVideo,
    ListVideos,
    DeleteVideo,
    RemixVideo,
    DownloadVideoContent,

    SetYourOwnAPI
};

UCLASS(Blueprintable, BlueprintType)
class OPENAI_API AAPIOverview : public AInfo
{
    GENERATED_BODY()

public:
    AAPIOverview();

    UPROPERTY(EditAnywhere, Category = "OpenAI")
    EAPIOverviewAction Action;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UOpenAIProvider* Provider;

    FOpenAIAuth Auth;

    TMap<EAPIOverviewAction, TFunction<void()>> ActionMap;

    void ListModels();
    void RetrieveModel();
    void DeleteFinedTuneModel();

    void CreateCompletionRequest();
    void CreateChatCompletionRequest();

    void CreateImageDALLE2();
    void CreateImageDALLE3();
    void CreateImageGptImage1();
    void CreateImageEdit();
    void CreateImageVariation();

    void CreateModerations();
    void CreateEmbeddings();

    void CreateSpeech();
    void CreateAudioTranscription();
    void CreateAudioTranscriptionVerbose();
    void CreateAudioTranslation();
    void CreateVoice();

    void UploadFile();
    void DeleteFile();
    void ListFiles();
    void RetrieveFile();
    void RetrieveFileContent();

    void CreateFineTuningJob();
    void ListFineTuningJobs();
    void ListFineTuningEvents();
    void ListFineTuningCheckpoints();
    void RetriveFineTuningJob();
    void CancelFineTuningJob();
    void PauseFineTuningJob();
    void ResumeFineTuningJob();

    void ListBatch();
    void CreateBatch();
    void RetrieveBatch();
    void CancelBatch();

    void CreateUpload();
    void AddUploadPart();
    void CompleteUpload();
    void CancelUpload();

    void CreateAssistant();
    void DeleteAssistant();
    void ListAssistants();
    void ModifyAssistant();
    void RetrieveAssistant();

    void CreateVideo();
    void RetrieveVideo();
    void ListVideos();
    void DeleteVideo();
    void RemixVideo();
    void DownloadVideoContent();

    void OnRequestError(const FString& URL, const FString& Content);

    void SetYourOwnAPI();
};

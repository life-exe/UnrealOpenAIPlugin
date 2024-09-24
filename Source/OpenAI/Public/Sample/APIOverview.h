// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Provider/Types/CommonTypes.h"
#include "APIOverview.generated.h"

class UOpenAIProvider;

UENUM(BlueprintType)
enum class EAPIOverviewAction : uint8
{
    ListModels = 0,
    RetrieveModel,
    DeleteFineTuneModel,
    CreateCompletionRequest,
    CreateChatCompletionRequest,
    CreateImageDALLE2,
    CreateImageDALLE3,
    CreateImageEdit,
    CreateImageVariation,
    CreateModerations,
    CreateEmbeddings,
    CreateSpeech,
    CreateAudioTranscription,
    CreateAudioTranscriptionVerbose,
    CreateAudioTranslation,
    UploadFile,
    DeleteFile,
    ListFiles,
    RetrieveFile,
    RetrieveFileContent,
    ListFineTuningJobs,
    CreateFineTuningJob,
    RetriveFineTuningJob,
    CancelFineTuningJob,
    ListFineTuningEvents,
    ListBatch,
    CreateBatch,
    RetrieveBatch,
    CancelBatch,
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
    void DeleteFineTuneModel();

    void CreateCompletionRequest();
    void CreateChatCompletionRequest();

    void CreateImageDALLE2();
    void CreateImageDALLE3();
    void CreateImageEdit();
    void CreateImageVariation();

    void CreateModerations();
    void CreateEmbeddings();

    void CreateSpeech();
    void CreateAudioTranscription();
    void CreateAudioTranscriptionVerbose();
    void CreateAudioTranslation();

    void UploadFile();
    void DeleteFile();
    void ListFiles();
    void RetrieveFile();
    void RetrieveFileContent();

    void ListFineTuningJobs();
    void CreateFineTuningJob();
    void RetriveFineTuningJob();
    void CancelFineTuningJob();
    void ListFineTuningEvents();

    void ListBatch();
    void CreateBatch();
    void RetrieveBatch();
    void CancelBatch();

    void OnRequestError(const FString& URL, const FString& Content);

    void SetYourOwnAPI();
};

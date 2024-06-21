// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Provider/CommonTypes.h"
#include "APIOverview.generated.h"

class UOpenAIProvider;

UCLASS(Blueprintable, BlueprintType)
class OPENAI_API AAPIOverview : public AInfo
{
    GENERATED_BODY()

public:
    AAPIOverview();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UOpenAIProvider* Provider;

    FOpenAIAuth Auth;

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

    void OnRequestError(const FString& URL, const FString& Content);

    void SetYourOwnAPI();
};

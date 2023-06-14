// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "APIOverview.generated.h"

UCLASS()
class OPENAI_API AAPIOverview : public AActor
{
    GENERATED_BODY()

public:
    AAPIOverview();

protected:
    virtual void BeginPlay() override;

private:
    FOpenAIAuth Auth;

    void ListModels();
    void RetrieveModel();

    void CreateCompletionRequest();
    void CreateChatCompletionRequest();

    void CreateImage();
    void CreateImageEdit();
    void CreateImageVariation();

    void CreateEdit();
    void CreateModerations();
    void CreateEmbeddings();

    void CreateAudioTranscription();
    void CreateAudioTranslation();

    void UploadFile();
    void DeleteFile();
    void ListFiles();
    void RetrieveFile();
    void RetrieveFileContent();

    void CreateFineTune();
    void CancelFineTune();
    void DeleteFineTuneModel();
    void ListFineTune();
    void ListFineTuneEvents();
    void RetrieveFineTune();

    void OnRequestError(const FString& URL, const FString& Content);
};

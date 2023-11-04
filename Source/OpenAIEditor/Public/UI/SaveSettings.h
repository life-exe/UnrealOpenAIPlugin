// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSettings.generated.h"

class UBaseService;

UCLASS()
class OPENAIEDITOR_API USaveSettings : public USaveGame
{
    GENERATED_BODY()

public:
    void RegisterService(const TSubclassOf<UBaseService>& ServiceClass);
    void UnRegisterService(const TSubclassOf<UBaseService>& ServiceClass);
    bool IsServerRegistered(const TSubclassOf<UBaseService>& ServiceClass) const;

    static TObjectPtr<USaveSettings> Load();
    static void Save(USaveGame* SaveGameObject);

private:
    UPROPERTY()
    TArray<TSubclassOf<UBaseService>> Services;
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "UI/SaveSettings.h"
#include "ChatGPT/BaseService.h"
#include "Kismet/GameplayStatics.h"

namespace
{
TSubclassOf<UBaseService> SearchForService(const TSubclassOf<UBaseService>& ServiceClass, const TArray<TSubclassOf<UBaseService>>& Services)
{
    auto* FoundService = Services.FindByPredicate([ServiceClass](const auto& Item) { return Item == ServiceClass; });
    return FoundService ? *FoundService : nullptr;
}

const FString SlotName = "UI.Settings";
}  // namespace

void USaveSettings::RegisterService(const TSubclassOf<UBaseService>& ServiceClass)
{
    Services.Add(ServiceClass);
    Save(this);
}

void USaveSettings::UnRegisterService(const TSubclassOf<UBaseService>& ServiceClass)
{
    if (auto FoundService = SearchForService(ServiceClass, Services))
    {
        Services.Remove(FoundService);
    }
    Save(this);
}

bool USaveSettings::IsServerRegistered(const TSubclassOf<UBaseService>& ServiceClass) const
{
    return SearchForService(ServiceClass, Services) != nullptr;
}

// saving / loading
TObjectPtr<USaveSettings> USaveSettings::Load()
{
    auto* SaveSettings = Cast<USaveSettings>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    return SaveSettings ? SaveSettings : NewObject<USaveSettings>();
}

void USaveSettings::Save(USaveGame* SaveGameObject)
{
    UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
}

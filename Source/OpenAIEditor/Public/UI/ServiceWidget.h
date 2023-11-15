// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ChatGPT/ChatGPT.h"
#include "ServiceWidget.generated.h"

class UServiceWidget;
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnServiceEnabled, bool, const TSubclassOf<UBaseService>&, const TObjectPtr<UServiceWidget>&);

class UTextBlock;
class UEditorUtilityCheckBox;
class UBaseService;

UCLASS()
class OPENAIEDITOR_API UServiceWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    void SetServiceClass(const TSubclassOf<UBaseService>& Class);
    bool IsEnabled() const;
    void SetEnabled(bool IsEnabled);

    FOnServiceEnabled& OnServiceEnabled() { return ServiceEnabled; }

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ServiceName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditorUtilityCheckBox> EnableCheckBox;

private:
    TSubclassOf<UBaseService> ServiceClass;
    FOnServiceEnabled ServiceEnabled;

    UFUNCTION()
    void OnCheckStateChanged(bool bIsChecked);
};

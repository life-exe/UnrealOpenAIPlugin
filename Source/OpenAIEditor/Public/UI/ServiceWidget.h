// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ChatGPT/ChatGPT.h"
#include "ServiceWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnServiceEnabled, bool, const TSubclassOf<UBaseService>&);

class UTextBlock;
class UEditorUtilityCheckBox;
class UBaseService;

UCLASS()
class OPENAIEDITOR_API UServiceWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ServiceName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditorUtilityCheckBox> EnableCheckBox;

    void SetServiceClass(const TSubclassOf<UBaseService>& Class);
    bool IsEnabled() const;
    void SetEnabled(bool IsEnabled);

    FOnServiceEnabled& OnServiceEnabled() { return ServiceEnabled; }

protected:
    virtual void NativeConstruct() override;

private:
    TSubclassOf<UBaseService> ServiceClass;
    FOnServiceEnabled ServiceEnabled;

    UFUNCTION()
    void OnCheckStateChanged(bool bIsChecked);
};

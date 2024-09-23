// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "UI/ChatMessageWidget.h"
#include "Provider/Types/ModelTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "ChatGPT/BaseService.h"
#include "ChatGPTWidget.generated.h"

class UVerticalBox;
class UScrollBox;
class UEditableText;
class UButton;
class UChatMessageWidget;
class UComboBoxString;
class UChatGPT;
class UServiceWidget;
class UGridPanel;
class USaveSettings;
class UTextBlock;
class UAttachedFilesContainerWidget;

UCLASS(BlueprintType)
class OPENAIEDITOR_API UChatGPTWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OpenAI")
    int32 MaxTokens{2000};

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> StartVerticalBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> ChatGPTModelComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> MessagesVerticalBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UScrollBox> ChatScrollBox;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "OpenAI")
    TObjectPtr<UEditableText> UserInputTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SendMessageButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> DumpChatButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ClearChatButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AttachImageButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UChatMessageWidget> ChatMessageWidgetClass;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGridPanel> ServiceContainer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UServiceWidget> ServiceWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OpenAI")
    TArray<TSubclassOf<UBaseService>> Services;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ServicesWarningTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UAttachedFilesContainerWidget> AttachedFilesContainer;

    virtual void NativeConstruct() override;

private:
    UPROPERTY()
    TObjectPtr<UChatGPT> ChatGPT;

    UPROPERTY()
    TArray<TObjectPtr<UChatMessageWidget>> ChatWidgets;

    UPROPERTY()
    TObjectPtr<USaveSettings> SaveSettings;

    UPROPERTY()
    TArray<TObjectPtr<UServiceWidget>> ServiceWidgets;

private:
    UFUNCTION()
    void OnSendMessage();

    UFUNCTION()
    void OnDumpChat();

    UFUNCTION()
    void OnClearChat();

    UFUNCTION()
    void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    UFUNCTION()
    void OnTextChanged(const FText& Text);

    UFUNCTION()
    void OnModelSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnImageSelected();

    void InitChatGPT();
    void InitModelsComboBox();

    void OnRequestCompleted();
    void OnRequestUpdated(const FMessage& Message, bool WasError);

    void CreateMessageWidget(const FMessage& Message);
    void EnableControls(bool Enabled);

    FString GenerateFilePath() const;

    void UpdateVisionElements();
    void UpdateSendMessageButton();
    void OnFileRemoved();

private:
    OpenAI::ServiceSecrets ServiceSecrets;

    void CreateServiceWidgets();
    void OnServiceEnabled(bool IsEnabled, const TSubclassOf<UBaseService>& ServiceClass, const TObjectPtr<UServiceWidget>& ServiceWidget);
};

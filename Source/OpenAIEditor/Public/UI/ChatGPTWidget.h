// // OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "UI/ChatMessageWidget.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "Provider/CommonTypes.h"
#include "FuncLib/ModelTypes.h"
#include "ChatGPTWidget.generated.h"

class UVerticalBox;
class UScrollBox;
class UEditableText;
class UButton;
class UChatMessageWidget;
class UComboBoxString;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UChatMessageWidget> ChatMessageWidgetClass;

    virtual void NativeConstruct() override;

private:
    FOpenAIAuth Auth;
    FString Model;
    TArray<FMessage> ChatHistory;
    UPROPERTY()
    TArray<TObjectPtr<UChatMessageWidget>> ChatWidgets;
    FMessage AssistantCurrentMessage;

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

    void MakeModelsComboBox();
    void CreateChatCompletionRequest();
    void HandleRequestCompletion();
    void UpdateAssistantCurrentMessage(const FString& Message, bool WasError = false);

    FString MessageFromErrorCode(EOpenAIResponseError Code) const;
    FString GenerateFilePath() const;

    void EnableControls(bool Enabled);
    void HandleError(const FString& Content);
};

// // OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "UI/ChatGPTWidget.h"
#include "Provider/OpenAIProvider.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/FileSystemFuncLib.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/ComboBoxString.h"
#include "Algo/ForEach.h"

DEFINE_LOG_CATEGORY_STATIC(LogChatGPTWidget, All, All);

namespace
{
const FMargin UserWidgetMargin = FMargin{100.0f, 10.0f, 10.0f, 10.0f};
const FMargin AssistantWidgetMargin = FMargin{10.0f, 10.0f, 100.0f, 10.0f};
const FMargin SystemWidgetMargin = FMargin{10.0f, 10.0f, 100.0f, 10.0f};

}  // namespace

void UChatGPTWidget::NativeConstruct()
{
    Super::NativeConstruct();

    check(SendMessageButton);
    check(DumpChatButton);
    check(ClearChatButton);
    check(MessagesVerticalBox);
    check(UserInputTextBox);
    check(ChatGPTModelComboBox);

    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));

    SendMessageButton->OnClicked.AddDynamic(this, &ThisClass::OnSendMessage);
    DumpChatButton->OnClicked.AddDynamic(this, &ThisClass::OnDumpChat);
    ClearChatButton->OnClicked.AddDynamic(this, &ThisClass::OnClearChat);
    UserInputTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnTextCommitted);
    UserInputTextBox->OnTextChanged.AddDynamic(this, &ThisClass::OnTextChanged);

    SendMessageButton->SetIsEnabled(false);
    DumpChatButton->SetIsEnabled(false);
    ClearChatButton->SetIsEnabled(false);

    MessagesVerticalBox->ClearChildren();
    StartVerticalBox->SetVisibility(ESlateVisibility::Visible);
    ChatScrollBox->SetVisibility(ESlateVisibility::HitTestInvisible);

    MakeModelsComboBox();

    FSlateApplication::Get().SetKeyboardFocus(UserInputTextBox->GetCachedWidget());
}

void UChatGPTWidget::MakeModelsComboBox()
{
    auto AddModel = [&](EMainModelEnum Model) -> void  //
    {                                                  //
        ChatGPTModelComboBox->AddOption(UOpenAIFuncLib::OpenAIMainModelToString(Model));
    };

    ChatGPTModelComboBox->ClearOptions();
    AddModel(EMainModelEnum::GPT_3_5_Turbo);
    AddModel(EMainModelEnum::GPT_3_5_Turbo_0301);
    AddModel(EMainModelEnum::GPT_4);
    AddModel(EMainModelEnum::GPT_4_0314);
    AddModel(EMainModelEnum::GPT_4_0613);
    ChatGPTModelComboBox->SetSelectedOption(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_3_5_Turbo));
}

void UChatGPTWidget::OnSendMessage()
{
    if (UserInputTextBox->GetText().IsEmpty()) return;

    Model = ChatGPTModelComboBox->GetSelectedOption();

    const auto Message = UserInputTextBox->GetText();
    UserInputTextBox->SetText({});

    const auto UserChatMessage = FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::User), Message.ToString()};
    ChatHistory.Add(UserChatMessage);

    auto* UserChatWidget = CreateWidget<UChatMessageWidget>(GetWorld(), ChatMessageWidgetClass);
    UserChatWidget->SetMessage(UserChatMessage);
    ChatWidgets.Add(UserChatWidget);
    MessagesVerticalBox->AddChild(UserChatWidget);
    if (auto* ChatSlot = Cast<UVerticalBoxSlot>(UserChatWidget->Slot))
    {
        ChatSlot->SetPadding(UserWidgetMargin);
    }

    AssistantCurrentMessage = FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant), {}};
    auto* AssistantChatWidget = CreateWidget<UChatMessageWidget>(GetWorld(), ChatMessageWidgetClass);
    AssistantChatWidget->SetMessage(AssistantCurrentMessage);
    ChatWidgets.Add(AssistantChatWidget);
    MessagesVerticalBox->AddChild(AssistantChatWidget);
    if (auto* ChatSlot = Cast<UVerticalBoxSlot>(AssistantChatWidget->Slot))
    {
        ChatSlot->SetPadding(AssistantWidgetMargin);
    }

    EnableControls(false);
    ChatScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Never);
    ChatScrollBox->SetVisibility(ESlateVisibility::Visible);
    StartVerticalBox->SetVisibility(ESlateVisibility::Collapsed);

    CreateChatCompletionRequest();
}

void UChatGPTWidget::CreateChatCompletionRequest()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->SetLogEnabled(true);
    Provider->OnRequestError().AddLambda(
        [&](const FString& URL, const FString& Content)
        {
            HandleError(Content);
            HandleRequestCompletion();
        });
    Provider->OnCreateChatCompletionStreamProgresses().AddLambda(
        [&](const TArray<FChatCompletionStreamResponse>& Responses)
        {
            FString OutputString{};
            Algo::ForEach(Responses, [&](const FChatCompletionStreamResponse& StreamResponse) {  //
                Algo::ForEach(StreamResponse.Choices, [&](const FChatStreamChoice& Choice) {     //
                    OutputString.Append(Choice.Delta.Content);
                });
            });
            UpdateAssistantCurrentMessage(OutputString);
        });
    Provider->OnCreateChatCompletionStreamCompleted().AddLambda([&](const TArray<FChatCompletionStreamResponse>& Responses)  //
        {                                                                                                                    //
            HandleRequestCompletion();
        });

    FChatCompletion ChatCompletion;
    ChatCompletion.Model = Model;
    ChatCompletion.Messages = ChatHistory;
    ChatCompletion.Stream = true;
    ChatCompletion.Max_Tokens = MaxTokens;

    Provider->CreateChatCompletion(ChatCompletion, Auth);
}

void UChatGPTWidget::HandleRequestCompletion()
{
    ChatHistory.Add(AssistantCurrentMessage);
    ChatScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
    EnableControls(true);
    FSlateApplication::Get().SetKeyboardFocus(UserInputTextBox->GetCachedWidget());
}

void UChatGPTWidget::UpdateAssistantCurrentMessage(const FString& Message, bool WasError)
{
    AssistantCurrentMessage.Content = Message;
    ChatWidgets.Last()->SetMessage(AssistantCurrentMessage, WasError);
    ChatScrollBox->ScrollToEnd();
}

void UChatGPTWidget::OnDumpChat()
{
    const auto FilePath = GenerateFilePath();
    if (UFileSystemFuncLib::SaveChatHistoryToFile(ChatHistory, Model, FilePath))
    {
        auto* SystemChatWidget = CreateWidget<UChatMessageWidget>(GetWorld(), ChatMessageWidgetClass);
        ChatWidgets.Add(SystemChatWidget);
        MessagesVerticalBox->AddChild(SystemChatWidget);
        if (auto* ChatSlot = Cast<UVerticalBoxSlot>(SystemChatWidget->Slot))
        {
            ChatSlot->SetPadding(SystemWidgetMargin);
        }

        FMessage Message;
        Message.Content = FString::Printf(TEXT("Chat history was saved to: %s"), *FPaths::ConvertRelativePathToFull(FilePath));
        Message.Role = "system";
        SystemChatWidget->SetMessage(Message);

        ChatScrollBox->ScrollToEnd();
    }
}

void UChatGPTWidget::OnClearChat()
{
    MessagesVerticalBox->ClearChildren();
    ChatWidgets.Empty();
    ChatHistory.Empty();
    StartVerticalBox->SetVisibility(ESlateVisibility::Visible);
    ChatScrollBox->SetVisibility(ESlateVisibility::HitTestInvisible);

    DumpChatButton->SetIsEnabled(false);
    ClearChatButton->SetIsEnabled(false);
}

void UChatGPTWidget::OnTextChanged(const FText& Text)
{
    SendMessageButton->SetIsEnabled(!UserInputTextBox->GetText().IsEmpty());
}

void UChatGPTWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::Type::OnEnter)
    {
        OnSendMessage();
    }
}

FString UChatGPTWidget::MessageFromErrorCode(EOpenAIResponseError Code) const
{
    switch (Code)
    {
        case EOpenAIResponseError::InvalidAPIKey: return "Invalid API key";
        case EOpenAIResponseError::NetworkError: return "Network error";
        case EOpenAIResponseError::Unknown: return "Unknown error";
    }

    return "Unknown error code";
}

FString UChatGPTWidget::GenerateFilePath() const
{
    const auto FileName = "ChatGptChatHistory";
    FString SavedDir = FPaths::ProjectDir().Append("Saved/");
    const FString Date = FDateTime::Now().ToString();
    return SavedDir.Append(FileName).Append("_").Append(Date).Append(".txt");
}

void UChatGPTWidget::EnableControls(bool Enabled)
{
    ChatScrollBox->SetAllowOverscroll(Enabled);
    SendMessageButton->SetIsEnabled(Enabled);
    DumpChatButton->SetIsEnabled(Enabled);
    ClearChatButton->SetIsEnabled(Enabled);
    UserInputTextBox->SetIsReadOnly(!Enabled);
}

void UChatGPTWidget::HandleError(const FString& Content)
{
    const auto Message = UOpenAIFuncLib::GetErrorMessage(Content);
    if (Message.IsEmpty())
    {
        UpdateAssistantCurrentMessage(MessageFromErrorCode(UOpenAIFuncLib::GetErrorCode(Content)), true);
        return;
    }

    if (UOpenAIFuncLib::GetErrorCode(Content) == EOpenAIResponseError::ModelNotFound && Message.Contains("gpt-4"))
    {
        UpdateAssistantCurrentMessage(
            FString(Message).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR).Append("Please check: https://openai.com/waitlist/gpt-4-api"),
            true);
    }
    else
    {
        UpdateAssistantCurrentMessage(Message, true);
    }
}

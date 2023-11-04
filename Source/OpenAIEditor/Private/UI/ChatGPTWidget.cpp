// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "UI/ChatGPTWidget.h"
#include "UI/ServiceWidget.h"
#include "UI/SaveSettings.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/ComboBoxString.h"
#include "Components/GridPanel.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/FileSystemFuncLib.h"
#include "ChatGPT/Services/Weather/WeatherService.h"
#include "ChatGPT/Services/News/NewsService.h"

DEFINE_LOG_CATEGORY_STATIC(LogChatGPTWidget, All, All);

namespace
{
const FMargin UserWidgetMargin = FMargin{100.0f, 10.0f, 10.0f, 10.0f};
const FMargin AssistantWidgetMargin = FMargin{10.0f, 10.0f, 100.0f, 10.0f};
const FMargin SystemWidgetMargin = FMargin{10.0f, 10.0f, 100.0f, 10.0f};
const int32 ServicesAmountInRow = 3;
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
    check(ServiceContainer);

    SendMessageButton->OnClicked.AddDynamic(this, &ThisClass::OnSendMessage);
    DumpChatButton->OnClicked.AddDynamic(this, &ThisClass::OnDumpChat);
    ClearChatButton->OnClicked.AddDynamic(this, &ThisClass::OnClearChat);
    UserInputTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnTextCommitted);
    UserInputTextBox->OnTextChanged.AddDynamic(this, &ThisClass::OnTextChanged);
    ChatGPTModelComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnModelSelectionChanged);

    SendMessageButton->SetIsEnabled(false);
    DumpChatButton->SetIsEnabled(false);
    ClearChatButton->SetIsEnabled(false);

    MessagesVerticalBox->ClearChildren();
    StartVerticalBox->SetVisibility(ESlateVisibility::Visible);
    ChatScrollBox->SetVisibility(ESlateVisibility::HitTestInvisible);

    InitChatGPT();
    InitModelsComboBox();
    CreateServiceWidgets();

    FSlateApplication::Get().SetKeyboardFocus(UserInputTextBox->GetCachedWidget());
}

void UChatGPTWidget::InitChatGPT()
{
    const auto Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));
    ChatGPT = NewObject<UChatGPT>();
    check(ChatGPT);
    ChatGPT->SetAuth(Auth);
    ChatGPT->SetMaxTokens(MaxTokens);
    ChatGPT->OnRequestCompleted().AddUObject(this, &ThisClass::OnRequestCompleted);
    ChatGPT->OnRequestUpdated().AddUObject(this, &ThisClass::OnRequestUpdated);

    ServiceSecrets = UOpenAIFuncLib::LoadServiceSecretsFromFile(FPaths::ProjectDir().Append("OnlineServicesAuth.ini"));

    SaveSettings = USaveSettings::Load();
    check(SaveSettings);
}

void UChatGPTWidget::InitModelsComboBox()
{
    auto AddModel = [&](EMainModelEnum Model) -> void  //
    {                                                  //
        ChatGPTModelComboBox->AddOption(UOpenAIFuncLib::OpenAIMainModelToString(Model));
    };

    ChatGPTModelComboBox->ClearOptions();
    AddModel(EMainModelEnum::GPT_4);
    AddModel(EMainModelEnum::GPT_4_0314);
    AddModel(EMainModelEnum::GPT_4_0613);
    AddModel(EMainModelEnum::GPT_3_5_Turbo);
    AddModel(EMainModelEnum::GPT_3_5_Turbo_0301);
    ChatGPTModelComboBox->SetSelectedOption(UOpenAIFuncLib::OpenAIMainModelToString(EMainModelEnum::GPT_4));

    ChatGPT->SetModel(ChatGPTModelComboBox->GetSelectedOption());
}

void UChatGPTWidget::CreateServiceWidgets()
{
    ServiceContainer->ClearChildren();
    int32 Row{0}, Column{0};
    for (const auto& ServiceClass : Services)
    {
        const auto ServiceWidget = CreateWidget<UServiceWidget>(GetWorld(), ServiceWidgetClass);
        ServiceWidget->SetServiceClass(ServiceClass);
        ServiceWidget->OnServiceEnabled().AddUObject(this, &ThisClass::OnServiceEnabled);
        ServiceContainer->AddChildToGrid(ServiceWidget, Row, Column);
        ServiceWidget->SetEnabled(SaveSettings->IsServerRegistered(ServiceClass));
        if (ServiceWidget->IsEnabled())
        {
            OnServiceEnabled(true, ServiceClass);
        }

        if (++Column >= ServicesAmountInRow)
        {
            Column = 0;
            Row++;
        }
    }
}

void UChatGPTWidget::OnServiceEnabled(bool IsEnabled, const TSubclassOf<UBaseService>& ServiceClass)
{
    if (!IsEnabled)
    {
        ChatGPT->UnRegisterService(ServiceClass);
        SaveSettings->UnRegisterService(ServiceClass);
        return;
    }
    ChatGPT->RegisterService(ServiceClass, ServiceSecrets);
    SaveSettings->RegisterService(ServiceClass);
}

void UChatGPTWidget::OnSendMessage()
{
    if (UserInputTextBox->GetText().IsEmpty()) return;

    const auto InputText = UserInputTextBox->GetText();
    UserInputTextBox->SetText({});

    const auto Message = FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::User), InputText.ToString()};
    ChatGPT->AddMessage(Message);

    const auto AssistantMessage = FMessage{UOpenAIFuncLib::OpenAIRoleToString(ERole::Assistant), {}};
    ChatGPT->SetAssistantMessage(AssistantMessage);

    CreateMessageWidget(Message);
    ChatGPT->MakeRequest();
}

void UChatGPTWidget::CreateMessageWidget(const FMessage& Message)
{
    auto* UserChatWidget = CreateWidget<UChatMessageWidget>(GetWorld(), ChatMessageWidgetClass);
    UserChatWidget->SetMessage(Message);
    ChatWidgets.Add(UserChatWidget);
    MessagesVerticalBox->AddChild(UserChatWidget);
    if (auto* ChatSlot = Cast<UVerticalBoxSlot>(UserChatWidget->Slot))
    {
        ChatSlot->SetPadding(UserWidgetMargin);
    }

    auto* AssistantChatWidget = CreateWidget<UChatMessageWidget>(GetWorld(), ChatMessageWidgetClass);
    AssistantChatWidget->SetMessage(ChatGPT->GetAssistantMessage());
    ChatWidgets.Add(AssistantChatWidget);
    MessagesVerticalBox->AddChild(AssistantChatWidget);
    if (auto* ChatSlot = Cast<UVerticalBoxSlot>(AssistantChatWidget->Slot))
    {
        ChatSlot->SetPadding(AssistantWidgetMargin);
    }

    EnableControls(false);
    ChatScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Never);
    ChatScrollBox->SetVisibility(ESlateVisibility::Visible);
    ChatScrollBox->ScrollToEnd();
    StartVerticalBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UChatGPTWidget::OnRequestCompleted()
{
    ChatScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
    EnableControls(true);
    FSlateApplication::Get().SetKeyboardFocus(UserInputTextBox->GetCachedWidget());
}

void UChatGPTWidget::OnRequestUpdated(const FMessage& Message, bool WasError)
{
    ChatWidgets.Last()->SetMessage(Message, WasError);
    ChatScrollBox->ScrollToEnd();
}

void UChatGPTWidget::OnDumpChat()
{
    const FString FilePath = GenerateFilePath();
    const FString Model = ChatGPT->GetModel();

    if (UFileSystemFuncLib::SaveChatHistoryToFile(ChatGPT->GetHistory(), Model, FilePath))
    {
        auto* SystemChatWidget = CreateWidget<UChatMessageWidget>(GetWorld(), ChatMessageWidgetClass);
        ChatWidgets.Add(SystemChatWidget);
        MessagesVerticalBox->AddChild(SystemChatWidget);
        if (auto* ChatSlot = Cast<UVerticalBoxSlot>(SystemChatWidget->Slot))
        {
            ChatSlot->SetPadding(SystemWidgetMargin);
        }

        FMessage Message;
        Message.Content = FString::Format(TEXT("Chat history was saved to: {0}"), {FPaths::ConvertRelativePathToFull(FilePath)});
        Message.Role = "system";
        SystemChatWidget->SetMessage(Message);

        ChatScrollBox->ScrollToEnd();
    }
}

void UChatGPTWidget::OnClearChat()
{
    MessagesVerticalBox->ClearChildren();
    ChatWidgets.Empty();
    ChatGPT->ClearHistory();
    StartVerticalBox->SetVisibility(ESlateVisibility::Visible);
    ChatScrollBox->SetVisibility(ESlateVisibility::HitTestInvisible);

    DumpChatButton->SetIsEnabled(false);
    ClearChatButton->SetIsEnabled(false);
}

void UChatGPTWidget::OnTextChanged(const FText& Text)
{
    SendMessageButton->SetIsEnabled(!UserInputTextBox->GetText().IsEmpty());
}

void UChatGPTWidget::OnModelSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    ChatGPT->SetModel(ChatGPTModelComboBox->GetSelectedOption());
}

void UChatGPTWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::Type::OnEnter)
    {
        OnSendMessage();
    }
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

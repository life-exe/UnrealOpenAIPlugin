// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "UI/ChatMessageWidget.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "UI/ImagePreviewWidget.h"

namespace
{
const FMargin ImageMargin = FMargin{20.0f};
}  // namespace

void UChatMessageWidget::SetMessage(const FMessage& Message, bool WasError)
{
    RoleTextBox->SetText(FText::FromString(Message.Role));

    FString UserMessage{};
    if (Message.Content.IsEmpty())
    {
        for (const auto& Content : Message.ContentArray)
        {
            if (Content.Type.Equals(UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Text)))
            {
                UserMessage.Append(Content.Text);
            }
            else if (Content.Type.Equals(UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Image_URL)))
            {
                if (auto* ImageWidget = CreateWidget<UImagePreviewWidget>(GetWorld(), ImagePreviewWidgetClass))
                {
                    ImageWidget->SetBase64(Content.Image_URL.URL);
                    ImageHorizontalBox->AddChild(ImageWidget);
                    if (auto* ImageSlot = Cast<UHorizontalBoxSlot>(ImageWidget->Slot))
                    {
                        ImageSlot->SetPadding(ImageMargin);
                    }
                    ImageWidgets.Add(ImageWidget);
                }
            }
        }
    }
    else
    {
        UserMessage = Message.Content;
    }

    MessageText->SetText(FText::FromString(UserMessage));

    const auto Role = UOpenAIFuncLib::StringToOpenAIRole(Message.Role);
    if (WasError)
    {
        MainBorder->SetBrushColor(ErrorColor);
    }
    else if (ColorMap.Contains(Role))
    {
        MainBorder->SetBrushColor(ColorMap[Role]);
    }
    else
    {
        MainBorder->SetBrushColor(DefaultColor);
    }

    const FDateTime Date = FDateTime::Now();
    const FString DisplayDate = FString::Printf(TEXT("%02i:%02i"), Date.GetHour(), Date.GetMinute());
    TimeTextBox->SetText(FText::FromString(DisplayDate));

    if (!UserMessage.IsEmpty() || !ImageWidgets.IsEmpty())
    {
        WidgetSwitcher->SetActiveWidgetIndex(1);
    }
}

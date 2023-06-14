// // OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "UI/ChatMessageWidget.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "FuncLib/OpenAIFuncLib.h"

void UChatMessageWidget::SetMessage(const FMessage& Message, bool WasError)
{
    RoleTextBox->SetText(FText::FromString(Message.Role));
    MessageText->SetText(FText::FromString(Message.Content));

    if (!WasError)
    {
        switch (UOpenAIFuncLib::StringToOpenAIRole(Message.Role))
        {
            case ERole::Assistant:  //
                MainBorder->SetBrushColor(AssistantColor);
                break;

            case ERole::User:  //
                MainBorder->SetBrushColor(UserColor);
                break;

            case ERole::System:  //
                MainBorder->SetBrushColor(SystemColor);
                break;
        }
    }
    else
    {
        MainBorder->SetBrushColor(ErrorColor);
    }

    const FDateTime Date = FDateTime::Now();
    const FString DisplayDate = FString::Printf(TEXT("%02i:%02i"), Date.GetHour(), Date.GetMinute());
    TimeTextBox->SetText(FText::FromString(DisplayDate));
}

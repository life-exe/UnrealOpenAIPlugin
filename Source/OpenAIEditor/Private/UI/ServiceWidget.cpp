// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "UI/ServiceWidget.h"
#include "Components/TextBlock.h"
#include "EditorUtilityWidgetComponents.h"
#include "ChatGPT/BaseService.h"

namespace
{
FString TruncateStringWithEllipsis(const FString& InString, int32 MaxLength)
{
    return InString.Len() > MaxLength ? InString.Left(MaxLength - 2).Append("..") : InString;
}

constexpr int32 NameMaxLenght = 8;
}  // namespace

void UServiceWidget::NativeConstruct()
{
    Super::NativeConstruct();

    check(ServiceName);
    check(EnableCheckBox);
    EnableCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCheckStateChanged);
}

void UServiceWidget::SetServiceClass(const TSubclassOf<UBaseService>& Class)
{
    ServiceClass = Class;
    // not perfect, but have no other idea right now how to get service name and description better
    auto* Service = NewObject<UBaseService>(this, Class);
    check(Service);

    const auto NameToDisplay = TruncateStringWithEllipsis(Service->Name(), NameMaxLenght);
    ServiceName->SetText(FText::FromString(NameToDisplay));
    const auto TooltipToDisplay =
        FString(Service->Name()).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR).Append(Service->TooltipDescription());
    ServiceName->SetToolTipText(FText::FromString(TooltipToDisplay));
}

bool UServiceWidget::IsEnabled() const
{
    return EnableCheckBox->IsChecked();
}

void UServiceWidget::SetEnabled(bool IsEnabled)
{
    EnableCheckBox->SetCheckedState(IsEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UServiceWidget::OnCheckStateChanged(bool bIsChecked)
{
    ServiceEnabled.Broadcast(bIsChecked, ServiceClass, this);
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "UI/AttachedFilesContainerWidget.h"
#include "UI/AttachedImageWidget.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"

namespace
{
const FMargin FileMargin = FMargin{20.0f, 0.0f, 0.0f, 0.0f};
}  // namespace

TSharedRef<SWidget> UAttachedFilesContainerWidget::RebuildWidget()
{
    ImageHorizontalBox = Cast<UHorizontalBox>(GetChildAt(0));
    check(ImageHorizontalBox);
    return Super::RebuildWidget();
}

void UAttachedFilesContainerWidget::AddImage(const FString& ImagePath)
{
    SelectedFiles.Add(ImagePath);

    auto* ImageWidget = CreateWidget<UAttachedImageWidget>(GetWorld(), AttachedImageWidgetWidgetClass);
    check(ImageWidget);
    ImageWidget->SetImage(ImagePath);
    ImageWidget->OnRemoved().AddUObject(this, &ThisClass::OnRemoved);
    ImageHorizontalBox->AddChild(ImageWidget);
    if (auto* ImageSlot = Cast<UHorizontalBoxSlot>(ImageWidget->Slot))
    {
        ImageSlot->SetPadding(FileMargin);
    }
    AttachedImageWidgets.Add(ImageWidget);
}

void UAttachedFilesContainerWidget::Clear()
{
    for (const auto& ImageWidget : AttachedImageWidgets)
    {
        ImageHorizontalBox->RemoveChild(ImageWidget);
    }
    AttachedImageWidgets.Empty();
    SelectedFiles.Empty();
}

bool UAttachedFilesContainerWidget::HasFiles() const
{
    return !SelectedFiles.IsEmpty();
}

void UAttachedFilesContainerWidget::OnRemoved(const FString& ImagePath, const TObjectPtr<UAttachedImageWidget>& Widget)
{
    SelectedFiles.Remove(ImagePath);
    AttachedImageWidgets.Remove(Widget);
    ImageHorizontalBox->RemoveChild(Widget);
    FileRemoved.Broadcast();
}
// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "UI/AttachedImageWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

namespace
{
constexpr int32 MaxNameLength = 12;
}

void UAttachedImageWidget::NativeConstruct()
{
    Super::NativeConstruct();

    check(RemoveImageButton);
    RemoveImageButton->OnClicked.AddDynamic(this, &ThisClass::RemoveImage);
}

void UAttachedImageWidget::SetImage(const FString& Path)
{
    ImagePath = Path;

    FString DisplayName = FPaths::GetBaseFilename(Path);
    if (DisplayName.Len() > MaxNameLength)
    {
        DisplayName = DisplayName.Left(MaxNameLength - 2).Append("..");
    }
    FilePathTextBlock->SetText(FText::FromString(DisplayName));
}

void UAttachedImageWidget::RemoveImage()
{
    Removed.Broadcast(ImagePath, this);
}

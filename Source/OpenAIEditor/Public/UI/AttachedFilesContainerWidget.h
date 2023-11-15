// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "AttachedFilesContainerWidget.generated.h"

class UAttachedImageWidget;
class UHorizontalBox;

DECLARE_MULTICAST_DELEGATE(FOnFileRemoved);

UCLASS()
class OPENAIEDITOR_API UAttachedFilesContainerWidget : public UHorizontalBox
{
    GENERATED_BODY()

public:
    void AddImage(const FString& ImagePath);
    void Clear();
    bool HasFiles() const;
    TArray<FString> FilePaths() const { return SelectedFiles; }
    FOnFileRemoved& OnFileRemoved() { return FileRemoved; }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UAttachedImageWidget> AttachedImageWidgetWidgetClass;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

private:
    UPROPERTY()
    TArray<TObjectPtr<UAttachedImageWidget>> AttachedImageWidgets;

    UPROPERTY()
    TObjectPtr<UHorizontalBox> ImageHorizontalBox;

    TArray<FString> SelectedFiles;
    FOnFileRemoved FileRemoved;

    void OnRemoved(const FString& ImagePath, const TObjectPtr<UAttachedImageWidget>& Widget);
};

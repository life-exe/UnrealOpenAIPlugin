// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "AttachedImageWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRemoved, const FString&, const TObjectPtr<UAttachedImageWidget>&);

UCLASS()
class OPENAIEDITOR_API UAttachedImageWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    void SetImage(const FString& ImagePath);
    FOnRemoved& OnRemoved() { return Removed; }

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> RemoveImageButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> FilePathTextBlock;

private:
    FOnRemoved Removed;
    FString ImagePath{};

    UFUNCTION()
    void RemoveImage();
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ImagePreviewWidget.generated.h"

class USizeBox;
class UImage;

UCLASS()
class OPENAIEDITOR_API UImagePreviewWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USizeBox> ImageSizeBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PreviewImage;

public:
    void SetBase64(const FString& Base64String);
};

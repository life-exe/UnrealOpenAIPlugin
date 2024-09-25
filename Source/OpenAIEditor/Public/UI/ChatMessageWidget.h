// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Provider/Types/Chat/ChatCompletionChunkTypes.h"
#include "Provider/Types/ModelTypes.h"
#include "ChatMessageWidget.generated.h"

class UBorder;
class UMultiLineEditableText;
class UTextBlock;
class UWidgetSwitcher;
class UHorizontalBox;
class UImagePreviewWidget;

UCLASS()
class OPENAIEDITOR_API UChatMessageWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> MainBorder;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> RoleTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UMultiLineEditableText> MessageText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TimeTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;  // preloader -> text;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> ImageHorizontalBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TMap<ERole, FLinearColor> ColorMap{
        {ERole::Assistant, FColor::FromHex("444654FF")},
        {ERole::User, FColor::FromHex("343541FF")},
        {ERole::System, FColor::FromHex("595a68FF")},
    };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor DefaultColor{FColor::FromHex("0e0e0eFF")};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor ErrorColor{FColor::FromHex("41343dFF")};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UImagePreviewWidget> ImagePreviewWidgetClass;

public:
    void SetMessage(const FMessage& Message, bool WasError = false);

private:
    TArray<TObjectPtr<UImagePreviewWidget>> ImageWidgets;
};

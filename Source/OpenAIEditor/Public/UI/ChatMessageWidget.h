// // OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Provider/CommonTypes.h"
#include "ChatMessageWidget.generated.h"

class UBorder;
class UMultiLineEditableText;
class UTextBlock;
class UWidgetSwitcher;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor UserColor{FColor::FromHex("343541FF")};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor AssistantColor{FColor::FromHex("444654FF")};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor SystemColor{FColor::FromHex("595a68FF")};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor ErrorColor{FColor::FromHex("41343dFF")};

public:
    void SetMessage(const FMessage& Message, bool WasError = false);
};

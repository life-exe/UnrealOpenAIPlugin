// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "UI/ImagePreviewWidget.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "FuncLib/ImageFuncLib.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

void UImagePreviewWidget::SetBase64(const FString& Base64String)
{
    const auto CleanBase64 = UOpenAIFuncLib::UnWrapBase64(Base64String);
    auto* ImageTexture = UImageFuncLib::Texture2DFromBytes(CleanBase64);
    PreviewImage->SetBrushFromTexture(ImageTexture, true);

    const float AspectRatio = static_cast<float>(ImageTexture->GetSizeX()) / ImageTexture->GetSizeY();
    ImageSizeBox->SetMinAspectRatio(AspectRatio);
    ImageSizeBox->SetMaxAspectRatio(AspectRatio);

    float TranslationX{0.0f};
    float TranslationY{0.0f};
    const float BoxSize = ImageSizeBox->GetWidthOverride();

    if (AspectRatio > 1.0f)
    {
        TranslationY = (BoxSize - BoxSize / AspectRatio) * 0.5f;
    }
    else
    {
        TranslationX = (BoxSize - BoxSize * AspectRatio) * 0.5f;
    }

    PreviewImage->SetRenderTranslation({TranslationX, TranslationY});
}

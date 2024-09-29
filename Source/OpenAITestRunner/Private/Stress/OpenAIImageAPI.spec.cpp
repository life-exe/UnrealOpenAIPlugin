// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/ImageTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIImageAPI, All, All);

BEGIN_DEFINE_SPEC(FOpenAIProviderImage, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderImage)

using namespace OpenAI::Tests;

namespace
{
void TestImageResponse(FAutomationTestBase* Test, const TArray<FImageObject>& Data, int32 Num)
{
    if (!Test)
    {
        UE_LOGFMT(LogOpenAIImageAPI, Error, "Automation test object is invalid");
        return;
    }

    Test->TestTrue("Images amount should be valid", Data.Num() == Num);
    for (const auto& Image : Data)
    {
        Test->TestTrue("Image url should be valud", TestUtils::IsValidURL(Image.URL));
    }
}
}  // namespace

void FOpenAIProviderImage::Define()
{
    Describe("ActualAPI",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));
                    OpenAIProvider = NewObject<UOpenAIProvider>();
                    OpenAIProvider->SetLogEnabled(true);
                    OpenAIProvider->OnRequestError().AddLambda(
                        [&](const FString& URL, const FString& Content)
                        {
                            RequestCompleted = true;
                            AddError(FString::Format(TEXT("URL: {0} Content: {1}"), {URL, Content}));
                        });
                    RequestCompleted = false;
                });
            It("Image.CreateImageRequestShouldResponseCorrectly.Dalle2",
                [this]()
                {
                    OpenAIProvider->OnCreateImageCompleted().AddLambda(
                        [&](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImage OpenAIImage;
                    OpenAIImage.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::DALL_E_2);
                    OpenAIImage.N = 2;
                    OpenAIImage.Prompt = "Bear with beard drinking beer";
                    OpenAIImage.Size = UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_256x256);
                    OpenAIImage.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

                    OpenAIProvider->CreateImage(OpenAIImage, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageRequestShouldResponseCorrectly.Dalle3",
                [this]()
                {
                    OpenAIProvider->OnCreateImageCompleted().AddLambda(
                        [&](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 1);
                            TestTrueExpr(!Response.Data[0].Revised_Prompt.IsEmpty());
                            RequestCompleted = true;
                        });

                    FOpenAIImage OpenAIImage;
                    OpenAIImage.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::DALL_E_3);
                    OpenAIImage.N = 1;  // only one image is now supported.
                    OpenAIImage.Prompt = "Bear with beard drinking beer";
                    OpenAIImage.Size = UOpenAIFuncLib::OpenAIImageSizeDalle3ToString(EImageSizeDalle3::Size_1024x1024);
                    OpenAIImage.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(
                        EOpenAIImageFormat::URL);  // B64_JSON is not currently supported by the plugin.
                    OpenAIImage.Quality = UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality::Standard);
                    OpenAIImage.Style = UOpenAIFuncLib::OpenAIImageStyleToString(EOpenAIImageStyle::Natural);

                    OpenAIProvider->CreateImage(OpenAIImage, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageVariationRequestShouldResponseCorrectly.Dalle2",
                [this]()
                {
                    OpenAIProvider->OnCreateImageVariationCompleted().AddLambda(
                        [&](const FImageVariationResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImageVariation OpenAIImageVariation;
                    OpenAIImageVariation.N = 2;
                    OpenAIImageVariation.Image = TestUtils::FileFullPath("whale.png");
                    OpenAIImageVariation.Size = UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_256x256);
                    OpenAIImageVariation.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

                    OpenAIProvider->CreateImageVariation(OpenAIImageVariation, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageEditRequestShouldResponseCorrectly.Dalle2",
                [this]()
                {
                    OpenAIProvider->OnCreateImageEditCompleted().AddLambda(
                        [&](const FImageEditResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImageEdit OpenAIImageEdit;
                    OpenAIImageEdit.N = 2;
                    OpenAIImageEdit.Image = TestUtils::FileFullPath("whale.png");
                    OpenAIImageEdit.Mask = TestUtils::FileFullPath("whale_mask.png");
                    OpenAIImageEdit.Prompt = "put the hat on the whale's head";
                    OpenAIImageEdit.Size = UOpenAIFuncLib::OpenAIImageSizeDalle2ToString(EImageSizeDalle2::Size_256x256);
                    OpenAIImageEdit.Response_Format = UOpenAIFuncLib::OpenAIImageFormatToString(EOpenAIImageFormat::URL);

                    OpenAIProvider->CreateImageEdit(OpenAIImageEdit, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

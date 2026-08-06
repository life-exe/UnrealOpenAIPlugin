// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/ImageTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIImageAPI, All, All);

BEGIN_DEFINE_SPEC(FOpenAIProviderImage, "OpenAI.Provider",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderImage)

using namespace OpenAI::Tests;

namespace
{
// GPT image models always return base64-encoded images, never a URL.
void TestGptImageResponse(FAutomationTestBase* Test, const TArray<FImageObject>& Data, int32 Num)
{
    if (!Test)
    {
        UE_LOGFMT(LogOpenAIImageAPI, Error, "Automation test object is invalid");
        return;
    }

    Test->TestTrue("Images amount should be valid", Data.Num() == Num);
    for (const auto& Image : Data)
    {
        Test->TestFalse("Image b64_json should not be empty", Image.B64_JSON.IsEmpty());
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
            // dall-e-2 has been retired from the Images API (OpenAI now returns "The model 'dall-e-2' does not exist"),
            // so this scenario is covered against gpt-image-1 instead.
            It("Image.CreateImageRequestShouldResponseCorrectly.GptImage1_MultipleImages",
                [this]()
                {
                    OpenAIProvider->OnCreateImageCompleted().AddLambda(
                        [&](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestGptImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImage OpenAIImage;
                    OpenAIImage.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::GPT_Image_1);
                    OpenAIImage.N = 2;
                    OpenAIImage.Prompt = "Bear with beard drinking beer";
                    OpenAIImage.Size = UOpenAIFuncLib::OpenAIImageSizeGptImage1ToString(EImageSizeGptImage1::Size_1024x1024);

                    OpenAIProvider->CreateImage(OpenAIImage, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            // dall-e-3 has been retired from the Images API (OpenAI now returns "The model 'dall-e-3' does not exist"),
            // so this scenario is covered against gpt-image-1 instead.
            It("Image.CreateImageRequestShouldResponseCorrectly.GptImage1_LandscapeSize",
                [this]()
                {
                    OpenAIProvider->OnCreateImageCompleted().AddLambda(
                        [&](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestGptImageResponse(this, Response.Data, 1);
                            RequestCompleted = true;
                        });

                    FOpenAIImage OpenAIImage;
                    OpenAIImage.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::GPT_Image_1);
                    OpenAIImage.N = 1;
                    OpenAIImage.Prompt = "Bear with beard drinking beer";
                    OpenAIImage.Size = UOpenAIFuncLib::OpenAIImageSizeGptImage1ToString(EImageSizeGptImage1::Size_1536x1024);
                    OpenAIImage.Quality.Set(UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality::High));

                    OpenAIProvider->CreateImage(OpenAIImage, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Image.CreateImageRequestShouldResponseCorrectly.GptImage1",
                [this]()
                {
                    OpenAIProvider->OnCreateImageCompleted().AddLambda(
                        [&](const FImageResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestTrue("Images amount should be valid", Response.Data.Num() == 1);
                            TestTrueExpr(!Response.Data[0].B64_JSON.IsEmpty());
                            RequestCompleted = true;
                        });

                    FOpenAIImage OpenAIImage;
                    OpenAIImage.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::GPT_Image_1);
                    OpenAIImage.N = 1;  // gpt-image-1 only supports one image at the moment.
                    OpenAIImage.Prompt = "Bear with beard drinking beer";
                    OpenAIImage.Size = UOpenAIFuncLib::OpenAIImageSizeGptImage1ToString(EImageSizeGptImage1::Size_1024x1024);
                    OpenAIImage.Background.Set(UOpenAIFuncLib::OpenAIImageBackgroundToString(EOpenAIImageBackground::Transparent));
                    OpenAIImage.Moderation.Set(UOpenAIFuncLib::OpenAIImageModerationToString(EOpenAIImageModeration::Low));
                    OpenAIImage.Quality.Set(UOpenAIFuncLib::OpenAIImageQualityToString(EOpenAIImageQuality::Low));
                    OpenAIImage.Output_Format.Set(UOpenAIFuncLib::OpenAIImageOutputFormatToString(EOpenAIImageOutputFormat::Png));

                    OpenAIProvider->CreateImage(OpenAIImage, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            // dall-e-2 has been retired from the Images API (OpenAI now returns "The model 'dall-e-2' does not exist"),
            // so this scenario is covered against gpt-image-1 instead.
            It("Image.CreateImageEditRequestShouldResponseCorrectly.GptImage1",
                [this]()
                {
                    OpenAIProvider->OnCreateImageEditCompleted().AddLambda(
                        [&](const FImageEditResponse& Response, const FOpenAIResponseMetadata& Metadata)
                        {
                            TestTrueExpr(Response.Created > 0);
                            TestGptImageResponse(this, Response.Data, 2);
                            RequestCompleted = true;
                        });

                    FOpenAIImageEdit OpenAIImageEdit;
                    OpenAIImageEdit.Model = UOpenAIFuncLib::OpenAIImageModelToString(EImageModelEnum::GPT_Image_1);
                    OpenAIImageEdit.N = 2;
                    OpenAIImageEdit.Image.Add(TestUtils::FileFullPath("whale.png"));
                    OpenAIImageEdit.Mask = TestUtils::FileFullPath("whale_mask.png");
                    OpenAIImageEdit.Prompt = "put the hat on the whale's head";
                    OpenAIImageEdit.Size = UOpenAIFuncLib::OpenAIImageSizeGptImage1ToString(EImageSizeGptImage1::Size_1024x1024);

                    OpenAIProvider->CreateImageEdit(OpenAIImageEdit, Auth);
                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

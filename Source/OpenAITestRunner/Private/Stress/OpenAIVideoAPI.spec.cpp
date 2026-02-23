// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/VideoTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIVideoAPI, "OpenAI.Provider",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
FString VideoId;
END_DEFINE_SPEC(FOpenAIVideoAPI)

DEFINE_LOG_CATEGORY_STATIC(LogOpenAIVideoAPI, All, All);

using namespace OpenAI::Tests;

void FOpenAIVideoAPI::Define()
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

            It("Video.CreateVideoShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video created: id={0} status={1}", VideoObject.Id, VideoObject.Status);
                            TestFalse("Video id should not be empty", VideoObject.Id.IsEmpty());
                            TestFalse("Video status should not be empty", VideoObject.Status.IsEmpty());
                            TestFalse("Video prompt should not be empty", VideoObject.Prompt.IsEmpty());
                            RequestCompleted = true;
                        });

                    FCreateVideo CreateVideo;
                    CreateVideo.Prompt = "A serene mountain lake at sunset with golden reflections";
                    OpenAIProvider->CreateVideo(CreateVideo, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Video.ListVideosShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnListVideosCompleted().AddLambda(
                        [&](const FListVideosResponse& ListVideosResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Videos listed: count={0}", ListVideosResponse.Data.Num());
                            RequestCompleted = true;
                        });

                    FListVideos ListVideos;
                    OpenAIProvider->ListVideos(ListVideos, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Video.CreateAndRetrieveVideoShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video created: id={0}", VideoObject.Id);
                            VideoId = VideoObject.Id;
                            OpenAIProvider->RetrieveVideo(VideoId, Auth);
                        });

                    OpenAIProvider->OnRetrieveVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video retrieved: id={0} status={1}", VideoObject.Id, VideoObject.Status);
                            TestTrueExpr(VideoObject.Id.Equals(VideoId));
                            RequestCompleted = true;
                        });

                    FCreateVideo CreateVideo;
                    CreateVideo.Prompt = "A futuristic city skyline at night with neon lights";
                    OpenAIProvider->CreateVideo(CreateVideo, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Video.CreateAndDeleteVideoShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video created: id={0}", VideoObject.Id);
                            VideoId = VideoObject.Id;
                            // Poll until the video reaches a terminal state (completed/failed),
                            // since delete is only allowed on completed or failed videos.
                            OpenAIProvider->RetrieveVideo(VideoId, Auth);
                        });

                    OpenAIProvider->OnRetrieveVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video status: id={0} status={1}", VideoObject.Id, VideoObject.Status);
                            const bool bTerminal =
                                VideoObject.Status.Equals(TEXT("completed")) || VideoObject.Status.Equals(TEXT("failed"));
                            if (bTerminal)
                            {
                                OpenAIProvider->DeleteVideo(VideoId, Auth);
                            }
                            else
                            {
                                OpenAIProvider->RetrieveVideo(VideoId, Auth);
                            }
                        });

                    OpenAIProvider->OnDeleteVideoCompleted().AddLambda(
                        [&](const FDeleteVideoResponse& DeleteVideoResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video deleted: id={0}", DeleteVideoResponse.Id);
                            TestTrueExpr(DeleteVideoResponse.Id.Equals(VideoId));
                            TestTrueExpr(DeleteVideoResponse.Deleted);
                            RequestCompleted = true;
                        });

                    FCreateVideo CreateVideo;
                    CreateVideo.Prompt = "A peaceful forest with sunlight filtering through the trees";
                    OpenAIProvider->CreateVideo(CreateVideo, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Video.CreateAndDownloadVideoShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video created: id={0}", VideoObject.Id);
                            VideoId = VideoObject.Id;
                            // Poll until completed — download is only available on completed videos.
                            OpenAIProvider->RetrieveVideo(VideoId, Auth);
                        });

                    OpenAIProvider->OnRetrieveVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video status: id={0} status={1}", VideoObject.Id, VideoObject.Status);
                            if (VideoObject.Status.Equals(TEXT("completed")))
                            {
                                OpenAIProvider->DownloadVideoContent(VideoId, FDownloadVideoContent{}, Auth);
                            }
                            else if (VideoObject.Status.Equals(TEXT("failed")))
                            {
                                AddError(FString::Format(TEXT("Video generation failed, cannot download: id={0}"), {VideoId}));
                                RequestCompleted = true;
                            }
                            else
                            {
                                OpenAIProvider->RetrieveVideo(VideoId, Auth);
                            }
                        });

                    OpenAIProvider->OnDownloadVideoContentCompleted().AddLambda(
                        [&](const FDownloadVideoContentResponse& DownloadResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video downloaded: bytes={0}", DownloadResponse.Content.Num());
                            TestTrue("Downloaded content should not be empty", DownloadResponse.Content.Num() > 0);
                            RequestCompleted = true;
                        });

                    FCreateVideo CreateVideo;
                    CreateVideo.Prompt = "A gentle ocean wave rolling onto a sandy beach";
                    OpenAIProvider->CreateVideo(CreateVideo, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Video.CreateAndRemixVideoShouldResponseCorrectly",
                [this]()
                {
                    const FString RemixPrompt = "The same scene but during a winter snowstorm";

                    OpenAIProvider->OnCreateVideoCompleted().AddLambda(
                        [&](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video created: id={0}", VideoObject.Id);
                            VideoId = VideoObject.Id;
                            // Remix requires a completed source video — poll until terminal.
                            OpenAIProvider->RetrieveVideo(VideoId, Auth);
                        });

                    OpenAIProvider->OnRetrieveVideoCompleted().AddLambda(
                        [&, RemixPrompt](const FVideoObject& VideoObject, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video status: id={0} status={1}", VideoObject.Id, VideoObject.Status);
                            if (VideoObject.Status.Equals(TEXT("completed")))
                            {
                                FRemixVideo RemixVideo;
                                RemixVideo.Prompt = RemixPrompt;
                                OpenAIProvider->RemixVideo(VideoId, RemixVideo, Auth);
                            }
                            else if (VideoObject.Status.Equals(TEXT("failed")))
                            {
                                AddError(FString::Format(TEXT("Video generation failed, cannot remix: id={0}"), {VideoId}));
                                RequestCompleted = true;
                            }
                            else
                            {
                                OpenAIProvider->RetrieveVideo(VideoId, Auth);
                            }
                        });

                    OpenAIProvider->OnRemixVideoCompleted().AddLambda(
                        [&, RemixPrompt](const FVideoObject& RemixedVideo, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            UE_LOGFMT(LogOpenAIVideoAPI, Display, "Video remixed: id={0} source={1}", RemixedVideo.Id,
                                RemixedVideo.Remixed_From_Video_Id);
                            TestFalse("Remixed video id should not be empty", RemixedVideo.Id.IsEmpty());
                            TestTrueExpr(RemixedVideo.Remixed_From_Video_Id.Equals(VideoId));
                            TestTrueExpr(RemixedVideo.Prompt.Equals(RemixPrompt));
                            TestFalse("Remixed video status should not be empty", RemixedVideo.Status.IsEmpty());
                            RequestCompleted = true;
                        });

                    FCreateVideo CreateVideo;
                    CreateVideo.Prompt = "A golden wheat field swaying in a warm summer breeze";
                    OpenAIProvider->CreateVideo(CreateVideo, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif

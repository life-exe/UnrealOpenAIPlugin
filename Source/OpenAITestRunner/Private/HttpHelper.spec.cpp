// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Http/HttpHelper.h"
#include "TestUtils.h"

DEFINE_SPEC(
    FHttpHelper, "OpenAI", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

void FHttpHelper::Define()
{
    Describe("HttpHelper",
        [this]()
        {
            It("MIMETypeFromExtReturnsCorrectly",
                [this]()
                {
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("png").Equals("image/png"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("jpeg").Equals("image/jpeg"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("jpg").Equals("image/jpeg"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("JpG").Equals("image/jpeg"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("webp").Equals("image/webp"));

                    TestTrueExpr(HttpHelper::MIMETypeFromExt("mp3").Equals("audio/mpeg"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("mpga").Equals("audio/mpeg"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("mpeg").Equals("audio/mpeg"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("m4a").Equals("audio/m4a"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("wav").Equals("audio/wav"));

                    TestTrueExpr(HttpHelper::MIMETypeFromExt("mp4").Equals("video/mp4"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("webm").Equals("video/webm"));

                    TestTrueExpr(HttpHelper::MIMETypeFromExt("json").Equals("application/json"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("jsonl").Equals("application/jsonl"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("pdf").Equals("application/pdf"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("txt").Equals("text/plain"));

                    AddExpectedMessage(TEXT("Unknown file extension"), EAutomationExpectedErrorFlags::Contains, 2);
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("xyz").Equals("application/octet-stream"));
                    TestTrueExpr(HttpHelper::MIMETypeFromExt("").Equals("application/octet-stream"));
                });

            It("AddMIMEFileShouldAddContentCorrectly",
                [this]()
                {
                    const FString FilePath = OpenAI::Tests::TestUtils::FileFullPath("test_image.png");

                    TArray<uint8> FileContent;
                    FFileHelper::LoadFileToArray(FileContent, *FilePath);

                    TArray<uint8> Content;
                    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary(99999);

                    Content.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundary), BeginBoundary.Len());

                    FString Header = "Content-Disposition: form-data;";
                    Header.Append(FString::Printf(TEXT("name=\"randomParamName\";")));
                    Header.Append("filename=\"test_image.png\"").Append(LINE_TERMINATOR);
                    Header.Append(FString::Printf(TEXT("Content-Type: image/png"))).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR);

                    Content.Append((uint8*)TCHAR_TO_ANSI(*Header), Header.Len());
                    Content.Append(FileContent);

                    TestTrueExpr(HttpHelper::AddMIMEFile(FilePath, "randomParamName", BeginBoundary) == Content);
                });

            It("AddMIMEShouldAddContentCorrectly",
                [this]()
                {
                    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary(99999);

                    TArray<uint8> Content;
                    Content.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundary), BeginBoundary.Len());

                    FString Header = "Content-Disposition: form-data;";
                    Header.Append(FString::Printf(TEXT("name=\"randomParamName\""))).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR);
                    Header.Append(FString::Printf(TEXT("randomParamValue")));
                    Content.Append((uint8*)TCHAR_TO_ANSI(*Header), Header.Len());

                    TestTrueExpr(HttpHelper::AddMIME("randomParamName", "randomParamValue", BeginBoundary) == Content);
                });

            It("MakeBoundaryCreatedCorrectly",
                [this]()
                {
                    const auto& [Boundary, BeginBoundary, EndBoundary] = HttpHelper::MakeBoundary(99999);
                    TestTrueExpr(Boundary.Equals("---------------------------99999"));
                    TestTrueExpr(BeginBoundary.Equals(FString(LINE_TERMINATOR).Append("--").Append(Boundary).Append(LINE_TERMINATOR)));
                    TestTrueExpr(
                        EndBoundary.Equals(FString(LINE_TERMINATOR).Append("--").Append(Boundary).Append("--").Append(LINE_TERMINATOR)));
                });
        });
}

#endif

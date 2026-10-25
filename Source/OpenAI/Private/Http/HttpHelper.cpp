// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Http/HttpHelper.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogHttpHelper, All, All);

FString HttpHelper::MIMETypeFromExt(const FString& Ext)
{
    // clang-format off
    static const TMap<FString, FString> MIMETypes{
        {TEXT("png"), TEXT("image/png")},
        {TEXT("jpg"), TEXT("image/jpeg")},
        {TEXT("jpeg"), TEXT("image/jpeg")},
        {TEXT("webp"), TEXT("image/webp")},
        {TEXT("gif"), TEXT("image/gif")},
        {TEXT("mp3"), TEXT("audio/mpeg")},
        {TEXT("mpga"), TEXT("audio/mpeg")},
        {TEXT("mpeg"), TEXT("audio/mpeg")},
        {TEXT("m4a"), TEXT("audio/m4a")},
        {TEXT("wav"), TEXT("audio/wav")},
        {TEXT("ogg"), TEXT("audio/ogg")},
        {TEXT("flac"), TEXT("audio/flac")},
        {TEXT("aac"), TEXT("audio/aac")},
        {TEXT("mp4"), TEXT("video/mp4")},
        {TEXT("webm"), TEXT("video/webm")},
        {TEXT("json"), TEXT("application/json")},
        {TEXT("jsonl"), TEXT("application/jsonl")},
        {TEXT("pdf"), TEXT("application/pdf")},
        {TEXT("txt"), TEXT("text/plain")},
        {TEXT("md"), TEXT("text/plain")},
    };
    // clang-format on

    // TMap<FString, ...> key lookup is case-insensitive
    if (const FString* MIMEType = MIMETypes.Find(Ext))
    {
        return *MIMEType;
    }

    UE_LOGFMT(LogHttpHelper, Warning, "Unknown file extension '{0}', falling back to application/octet-stream", Ext);
    return TEXT("application/octet-stream");
}

TArray<uint8> HttpHelper::AddMIMEFile(const FString& FilePath, const FString& ParamName, const FString& BeginBoundary)
{
    TArray<uint8> Content;

    TArray<uint8> FileContent;
    if (FFileHelper::LoadFileToArray(FileContent, *FilePath))
    {
        Content.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundary), BeginBoundary.Len());

        FString Header = "Content-Disposition: form-data;";
        Header.Append(FString::Printf(TEXT("name=\"%s\";"), *ParamName));
        Header.Append("filename=\"" + FPaths::GetCleanFilename(FilePath) + "\"").Append(LINE_TERMINATOR);
        const FString MIMEType = MIMETypeFromExt(FPaths::GetExtension(FilePath));
        Header.Append(FString::Printf(TEXT("Content-Type: %s"), *MIMEType)).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR);

        Content.Append((uint8*)TCHAR_TO_ANSI(*Header), Header.Len());
        Content.Append(FileContent);
    }
    else
    {
        UE_LOGFMT(LogHttpHelper, Error, "Failed to load file's bytes {0}", FilePath);
    }

    return Content;
}

TArray<uint8> HttpHelper::AddMIME(const FString& ParamName, const FString& ParamValue, const FString& BeginBoundary)
{
    TArray<uint8> Content;
    Content.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundary), BeginBoundary.Len());

    FString Header = "Content-Disposition: form-data;";
    Header.Append(FString::Printf(TEXT("name=\"%s\""), *ParamName)).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR);
    Header.Append(FString::Printf(TEXT("%s"), *ParamValue));
    Content.Append((uint8*)TCHAR_TO_ANSI(*Header), Header.Len());

    return Content;
}

TTuple<FString, FString, FString> HttpHelper::MakeBoundary(int64 Ticks)
{
    const FString Boundary = "---------------------------" + FString::FromInt(Ticks);
    const FString BeginBoundary = FString(LINE_TERMINATOR).Append("--").Append(Boundary).Append(LINE_TERMINATOR);
    const FString EndBoundary = FString(LINE_TERMINATOR).Append("--").Append(Boundary).Append("--").Append(LINE_TERMINATOR);
    return MakeTuple(Boundary, BeginBoundary, EndBoundary);
}

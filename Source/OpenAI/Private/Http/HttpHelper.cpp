// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "Http/HttpHelper.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogHttpHelper, All, All);

FString HttpHelper::MIMETypeFromExt(const FString& Ext)
{
    const FString Image = "png jpeg jpg";
    const FString Audio = "mp3 wav";
    const FString Video = "mp4 mpeg mpga m4a webm";
    const FString JSON = "json jsonl";

    if (Image.Contains(Ext)) return FString("image/").Append(Ext.ToLower());
    if (Audio.Contains(Ext)) return FString("audio/").Append(Ext.ToLower());
    if (Video.Contains(Ext)) return FString("video/").Append(Ext.ToLower());
    if (JSON.Contains(Ext)) return FString("application/").Append(Ext.ToLower());

    checkNoEntry();
    return {};
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

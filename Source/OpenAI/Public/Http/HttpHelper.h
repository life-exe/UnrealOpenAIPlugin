// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class OPENAI_API HttpHelper
{
public:
    static FString MIMETypeFromExt(const FString& Ext);

    // Multipart parts are UTF-8 encoded. AddMIMEFile returns an empty array if the file can't be read.
    static TArray<uint8> AddMIMEFile(const FString& FilePath, const FString& ParamName, const FString& BeginBoundary);
    static TArray<uint8> AddMIME(const FString& ParamName, const FString& ParamValue, const FString& BeginBoundary);
    static TArray<uint8> StringToUTF8(const FString& String);
    static TTuple<FString, FString, FString> MakeBoundary(int64 Ticks = FDateTime::Now().GetTicks());

    // multipart/form-data requires CRLF on every platform
    static constexpr const TCHAR* CRLF = TEXT("\r\n");
};

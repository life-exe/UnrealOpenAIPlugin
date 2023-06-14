// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class OPENAI_API HttpHelper
{
public:
    static FString MIMETypeFromExt(const FString& Ext);
    static TArray<uint8> AddMIMEFile(const FString& FilePath, const FString& ParamName, const FString& BeginBoundary);
    static TArray<uint8> AddMIME(const FString& ParamName, const FString& ParamValue, const FString& BeginBoundary);
    static TTuple<FString, FString, FString> MakeBoundary(int64 Ticks = FDateTime::Now().GetTicks());
};
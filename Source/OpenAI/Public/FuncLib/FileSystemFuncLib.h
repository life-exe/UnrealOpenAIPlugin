// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Provider/Types/CommonTypes.h"
#include "FileSystemFuncLib.generated.h"

UENUM(BlueprintType)
enum class EFileType : uint8
{
    Image,
    Media
};

UCLASS()
class OPENAI_API UFileSystemFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "FileSystem")
    static bool OpenFile(const FString& FileExtensions, TArray<FString>& OutFileNames);

    UFUNCTION(BlueprintCallable, Category = "FileSystem")
    static bool SaveImageToFile(UTexture2D* Texture);

    UFUNCTION(BlueprintPure, Category = "FileSystem")
    static FString GetFileExtensions(EFileType FileType);

    UFUNCTION(BlueprintPure, Category = "FileSystem")
    static bool SaveChatHistoryToFile(const TArray<FMessage>& History, const FString& ModelName, const FString& FilePath);
};
// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IImageWrapper.h"
#include "ImageFuncLib.generated.h"

UCLASS()
class OPENAI_API UImageFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "ImageHelper")
    static UTexture2D* Texture2DFromBytes(const FString& RawFileData);

    static bool BytesFromTexture2D(UTexture2D* Texture, TArray<uint8>& OutBytes, EImageFormat ImageFormat = EImageFormat::PNG);
    static UTexture2D* CreateTexture(const TArray<uint8>& RawFileData);

private:
    static void WEBPFormatCheck(const TArray<uint8>& Bytes);
};
// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/ImageFuncLib.h"
#include "IImageWrapperModule.h"
#include "Misc/Base64.h"
#include "PixelFormat.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapperModule.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogImageFuncLib, All, All);

namespace
{
FString ImageFormatToString(EImageFormat ImageFormat)
{
    switch (ImageFormat)
    {
        case EImageFormat::Invalid: return "Invalid";
        case EImageFormat::PNG: return "PNG";
        case EImageFormat::JPEG: return "JPEG";
        case EImageFormat::GrayscaleJPEG: return "GrayscaleJPEG";
        case EImageFormat::BMP: return "BMP";
        case EImageFormat::ICO: return "ICO";
        case EImageFormat::EXR: return "EXR";
        case EImageFormat::ICNS: return "ICNS";
        case EImageFormat::TGA: return "TGA";
        case EImageFormat::HDR: return "HDR";
        case EImageFormat::TIFF: return "TIFF";
        case EImageFormat::DDS: return "DDS";
    }

    return "Unknown";
}
}  // namespace

void UImageFuncLib::WEBPFormatCheck(const TArray<uint8>& Bytes)
{
    // Check if the 'WEBP' signature is present after the 'RIFF' header
    if (Bytes.Num() > 12     //
        && Bytes[8] == 'W'   //
        && Bytes[9] == 'E'   //
        && Bytes[10] == 'B'  //
        && Bytes[11] == 'P')
    {
        UE_LOGFMT(LogImageFuncLib, Error,
            "This is probably WebP format, Unreal Engine doesn't support it. If this happens during a DALLE 3 request please use the "
            "URL "
            "format option.");
        return;
    }

    UE_LOGFMT(LogImageFuncLib, Error, "Unknown image format with RIFF header");
}

UTexture2D* UImageFuncLib::Texture2DFromBytes(const FString& RawFileStr)
{
    TArray<uint8> RawFileData;
    if (!FBase64::Decode(RawFileStr, RawFileData))
    {
        UE_LOGFMT(LogTemp, Error, "Decode failed");
        return nullptr;
    }

    return CreateTexture(RawFileData);
}

UTexture2D* UImageFuncLib::CreateTexture(const TArray<uint8>& RawFileData)
{
    auto& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    const EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(RawFileData.GetData(), RawFileData.Num());
    UE_LOGFMT(LogImageFuncLib, Display, "Detected image format {0}", ImageFormatToString(ImageFormat));

    if (ImageFormat == EImageFormat::Invalid)
    {
        WEBPFormatCheck(RawFileData);
        return nullptr;
    }

    auto ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);

    if (!ImageWrapper.IsValid())
    {
        UE_LOGFMT(LogTemp, Error, "Image wrapper is invalid");
        return nullptr;
    }

    if (!ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
    {
        UE_LOGFMT(LogTemp, Error, "Setting raw data failed");
        return nullptr;
    }

    UTexture2D* Texture{nullptr};
    TArray<uint8> UncompressedRGBA;
    if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
    {
        Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
        if (!Texture) return nullptr;

        void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
        FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
        Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
        Texture->UpdateResource();
    }

    return Texture;
}

bool UImageFuncLib::BytesFromTexture2D(UTexture2D* Texture, TArray<uint8>& OutBytes, EImageFormat ImageFormat)
{
    if (!Texture || !Texture->GetPlatformData()) return false;

    FTexture2DMipMap& MipMap = Texture->GetPlatformData()->Mips[0];
    const uint8* RawData = (const uint8*)MipMap.BulkData.Lock(LOCK_READ_ONLY);
    const int32 NumPixels = Texture->GetSizeX() * Texture->GetSizeY();
    const int32 NumBytes = NumPixels * sizeof(FColor);

    TArray<uint8> ImageBytes;
    ImageBytes.SetNumUninitialized(NumBytes);
    FMemory::Memcpy(ImageBytes.GetData(), RawData, NumBytes);

    MipMap.BulkData.Unlock();

    auto& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    auto ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
    ImageWrapper->SetRaw(ImageBytes.GetData(), ImageBytes.Num(), Texture->GetSizeX(), Texture->GetSizeY(), ERGBFormat::RGBA, 8);

    OutBytes = ImageWrapper->GetCompressed();
    return true;
}
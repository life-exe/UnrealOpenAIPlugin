// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/ImageFuncLib.h"
#include "IImageWrapperModule.h"
#include "Misc/Base64.h"
#include "PixelFormat.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"

UTexture2D* UImageFuncLib::Texture2DFromBytes(const FString& RawFileStr)
{
    UTexture2D* Texture = nullptr;

    auto& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    auto ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    TArray<uint8> RawFileData;
    FBase64::Decode(RawFileStr, RawFileData);

    if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
    {
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
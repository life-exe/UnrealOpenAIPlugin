// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/FileSystemFuncLib.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "FuncLib/ImageFuncLib.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SWindow.h"
#include "Framework/Application/SlateApplication.h"

DEFINE_LOG_CATEGORY_STATIC(LogFileSystemFuncLib, All, All);

namespace
{
const FText OpenDialogTitle = NSLOCTEXT("FileSystem", "Choosefile", "Choose file");
const FText SaveDialogTitle = NSLOCTEXT("FileSystem", "Savefile", "Save file");
const FString DefaultPath = FPaths::ProjectDir();
const FString DefaultFileNameToSave = TEXT("OpenAIImage");
}  // namespace

bool UFileSystemFuncLib::OpenFile(const FString& FileExtensions, TArray<FString>& OutFileNames)
{
    if (!GEngine || !GEngine->GameViewport) return false;

    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!DesktopPlatform) return false;

    auto* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
    const bool Success = DesktopPlatform->OpenFileDialog(
        ParentWindowHandle, OpenDialogTitle.ToString(), DefaultPath, {}, FileExtensions, EFileDialogFlags::None, OutFileNames);

    for (auto& FileName : OutFileNames)
    {
        FileName = FPaths::ConvertRelativePathToFull(FileName);
    }
    return Success && OutFileNames.Num() > 0;
}

bool UFileSystemFuncLib::SaveImageToFile(UTexture2D* Texture)
{
    if (!Texture) return false;

    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!DesktopPlatform) return false;

    TArray<FString> OutFileNames;
    const bool Success = DesktopPlatform->SaveFileDialog(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),  //
        SaveDialogTitle.ToString(),                                                                                               //
        DefaultPath,                                                                                                              //
        DefaultFileNameToSave,                                                                                                    //
        GetFileExtensions(EFileType::Image),                                                                                      //
        EFileDialogFlags::None,                                                                                                   //
        OutFileNames);
    if (!Success || OutFileNames.Num() == 0) return false;

    TArray<uint8> ImageBytes;
    if (UImageFuncLib::BytesFromTexture2D(Texture, ImageBytes))
    {
        return FFileHelper::SaveArrayToFile(ImageBytes, *OutFileNames[0]);
    }

    return false;
}

FString UFileSystemFuncLib::GetFileExtensions(EFileType FileType)
{
    switch (FileType)
    {
        case EFileType::Image: return "ImageFiles (png, jpg, jpeg)|*.png;*.jpg;*.jpeg";
        case EFileType::Media: return "MediaFiles (mp3, mp4, mpeg, m4a, mpga, wav, webm)|*.mp3;*.mp4;*.mpeg;*.m4a;*.mpga;*.wav;*.webm";
    }

    checkNoEntry();
    return {};
}

bool UFileSystemFuncLib::SaveChatHistoryToFile(const TArray<FMessage>& History, const FString& ModelName, const FString& FilePath)
{
    FString FileContent;

    FString Line = FString::Printf(TEXT("[model]: %s"), *ModelName);
    FileContent.Append(Line).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR);

    for (const auto& Message : History)
    {
        if (Message.Content.IsEmpty())
        {
            Line = FString::Printf(
                TEXT("[%s]: [function: %s, args: %s]"), *Message.Role, *Message.Function_Call.Name, *Message.Function_Call.Arguments);
        }
        else
        {
            Line = FString::Printf(TEXT("[%s]: %s"), *Message.Role, *Message.Content);
        }
        FileContent.Append(Line).Append(LINE_TERMINATOR);
    }

    const auto FullFilePath = FPaths::ConvertRelativePathToFull(FilePath);
    if (!FFileHelper::SaveStringToFile(FileContent, *FilePath))
    {
        UE_LOG(LogFileSystemFuncLib, Error, TEXT("File saving error: %s"), *FullFilePath);
        return false;
    }

    UE_LOG(LogFileSystemFuncLib, Display, TEXT("Chat history saved to the %s"), *FullFilePath);
    return true;
}

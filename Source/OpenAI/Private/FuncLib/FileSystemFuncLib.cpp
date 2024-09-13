// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#include "FuncLib/FileSystemFuncLib.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "FuncLib/ImageFuncLib.h"
#include "FuncLib/OpenAIFuncLib.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Widgets/SWindow.h"
#include "Framework/Application/SlateApplication.h"
#include "Logging/StructuredLog.h"

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
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!DesktopPlatform) return false;

    const bool Success = DesktopPlatform->OpenFileDialog(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),  //
        OpenDialogTitle.ToString(),                                                                                               //
        DefaultPath,                                                                                                              //
        {},                                                                                                                       //
        FileExtensions,                                                                                                           //
        EFileDialogFlags::Multiple,                                                                                               //
        OutFileNames);

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

    FString Line = FString::Format(TEXT("[model]: {0}"), {ModelName});
    FileContent.Append(Line).Append(LINE_TERMINATOR).Append(LINE_TERMINATOR);

    for (const auto& Message : History)
    {
        if (!Message.ContentArray.IsEmpty())
        {
            FString ContentString{};
            for (const auto& Content : Message.ContentArray)
            {
                if (Content.Type == UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Text))
                {
                    ContentString.Append(Content.Text);
                }
                else if (Content.Type == UOpenAIFuncLib::OpenAIMessageContentTypeToString(EMessageContentType::Image_URL))
                {
                    ContentString.Append("[IMAGE] ");
                }
                Line = FString::Format(TEXT("[{0}]: {1}"), {Message.Role, ContentString});
                FileContent.Append(Line).Append(LINE_TERMINATOR);
            }
        }
        else if (Message.Content.IsEmpty())
        {
            for (const auto& Tool : Message.Tool_Calls)
            {
                Line =
                    FString::Format(TEXT("[{0}]: [function:{1}, args:{2}]"), {Message.Role, Tool.Function.Name, Tool.Function.Arguments});
                FileContent.Append(Line).Append(LINE_TERMINATOR);
            }
        }
        else
        {
            Line = FString::Format(TEXT("[{0}]: {1}"), {Message.Role, Message.Content});
            FileContent.Append(Line).Append(LINE_TERMINATOR);
        }
    }

    const auto FullFilePath = FPaths::ConvertRelativePathToFull(FilePath);
    if (!FFileHelper::SaveStringToFile(FileContent, *FilePath))
    {
        UE_LOGFMT(LogFileSystemFuncLib, Error, "File saving error: {0}", FullFilePath);
        return false;
    }

    UE_LOGFMT(LogFileSystemFuncLib, Display, "Chat history saved to the {0}", FullFilePath);
    return true;
}

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModelTypes.generated.h"

UENUM(BlueprintType)
enum class EAllModelEnum : uint8
{
    Babbage,
    Davinci,
    Text_Davinci_Edit_001,
    Babbage_Code_Search_Code,
    Text_Similarity_Babbage_001,
    Code_Davinci_Edit_001,
    Text_Davinci_001,
    Text_Davinci_003,
    Ada,
    Curie_Instruct_Beta,
    Babbage_Code_Search_Text,
    Babbage_Similarity,
    Whisper_1,
    Code_Search_Babbage_Text_001,
    GPT_3_5_Turbo_0301,
    GPT_3_5_Turbo,
    GPT_3_5_Turbo_16k_0613,
    GPT_3_5_Turbo_16k,
    GPT_3_5_Turbo_0613,
    GPT_3_5_Turbo_instruct_0914,
    GPT_3_5_Turbo_instruct,
    Code_Search_Babbage_Code_001,
    Text_Ada_001,
    Text_Embedding_Ada_002,
    Text_Similarity_Ada_001,
    Ada_Code_Search_Code,
    Ada_Similarity,
    Code_Search_Ada_Text_001,
    Text_Search_Ada_Query_001,
    Davinci_Search_Document,
    Ada_Code_Search_Text,
    Text_Search_Ada_Doc_001,
    Davinci_Instruct_Beta,
    Text_Similarity_Curie_001,
    Code_Search_Ada_Code_001,
    Ada_Search_Query,
    Text_Search_Davinci_Query_001,
    Curie_Search_Query,
    Davinci_Search_Query,
    Babbage_Search_Document,
    Ada_Search_Document,
    Text_Search_Curie_Query_001,
    Text_Search_Babbage_Doc_001,
    Curie_Search_Document,
    Text_Search_Curie_Doc_001,
    Babbage_Search_Query,
    Text_Babbage_001,
    Text_Search_Davinci_Doc_001,
    Text_Search_Babbage_Query_001,
    Curie_Similarity,
    Curie,
    Text_Similarity_Davinci_001,
    Text_Davinci_002,
    Davinci_Similarity,
    Babbage_002,
    Davinci_002,
    Text_Curie_001,
    GPT_4,
    GPT_4_0613,
    GPT_4_0314,
    Fanw_Json_Eval
};

UENUM(BlueprintType)
enum class EMainModelEnum : uint8
{
    GPT_4,
    GPT_4_0613,
    GPT_4_0314,
    GPT_3_5_Turbo_0301,
    GPT_3_5_Turbo,
    Text_Davinci_003,
    Text_Davinci_002
};

UENUM(BlueprintType)
enum class EImageSize : uint8
{
    Size_256x256,
    Size_512x512,
    Size_1024x1024
};

UENUM(BlueprintType)
enum class EOpenAIImageFormat : uint8
{
    URL,
    B64_JSON
};

UENUM(BlueprintType)
enum class ERole : uint8
{
    System,
    User,
    Assistant
};

UENUM(BlueprintType)
enum class ETranscriptFormat : uint8
{
    JSON,
    Text,
    Str,
    Verbose_JSON,
    Vtt
};

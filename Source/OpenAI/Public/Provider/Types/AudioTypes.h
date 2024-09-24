// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AudioTypes.generated.h"

UENUM(BlueprintType)
enum class EAudioModel : uint8
{
    Whisper_1
};

UENUM(BlueprintType)
enum class ETTSModel : uint8
{
    TTS_1,
    TTS_1_HD
};

UENUM(BlueprintType)
enum class EVoice : uint8
{
    Alloy,
    Echo,
    Fable,
    Onyx,
    Nova,
    Shimmer,
};

// Make sure that WMFCodecs plugin is enabled (Edit->Plugins->WMFCodecs)
UENUM(BlueprintType)
enum class ETTSAudioFormat : uint8
{
    MP3,
    OPUS,  // Not supported by UnrealEngine
    AAC,
    FLAC  // Not supported by UnrealEngine
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

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FAudioBase
{
    GENERATED_BODY()

    /**
      The audio file to transcribe, in one of these formats: mp3, mp4, mpeg, mpga, m4a, wav, or webm.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString File;

    /**
      ID of the model to use. Only whisper-1 (which is powered by our open source Whisper V2 model)
      is currently available.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model{"whisper-1"};

    /**
      An optional text to guide the model's style or continue a previous audio segment.
      The prompt should match the audio language.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Prompt;

    /**
      The format of the transcript output, in one of these options: json, text, srt, verbose_json, or vtt.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"json"};

    /**
      The sampling temperature, between 0 and 1.
      Higher values like 0.8 will make the output more random,
      while lower values like 0.2 will make it more focused and deterministic.
      If set to 0, the model will use log probability to automatically increase
      the temperature until certain thresholds are hit.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Temperature{0.0f};
};

USTRUCT(BlueprintType)
struct FAudioTranscription : public FAudioBase
{
    GENERATED_BODY()

    /**
      The language of the input audio.
      Supplying the input language in ISO-639-1 (https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) format
      will improve accuracy and latency.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Language;

    /**
      The timestamp granularities to populate for this transcription.
      response_format must be set verbose_json to use timestamp granularities.
      Either or both of these options are supported: word, or segment.
      Note: There is no additional latency for segment timestamps, but generating
      word timestamps incurs additional latency.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<int32> Timestamp_Granularities;
};

USTRUCT(BlueprintType)
struct FAudioTranslation : public FAudioBase
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FSpeech
{
    GENERATED_BODY()

    /**
      One of the available TTS models: tts-1 or tts-1-hd:
      https://platform.openai.com/docs/models/tts
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model{"tts-1"};

    /**
      The text to generate audio for. The maximum length is 4096 characters.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Input;

    /**
      The voice to use when generating the audio. Supported voices are alloy, echo, fable, onyx, nova, and shimmer.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Voice{"alloy"};

    /**
      The format to audio in. Supported formats are mp3, opus, aac, and flac.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Response_Format{"mp3"};

    /**
      The speed of the generated audio. Select a value from 0.25 to 4.0. 1.0 is the default.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Speed{1.0f};
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

/**
  Represents a transcription response returned by model, based on the provided input.
*/
USTRUCT(BlueprintType)
struct FAudioTranscriptionResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Text;
};

USTRUCT(BlueprintType)
struct FAudioTranscriptionWord
{
    GENERATED_BODY()

    /**
      The text content of the word.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Word;

    /**
      Start time of the word in seconds.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Start{};

    /**
      End time of the word in seconds.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 End{};
};

USTRUCT(BlueprintType)
struct FAudioTranscriptionSegment
{
    GENERATED_BODY()

    /**
      Unique identifier of the segment.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Id{};

    /**
      Seek offset of the segment.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Seek{};

    /**
      Start time of the segment in seconds.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Start{};

    /**
      End time of the segment in seconds.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 End{};

    /**
      Text content of the segment.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Text;

    /**
      Array of token IDs for the text content.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<int32> Tokens;

    /**
      Temperature parameter used for generating the segment.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Temperature{};

    /**
      Average logprob of the segment. If the value is lower than -1, consider the logprobs failed.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Avg_Logprob{};

    /**
      Compression ratio of the segment. If the value is greater than 2.4, consider the compression failed.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double Compression_Ratio{};

    /**
      CProbability of no speech in the segment.
      If the value is higher than 1.0 and the avg_logprob is below -1, consider this segment silent.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    double No_Speech_Prob{};
};

/**
  Represents a verbose json transcription response returned by model, based on the provided input.
*/
USTRUCT(BlueprintType)
struct FAudioTranscriptionVerboseResponse
{
    GENERATED_BODY()

    /**
      The language of the input audio.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Language;

    /**
      The duration of the input audio.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Duration;

    /**
      The transcribed text.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Text;

    /**
      Extracted words and their corresponding timestamps.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FAudioTranscriptionWord> Words;

    /**
      Segments of the transcribed text and their corresponding details.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FAudioTranscriptionSegment> Segments;
};

USTRUCT(BlueprintType)
struct FAudioTranslationResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Text;
};

USTRUCT(BlueprintType)
struct FSpeechResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TArray<uint8> Bytes;
};

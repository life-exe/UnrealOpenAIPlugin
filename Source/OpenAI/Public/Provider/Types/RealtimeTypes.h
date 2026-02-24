// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "RealtimeTypes.generated.h"

///////////////////////////////////////////////////////
//                 AUDIO CONFIG
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRealtimePCMAudioFormat
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    int32 Rate{24000};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"audio/pcm"};
};

USTRUCT(BlueprintType)
struct FRealtimePCMUAudioFormat
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"audio/pcmu"};
};

USTRUCT(BlueprintType)
struct FRealtimePCMAAudioFormat
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"audio/pcma"};
};

USTRUCT(BlueprintType)
struct FRealtimeNoiseReduction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Type;  // "near_field", "far_field"

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};
};

USTRUCT(BlueprintType)
struct FRealtimeAudioTranscription
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Language;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Model;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Prompt;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};
};

USTRUCT(BlueprintType)
struct FRealtimeServerVad
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"server_vad"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Create_Response;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Idle_Timeout_Ms;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Interrupt_Response;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Prefix_Padding_Ms;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Silence_Duration_Ms;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalFloat Threshold;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};
};

USTRUCT(BlueprintType)
struct FRealtimeSemanticVad
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"semantic_vad"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Create_Response;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Eagerness;  // "low", "medium", "high", "auto"

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Interrupt_Response;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};
};

///////////////////////////////////////////////////////
//                 TOOLS & PROMPTS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRealtimeResponsePrompt
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Id;
};

USTRUCT(BlueprintType)
struct FRealtimeFunctionTool
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Parameters;  // JSON string

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"function"};
};

///////////////////////////////////////////////////////
//                 TRACING & TRUNCATION
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRealtimeTracingConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Group_Id;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Workflow_Name;
};

USTRUCT(BlueprintType)
struct FRealtimeTruncation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Type;  // "auto", "disabled", "retention_ratio"

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalFloat Retention_Ratio;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};
};

///////////////////////////////////////////////////////
//                 SESSION CONFIG
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRealtimeAudioInputConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimePCMAudioFormat Format;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeNoiseReduction Noise_Reduction;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeAudioTranscription Transcription;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeServerVad Turn_Detection;
};

USTRUCT(BlueprintType)
struct FRealtimeAudioOutputConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimePCMAudioFormat Format;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalFloat Speed;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Voice;
};

USTRUCT(BlueprintType)
struct FRealtimeAudioConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeAudioInputConfig Input;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeAudioOutputConfig Output;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool IsSet{false};
};

USTRUCT(BlueprintType)
struct FRealtimeSessionCreateRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Type{"realtime"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeAudioConfig Audio;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FString> Include;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Instructions;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Max_Output_Tokens;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Model;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FString> Output_Modalities;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FRealtimeFunctionTool> Tools;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeTruncation Truncation;
};

///////////////////////////////////////////////////////
//                 REST ENDPOINTS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRealtimeExpiresAfter
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Anchor{"created_at"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    int32 Seconds{600};
};

USTRUCT(BlueprintType)
struct FCreateRealtimeClientSecret
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeExpiresAfter Expires_After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FRealtimeSessionCreateRequest Session;
};

USTRUCT(BlueprintType)
struct FRealtimeSessionResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Type;
};

USTRUCT(BlueprintType)
struct FCreateRealtimeClientSecretResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Expires_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Value;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FRealtimeSessionResponse Session;
};

using FRealtimeClientSecretResponse = FCreateRealtimeClientSecretResponse;

///////////////////////////////////////////////////////
//                 CALLS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRealtimeAcceptCall : public FRealtimeSessionCreateRequest
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FRealtimeReferCall
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Target_Uri;
};

USTRUCT(BlueprintType)
struct FRealtimeRejectCall
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Status_Code;
};

// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ToolsTypes.h"
#include "CommonTypes.generated.h"

USTRUCT(BlueprintType)
struct FOpenAIAuth
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString APIKey{};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString OrganizationID{};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString ProjectID{};

    bool IsEmpty() const { return APIKey.IsEmpty() || OrganizationID.IsEmpty() || ProjectID.IsEmpty(); }
};

USTRUCT(BlueprintType)
struct FOpenAIError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString RawContent{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool WasError{false};
};

UENUM(BlueprintType)
enum class EOpenAIHttpHeaderType : uint8
{
    XRequestId = 0,
    OpenAIProcessingMS,
    OpenAIOrganization,
    OpenAIVersion
};

USTRUCT(BlueprintType)
struct FOpenAIResponseMetadata
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FString> HttpHeaders{};
};

/*
    @todo: add more errors https://platform.openai.com/docs/guides/error-codes
*/
UENUM(BlueprintType)
enum class EOpenAIResponseError : uint8
{
    InvalidAPIKey = 0,
    NetworkError,
    ModelNotFound,
    InsufficientQuota,
    InvalidLanguageFormat,
    Unknown
};

UENUM(BlueprintType)
enum class ERole : uint8
{
    System,
    User,
    Assistant,
    Function,
    Tool
};

UENUM(BlueprintType)
enum class EOpenAIFinishReason : uint8
{
    Stop = 0,
    Length,
    Content_Filter,
    Tool_Calls,
    Null
};

namespace OpenAI
{
using ServiceSecrets = TArray<TTuple<FString, FString>>;
using QueryPairs = TArray<TTuple<FString, FString>>;

}  // namespace OpenAI

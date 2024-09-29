// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/Types/AudioTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Misc/Paths.h"
#include "SpeechAction.generated.h"

USTRUCT(BlueprintType)
struct FSpeechPayload
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FSpeechResponse Response;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString FilePath{};
};

USTRUCT(BlueprintType)
struct FSpeechSettings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString AbsolutePath{FPaths::ProjectPluginsDir().Append("OpenAI/Saved")};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString BaseName{"speech"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool AppendDate{true};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    bool SaveToFile{true};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FOnSpeech, const FSpeechPayload&, Response, const FOpenAIResponseMetadata&, ResponseMetadata, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class USpeechAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnSpeech OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static USpeechAction* CreateSpeech(
        const FSpeech& Speech, const FOpenAIAuth& Auth, const FString& URLOverride, const FSpeechSettings& Settings);

    void TryToOverrideURL();

    void OnCreateSpeechCompleted(const FSpeechResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    UPROPERTY()
    TObjectPtr<UOpenAIProvider> Provider;

    FSpeech Speech;
    FOpenAIAuth Auth;
    FString URLOverride{};
    FSpeechSettings Settings;
};

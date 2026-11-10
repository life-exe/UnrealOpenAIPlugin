// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Audio/CreateVoiceConsentAction.h"

UCreateVoiceConsentAction* UCreateVoiceConsentAction::CreateVoiceConsent(
    const FCreateVoiceConsent& CreateVoiceConsent, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* Action = NewObject<UCreateVoiceConsentAction>();
    Action->CreateVoiceConsentParams = CreateVoiceConsent;
    Action->Auth = Auth;
    Action->URLOverride = URLOverride;
    return Action;
}

void UCreateVoiceConsentAction::Activate()
{
    auto* Provider = CreateProvider();
    Provider->OnCreateVoiceConsentCompleted().AddUObject(this, &ThisClass::OnCreateVoiceConsentCompleted);
    Provider->CreateVoiceConsent(CreateVoiceConsentParams, Auth);
}

void UCreateVoiceConsentAction::OnCreateVoiceConsentCompleted(
    const FVoiceConsentResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
{
    OnCompleted.Broadcast(Response, ResponseMetadata, {});
}

void UCreateVoiceConsentAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, {}, FOpenAIError{Content, true});
}

void UCreateVoiceConsentAction::SetEndpoint(OpenAI::V1::FOpenAIEndpoints& Endpoints, const FString& URL) const
{
    Endpoints.AudioVoiceConsents = URL;
}

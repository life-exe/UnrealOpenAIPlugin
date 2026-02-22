// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/OpenAIActionBase.h"

UOpenAIProvider* UOpenAIActionBase::CreateProvider()
{
    ActiveProvider = NewObject<UOpenAIProvider>();
    ActiveProvider->OnRequestError().AddUObject(this, &UOpenAIActionBase::OnRequestError);
    if (!URLOverride.IsEmpty())
    {
        OpenAI::V1::FOpenAIEndpoints Endpoints;
        SetEndpoint(Endpoints, URLOverride);
        const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
        ActiveProvider->SetAPI(API);
    }
    return ActiveProvider;
}

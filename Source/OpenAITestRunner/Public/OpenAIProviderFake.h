// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIProvider.h"
#include "OpenAIProviderFake.generated.h"

class FFakeHttpResponse : public IHttpResponse
{
public:
    FFakeHttpResponse(const FString& ResponseStr) : ReponseData(ResponseStr) {}
    virtual int32 GetResponseCode() const override { return static_cast<int32>(EHttpResponseCodes::Ok); }
    virtual FString GetContentAsString() const override { return ReponseData; }
    virtual FString GetURL() const override { return FString(); }
    virtual FString GetURLParameter(const FString& ParameterName) const override { return FString(); }
    virtual FString GetHeader(const FString& HeaderName) const override { return FString(); }
    virtual TArray<FString> GetAllHeaders() const override { return TArray<FString>(); }
    virtual FString GetContentType() const override { return FString(); }
    virtual uint64 GetContentLength() const override { return uint64(); }
    virtual const TArray<uint8>& GetContent() const override
    {
        static TArray<uint8> None;
        return None;
    }
    virtual const FString& GetEffectiveURL() const override { return EffectiveURL; }
    virtual EHttpRequestStatus::Type GetStatus() const override { return EHttpRequestStatus::Type::Succeeded; }
    virtual EHttpFailureReason GetFailureReason() const override { return EHttpFailureReason::None; }

private:
    FString ReponseData;
    FString EffectiveURL;
};

class FFakeHttpRequest : public IHttpRequest
{
public:
    FFakeHttpRequest(const FString& ResponseStr) : ReponseData(ResponseStr) {}
    virtual FString GetURL() const override { return FString(); }
    virtual FHttpRequestWillRetryDelegate& OnRequestWillRetry() override { return HttpRequestWillRetryDelegate; }
    virtual FString GetURLParameter(const FString& ParameterName) const override { return FString(); }
    virtual FString GetHeader(const FString& HeaderName) const override { return FString(); }
    virtual TArray<FString> GetAllHeaders() const override { return TArray<FString>(); }
    virtual FString GetContentType() const override { return FString(); }
    virtual uint64 GetContentLength() const override { return uint64(); }
    virtual const TArray<uint8>& GetContent() const override
    {
        static TArray<uint8> None;
        return None;
    }
    virtual FString GetVerb() const override { return FString{}; }
    virtual void SetVerb(const FString& Verb) override {}
    virtual void SetURL(const FString& URL) override {}
    virtual void SetContent(const TArray<uint8>& ContentPayload) override {}
    virtual void SetContent(TArray<uint8>&& ContentPayload) override {}
    virtual void SetContentAsString(const FString& ContentString) override {}
    virtual bool SetContentAsStreamedFile(const FString& Filename) override { return false; }
    virtual bool SetContentFromStream(TSharedRef<FArchive, ESPMode::ThreadSafe> Stream) override { return false; }
    virtual void SetHeader(const FString& HeaderName, const FString& HeaderValue) override {}
    virtual void AppendToHeader(const FString& HeaderName, const FString& AdditionalHeaderValue) override {}
    virtual void SetTimeout(float InTimeoutSecs) override {}
    virtual void ClearTimeout() override {}
    virtual TOptional<float> GetTimeout() const override { return TOptional<float>(); }
    virtual bool ProcessRequest() override
    {
        HttpRequestCompleteDelegate.Execute(SharedThis(this), GetResponse(), true);
        return true;
    }
    virtual FHttpRequestCompleteDelegate& OnProcessRequestComplete() override { return HttpRequestCompleteDelegate; }
    virtual FHttpRequestProgressDelegate& OnRequestProgress() override { return HttpRequestProgressDelegate; }
    virtual FHttpRequestHeaderReceivedDelegate& OnHeaderReceived() override { return HttpHeaderReceivedDelegate; }
    virtual FHttpRequestProgressDelegate64& OnRequestProgress64() override { return HttpRequestProgressDelegate64; }
    virtual FHttpRequestStatusCodeReceivedDelegate& OnStatusCodeReceived() override { return HttpRequestStatusCodeReceivedDelegate; }
    virtual void CancelRequest() override {}
    virtual EHttpRequestStatus::Type GetStatus() const override { return EHttpRequestStatus::Type::NotStarted; }
    virtual const FHttpResponsePtr GetResponse() const override { return MakeShareable(new FFakeHttpResponse(ReponseData)); }
    virtual void Tick(float DeltaSeconds) override {}
    virtual float GetElapsedTime() const override { return float{}; }
    virtual bool SetResponseBodyReceiveStream(TSharedRef<FArchive> Stream) override { return false; }
    virtual void SetDelegateThreadPolicy(EHttpRequestDelegateThreadPolicy InThreadPolicy) override {}
    virtual EHttpRequestDelegateThreadPolicy GetDelegateThreadPolicy() const override
    {
        return EHttpRequestDelegateThreadPolicy::CompleteOnGameThread;
    }
    virtual const FString& GetEffectiveURL() const override { return EffectiveURL; }
    virtual EHttpFailureReason GetFailureReason() const override { return EHttpFailureReason::None; }
    virtual void ProcessRequestUntilComplete() override {}
    virtual void SetActivityTimeout(float InTimeoutSecs) override {}

public:
    FHttpRequestProgressDelegate HttpRequestProgressDelegate;
    FHttpRequestProgressDelegate64 HttpRequestProgressDelegate64;
    FHttpRequestCompleteDelegate HttpRequestCompleteDelegate;
    FHttpRequestHeaderReceivedDelegate HttpHeaderReceivedDelegate;
    FHttpRequestWillRetryDelegate HttpRequestWillRetryDelegate;
    FHttpRequestStatusCodeReceivedDelegate HttpRequestStatusCodeReceivedDelegate;

private:
    FString ReponseData;
    FString EffectiveURL;
};

UCLASS()
class OPENAITESTRUNNER_API UOpenAIProviderFake : public UOpenAIProvider
{
    GENERATED_BODY()

public:
    void SetResponse(const FString& ResponseStr) { ReponseData = ResponseStr; };

private:
    FString ReponseData;

    virtual TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateRequest() const override
    {  //
        return MakeShareable(new FFakeHttpRequest(ReponseData));
    }
};

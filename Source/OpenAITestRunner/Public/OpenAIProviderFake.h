// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIProvider.h"
#include "OpenAIProviderFake.generated.h"

struct FFakeResponseData
{
    FString Content;
    int32 ResponseCode{static_cast<int32>(EHttpResponseCodes::Ok)};
    FString ContentType;
};

class FFakeHttpResponse : public IHttpResponse
{
public:
    FFakeHttpResponse(const FFakeResponseData& InData)
        : ReponseData(InData.Content), ResponseCode(InData.ResponseCode), ContentType(InData.ContentType)
    {
        const FTCHARToUTF8 Converter(*ReponseData);
        Bytes.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
    }
    virtual int32 GetResponseCode() const override { return ResponseCode; }
    virtual FString GetContentAsString() const override { return ReponseData; }
    virtual const FString& GetURL() const override { return _URL; }
    virtual FString GetURLParameter(const FString& ParameterName) const override { return FString(); }
    virtual FString GetHeader(const FString& HeaderName) const override { return FString(); }
    virtual TArray<FString> GetAllHeaders() const override { return TArray<FString>(); }
    virtual FString GetContentType() const override { return ContentType; }
    virtual uint64 GetContentLength() const override { return Bytes.Num(); }
    virtual const TArray<uint8>& GetContent() const override { return Bytes; }
    virtual TArray<uint8> TakeContent() override { return Bytes; }
    virtual const FString& GetEffectiveURL() const override { return EffectiveURL; }
    virtual EHttpRequestStatus::Type GetStatus() const override { return EHttpRequestStatus::Type::Succeeded; }
    virtual EHttpFailureReason GetFailureReason() const override { return EHttpFailureReason::None; }
    virtual FUtf8StringView GetContentAsUtf8StringView() const override { return FUtf8StringView{}; }

private:
    FString ReponseData;
    int32 ResponseCode;
    FString ContentType;
    TArray<uint8> Bytes;
    FString EffectiveURL;
    FString _URL;
};

class FFakeHttpRequest : public IHttpRequest
{
public:
    FFakeHttpRequest(const FFakeResponseData& InResponseData) : ResponseData(InResponseData) {}
    virtual const FString& GetURL() const override { return _URL; }
    virtual FHttpRequestWillRetryDelegate& OnRequestWillRetry() override { return HttpRequestWillRetryDelegate; }
    virtual FString GetURLParameter(const FString& ParameterName) const override { return FString(); }
    virtual FString GetHeader(const FString& HeaderName) const override
    {
        const FString* Value = Headers.Find(HeaderName);
        return Value ? *Value : FString();
    }
    virtual TArray<FString> GetAllHeaders() const override { return TArray<FString>(); }
    virtual FString GetContentType() const override { return GetHeader(TEXT("Content-Type")); }
    virtual uint64 GetContentLength() const override { return Content.Num(); }
    virtual const TArray<uint8>& GetContent() const override { return Content; }
    virtual FString GetVerb() const override { return Verb; }
    virtual void SetVerb(const FString& InVerb) override { Verb = InVerb; }
    virtual void SetURL(const FString& URL) override { _URL = URL; }
    virtual void SetContent(const TArray<uint8>& ContentPayload) override { Content = ContentPayload; }
    virtual void SetContent(TArray<uint8>&& ContentPayload) override { Content = MoveTemp(ContentPayload); }
    virtual void SetContentAsString(const FString& ContentString) override
    {
        const FTCHARToUTF8 Converter(*ContentString);
        Content = TArray<uint8>(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
    }
    virtual bool SetContentAsStreamedFile(const FString& Filename) override { return false; }
    virtual bool SetContentFromStream(TSharedRef<FArchive, ESPMode::ThreadSafe> Stream) override { return false; }
    virtual void SetHeader(const FString& HeaderName, const FString& HeaderValue) override { Headers.Add(HeaderName, HeaderValue); }
    virtual void AppendToHeader(const FString& HeaderName, const FString& AdditionalHeaderValue) override {}
    virtual void SetTimeout(float InTimeoutSecs) override {}
    virtual void ClearTimeout() override {}
    virtual TOptional<float> GetTimeout() const override { return TOptional<float>(); }
    virtual bool ProcessRequest() override
    {
        bProcessed = true;
        HttpRequestCompleteDelegate.Execute(SharedThis(this), GetResponse(), true);
        return true;
    }
    virtual FHttpRequestCompleteDelegate& OnProcessRequestComplete() override { return HttpRequestCompleteDelegate; }
    virtual FHttpRequestHeaderReceivedDelegate& OnHeaderReceived() override { return HttpHeaderReceivedDelegate; }
    virtual FHttpRequestProgressDelegate64& OnRequestProgress64() override { return HttpRequestProgressDelegate64; }
    virtual FHttpRequestStatusCodeReceivedDelegate& OnStatusCodeReceived() override { return HttpRequestStatusCodeReceivedDelegate; }
    virtual void CancelRequest() override {}
    virtual EHttpRequestStatus::Type GetStatus() const override { return EHttpRequestStatus::Type::NotStarted; }
    virtual const FHttpResponsePtr GetResponse() const override { return MakeShareable(new FFakeHttpResponse(ResponseData)); }
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
    virtual FString GetOption(const FName Option) const override { return {}; }
    virtual void SetOption(const FName Option, const FString& OptionValue) override {}
    virtual void ResetTimeoutStatus() override {}
    virtual void SetPriority(EHttpRequestPriority InPriority) override { Priority = InPriority; }
    virtual EHttpRequestPriority GetPriority() const override { return Priority; }

public:
    FHttpRequestProgressDelegate HttpRequestProgressDelegate;
    FHttpRequestProgressDelegate64 HttpRequestProgressDelegate64;
    FHttpRequestCompleteDelegate HttpRequestCompleteDelegate;
    FHttpRequestHeaderReceivedDelegate HttpHeaderReceivedDelegate;
    FHttpRequestWillRetryDelegate HttpRequestWillRetryDelegate;
    FHttpRequestStatusCodeReceivedDelegate HttpRequestStatusCodeReceivedDelegate;

    bool bProcessed{false};

private:
    FFakeResponseData ResponseData;
    TMap<FString, FString> Headers;
    TArray<uint8> Content;
    FString Verb;
    FString EffectiveURL;
    FString _URL;
    EHttpRequestPriority Priority = EHttpRequestPriority::Normal;
};

UCLASS()
class OPENAITESTRUNNER_API UOpenAIProviderFake : public UOpenAIProvider
{
    GENERATED_BODY()

public:
    void SetResponse(
        const FString& ResponseStr, int32 ResponseCode = static_cast<int32>(EHttpResponseCodes::Ok), const FString& ContentType = {})
    {
        ResponseData = {ResponseStr, ResponseCode, ContentType};
    };

    // the last created request: headers, content and whether it was sent
    TSharedPtr<FFakeHttpRequest, ESPMode::ThreadSafe> GetLastRequest() const { return LastRequest; }

private:
    FFakeResponseData ResponseData;
    mutable TSharedPtr<FFakeHttpRequest, ESPMode::ThreadSafe> LastRequest;

    virtual TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateRequest() const override
    {
        TSharedRef<FFakeHttpRequest, ESPMode::ThreadSafe> Request = MakeShareable(new FFakeHttpRequest(ResponseData));
        LastRequest = Request;
        return Request;
    }
};

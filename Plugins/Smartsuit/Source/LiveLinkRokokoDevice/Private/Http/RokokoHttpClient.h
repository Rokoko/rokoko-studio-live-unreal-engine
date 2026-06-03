// Copyright Rokoko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

class FJsonObject;

struct FRokokoHttpRequestConfig
{
    FString Host = TEXT("127.0.0.1");
    int32 Port = 14053;
    FString ApiKey = TEXT("1234");
    FString ApiVersion = TEXT("v1");
    float TimeoutSeconds = 3.0f;
};

struct FRokokoHttpResponse
{
    bool bSucceeded = false;
    int32 HttpStatusCode = 0;
    FString ErrorText;
    FString ResponseText;
    TSharedPtr<FJsonObject> JsonBody;
};

class FRokokoHttpClient : public TSharedFromThis<FRokokoHttpClient>
{
public:
    using FResponseCallback = TFunction<void(FRokokoHttpResponse&&)>;

    FRokokoHttpClient() = default;
    ~FRokokoHttpClient();

    bool PostCommand(
        const FRokokoHttpRequestConfig& InConfig,
        const FString& InCommandPath,
        const TSharedPtr<FJsonObject>& InPayload,
        FResponseCallback&& InCallback);

    void CancelAllRequests();

private:
    static FString BuildUrl(const FRokokoHttpRequestConfig& InConfig, const FString& InCommandPath);
    static FString BuildJsonPayload(const TSharedPtr<FJsonObject>& InPayload);

    void HandleRequestComplete(
        const FHttpRequestPtr& InRequest,
        const FHttpResponsePtr& InResponse,
        bool bWasSuccessful,
        FResponseCallback&& InCallback);

private:
    FCriticalSection ActiveRequestsMutex;
    TArray<FHttpRequestPtr> ActiveRequests;
};

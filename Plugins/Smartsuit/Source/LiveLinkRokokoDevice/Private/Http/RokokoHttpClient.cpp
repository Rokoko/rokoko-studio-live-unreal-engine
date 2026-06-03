// Copyright Rokoko. All Rights Reserved.

#include "Http/RokokoHttpClient.h"

#include "Dom/JsonObject.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

FRokokoHttpClient::~FRokokoHttpClient()
{
    CancelAllRequests();
}

bool FRokokoHttpClient::PostCommand(
    const FRokokoHttpRequestConfig& InConfig,
    const FString& InCommandPath,
    const TSharedPtr<FJsonObject>& InPayload,
    FResponseCallback&& InCallback)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(BuildUrl(InConfig, InCommandPath));
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
    HttpRequest->SetTimeout(InConfig.TimeoutSeconds);
    HttpRequest->SetContentAsString(BuildJsonPayload(InPayload));

    {
        FScopeLock ScopeLock(&ActiveRequestsMutex);
        ActiveRequests.Add(HttpRequest);
    }

    TWeakPtr<FRokokoHttpClient> WeakThis = AsShared();
    HttpRequest->OnProcessRequestComplete().BindLambda(
        [WeakThis, Callback = MoveTemp(InCallback)](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) mutable
        {
            TSharedPtr<FRokokoHttpClient> This = WeakThis.Pin();
            if (!This.IsValid())
            {
                return;
            }

            This->HandleRequestComplete(Request, Response, bWasSuccessful, MoveTemp(Callback));
        });

    return HttpRequest->ProcessRequest();
}

void FRokokoHttpClient::CancelAllRequests()
{
    TArray<FHttpRequestPtr> RequestsToCancel;

    {
        FScopeLock ScopeLock(&ActiveRequestsMutex);
        RequestsToCancel = ActiveRequests;
        ActiveRequests.Empty();
    }

    for (const FHttpRequestPtr& Request : RequestsToCancel)
    {
        if (Request.IsValid())
        {
            Request->OnProcessRequestComplete().Unbind();
            Request->CancelRequest();
        }
    }
}

FString FRokokoHttpClient::BuildUrl(const FRokokoHttpRequestConfig& InConfig, const FString& InCommandPath)
{
    return FString::Printf(
        TEXT("http://%s:%d/%s/%s/%s"),
        *InConfig.Host,
        InConfig.Port,
        *InConfig.ApiVersion,
        *InConfig.ApiKey,
        *InCommandPath);
}

FString FRokokoHttpClient::BuildJsonPayload(const TSharedPtr<FJsonObject>& InPayload)
{
    TSharedRef<FJsonObject> JsonObject = InPayload.IsValid() ? InPayload.ToSharedRef() : MakeShared<FJsonObject>();

    FString JsonPayload;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonPayload);
    FJsonSerializer::Serialize(JsonObject, JsonWriter);
    return JsonPayload;
}

void FRokokoHttpClient::HandleRequestComplete(
    const FHttpRequestPtr& InRequest,
    const FHttpResponsePtr& InResponse,
    bool bWasSuccessful,
    FResponseCallback&& InCallback)
{
    {
        FScopeLock ScopeLock(&ActiveRequestsMutex);
        ActiveRequests.RemoveSingleSwap(InRequest);
    }

    FRokokoHttpResponse Result;
    Result.bSucceeded = bWasSuccessful && InResponse.IsValid();

    if (InResponse.IsValid())
    {
        Result.HttpStatusCode = InResponse->GetResponseCode();
        Result.ResponseText = InResponse->GetContentAsString();
    }

    if (!InResponse.IsValid())
    {
        Result.bSucceeded = false;
        Result.ErrorText = TEXT("No response from Rokoko Studio.");
    }
    else if (!bWasSuccessful)
    {
        Result.bSucceeded = false;
        Result.ErrorText = TEXT("Request failed. Check network connectivity and Studio status.");
    }
    else if (Result.HttpStatusCode < 200 || Result.HttpStatusCode >= 300)
    {
        Result.bSucceeded = false;
        Result.ErrorText = FString::Printf(TEXT("HTTP %d: %s"), Result.HttpStatusCode, *Result.ResponseText);
    }

    if (!Result.ResponseText.IsEmpty())
    {
        TSharedPtr<FJsonObject> RootJson;
        const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Result.ResponseText);
        if (FJsonSerializer::Deserialize(Reader, RootJson) && RootJson.IsValid())
        {
            Result.JsonBody = RootJson;
        }
    }

    InCallback(MoveTemp(Result));
}

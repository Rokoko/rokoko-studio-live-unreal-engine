// Copyright 2019 Rokoko Electronics. All Rights Reserved.


#include "RokokoStudioCommandAPI.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

void URokokoStudioCommandAPI::Calibrate(const FString& SmartSuitName, int32 CountdownDelay)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("smartsuit_name", SmartSuitName);
	JsonObject->SetNumberField("coundtown_delay", CountdownDelay);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FString IPAddress = "127.0.0.1";
	FString Port = "14053";
	FString APIKey = "1234";
	FString URLPath = "http://" + IPAddress + ":" + Port + "/v1/" + APIKey + "/calibrate";

	FString TrimmedUrl = URLPath;
	TrimmedUrl.Trim();
	TrimmedUrl.TrimTrailing();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::StartRecording(const FString& FileName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("filename", FileName);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FString IPAddress = "127.0.0.1";
	FString Port = "14053";
	FString APIKey = "1234";
	FString URLPath = "http://" + IPAddress + ":" + Port + "/v1/" + APIKey + "/recording/start";

	FString TrimmedUrl = URLPath;
	TrimmedUrl.Trim();
	TrimmedUrl.TrimTrailing();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::StopRecording()
{
	FString IPAddress = "127.0.0.1";
	FString Port = "14053";
	FString APIKey = "1234";
	FString URLPath = "http://" + IPAddress + ":" + Port + "/v1/" + APIKey + "/recording/stop";

	FString TrimmedUrl = URLPath;
	TrimmedUrl.Trim();
	TrimmedUrl.TrimTrailing();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	int32 ResponseCode = HttpResponse->GetResponseCode();
	FString ResponseString = HttpResponse->GetContentAsString();

	OnCompletedRequest.Broadcast(ResponseCode, ResponseString, bSucceeded);
}

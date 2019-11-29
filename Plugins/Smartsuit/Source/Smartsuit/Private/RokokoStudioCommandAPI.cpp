// Copyright 2019 Rokoko Electronics. All Rights Reserved.


#include "RokokoStudioCommandAPI.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

URokokoStudioCommandAPI::URokokoStudioCommandAPI()
{
	Default_SmartSuitName = "H23";
	Default_IPInfo.IPAddress = "127.0.0.1";
	Default_IPInfo.Port = "14053";
	Default_IPInfo.APIKey = "1234";
}

void URokokoStudioCommandAPI::Restart(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	//JsonObject->SetStringField("smartsuit_name", SmartSuitName);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FString URLPath = "http://" + IPInfo.IPAddress + ":" + IPInfo.Port + "/v1/" + IPInfo.APIKey + "/restart";

	FString TrimmedUrl = URLPath;
	TrimmedUrl.TrimStartAndEndInline();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::Calibrate(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName, int32 CountdownDelay)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	//JsonObject->SetStringField("smartsuit_name", SmartSuitName);
	JsonObject->SetNumberField("coundtown_delay", CountdownDelay);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FString URLPath = "http://" + IPInfo.IPAddress + ":" + IPInfo.Port + "/v1/" + IPInfo.APIKey + "/calibrate";

	FString TrimmedUrl = URLPath;
	TrimmedUrl.TrimStartAndEndInline();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::StartRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& FileName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	//JsonObject->SetStringField("filename", FileName);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FString URLPath = "http://" + IPInfo.IPAddress + ":" + IPInfo.Port + "/v1/" + IPInfo.APIKey + "/recording/start";

	FString TrimmedUrl = URLPath;
	TrimmedUrl.TrimStartAndEndInline();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::StopRecording(const FRokokoCommandAPI_IPInfo& IPInfo)
{
	FString URLPath = "http://" + IPInfo.IPAddress + ":" + IPInfo.Port + "/v1/" + IPInfo.APIKey + "/recording/stop";

	FString TrimmedUrl = URLPath;
	TrimmedUrl.TrimStartAndEndInline();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

PRAGMA_DISABLE_OPTIMIZATION
void URokokoStudioCommandAPI::SaveConfigFile(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitname)
{
	Default_IPInfo = IPInfo;
	Default_SmartSuitName = SmartSuitname;
	SaveConfig();
}
PRAGMA_ENABLE_OPTIMIZATION

void URokokoStudioCommandAPI::OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	int32 ResponseCode = HttpResponse->GetResponseCode();
	FString ResponseString = HttpResponse->GetContentAsString();

	OnCompletedRequest.Broadcast(ResponseCode, ResponseString, bSucceeded);
}

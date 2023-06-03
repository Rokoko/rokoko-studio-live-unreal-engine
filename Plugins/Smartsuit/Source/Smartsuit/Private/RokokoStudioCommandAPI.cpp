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

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
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

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
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

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
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

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::Tracker(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& DeviceId, const FString& BoneName, const FTransform& transform, float timeoutTime, bool isQueryOnly)
{
	const double WORLD_SCALE{ 0.01 };

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	TSharedPtr<FJsonObject> positionJsonObject = MakeShareable(new FJsonObject);
	// convert UE coord system into target Studio coord system
	positionJsonObject->SetNumberField("X", WORLD_SCALE * transform.GetLocation().X);
	positionJsonObject->SetNumberField("Y", WORLD_SCALE * transform.GetLocation().Z);
	positionJsonObject->SetNumberField("Z", -WORLD_SCALE * transform.GetLocation().Y);

	TSharedPtr<FJsonObject> rotationJsonObject = MakeShareable(new FJsonObject);
	rotationJsonObject->SetNumberField("X", transform.GetRotation().X);
	rotationJsonObject->SetNumberField("Y", transform.GetRotation().Y);
	rotationJsonObject->SetNumberField("Z", transform.GetRotation().Z);
	rotationJsonObject->SetNumberField("W", transform.GetRotation().W);
	
	JsonObject->SetStringField("device_id", DeviceId);
	JsonObject->SetStringField("bone_attached", BoneName);
	JsonObject->SetObjectField("position", positionJsonObject);
	JsonObject->SetObjectField("rotation", rotationJsonObject);
	JsonObject->SetBoolField("is_query_only", isQueryOnly);
	JsonObject->SetNumberField("timeout", timeoutTime);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FString URLPath = FString::Printf(TEXT("http://%s:%s/v2/%s/tracker"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);
	
	FString TrimmedUrl = URLPath;
	TrimmedUrl.TrimStartAndEndInline();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnTrackerRequestComplete);
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

void URokokoStudioCommandAPI::OnTrackerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	const double WORLD_SCALE{ 100.0 };

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject)) 
	{
		auto arr = JsonObject->GetArrayField("parameters");
		
		if (!arr.IsEmpty())
		{
			auto objectPosition = arr[0]->AsObject();
			auto objectRotation = arr[1]->AsObject();

			FVector position(WORLD_SCALE * objectPosition->GetNumberField("X"), 
				-WORLD_SCALE * objectPosition->GetNumberField("Z"),
				WORLD_SCALE * objectPosition->GetNumberField("Y"));
			FQuat rotation(objectRotation->GetNumberField("X"), objectRotation->GetNumberField("Y"), objectRotation->GetNumberField("Z"),
				objectRotation->GetNumberField("W"));

			OnTrackerRequest.Broadcast(position, rotation);
		}
	}
}

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

void URokokoStudioCommandAPI::Info(const FRokokoCommandAPI_IPInfo& IPInfo, bool ShouldIncludeDevices, bool ShouldIncludeClips)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetBoolField("devices_info", ShouldIncludeDevices);
	JsonObject->SetBoolField("clips_info", ShouldIncludeClips);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v2/%s/info"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnInfoRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::Restart(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	//JsonObject->SetStringField("smartsuit_name", SmartSuitName);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v1/%s/restart"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::ResetActor(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& ActorName)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("device_id", ActorName);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v2/%s/resetactor"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::Calibrate(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_CalibrateInput& Params)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("device_id", Params.DeviceID);
	JsonObject->SetNumberField("coundtown_delay", Params.CountdownDelay);
	JsonObject->SetBoolField("skip_suit", Params.ShouldSkipSuit);
	JsonObject->SetNumberField("skip_gloves", Params.ShouldSkipGloves);
	JsonObject->SetNumberField("use_custom_pose", Params.UseCustomPose);
	JsonObject->SetStringField("pose", Params.CustomPoseName);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v1/%s/calibrate"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::StartRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FString FileName, const FTimecode StartTime)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("filename", FileName);
	JsonObject->SetStringField("time", StartTime.ToString());
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v1/%s/recording/start"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::StopRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FTimecode EndTime, bool ShouldBackToLive)
{
	// prepare command options in json form
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("time", EndTime.ToString()); // time is SMPTE format
	JsonObject->SetBoolField("back_to_live", ShouldBackToLive);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v1/%s/recording/stop"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
	HttpRequest->ProcessRequest();
}

void URokokoStudioCommandAPI::Tracker(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_TrackerInput& Params)
{
	const double WORLD_SCALE{ 0.01 };

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	TSharedPtr<FJsonObject> PositionJsonObject = MakeShareable(new FJsonObject);
	// convert UE coord system into target Studio coord system
	const FVector Location{ Params.Transform.GetLocation() };
	PositionJsonObject->SetNumberField("X", WORLD_SCALE * Location.X);
	PositionJsonObject->SetNumberField("Y", WORLD_SCALE * Location.Z);
	PositionJsonObject->SetNumberField("Z", -WORLD_SCALE * Location.Y);

	// TODO: do we need to convert rotation into Studio coord system ?
	TSharedPtr<FJsonObject> RotationJsonObject = MakeShareable(new FJsonObject);
	const FQuat Rotation{ Params.Transform.GetRotation() };
	RotationJsonObject->SetNumberField("X", Rotation.X);
	RotationJsonObject->SetNumberField("Y", Rotation.Y);
	RotationJsonObject->SetNumberField("Z", Rotation.Z);
	RotationJsonObject->SetNumberField("W", Rotation.W);
	
	JsonObject->SetStringField("device_id", Params.DeviceID);
	JsonObject->SetStringField("bone_attached", Params.BoneName);
	JsonObject->SetObjectField("position", PositionJsonObject);
	JsonObject->SetObjectField("rotation", RotationJsonObject);
	JsonObject->SetBoolField("is_query_only", Params.ShouldQueryOnly);
	JsonObject->SetNumberField("timeout", Params.TimeoutTime);
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v2/%s/tracker"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
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

void URokokoStudioCommandAPI::OnInfoRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto& Arr = JsonObject->GetArrayField("parameters");

		if (!Arr.IsEmpty())
		{
			int Count = static_cast<int>(Arr[0]->AsNumber());

			TArray<FString> Strings1;
			TArray<FString> Strings2;

			for (int i = 0; i < Count; ++i)
			{
				Strings1.Add(Arr[i + 1]->AsString());
			}

			if (Arr.Num() > Count)
			{
				int StartIndex = Count + 1;
				// read next array
				Count = static_cast<int>(Arr[StartIndex]->AsNumber());

				for (int i = 0; i < Count; ++i)
				{
					Strings2.Add(Arr[i + StartIndex + 1]->AsString());
				}
			}

			OnInfoRequest.Broadcast(Strings1, Strings2);
		}
	}
}

void URokokoStudioCommandAPI::OnTrackerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	const double WORLD_SCALE{ 100.0 };

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject)) 
	{
		auto& Arr = JsonObject->GetArrayField("parameters");
		
		if (!Arr.IsEmpty())
		{
			auto& ObjectPosition = Arr[0]->AsObject();
			auto& ObjectRotation = Arr[1]->AsObject();

			FVector Position(WORLD_SCALE * ObjectPosition->GetNumberField("X"), 
				-WORLD_SCALE * ObjectPosition->GetNumberField("Z"),
				WORLD_SCALE * ObjectPosition->GetNumberField("Y"));
			// TODO: do we need to convert rotation to UE coord system ?!
			FQuat Rotation(ObjectRotation->GetNumberField("X"), ObjectRotation->GetNumberField("Y"), ObjectRotation->GetNumberField("Z"),
				ObjectRotation->GetNumberField("W"));

			OnTrackerRequest.Broadcast(Position, Rotation);
		}
	}
}

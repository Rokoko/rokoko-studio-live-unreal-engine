// Copyright 2019 Rokoko Electronics. All Rights Reserved.


#include "RokokoStudioCommandAPI.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "RokokoRemote.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Runtime/Core/Public/Misc/Timecode.h"


void URokokoStudioCommandAPI::Info(const FRokokoCommandAPI_IPInfo& IPInfo, bool ShouldIncludeDevices, bool ShouldIncludeClips, bool ShouldIncludeActors, bool ShouldIncludeCharacters)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetBoolField("devices_info", ShouldIncludeDevices);
	JsonObject->SetBoolField("clips_info", ShouldIncludeClips);
	JsonObject->SetBoolField("actors_info", ShouldIncludeActors);
	JsonObject->SetBoolField("characters_info", ShouldIncludeCharacters);
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

	ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor();
	if (remote)
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnInfoRequestComplete);
	}

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
	//HttpRequest->OnProcessRequestComplete().BindUObject(this, &URokokoStudioCommandAPI::OnProcessRequestComplete);
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

	ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor();
	if (remote)
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnProcessRequestComplete);
	}
	
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
	
	ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor();
	if (remote)
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnProcessRequestComplete);
	}
	
	HttpRequest->ProcessRequest();
}


void URokokoStudioCommandAPI::Playback(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_PlaybackInput& Params)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	JsonObject->SetBoolField("is_playing", Params.IsPlaying);
	JsonObject->SetNumberField("current_time", Params.CurrentTime);
	JsonObject->SetNumberField("playback_speed", Params.PlaybackSpeed);
	JsonObject->SetNumberField("change_flag", Params.ChangeFlags);

	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v2/%s/playback"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);

	if (ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor())
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnPlaybackRequestComplete);
	}

	HttpRequest->ProcessRequest();
}


void URokokoStudioCommandAPI::Livestream(const FRokokoCommandAPI_IPInfo& IPInfo, bool ShouldLivestream)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetBoolField("enabled", ShouldLivestream);

	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	const FString URLPath = FString::Printf(TEXT("http://%s:%s/v2/%s/livestream"), *IPInfo.IPAddress, *IPInfo.Port, *IPInfo.APIKey);

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	HttpRequest->SetURL(URLPath);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetContentAsString(JsonString);

	if (ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor())
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnProcessRequestComplete);
	}

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
	ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor();
	if (remote)
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnProcessRequestComplete);
	}
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
	ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor();
	if (remote)
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnProcessRequestComplete);
	}
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
	
	if (ARokokoRemote* remote = ARokokoRemote::GetFirstAvailableActor())
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(remote, &ARokokoRemote::OnTrackerRequestComplete);
	}
	HttpRequest->ProcessRequest();
}

FRokokoCommandAPI_IPInfo URokokoStudioCommandAPI::GetDefaultIPInfo()
{
	FRokokoCommandAPI_IPInfo DefaultIPInfo;
	DefaultIPInfo.IPAddress = "127.0.0.1";
	DefaultIPInfo.Port = "14053";
	DefaultIPInfo.APIKey = "1234";
	return DefaultIPInfo;
}


/*
//PRAGMA_DISABLE_OPTIMIZATION
void URokokoStudioCommandAPI::SaveConfigFile(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitname)
{
	Default_IPInfo = IPInfo;
	Default_SmartSuitName = SmartSuitname;
	SaveConfig();
}
//PRAGMA_ENABLE_OPTIMIZATION
*/


// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "RokokoStudioCommandAPI.generated.h"

USTRUCT(BlueprintType)
struct FRokokoCommandAPI_IPInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Command API")
	FString IPAddress;

	UPROPERTY(BlueprintReadWrite, Category="Command API")
	FString Port;

	UPROPERTY(BlueprintReadWrite, Category="Command API")
	FString APIKey;
};

USTRUCT(BlueprintType)
struct FRokokoCommandAPI_CalibrateInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	FString DeviceID;

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	int32 CountdownDelay;

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	bool ShouldSkipSuit;

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	bool ShouldSkipGloves;

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	bool UseCustomPose;

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	FString CustomPoseName;
};

USTRUCT(BlueprintType)
struct FRokokoCommandAPI_TrackerInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	FString DeviceID;

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	FString BoneName;
	
	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	float TimeoutTime{ 2.f };

	UPROPERTY(BlueprintReadWrite, Category = "Command API")
	bool ShouldQueryOnly{ false };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCompletedRequest, int32, ResponseCode, const FString&, ResponseContentString, bool, bSucceeded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInfoRequest, const TArray<FString>&, Devices, const TArray<FString>&, Clips);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrackerRequest, FVector, Position, FQuat, Rotation);

/**
 * 
 */
UCLASS(Config="RokokoStudioCommandAPI",BlueprintType)
class SMARTSUIT_API URokokoStudioCommandAPI : public UObject
{
	GENERATED_BODY()
	
public:

	URokokoStudioCommandAPI();

	UFUNCTION(BlueprintCallable, Category = "Command API")
	void Info(const FRokokoCommandAPI_IPInfo& IPInfo, bool ShouldIncludeDevices, bool ShouldIncludeClips);

	UFUNCTION(BlueprintCallable, Category="Command API")
	void Restart(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName);

	UFUNCTION(BlueprintCallable, Category = "Command API")
	void ResetActor(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& ActorName);

	UFUNCTION(BlueprintCallable, Category="Command API")
	void Calibrate(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_CalibrateInput& Params);

	UFUNCTION(BlueprintCallable, Category="Command API")
	void StartRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FString FileName, const FTimecode StartTime);

	UFUNCTION(BlueprintCallable, Category="Command API")
	void StopRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FTimecode EndTime, bool ShouldBackToLive);

	UFUNCTION(BlueprintCallable, Category = "Command API")
	void Tracker(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_TrackerInput& Params);

	UPROPERTY(BlueprintAssignable, Category="Command API")
	FOnCompletedRequest OnCompletedRequest;

	UPROPERTY(BlueprintAssignable, Category = "Command API")
	FOnInfoRequest OnInfoRequest;

	UPROPERTY(BlueprintAssignable, Category = "Command API")
	FOnTrackerRequest OnTrackerRequest;

	UPROPERTY(Config, BlueprintReadOnly, Category="Command API")
	FRokokoCommandAPI_IPInfo Default_IPInfo;

	UPROPERTY(Config, BlueprintReadOnly, Category="Command API")
	FString Default_SmartSuitName;

	UFUNCTION(BlueprintCallable, Category="Command API")
	void SaveConfigFile(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitname);

	void OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnInfoRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnTrackerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};

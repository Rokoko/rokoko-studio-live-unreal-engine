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

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FString IPAddress;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FString Port;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FString APIKey;
};

USTRUCT(BlueprintType)
struct FRokokoCommandAPI_CalibrateInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FString DeviceID;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	int32 CountdownDelay;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool ShouldSkipSuit;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool ShouldSkipGloves;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool UseCustomPose;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FString CustomPoseName;
};

USTRUCT(BlueprintType)
struct FRokokoCommandAPI_TrackerInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FString DeviceID;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FString BoneName;
	
	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	float TimeoutTime{ 2.f };

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool ShouldQueryOnly{ false };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCompletedRequest, int32, ResponseCode, const FString&, ResponseContentString, bool, bSucceeded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInfoRequest, const TArray<FString>&, Devices, const TArray<FString>&, Clips);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrackerRequest, FVector, Position, FQuat, Rotation);

/**
 * Expose blueprint library of command API communication methods
 *  that could be used to calibrate, start recording, do a global position correct and etc.
 * Communication configuration is defined by FRokokoCommandAPI_IPInfo
 */
UCLASS(meta=(ScriptName = "RokokoStudioCommandAPI"))
class SMARTSUIT_API URokokoStudioCommandAPI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Request info about scene clips, input devices, actors, etc."))
	static void Info(const FRokokoCommandAPI_IPInfo& IPInfo, bool ShouldIncludeDevices, bool ShouldIncludeClips);

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Request a given actor restart operation"))
	static void Restart(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName);

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Request a given actor reset to a root position"))
	static void ResetActor(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& ActorName);

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Request a given actor calibration"))
	static void Calibrate(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_CalibrateInput& Params);

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Request a recording start"))
	static void StartRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FString FileName, const FTimecode StartTime);

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Request a recording stop"))
	static void StopRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FTimecode EndTime, bool ShouldBackToLive);

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Request a given actor global position correction"))
	static void Tracker(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_TrackerInput& Params);

	UFUNCTION(BlueprintPure, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Returns a default configuration for a command api calls"))
	static FRokokoCommandAPI_IPInfo GetDefaultIPInfo();

	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = CMDAPI_CATEGORY)
	//static void OnCompletedRequest(int32 ResponseCode, const FString& ResponseContentString, bool bSucceeded);

	//UPROPERTY(BlueprintAssignable, Category = CMDAPI_CATEGORY)
	//FOnCompletedRequest OnCompletedRequest;

	//UPROPERTY(BlueprintAssignable, Category = CMDAPI_CATEGORY)
	//FOnInfoRequest OnInfoRequest;

	//UPROPERTY(BlueprintAssignable, Category = CMDAPI_CATEGORY)
	//FOnTrackerRequest OnTrackerRequest;

	//UPROPERTY(Config, BlueprintReadOnly, Category = CMDAPI_CATEGORY)
	//FRokokoCommandAPI_IPInfo Default_IPInfo;

	//UPROPERTY(Config, BlueprintReadOnly, Category = CMDAPI_CATEGORY)
	//FString Default_SmartSuitName;

	//UFUNCTION(BlueprintCallable, Category = CMDAPI_CATEGORY)
	//void SaveConfigFile(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitname);

	
};

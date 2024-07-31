// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	int32 CountdownDelay{ 1 };

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool ShouldSkipSuit{ false };

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool ShouldSkipGloves{ false };

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool UseCustomPose{ false };

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

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EPlaybackChange : uint8
{
	NONE = 0 UMETA(Hidden),
	IsPlaying = 1 << 0,
	CurrentTime = 1 << 1,
	GotoFirstFrame = 1 << 2,
	GotoLastFrame = 1 << 3,
	PlaybackSpeed = 1 << 4,
};
ENUM_CLASS_FLAGS(EPlaybackChange);

USTRUCT(BlueprintType)
struct FRokokoCommandAPI_PlaybackInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	bool IsPlaying{ false };

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	double CurrentTime{ 0.0 };

	UPROPERTY(BlueprintReadWrite, Category = "Rokoko Command API")
	float PlaybackSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rokoko Command API", meta = (Bitmask, BitmaskEnum = EPlaybackChange))
	int32 ChangeFlags = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCompletedRequest, int32, ResponseCode, const FString&, ResponseContentString, bool, bSucceeded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnInfoRequest, const TArray<FString>&, Devices, const TArray<FString>&, Clips, const TArray<FString>&, Actors, const TArray<FString>&, Characters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrackerRequest, FVector, Position, FQuat, Rotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnPlaybackRequest, double, CurrentTime, bool, IsPlaying, double, MinTime, double, MaxTime, double, PlaybackSpeedMultiplier );

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
	static void Info(const FRokokoCommandAPI_IPInfo& IPInfo, bool ShouldIncludeDevices, bool ShouldIncludeClips, bool ShouldIncludeActors, bool ShouldIncludeCharacters);

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

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Take control over the scene timeline playback"))
	static void Playback(const FRokokoCommandAPI_IPInfo& IPInfo, const FRokokoCommandAPI_PlaybackInput& Params);

	UFUNCTION(BlueprintCallable, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Take control over the custom live stream enabled state"))
	static void Livestream(const FRokokoCommandAPI_IPInfo& IPInfo, bool ShouldLiveStream);

	UFUNCTION(BlueprintPure, Category = "Rokoko Command API", meta = (BlueprintThreadSafe, ToolTip = "Returns a default configuration for a command api calls"))
	static FRokokoCommandAPI_IPInfo GetDefaultIPInfo();

};

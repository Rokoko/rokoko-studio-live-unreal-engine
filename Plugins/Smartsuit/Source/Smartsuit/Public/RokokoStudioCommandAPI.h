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

	UPROPERTY(BlueprintReadWrite)
	FString IPAddress;

	UPROPERTY(BlueprintReadWrite)
	FString Port;

	UPROPERTY(BlueprintReadWrite)
	FString APIKey;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCompletedRequest, int32, ResponseCode, const FString&, ResponseContentString, bool, bSucceeded);

/**
 * 
 */
UCLASS(Config="RokokoStudioCommandAPI",BlueprintType)
class SMARTSUIT_API URokokoStudioCommandAPI : public UObject
{
	GENERATED_BODY()
	
public:

	URokokoStudioCommandAPI();

	UFUNCTION(BlueprintCallable)
	void Restart(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName);

	UFUNCTION(BlueprintCallable)
	void Calibrate(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName, int32 CountdownDelay=3);

	UFUNCTION(BlueprintCallable)
	void StartRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& FileName);

	UFUNCTION(BlueprintCallable)
	void StopRecording(const FRokokoCommandAPI_IPInfo& IPInfo);

	UPROPERTY(BlueprintAssignable)
	FOnCompletedRequest OnCompletedRequest;

	UPROPERTY(Config, BlueprintReadOnly)
	FRokokoCommandAPI_IPInfo Default_IPInfo;

	UPROPERTY(Config, BlueprintReadOnly)
	FString Default_SmartSuitName;

	UFUNCTION(BlueprintCallable)
	void SaveConfigFile(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitname);

	void OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};

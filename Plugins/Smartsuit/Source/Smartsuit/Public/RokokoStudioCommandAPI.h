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

	UFUNCTION(BlueprintCallable, Category="Command API")
	void Restart(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName);

	UFUNCTION(BlueprintCallable, Category="Command API")
	void Calibrate(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitName, int32 CountdownDelay=3);

	UFUNCTION(BlueprintCallable, Category="Command API")
	void StartRecording(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& FileName);

	UFUNCTION(BlueprintCallable, Category="Command API")
	void StopRecording(const FRokokoCommandAPI_IPInfo& IPInfo);

	UPROPERTY(BlueprintAssignable, Category="Command API")
	FOnCompletedRequest OnCompletedRequest;

	UPROPERTY(Config, BlueprintReadOnly, Category="Command API")
	FRokokoCommandAPI_IPInfo Default_IPInfo;

	UPROPERTY(Config, BlueprintReadOnly, Category="Command API")
	FString Default_SmartSuitName;

	UFUNCTION(BlueprintCallable, Category="Command API")
	void SaveConfigFile(const FRokokoCommandAPI_IPInfo& IPInfo, const FString& SmartSuitname);

	void OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};

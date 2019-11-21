// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "RokokoStudioCommandAPI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCompletedRequest, int32, ResponseCode, const FString&, ResponseContentString, bool, bSucceeded);

/**
 * 
 */
UCLASS(BlueprintType)
class SMARTSUIT_API URokokoStudioCommandAPI : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Calibrate(const FString& SmartSuitName, int32 CountdownDelay=3);

	UFUNCTION(BlueprintCallable)
	void StartRecording(const FString& FileName);

	UFUNCTION(BlueprintCallable)
	void StopRecording();

	UPROPERTY(BlueprintAssignable)
	FOnCompletedRequest OnCompletedRequest;

	void OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};

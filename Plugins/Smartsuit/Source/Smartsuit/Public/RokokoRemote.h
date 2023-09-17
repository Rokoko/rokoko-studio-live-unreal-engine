// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Common/UdpSocketReceiver.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "RokokoStudioCommandAPI.h"

#include "RokokoRemote.generated.h"

class FUdpSocketReceiver;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnected, const FString&, ResponseContentString);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisconnected);

UCLASS()
class SMARTSUIT_API ARokokoRemote : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARokokoRemote();

	//virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	static ARokokoRemote *GetFirstAvailableActor();

	UPROPERTY(BlueprintAssignable, Category = "Command API")
	FOnCompletedRequest OnCompletedRequest;

	UPROPERTY(BlueprintAssignable, Category = "Command API")
	FOnInfoRequest OnInfoRequest;

	UPROPERTY(BlueprintAssignable, Category = "Command API")
	FOnTrackerRequest OnTrackerRequest;

	UPROPERTY(BlueprintAssignable, Category = "Command API")
	FOnPlaybackRequest OnPlaybackRequest;

	void OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnInfoRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnTrackerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void OnPlaybackRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};

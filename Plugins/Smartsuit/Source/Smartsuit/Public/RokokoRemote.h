// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Common/UdpSocketReceiver.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include "RokokoRemote.generated.h"

class FUdpSocketReceiver;

USTRUCT()
struct FRokokoRemoteInstance /*: IValuable*/
{
public:
	GENERATED_BODY()

	FRokokoRemoteInstance() {}
	FRokokoRemoteInstance(TSharedPtr<FJsonObject> jsonObject);

	FString Serialize();

	void DisplayValues();

	FString type;
	int version;
	FString provider;
	FString faceId;
	FString deviceName;
	FString connectedTo;
	FString requestedFrom;
	FString commandKey;
	int commandPort;
	bool recording;
	float currentRecordingTime;
	int numberOfLiveSuits;
	bool commandApiOn;
	bool commandApiLicense;
	bool faceLicense;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnected, const FString&, ResponseContentString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisconnected);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default)
	FString IPAddress;

	UPROPERTY()
	FTimerHandle FTH_TestHandle;

	UPROPERTY(BlueprintAssignable)
	FOnConnected OnConnected;

	UPROPERTY(BlueprintAssignable)
	FOnDisconnected OnDisconnected;

	UFUNCTION(BlueprintPure, Category = Default)
	FString GetCommandAPIKey();

	UFUNCTION(BlueprintPure, Category = Default)
	int32 GetCommandAPIPort();

	TSharedPtr<FInternetAddr> RemoteAddr;

	FSocket* SenderSocket;

	bool StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);

	bool Sender_SendData();

	void SendData();

	bool StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);

	FSocket* ListenSocket;

	FUdpSocketReceiver* UDPReceiver;
	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	FRokokoRemoteInstance CurrentData;
};

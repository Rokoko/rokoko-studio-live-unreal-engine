// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "UdpSocketReceiver.h"
//
//#include "Dom/JsonObject.h"
//#include "Serialization/JsonWriter.h"
//#include "Serialization/JsonSerializer.h"

#include "UnrealSampleProjectGameModeBase.generated.h"

class FUdpSocketReceiver;

//UCLASS()
//class RokokoRemoteInstance /*: IValuable*/
//{
//	public string type;
//	public int version;
//	public string provider;
//	public string faceId;
//	public string deviceName;
//	public string connectedTo;
//	public string requestedFrom;
//	public string commandKey;
//	public int commandPort;
//	public bool recording;
//	public float currentRecordingTime;
//	public int numberOfLiveSuits;
//};

//USTRUCT()
//struct FRokokoRemoteInstance /*: IValuable*/
//{
//public:
//	GENERATED_BODY()
//
//	FRokokoRemoteInstance() {}
//	FRokokoRemoteInstance(TSharedPtr<FJsonObject> jsonObject);
//
//	FString Serialize();
//
//	void DisplayValues();
//
//	FString type;
//	int version;
//	FString provider;
//	FString faceId;
//	FString deviceName;
//	FString connectedTo;
//	FString requestedFrom;
//	FString commandKey;
//	int commandPort;
//	bool recording;
//	float currentRecordingTime;
//	int numberOfLiveSuits;
//	bool commandApiOn;
//	bool commandApiLicense;
//	bool faceLicense;
//};

/**
 * 
 */
UCLASS()
class UNREALSAMPLEPROJECT_API AUnrealSampleProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AUnrealSampleProjectGameModeBase();

 //   virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

 //   virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

 //   UPROPERTY()
 //   FTimerHandle FTH_TestHandle;

 //   UFUNCTION()
 //   void TestFunction();




	//TSharedPtr<FInternetAddr> RemoteAddr;

	//FSocket* SenderSocket;

	//bool StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);

	//bool Sender_SendData();





	//bool StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);

	//FSocket* ListenSocket;

	//FUdpSocketReceiver* UDPReceiver;
	//void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);


	//FRokokoRemoteInstance CurrentData;
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealSampleProjectGameModeBase.h"
#include "ArrayWriter.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "Runtime/Networking/Public/Common/UdpSocketReceiver.h"

AUnrealSampleProjectGameModeBase::AUnrealSampleProjectGameModeBase()
{
	SenderSocket = nullptr;
	ListenSocket = nullptr;

	UDPReceiver = nullptr;
}

void AUnrealSampleProjectGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	StartUDPSender("TESTSOCKETSENDER", "192.168.1.133", 55606);
	//StartUDPReceiver("TESTSOCKETLISTENER", "192.168.1.133", 55606);
	StartUDPReceiver("TESTSOCKETLISTENER", "0.0.0.0", 55606);

	GetWorld()->GetTimerManager().SetTimer(FTH_TestHandle, this, &AUnrealSampleProjectGameModeBase::TestFunction, 1.f, true);

}

void AUnrealSampleProjectGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (SenderSocket)
	{
		SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
	}

	delete UDPReceiver;
	UDPReceiver = nullptr;

	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

void AUnrealSampleProjectGameModeBase::TestFunction()
{
	Sender_SendData();
}



bool AUnrealSampleProjectGameModeBase::Sender_SendData()
{
	if (!SenderSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sender socket"));
		return false;
	}

	int32 BytesSent = 0;

	FRokokoRemoteInstance NewData;
	NewData.type = "Instance";
	NewData.version = 10;
	NewData.provider = "PROVIDERNAMEGOESHERE_Remote";
	NewData.faceId = "";
	NewData.deviceName = "DEVICENAMEGOESHERE";
	NewData.connectedTo = "";
	NewData.requestedFrom = "";
	NewData.commandKey = "";
	NewData.commandPort = 0;
	NewData.recording = false;
	NewData.currentRecordingTime = 0.f;
	NewData.numberOfLiveSuits = 1;

	FArrayWriter Writer;

	Writer << NewData;

	SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddr);

	if (BytesSent <= 0)
	{
		const FString Str = "Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sent %d bytes"), BytesSent);
	}

	return true;

}

bool AUnrealSampleProjectGameModeBase::StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAddr->SetIp(*TheIP, bIsValid);
	RemoteAddr->SetPort(ThePort);

	if (!bIsValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("IP address was not valid!"));
		return false;
	}

	SenderSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.WithBroadcast()
		;
	
	//check(SenderSocket->GetSocketType() == SOCKTYPE_Datagram);

	int32 SendSize = 2 * 1024 * 1024;
	SenderSocket->SetSendBufferSize(SendSize, SendSize);
	SenderSocket->SetReceiveBufferSize(SendSize, SendSize);

	UE_LOG(LogTemp, Log, TEXT("****UDP**** Sender Initialized Successfully!!!"));

	return true;
}

bool AUnrealSampleProjectGameModeBase::StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	UE_LOG(LogTemp, Warning, TEXT("RECEIVER INIT"));

	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);

	FIPv4Endpoint Endpoint(Addr, ThePort);

	int32 BufferSize = 2 * 1024 * 1024;

	ListenSocket = FUdpSocketBuilder(*YourChosenSocketName).AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);

	//ListenSocket = FUdpSocketBuilder(*YourChosenSocketName).AsReusable().WithBroadcast();

	if(!ListenSocket)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("ListenSocket is null!"));
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ListenSocket is valid!"));
	}

	ListenSocket->SetReceiveBufferSize(BufferSize, BufferSize);

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UDPReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
	UDPReceiver->OnDataReceived().BindUObject(this, &ThisClass::Recv);

	return true;
}


void AUnrealSampleProjectGameModeBase::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	UE_LOG(LogTemp, Warning, TEXT("Received bytes: %d"), ArrayReaderPtr->Num());

	FRokokoRemoteInstance Data;
	*ArrayReaderPtr << Data;


}

PRAGMA_DISABLE_OPTIMIZATION
FArchive& operator<<(FArchive& Ar, FRokokoRemoteInstance& TheStruct)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("type"), TheStruct.type);
	JsonObject->SetNumberField(TEXT("version"), TheStruct.version);
	JsonObject->SetStringField(TEXT("provider"), TheStruct.provider);
	JsonObject->SetStringField(TEXT("faceId"), TheStruct.faceId);
	JsonObject->SetStringField(TEXT("deviceName"), TheStruct.deviceName);
	JsonObject->SetStringField(TEXT("connectedTo"), TheStruct.connectedTo);
	JsonObject->SetStringField(TEXT("requestedFrom"), TheStruct.requestedFrom);
	JsonObject->SetStringField(TEXT("commandKey"), TheStruct.commandKey);
	JsonObject->SetNumberField(TEXT("commandPort"), TheStruct.commandPort);
	JsonObject->SetBoolField(TEXT("recording"), TheStruct.recording);
	JsonObject->SetNumberField(TEXT("currentRecordingTime"), TheStruct.currentRecordingTime);
	JsonObject->SetNumberField(TEXT("numberOfLiveSuits"), TheStruct.numberOfLiveSuits);
	JsonObject->SetBoolField(TEXT("commandApiOn"), TheStruct.commandApiOn);
	JsonObject->SetBoolField(TEXT("commandApiLicense"), TheStruct.commandApiLicense);
	JsonObject->SetBoolField(TEXT("faceLicense"), TheStruct.faceLicense);

	FString OutputString = "";

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Ar << OutputString;

	return Ar;
}
PRAGMA_ENABLE_OPTIMIZATION
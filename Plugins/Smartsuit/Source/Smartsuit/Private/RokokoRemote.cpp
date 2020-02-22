// Fill out your copyright notice in the Description page of Project Settings.


#include "RokokoRemote.h"
#include "ArrayWriter.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "Runtime/Networking/Public/Common/UdpSocketReceiver.h"
#include "Async/Async.h"

ARokokoRemote::ARokokoRemote()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SenderSocket = nullptr;
	ListenSocket = nullptr;

	UDPReceiver = nullptr;

	IPAddress = "192.168.1.133";
}

void ARokokoRemote::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentData.type = "Instance";
	CurrentData.version = 1;
	CurrentData.provider = "PROVIDERNAME";
	CurrentData.faceId = "KQ452";
	CurrentData.deviceName = FPlatformProcess::ComputerName();//"DEVICENAME";
	CurrentData.connectedTo = "";
	CurrentData.requestedFrom = "";
	CurrentData.commandKey = "";
	CurrentData.commandPort = 0;
	CurrentData.recording = false;
	CurrentData.currentRecordingTime = 0.f;
	CurrentData.numberOfLiveSuits = 1;

	StartUDPSender("TESTSOCKETSENDER", IPAddress, 55606);
	//StartUDPReceiver("TESTSOCKETLISTENER", "192.168.1.133", 55606);
	StartUDPReceiver("TESTSOCKETLISTENER", "0.0.0.0", 55606);

	GetWorld()->GetTimerManager().SetTimer(FTH_TestHandle, this, &ThisClass::SendData, 1.f, true);
}

void ARokokoRemote::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

void ARokokoRemote::SendData()
{
	Sender_SendData();
}

bool ARokokoRemote::Sender_SendData()
{
	if (!SenderSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sender socket"));
		return false;
	}

	int32 BytesSent = 0;

	FString MessageString = CurrentData.Serialize();
	TCHAR* serializedmessage = MessageString.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedmessage);
	int32 sent = 0;

	SenderSocket->SendTo((uint8*)TCHAR_TO_UTF8(serializedmessage), size, BytesSent, *RemoteAddr);

	if (BytesSent <= 0)
	{
		const FString Str = "Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		return false;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Sent %d bytes"), BytesSent);
	}

	return true;

}

FString ARokokoRemote::GetCommandAPIKey()
{
	return CurrentData.commandKey;
}

int32 ARokokoRemote::GetCommandAPIPort()
{
	return CurrentData.commandPort;
}

bool ARokokoRemote::StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
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

bool ARokokoRemote::StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	UE_LOG(LogTemp, Warning, TEXT("RECEIVER INIT"));

	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);

	FIPv4Endpoint Endpoint(Addr, ThePort);

	int32 BufferSize = 2 * 1024 * 1024;

	ListenSocket = FUdpSocketBuilder(*YourChosenSocketName).AsNonBlocking().AsReusable().BoundToEndpoint(Endpoint).WithReceiveBufferSize(BufferSize);

	//ListenSocket = FUdpSocketBuilder(*YourChosenSocketName).AsReusable().WithBroadcast();

	if (!ListenSocket)
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
	UDPReceiver->Start();

	return true;
}

FString WriteBoolValue(bool InValue)
{
	return InValue ? "True" : "False";
}

FString testfunc(const uint8* SrcBuffer, const uint32 SrcSize)
{
	FString Result;
	Result.Reserve(SrcSize * 2);
	// Convert and append each byte in the buffer
	for (uint32 Count = 0; Count < SrcSize; Count++)
	{
		Result += TCHAR(SrcBuffer[Count]);
	}
	return Result;
}

void ARokokoRemote::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	//UE_LOG(LogTemp, Warning, TEXT("Received bytes: %d"), ArrayReaderPtr->Num());

	FRokokoRemoteInstance Data2;

	//FString DataString = FString::FromHexBlob(ArrayReaderPtr->GetData(), ArrayReaderPtr->Num());
	//FString DataString = BytesToString(ArrayReaderPtr->GetData(), ArrayReaderPtr->Num());
	FString DataString = testfunc(ArrayReaderPtr->GetData(), ArrayReaderPtr->Num());

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(DataString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		Data2 = FRokokoRemoteInstance(JsonObject);
	}


	if (Data2.type == "studio-keep-alive")
	{
		CurrentData.commandPort = Data2.commandPort;
		CurrentData.commandKey = Data2.commandKey;
	}
	else
	if (Data2.type == "studio-request")
	{
		CurrentData.connectedTo = Data2.deviceName;
		

		FString PCName = CurrentData.connectedTo;

		CurrentData.commandPort = Data2.commandPort;
		CurrentData.commandKey = Data2.commandKey;

		AsyncTask(ENamedThreads::GameThread, [this, PCName]{OnConnected.Broadcast(PCName);});
	}
	else
	if (Data2.type == "studio-cancel-request")
	{
		CurrentData.connectedTo = "";
		UE_LOG(LogTemp, Warning, TEXT("DISCONNECT"));

		AsyncTask(ENamedThreads::GameThread, [this] {OnDisconnected.Broadcast();});
	}
	else
	{
		
	}

	//Data2.DisplayValues();
}

FString FRokokoRemoteInstance::Serialize()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("type"), type);
	JsonObject->SetNumberField(TEXT("version"), version);
	JsonObject->SetStringField(TEXT("provider"), provider);
	JsonObject->SetStringField(TEXT("faceId"), faceId);
	JsonObject->SetStringField(TEXT("deviceName"), deviceName);
	JsonObject->SetStringField(TEXT("connectedTo"), connectedTo);
	JsonObject->SetStringField(TEXT("requestedFrom"), requestedFrom);
	JsonObject->SetStringField(TEXT("commandKey"), commandKey);
	JsonObject->SetNumberField(TEXT("commandPort"), commandPort);
	JsonObject->SetBoolField(TEXT("recording"), recording);
	JsonObject->SetNumberField(TEXT("currentRecordingTime"), currentRecordingTime);
	JsonObject->SetNumberField(TEXT("numberOfLiveSuits"), numberOfLiveSuits);
	JsonObject->SetBoolField(TEXT("commandApiOn"), commandApiOn);
	JsonObject->SetBoolField(TEXT("commandApiLicense"), commandApiLicense);
	JsonObject->SetBoolField(TEXT("faceLicense"), faceLicense);

	FString OutputString = "";

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
}

FRokokoRemoteInstance::FRokokoRemoteInstance(TSharedPtr<FJsonObject> jsonObject)
{
	jsonObject->TryGetStringField("type", type);
	jsonObject->TryGetNumberField(TEXT("version"), version);
	jsonObject->TryGetStringField(TEXT("provider"), provider);
	jsonObject->TryGetStringField(TEXT("faceId"), faceId);
	jsonObject->TryGetStringField(TEXT("deviceName"), deviceName);
	jsonObject->TryGetStringField(TEXT("connectedTo"), connectedTo);
	jsonObject->TryGetStringField(TEXT("requestedFrom"), requestedFrom);
	jsonObject->TryGetStringField(TEXT("commandKey"), commandKey);
	jsonObject->TryGetNumberField(TEXT("commandPort"), commandPort);
	jsonObject->TryGetBoolField(TEXT("recording"), recording);
	double recordingTimeVal;
	if (jsonObject->TryGetNumberField(TEXT("currentRecordingTime"), recordingTimeVal))
	{
		currentRecordingTime = recordingTimeVal;
	}
	jsonObject->TryGetNumberField(TEXT("numberOfLiveSuits"), numberOfLiveSuits);
	jsonObject->TryGetBoolField(TEXT("commandApiOn"), commandApiOn);
	jsonObject->TryGetBoolField(TEXT("commandApiLicense"), commandApiLicense);
	jsonObject->TryGetBoolField(TEXT("faceLicense"), faceLicense);
}

void FRokokoRemoteInstance::DisplayValues()
{
	UE_LOG(LogTemp, Warning, TEXT("type: %s"), *type);
	UE_LOG(LogTemp, Warning, TEXT("version: %d"), version);
	UE_LOG(LogTemp, Warning, TEXT("provider: %s"), *provider);
	UE_LOG(LogTemp, Warning, TEXT("faceId: %s"), *faceId);
	UE_LOG(LogTemp, Warning, TEXT("deviceName: %s"), *deviceName);
	UE_LOG(LogTemp, Warning, TEXT("connectedTo: %s"), *connectedTo);
	UE_LOG(LogTemp, Warning, TEXT("requestedFrom: %s"), *requestedFrom);
	UE_LOG(LogTemp, Warning, TEXT("commandKey: %s"), *commandKey);
	UE_LOG(LogTemp, Warning, TEXT("commandPort: %d"), commandPort);
	UE_LOG(LogTemp, Warning, TEXT("recording: %s"), *WriteBoolValue(recording));
	UE_LOG(LogTemp, Warning, TEXT("currentRecordingTime: %f"), currentRecordingTime);
	UE_LOG(LogTemp, Warning, TEXT("numberOfLiveSuits: %d"), numberOfLiveSuits);
	UE_LOG(LogTemp, Warning, TEXT("commandApiOn: %s"), *WriteBoolValue(commandApiOn));
	UE_LOG(LogTemp, Warning, TEXT("commandApiLicense: %s"), *WriteBoolValue(commandApiLicense));
	UE_LOG(LogTemp, Warning, TEXT("faceLicense: %s"), *WriteBoolValue(faceLicense));
}
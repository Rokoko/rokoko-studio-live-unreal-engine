// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitStreamingNetwork.h"
#include "../Lib//V8/Includes/SmartsuitDef.h"
#include "../Lib//V8/Includes/SmartsuitLib.h"
#include "Smartsuit.h"

SmartsuitStreamingNetwork::SmartsuitStreamingNetwork()
{
	//we support maximum 10 suits.
	suitCount = 0;
	suits = new SuitData[10];
	for (int i = 0; i < 10; i++) {
#if PLATFORM_WINDOWS
		strcpy_s(suits[i].suitname, 5, "\0\0\0\0\0");
#else // using strlcpy on Android, Mac and iOS
		strlcpy(suits[i].suitname, "\0\0\0\0\0", 5);
#endif
		suits[i].fps = 0;
		suits[i].currFPSCount = 0;
		suits[i].lastFPSSecond = -1;
	}
}

SmartsuitStreamingNetwork::~SmartsuitStreamingNetwork()
{
	//	// Stop the runnable
	Stop();

	if (Socket) {
		Socket->Close();
	}

	//	// And last but not least stop the main thread
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

void SmartsuitStreamingNetwork::Start(int port) {
	streaming_port = port;
	FString ThreadName(FString::Printf(TEXT("SmartsuitStreamingNetwork%ld"), (long)(FDateTime::UtcNow().ToUnixTimestamp())));
	Thread = FRunnableThread::Create(this, *ThreadName, 8 * 1024, TPri_Normal);
}


bool SmartsuitStreamingNetwork::Init()
{
	if (Socket == NULL)
	{
		Socket = FUdpSocketBuilder(TEXT("SmartsuitStreamingNetwork")).BoundToAddress(FIPv4Address(0, 0, 0, 0)).BoundToPort(streaming_port).AsReusable().Build();
		//allow the socket to listen to an already bounded address.
		Socket->SetReuseAddr(true);
		Stopping = false;
	}
	return true;
}

uint32 SmartsuitStreamingNetwork::Run()
{
	SmartsuitLib lib;
	while (!Stopping)
	{
		auto addr_in = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		int32 bytes_read = 0;
		FString ret;

		uint8 data[2048];
		memset(data, '\0', 2048);

		FDateTime time = FDateTime::UtcNow();
		int seconds = time.ToUnixTimestamp();

		for (int i = 0; i < 10; i++) {
			if (strcmp(suits[i].suitname, "\0\0\0\0") != 0) {
				//update FPS for this suit.
				if (suits[i].lastFPSSecond != seconds) {
					suits[i].lastFPSSecond = seconds;
					suits[i].fps = suits[i].currFPSCount;
					suits[i].currFPSCount = 0;
				}
			}
		}

		if (Socket) {

			if (Socket->RecvFrom(data, sizeof(data), bytes_read, *addr_in)) {
				if (bytes_read == 0) {
					continue;
				}
				//UE_LOG(LogTemp, Warning, TEXT("received something3....."));

				SuitDataDef *def = lib.ParseFrame(data, bytes_read);
				SuitData sd;

				// If you want to wait for the code above to complete do this:
				memcpy(sd.suitname, def->suitname, 4);
				sd.url = 0;
				addr_in->GetIp(sd.url);

				//if (wifiSupportedSuits.Contains(FString(sd.suitname))) {
				sd.isBroadcasting = def->isBroadcasting;
				sd.hasProfile = def->hasProfile;
				sd.profileToggle = def->profileToggle;
				for (int i = 0; i < def->sensorCount; i++)
				{
					sd.sensors[i].addr = def->sensors[i].addr;
					sd.sensors[i].isAnotherSensorConnected = def->sensors[i].isAnotherSensorConnected;
					sd.sensors[i].behavior = def->sensors[i].behavior;
					sd.sensors[i].command = def->sensors[i].command;
					sd.sensors[i].acceleration.X = def->sensors[i].acceleration[0]; //X
					sd.sensors[i].acceleration.Y = def->sensors[i].acceleration[1]; //Y
					sd.sensors[i].acceleration.Z = def->sensors[i].acceleration[2];; ///Z
					sd.sensors[i].quaternion.W = def->sensors[i].quaternion[0]; //W
					sd.sensors[i].quaternion.X = def->sensors[i].quaternion[1]; //X
					sd.sensors[i].quaternion.Y = def->sensors[i].quaternion[2]; //Y
					sd.sensors[i].quaternion.Z = def->sensors[i].quaternion[3]; //Z
					sd.sensors[i].gyro.X = def->sensors[i].gyro[0];//X
					sd.sensors[i].gyro.Y = def->sensors[i].gyro[1];//Y
					sd.sensors[i].gyro.Z = def->sensors[i].gyro[2];//Z
					sd.sensors[i].position.X = def->sensors[i].position[0];//X
					sd.sensors[i].position.Y = def->sensors[i].position[1];//Y
					sd.sensors[i].position.Z = def->sensors[i].position[2];//Z
					sd.sensors[i].microseconds = def->sensors[i].microseconds;
				}
				delete def;
				def = nullptr;
				//find suit...
				int index = -1;
				bool newSuit = false;
				for (int i = 0; i < 10; i++) {
					if (strcmp(suits[i].suitname, "\0\0\0\0") == 0) {
						newSuit = true;
						index = i;
						break;
					}
					else if (strcmp(suits[i].suitname, sd.suitname) == 0) {
						index = i;
						//UE_LOG(LogTemp, Warning, TEXT("Saving... : %d"), index);
						break;
					}
				}

				if (index != -1) {
					if (newSuit) {
						suitCount++;
						sd.lastFPSSecond = seconds;
						sd.fps = 0;
						sd.currFPSCount = 1;
					}
					else {
						sd.lastFPSSecond = suits[index].lastFPSSecond;
						sd.fps = suits[index].fps;
						sd.currFPSCount = suits[index].currFPSCount + 1;
					}
					memcpy(&(suits[index]), &sd, sizeof(SuitData));
				}

				auto livelink = FVirtualProductionSource::Get();
				if (livelink.IsValid()) 
				{
					SendSuitsToLiveLink();
				}
			}
		}
	}
	return 0;
}

void SmartsuitStreamingNetwork::SendSuitsToLiveLink() 
{
	TArray<SuitData> suitArray;
	for (int i = 0; i < suitCount; i++) 
	{
		suitArray.Add(suits[i]);
	}
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid()) 
	{
		livelink->HandleSuits(suitArray);
	}
}
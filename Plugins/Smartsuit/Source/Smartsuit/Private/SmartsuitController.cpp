// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitController.h"
#include "VirtualProductionSource.h"

// Sets default values
ASmartsuitController::ASmartsuitController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bodyModelRequested = false;
}

// Called when the game starts or when spawned
void ASmartsuitController::BeginPlay()
{
	Super::BeginPlay();
	realLife = true;
	if (AutoConnect) 
	{
		suitname = "autoconnect";
	}
}

// Called every frame
void ASmartsuitController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto livelink = FVirtualProductionSource::Get();
	if (!livelink.IsValid())
	{
		return;
	}

	if (AutoConnect) 
	{
		TArray<FString> suitnames = livelink->GetAvailableSmartsuitNames();
		if (suitnames.Num() > 0)
		{
			suitname = suitnames[0];
			AutoConnect = false;
		}
	}
	if (AutoConnect) 
	{
		return;
	}

	FSuitData *data = livelink->GetSmartsuitByName(suitname);
	if (!data) 
	{
		return;
	}
	
}


uint8 ASmartsuitController::GetByte(uint32 value, int i) 
{
	switch (i) {
	case 0:
		return 0xff & (value >> 24);
	case 1:
		return 0xff & (value >> 16);
	case 2:
		return 0xff & (value >> 8);
	case 3:
		return 0xff & value;
	default:
		return 0;
	}
}

#if PLATFORM_MAC

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

uint32 ASmartsuitController::GetLocalMacIP() {
    
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void *tmpAddrPtr = NULL;
    uint32 validIp = 0;
    
    getifaddrs(&ifAddrStruct);
    
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            FString addressStr = FString(UTF8_TO_TCHAR(addressBuffer));
            if(!addressStr.Contains(":") && addressStr.Compare(FString("0.0.0.0")) != 0 && addressStr.Compare(FString("127.0.0.1")) != 0) {
                validIp = (uint32)((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;
                UE_LOG(LogTemp, Display, TEXT("IPv4 Address: %s (%ld)"), *addressStr, validIp);
            }
        }
    }
    if (ifAddrStruct != NULL) {
        freeifaddrs(ifAddrStruct);
    }
    if(validIp != 0) { // Reversing byte order
        validIp = (GetByte(validIp, 3) << 24) | (GetByte(validIp, 2) << 16) | (GetByte(validIp, 1) << 8) | GetByte(validIp, 0);
    }
    
    return validIp;
}

#else

uint32 ASmartsuitController::GetLocalMacIP() { return 0; }

#endif

void ASmartsuitController::DoTests() {
	
}

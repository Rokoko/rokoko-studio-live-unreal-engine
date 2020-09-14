// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitReceiver.h"
#include "Smartsuit.h"



// Sets default values
ASmartsuitReceiver::ASmartsuitReceiver()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmartsuitReceiver::BeginPlay()
{
	Super::BeginPlay();
	StartListener();
	enabled = true;
	realLife = true;
}

void ASmartsuitReceiver::BeginDestroy() 
{
	Super::BeginDestroy();
	StopListener();
}

// Called every frame
void ASmartsuitReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASmartsuitReceiver::StartListener()
{
	UE_LOG(LogTemp, Warning, TEXT("Listening..."));
	//listener.Start(streamingDataPort);
	VPlistener.Start(VPListenPort);
}

void ASmartsuitReceiver::StopListener() 
{
	//listener.Stop();
	VPlistener.Stop();
	UE_LOG(LogTemp, Warning, TEXT("Not listening..."));
}

FFace ASmartsuitReceiver::GetFaceByFaceID(FString faceId)
{
	return VPlistener.GetFaceByFaceID(faceId);
}

FFace ASmartsuitReceiver::GetFaceByProfileName(const FString& faceName, bool& found)
{
	//return *VPlistener.GetFaceByProfileName(faceName);



	found = false;
	FFace returnval;
	FFace* temp = VPlistener.GetFaceByProfileName(faceName);
	if (temp)
	{
		returnval = *temp;
		found = true;
	}
	return returnval;
}

TArray<FFace> ASmartsuitReceiver::GetAllFaces()
{
	return VPlistener.GetAllFaces();
}

TArray<FFace> ASmartsuitReceiver::GetFacesNotAssociatedWithActor()
{
	TArray<FFace> FacesNotPairedWithSuit;
	for (auto CurrentFace : GetAllFaces())
	{
		bool FoundExistingProfileForFace = false;

		for (auto CurrentSuit : GetAllSmartsuits())
		{
			if (CurrentSuit.id == CurrentFace.profileName)
			{
				FoundExistingProfileForFace = true;
			}
		}

		if (!FoundExistingProfileForFace)
		{
			FacesNotPairedWithSuit.Add(CurrentFace);
		}
	}
	return FacesNotPairedWithSuit;
}

FSuitData* ASmartsuitReceiver::GetSmartsuit(FString suitName)
{
	return VPlistener.GetSmartsuitByName(suitName);
}

bool ASmartsuitReceiver::GetSmartsuitByName(const FString& suitName, FSuitData& SuitData)
{
	//return *VPlistener.GetSmartsuitByName(suitName);

	if (FSuitData* smartsuit = GetSmartsuit(suitName))
	{
		SuitData = *smartsuit;
		return true;
	}
	SuitData = FSuitData();
	return false;
}

TArray<FSuitData> ASmartsuitReceiver::GetAllSmartsuits()
{
	return VPlistener.GetAllSmartsuits();
}

TArray<FString> ASmartsuitReceiver::GetAvailableSmartsuitNames()
{
	return VPlistener.GetAvailableSmartsuitNames();
}


FProp* ASmartsuitReceiver::GetPropByNameFromVP(FString name, bool isLive) 
{
	return VPlistener.GetPropByName(name, isLive);
}
PRAGMA_DISABLE_OPTIMIZATION
TArray<FProp> ASmartsuitReceiver::GetAllProps()
{
	TArray<FProp> result;
	//UE_LOG(LogTemp, Display, TEXT("Yeeee1"));
	bool found = false;
	int i = 0;
	for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
	{
		//UE_LOG(LogTemp, Display, TEXT("Looking up receiver %d"), i);
		i++;
		if (It->realLife)
		{
			found = true;
			//UE_LOG(LogTemp, Display, TEXT("Real life!"));
			result = It->VPlistener.GetAllProps();
		}
	}
	if (!found)
	{
		//UE_LOG(LogTemp, Display, TEXT("not Real life..."));
	}
	//UE_LOG(LogTemp, Display, TEXT("Yeeee2 %d"), result.Num());
	return result;
}
PRAGMA_ENABLE_OPTIMIZATION
FProp ASmartsuitReceiver::GetProp(FString name, bool isLive)
{
	FProp result;
	for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
	{
		if (It->realLife)
		{
			result = *It->GetPropByNameFromVP(name, isLive);
		}
	}
	return result;
}

FTracker* ASmartsuitReceiver::GetTrackerByNameFromVP(FString name, bool isLive) 
{
	return VPlistener.GetTrackerByName(name, isLive);
}

FTracker ASmartsuitReceiver::GetTracker(FString name, bool isLive)
{
	FTracker result;
	for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
	{
		if (It->realLife)
		{
			result = *It->GetTrackerByNameFromVP(name, isLive);
		}
	}
	return result;
}

FTracker ASmartsuitReceiver::GetTrackerByConnectionIDFromVP(const FString& name, bool isLive, bool& found)
{
	found = false;
	FTracker returnval;
	FTracker* temp = VPlistener.GetTrackerByConnectionID(name, isLive);
	if (temp)
	{
		returnval = *temp;
		found = true;
	}
	return returnval;
}

void ASmartsuitReceiver::SetSupportsWiFiAPI(FString suitname) 
{
	//listener.wifiSupportedSuits.Add(suitname);
}
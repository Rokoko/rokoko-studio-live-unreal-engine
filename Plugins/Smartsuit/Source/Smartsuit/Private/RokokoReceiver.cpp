// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "RokokoReceiver.h"
#include "VirtualProductionSource.h"


// Sets default values
ARokokoReceiver::ARokokoReceiver()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RokokoPortNumber = 14045;
}

// Called when the game starts or when spawned
void ARokokoReceiver::BeginPlay()
{
	Super::BeginPlay();
	//StartListener();
	//enabled = true;
	//realLife = true;
}

void ARokokoReceiver::BeginDestroy()
{
	Super::BeginDestroy();
	//StopListener();
}

// Called every frame
void ARokokoReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARokokoReceiver::StartListener()
{
	//UE_LOG(LogTemp, Warning, TEXT("Listening..."));
	////listener.Start(streamingDataPort);
	//VPlistener.Start(RokokoPortNumber);
}

void ARokokoReceiver::StopListener()
{
	////listener.Stop();
	//VPlistener.Stop();
	//UE_LOG(LogTemp, Warning, TEXT("Not listening..."));
}

FFace ARokokoReceiver::GetFaceByFaceID(FString faceId)
{
	//return VPlistener.GetFaceByFaceID(faceId);

	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		return livelink->GetFaceByFaceID(faceId);
	}

	return FFace();
}

FFace ARokokoReceiver::GetFaceByProfileName(const FString& faceName, bool& found)
{
	//return *VPlistener.GetFaceByProfileName(faceName);

	found = false;
	FFace returnval;

	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{ 
		FFace* temp = livelink->GetFaceByProfileName(faceName);
		if (temp)
		{
			returnval = *temp;
			found = true;
		}
	}
	return returnval;
}

TArray<FFace> ARokokoReceiver::GetAllFaces()
{
	//return VPlistener.GetAllFaces();
	TArray<FFace> Faces;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		Faces =  livelink->GetAllFaces();
	}

	return Faces;
}

TArray<FFace> ARokokoReceiver::GetFacesNotAssociatedWithActor()
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

FSuitData* ARokokoReceiver::GetSmartsuit(FString suitName)
{
	//return VPlistener.GetSmartsuitByName(suitName);
	FSuitData* ReturnValue = nullptr;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		ReturnValue = livelink->GetSmartsuitByName(suitName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("can not get virtual production source!!!"));
	}

	return ReturnValue;
}

bool ARokokoReceiver::GetSmartsuitByName(const FString& suitName, FSuitData& SuitData)
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

TArray<FSuitData> ARokokoReceiver::GetAllSmartsuits()
{
	//return VPlistener.GetAllSmartsuits();

	TArray<FSuitData> Smartsuits;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		Smartsuits = livelink->GetAllSmartsuits();
	}

	return Smartsuits;
}

TArray<FString> ARokokoReceiver::GetAvailableSmartsuitNames()
{
	//return VPlistener.GetAvailableSmartsuitNames();
	TArray<FString> SmartsuitNames;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		SmartsuitNames = livelink->GetAvailableSmartsuitNames();
	}
	return SmartsuitNames;
}


FProp* ARokokoReceiver::GetPropByNameFromVP(FString name, bool isLive)
{
	//return VPlistener.GetPropByName(name, isLive);

	FProp* ReturnValue = nullptr;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		ReturnValue = livelink->GetPropByName(name, isLive);
	}

	return ReturnValue;
}
PRAGMA_DISABLE_OPTIMIZATION
TArray<FProp> ARokokoReceiver::GetAllProps()
{
	//TArray<FProp> result;
	////UE_LOG(LogTemp, Display, TEXT("Yeeee1"));
	//bool found = false;
	//int i = 0;
	//for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("Looking up receiver %d"), i);
	//	i++;
	//	if (It->realLife)
	//	{
	//		found = true;
	//		//UE_LOG(LogTemp, Display, TEXT("Real life!"));
	//		result = It->VPlistener.GetAllProps();
	//	}
	//}
	//if (!found)
	//{
	//	//UE_LOG(LogTemp, Display, TEXT("not Real life..."));
	//}
	////UE_LOG(LogTemp, Display, TEXT("Yeeee2 %d"), result.Num());
	//return result;


	TArray<FProp> AllProps;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		AllProps = livelink->GetAllProps();
	}

	return AllProps;
}
PRAGMA_ENABLE_OPTIMIZATION
bool ARokokoReceiver::GetProp(FString name, /*bool isLive, */FProp& OutProp)
{
	//for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	//{
	//	if (It->realLife)
	//	{
	//		OutProp = *It->GetPropByNameFromVP(name, true);
	//		return true;
	//	}
	//}
	//return false;

	//FProp result;
	//for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	//{
	//	if (It->realLife)
	//	{
	//		result = *It->GetPropByNameFromVP(name, isLive);
	//	}
	//}
	//return result;

	if (FProp* prop = GetPropByNameFromVP(name, true))
	{
		OutProp = *prop;
		return true;
	}
	OutProp = FProp();
	return false;
}

FTracker* ARokokoReceiver::GetTrackerByNameFromVP(FString name, bool isLive)
{
	//return VPlistener.GetTrackerByName(name, isLive);
	FTracker* Tracker = nullptr;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		Tracker = livelink->GetTrackerByName(name, isLive);
	}

	return Tracker;
}

FTracker ARokokoReceiver::GetTracker(FString name, bool isLive)
{
	FTracker result;
	for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	{
		if (It->realLife)
		{
			result = *It->GetTrackerByNameFromVP(name, isLive);
		}
	}
	return result;
}

FTracker ARokokoReceiver::GetTrackerByConnectionIDFromVP(const FString& name, bool isLive, bool& found)
{
	found = false;
	FTracker returnval;
	FTracker* temp = nullptr;// = VPlistener.GetTrackerByConnectionID(name, isLive);
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		temp = livelink->GetTrackerByConnectionID(name, isLive);
	}
	if (temp)
	{
		returnval = *temp;
		found = true;
	}
	return returnval;
}

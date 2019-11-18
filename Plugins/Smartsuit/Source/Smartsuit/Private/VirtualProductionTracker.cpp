// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionTracker.h"


UVirtualProductionTracker::UVirtualProductionTracker()
{
	PrimaryComponentTick.bCanEverTick = true;

	name = "Default";
	isLive = false;
	useLocalSpace = false;
	scalePosition = 1.f;
}

void UVirtualProductionTracker::BeginPlay()
{
	Super::BeginPlay();
	
	parent = GetOwner();
	CurrentLocation = parent->GetActorLocation();
}

ASmartsuitReceiver* UVirtualProductionTracker::GetReceiver() 
{
	ASmartsuitReceiver* listener = nullptr;

	for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
	{
		if (It->enabled) 
		{
			listener = *It;
			break;
		}
	}
	return listener;
}

void UVirtualProductionTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ASmartsuitReceiver* listener = GetReceiver();
	if (!listener) 
	{
		return;
	}

	FTracker* tracker = listener->GetTrackerByNameFromVP(name, isLive);
	if (!tracker) 
	{
		return;
	}

	//parent->SetActorLocation(tracker->UPosition() * scalePosition);
	//parent->SetActorRotation(tracker->FQuatToRotator());

	if (useLocalSpace)
	{
		parent->SetActorRelativeLocation(tracker->UPosition() * scalePosition);
		parent->SetActorRelativeRotation(tracker->FQuatToRotator());
	}
	else
	{
		parent->SetActorLocation(tracker->UPosition() * scalePosition);
		parent->SetActorRotation(tracker->FQuatToRotator());
	}
}
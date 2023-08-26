// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionTracker.h"
#include "VirtualProductionFrame.h"
#include "VirtualProductionSource.h"

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

void UVirtualProductionTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto livelink = FVirtualProductionSource::Get();
	if (!livelink.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("can not get virtual production source!!!"));
		return;
	}

	FTracker* tracker = livelink->GetTrackerByName(name, isLive);
	if (!tracker) 
	{
		return;
	}

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
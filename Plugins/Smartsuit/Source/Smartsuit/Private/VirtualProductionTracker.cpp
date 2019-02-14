// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionTracker.h"


// Sets default values for this component's properties
UVirtualProductionTracker::UVirtualProductionTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVirtualProductionTracker::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	parent = GetOwner();
	parent->GetActorLocation();
	CurrentLocation = parent->GetActorLocation();
}

ASmartsuitReceiver* UVirtualProductionTracker::GetReceiver() {
	ASmartsuitReceiver * listener = nullptr;
	// Find UObjects by type
	for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
	{
		if (It->enabled) {
			listener = *It;
			break;
		}
		// ...
	}
	return listener;
}

// Called every frame
void UVirtualProductionTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	ASmartsuitReceiver * listener = GetReceiver();
	if (!listener) {
		return;
	}

	FTracker* tracker = listener->GetTrackerByNameFromVP(name, isLive);
	if (!tracker) {
		return;
	}

	parent->SetActorLocation(tracker->UPosition());
	parent->SetActorRotation(tracker->FQuatToRotator());
}
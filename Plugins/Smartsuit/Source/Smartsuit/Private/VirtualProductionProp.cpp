// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionProp.h"


// Sets default values for this component's properties
UVirtualProductionProp::UVirtualProductionProp()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVirtualProductionProp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	parent = GetOwner();
	parent->GetActorLocation();
	CurrentLocation = parent->GetActorLocation();
	FVector a = parent->GetActorLocation();
}

ASmartsuitReceiver* UVirtualProductionProp::GetReceiver() {
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
void UVirtualProductionProp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ASmartsuitReceiver * listener = GetReceiver();
	if (!listener) {
		return;
	}

	FProp* prop = listener->GetPropByNameFromVP(name, isLive);
	if (!prop) {
		return;
	}

	parent->SetActorLocation(prop->UPosition());
	FRotator rotator = prop->FQuatToRotator();
	parent->SetActorRotation(rotator);
}


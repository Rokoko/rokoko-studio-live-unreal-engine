// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionProp.h"
#include "VirtualProductionSource.h"

UVirtualProductionProp::UVirtualProductionProp()
{
	PrimaryComponentTick.bCanEverTick = true;

	name = "Default";
	isLive = false;
	useLocalSpace = false;
	scalePosition = 1.f;
}

void UVirtualProductionProp::BeginPlay()
{
	Super::BeginPlay();

	parent = GetOwner();
	CurrentLocation = parent->GetActorLocation();
}

void UVirtualProductionProp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto livelink = FVirtualProductionSource::Get();
	if (!livelink.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UVirtualProductionProp::TickComponent] can not get virtual production source!!!"));
		return;
	}

	FProp* prop = livelink->GetPropByName(name, isLive);
	if (!prop) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("could not get prop!!!"));
		return;
	}

	if (useLocalSpace)
	{
		parent->SetActorRelativeLocation(prop->UPosition() * scalePosition);
		parent->SetActorRelativeRotation(prop->FQuatToRotator());
	}
	else
	{
		parent->SetActorLocation(prop->UPosition() * scalePosition);
		parent->SetActorRotation(prop->FQuatToRotator());
	}
}
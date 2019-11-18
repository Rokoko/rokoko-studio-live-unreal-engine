// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionProp.h"


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

ASmartsuitReceiver* UVirtualProductionProp::GetReceiver() 
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

void UVirtualProductionProp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ASmartsuitReceiver* listener = GetReceiver();
	if (!listener) 
	{
		return;
	}

	FProp* prop = listener->GetPropByNameFromVP(name, isLive);
	if (!prop) 
	{
		return;
	}

	//FVector LocalOffset(10.f, 10.f, 0.f);

	//FTransform TestTransform(prop->FQuatToRotator(), prop->UPosition(), FVector(scalePosition, scalePosition, scalePosition));

	//if (useLocalSpace)
	//{
	//	TestTransform.TransformPosition(LocalOffset);
	//}

	//parent->SetActorLocation(TestTransform.GetLocation());
	//parent->SetActorRotation(TestTransform.GetRotation());

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

	//parent->SetActorRotation(prop->FQuatToRotator());
}
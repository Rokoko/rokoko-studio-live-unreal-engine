// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkSourceSettings.h"
#include "VirtualProductionSourceSettings.generated.h"

UCLASS()
class SMARTSUIT_API UVirtualProductionSourceSettings : public ULiveLinkSourceSettings
{
	GENERATED_BODY()

public:
	/** keep a base name for received bone names, strip out all namespace **/
	UPROPERTY(EditAnywhere, Category = "Source")
	bool bTrimNamespace = false;

	/** do a position and rotation a mixamo compatible, ZYX rotation order **/
	UPROPERTY(EditAnywhere, Category = "Source")
	bool bMixamoCompatible = false;
};

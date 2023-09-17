// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "LiveLinkTypes.h"
#include "Roles/LiveLinkTransformTypes.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "Roles/LiveLinkAnimationBlueprintStructs.h"
#include "LiveLinkSmartsuitTypes.generated.h"

/**
 * Static data for Smartsuit data.
 */
USTRUCT(BlueprintType)
struct SMARTSUIT_API FLiveLinkSmartsuitStaticData : public FLiveLinkSkeletonStaticData
{
	GENERATED_BODY()

public:

};

/**
 * Dynamic data for Smartsuit
 */
USTRUCT(BlueprintType)
struct SMARTSUIT_API FLiveLinkSmartsuitFrameData : public FLiveLinkAnimationFrameData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LiveLink", Interp)
	bool HasLeftGlove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LiveLink", Interp)
	bool HasRightGlove = false;

};

USTRUCT(BlueprintType)
struct SMARTSUIT_API FLiveLinkCharactersFrameData : public FLiveLinkAnimationFrameData
{
	GENERATED_BODY()
public:
};

/**
 * Facility structure to handle camera data in blueprint
 */
USTRUCT(BlueprintType)
struct SMARTSUIT_API FLiveLinkSmartsuitBlueprintData : public FSubjectFrameHandle
{
	GENERATED_BODY()

	//// Static data that should not change every frame
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LiveLink")
	//FLiveLinkSmartsuitStaticData StaticData;

	//// Dynamic data that can change every frame
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LiveLink")
	//FLiveLinkSmartsuitFrameData FrameData;
};

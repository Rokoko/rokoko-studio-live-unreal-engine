// Copyright 2023 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "LiveLinkCustomVersion.h"
#include "LiveLinkRemapAsset.h"

#include "RokokoBodyRemapAsset.generated.h"


UCLASS(BlueprintType)
class SMARTSUIT_API URokokoBodyMapData : public ULiveLinkRemapAsset
{
	GENERATED_BODY()

	//!< a constructor
	URokokoBodyMapData();

public:

	FName GetRemappedBoneName_Implementation(FName CurveName) const override;
	
	virtual void Initialize() override
	{
		InitializeTMap();
	}

	UFUNCTION(BlueprintCallable, Category = BoneRemapping)
	void InitializeTMap();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		TMap<FName, FName> NameMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName hip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName stomach;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName chest;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName neck;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName head;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftShoulder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftForearm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightShoulder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightForearm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftUpleg;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftLeg;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftFoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftToe;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightUpleg;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightLeg;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightFoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightToe;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftThumbProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftThumbMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftThumbDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftThumbTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftIndexProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftIndexMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftIndexDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftIndexTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftMiddleProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftMiddleMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftMiddleDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftMiddleTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftRingProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftRingMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftRingDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftRingTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftLittleProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftLittleMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftLittleDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName leftLittleTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightThumbProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightThumbMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightThumbDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightThumbTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightIndexProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightIndexMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightIndexDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightIndexTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightMiddleProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightMiddleMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightMiddleDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightMiddleTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightRingProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightRingMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightRingDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightRingTip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightLittleProximal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightLittleMedial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightLittleDistal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoneRemapping)
		FName rightLittleTip;

};
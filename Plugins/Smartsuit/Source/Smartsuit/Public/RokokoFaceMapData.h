#pragma once

#include "ILiveLinkClient.h"
#include "ILiveLinkSource.h"
#include "Runtime/RenderCore/Public/RenderCore.h"
#include "MessageEndpoint.h"
#include "VirtualProductionFrame.h"
#include "RokokoSkeletonData.h"
#include "Engine/DataAsset.h"
#include "LiveLinkRemapAsset.h"

#include "RokokoFaceMapData.generated.h"


UCLASS(BlueprintType)
class SMARTSUIT_API URokokoFaceMapData : public ULiveLinkRemapAsset
{
	GENERATED_BODY()

	URokokoFaceMapData()
	{
		browDownLeft = "browDownLeft";
		browDownRight = "browDownRight";
		browInnerUp = "browInnerUp";
		browOuterUpLeft = "browOuterUpLeft";
		browOuterUpRight = "browOuterUpRight";
		cheekPuff = "cheekPuff";
		cheekSquintLeft = "cheekSquintLeft";
		cheekSquintRight = "cheekSquintRight";
		eyeBlinkLeft = "eyeBlinkLeft";
		eyeBlinkRight = "eyeBlinkRight";
		eyeLookDownLeft = "eyeLookDownLeft";
		eyeLookDownRight = "eyeLookDownRight";
		eyeLookInLeft = "eyeLookInLeft";
		eyeLookInRight = "eyeLookInRight";
		eyeLookOutLeft = "eyeLookOutLeft";
		eyeLookOutRight = "eyeLookOutRight";
		eyeLookUpLeft = "eyeLookUpLeft";
		eyeLookUpRight = "eyeLookUpRight";
		eyeSquintLeft = "eyeSquintLeft";
		eyeSquintRight = "eyeSquintRight";
		eyeWideLeft = "eyeWideLeft";
		eyeWideRight = "eyeWideRight";
		jawOpen = "jawOpen";
		jawForward = "jawForward";
		jawLeft = "jawLeft";
		jawRight = "jawRight";
		mouthClose = "mouthClose";
		mouthDimpleLeft = "mouthDimpleLeft";
		mouthDimpleRight = "mouthDimpleRight";
		mouthFrownLeft = "mouthFrownLeft";
		mouthFrownRight = "mouthFrownRight";
		mouthFunnel = "mouthFunnel";
		mouthLeft = "mouthLeft";
		mouthLowerDownLeft = "mouthLowerDownLeft";
		mouthLowerDownRight = "mouthLowerDownRight";
		mouthPressLeft = "mouthPressLeft";
		mouthPressRight = "mouthPressRight";
		mouthPucker = "mouthPucker";
		mouthRight = "mouthRight";
		mouthRollLower = "mouthRollLower";
		mouthRollUpper = "mouthRollUpper";
		mouthShrugLower = "mouthShrugLower";
		mouthShrugUpper = "mouthShrugUpper";
		mouthSmileLeft = "mouthSmileLeft";
		mouthSmileRight = "mouthSmileRight";
		mouthStretchLeft = "mouthStretchLeft";
		mouthStretchRight = "mouthStretchRight";
		mouthUpperUpLeft = "mouthUpperUpLeft";
		mouthUpperUpRight = "mouthUpperUpRight";
		noseSneerLeft = "noseSneerLeft";
		noseSneerRight = "noseSneerRight";
		tongueOut = "tongueOut";
	}
public:

	FName GetRemappedCurveName_Implementation(FName CurveName) const override;

	virtual void Initialize() override;

	UFUNCTION(BlueprintCallable, Category = MorphTargetRemapping)
	void InitializeTMap()
	{
		NameMapping.Add("browDownLeft", browDownLeft);
		NameMapping.Add("browDownRight", browDownRight);
		NameMapping.Add("browInnerUp", browInnerUp);
		NameMapping.Add("browOuterUpLeft", browOuterUpLeft);
		NameMapping.Add("browOuterUpRight", browOuterUpRight);
		NameMapping.Add("cheekPuff", cheekPuff);
		NameMapping.Add("cheekSquintLeft", cheekSquintLeft);
		NameMapping.Add("cheekSquintRight", cheekSquintRight);
		NameMapping.Add("eyeBlinkLeft", eyeBlinkLeft);
		NameMapping.Add("eyeBlinkRight", eyeBlinkRight);
		NameMapping.Add("eyeLookDownLeft", eyeLookDownLeft);
		NameMapping.Add("eyeLookDownRight", eyeLookDownRight);
		NameMapping.Add("eyeLookInLeft", eyeLookInLeft);
		NameMapping.Add("eyeLookInRight", eyeLookInRight);
		NameMapping.Add("eyeLookOutLeft", eyeLookOutLeft);
		NameMapping.Add("eyeLookOutRight", eyeLookOutRight);
		NameMapping.Add("eyeLookUpLeft", eyeLookUpLeft);
		NameMapping.Add("eyeLookUpRight", eyeLookUpRight);
		NameMapping.Add("eyeSquintLeft", eyeSquintLeft);
		NameMapping.Add("eyeSquintRight", eyeSquintRight);
		NameMapping.Add("eyeWideLeft", eyeWideLeft);
		NameMapping.Add("eyeWideRight", eyeWideRight);
		NameMapping.Add("jawOpen", jawOpen);
		NameMapping.Add("jawForward", jawForward);
		NameMapping.Add("jawLeft", jawLeft);
		NameMapping.Add("jawRight", jawRight);
		NameMapping.Add("mouthClose", mouthClose);
		NameMapping.Add("mouthDimpleLeft", mouthDimpleLeft);
		NameMapping.Add("mouthDimpleRight", mouthDimpleRight);
		NameMapping.Add("mouthFrownLeft", mouthFrownLeft);
		NameMapping.Add("mouthFrownRight", mouthFrownRight);
		NameMapping.Add("mouthFunnel", mouthFunnel);
		NameMapping.Add("mouthLeft", mouthLeft);
		NameMapping.Add("mouthLowerDownLeft", mouthLowerDownLeft);
		NameMapping.Add("mouthLowerDownRight", mouthLowerDownRight);
		NameMapping.Add("mouthPressLeft", mouthPressLeft);
		NameMapping.Add("mouthPressRight", mouthPressRight);
		NameMapping.Add("mouthPucker", mouthPucker);
		NameMapping.Add("mouthRight", mouthRight);
		NameMapping.Add("mouthRollLower", mouthRollLower);
		NameMapping.Add("mouthRollUpper", mouthRollUpper);
		NameMapping.Add("mouthShrugLower", mouthShrugLower);
		NameMapping.Add("mouthShrugUpper", mouthShrugUpper);
		NameMapping.Add("mouthSmileLeft", mouthSmileLeft);
		NameMapping.Add("mouthSmileRight", mouthSmileRight);
		NameMapping.Add("mouthStretchLeft", mouthStretchLeft);
		NameMapping.Add("mouthStretchRight", mouthStretchRight);
		NameMapping.Add("mouthUpperUpLeft", mouthUpperUpLeft);
		NameMapping.Add("mouthUpperUpRight", mouthUpperUpRight);
		NameMapping.Add("noseSneerLeft", noseSneerLeft);
		NameMapping.Add("noseSneerRight", noseSneerRight);
		NameMapping.Add("tongueOut", tongueOut);
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	TMap<FName, FName> NameMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName browDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName browDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName browInnerUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName browOuterUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName browOuterUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName cheekPuff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName cheekSquintLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName cheekSquintRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeBlinkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeBlinkRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookInLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookInRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookOutLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookOutRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeLookUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeSquintLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeSquintRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeWideLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName eyeWideRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName jawOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName jawForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName jawLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName jawRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthClose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthDimpleLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthDimpleRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthFrownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthFrownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthFunnel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthLowerDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthLowerDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthPressLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthPressRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthPucker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthRollLower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthRollUpper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthShrugLower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthShrugUpper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthSmileLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthSmileRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthStretchLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthStretchRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthUpperUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName mouthUpperUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName noseSneerLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName noseSneerRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MorphTargetRemapping)
	FName tongueOut;

};

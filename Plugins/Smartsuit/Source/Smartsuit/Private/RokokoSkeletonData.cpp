// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "RokokoSkeletonData.h"
#include "SmartsuitBlueprintLibrary.h"

namespace SmartsuitBones
{
	const FName hip = FName(TEXT("hip"));
	const FName spine = FName(TEXT("spine"));
	const FName chest = FName(TEXT("chest"));
	const FName neck = FName(TEXT("neck"));
	const FName head = FName(TEXT("head"));
	const FName leftShoulder = FName(TEXT("leftShoulder"));
	const FName leftUpperArm = FName(TEXT("leftUpperArm"));
	const FName leftLowerArm = FName(TEXT("leftLowerArm"));
	const FName leftHand = FName(TEXT("leftHand"));
	const FName rightShoulder = FName(TEXT("rightShoulder"));
	const FName rightUpperArm = FName(TEXT("rightUpperArm"));
	const FName rightLowerArm = FName(TEXT("rightLowerArm"));
	const FName rightHand = FName(TEXT("rightHand"));
	const FName leftUpLeg = FName(TEXT("leftUpLeg"));
	const FName leftLeg = FName(TEXT("leftLeg"));
	const FName leftFoot = FName(TEXT("leftFoot"));
	const FName leftToe = FName(TEXT("leftToe"));
	const FName leftToeEnd = FName(TEXT("leftToeEnd"));
	const FName rightUpLeg = FName(TEXT("rightUpLeg"));
	const FName rightLeg = FName(TEXT("rightLeg"));
	const FName rightFoot = FName(TEXT("rightFoot"));
	const FName rightToe = FName(TEXT("rightToe"));
	const FName rightToeEnd = FName(TEXT("rightToeEnd"));
	const FName leftThumbProximal = FName(TEXT("leftThumbProximal"));
	const FName leftThumbMedial = FName(TEXT("leftThumbMedial"));
	const FName leftThumbDistal = FName(TEXT("leftThumbDistal"));
	const FName leftThumbTip = FName(TEXT("leftThumbTip"));
	const FName leftIndexProximal = FName(TEXT("leftIndexProximal"));
	const FName leftIndexMedial = FName(TEXT("leftIndexMedial"));
	const FName leftIndexDistal = FName(TEXT("leftIndexDistal"));
	const FName leftIndexTip = FName(TEXT("leftIndexTip"));
	const FName leftMiddleProximal = FName(TEXT("leftMiddleProximal"));
	const FName leftMiddleMedial = FName(TEXT("leftMiddleMedial"));
	const FName leftMiddleDistal = FName(TEXT("leftMiddleDistal"));
	const FName leftMiddleTip = FName(TEXT("leftMiddleTip"));
	const FName leftRingProximal = FName(TEXT("leftRingProximal"));
	const FName leftRingMedial = FName(TEXT("leftRingMedial"));
	const FName leftRingDistal = FName(TEXT("leftRingDistal"));
	const FName leftRingTip = FName(TEXT("leftRingTip"));
	const FName leftLittleProximal = FName(TEXT("leftLittleProximal"));
	const FName leftLittleMedial = FName(TEXT("leftLittleMedial"));
	const FName leftLittleDistal = FName(TEXT("leftLittleDistal"));
	const FName leftLittleTip = FName(TEXT("leftLittleTip"));
	const FName rightThumbProximal = FName(TEXT("rightThumbProximal"));
	const FName rightThumbMedial = FName(TEXT("rightThumbMedial"));
	const FName rightThumbDistal = FName(TEXT("rightThumbDistal"));
	const FName rightThumbTip = FName(TEXT("rightThumbTip"));
	const FName rightIndexProximal = FName(TEXT("rightIndexProximal"));
	const FName rightIndexMedial = FName(TEXT("rightIndexMedial"));
	const FName rightIndexDistal = FName(TEXT("rightIndexDistal"));
	const FName rightIndexTip = FName(TEXT("rightIndexTip"));
	const FName rightMiddleProximal = FName(TEXT("rightMiddleProximal"));
	const FName rightMiddleMedial = FName(TEXT("rightMiddleMedial"));
	const FName rightMiddleDistal = FName(TEXT("rightMiddleDistal"));
	const FName rightMiddleTip = FName(TEXT("rightMiddleTip"));
	const FName rightRingProximal = FName(TEXT("rightRingProximal"));
	const FName rightRingMedial = FName(TEXT("rightRingMedial"));
	const FName rightRingDistal = FName(TEXT("rightRingDistal"));
	const FName rightRingTip = FName(TEXT("rightRingTip"));
	const FName rightLittleProximal = FName(TEXT("rightLittleProximal"));
	const FName rightLittleMedial = FName(TEXT("rightLittleMedial"));
	const FName rightLittleDistal = FName(TEXT("rightLittleDistal"));
	const FName rightLittleTip = FName(TEXT("rightLittleTip"));
}

SmartsuitDefinitions::SmartsuitDefinitions()
{
}

SmartsuitDefinitions::~SmartsuitDefinitions()
{
}

FSuitData::FSuitData(bool InIsLive, TSharedPtr<FJsonObject> jsonObject)
{
	suitname = jsonObject->GetStringField("name");

	//temp
	profileName = suitname;

	//timestamp = jsonObject->GetNumberField("timestamp");
	//id = jsonObject->GetStringField("id");
	//isLive = jsonObject->GetBoolField("isLive");
	isLive = InIsLive;
	//profileName = jsonObject->GetStringField("profileName");
	//color = USmartsuitBlueprintLibrary::GetFLinearColorField(jsonObject->GetObjectField("color"));
	//TArray<TSharedPtr<FJsonValue>> ColorArray = jsonObject->GetArrayField("color");
	color = USmartsuitBlueprintLibrary::GetColorField(jsonObject);

	TSharedPtr<FJsonObject> Meta = jsonObject->GetObjectField("meta");
	hasGloves = Meta->GetBoolField("hasGloves");
	hasLeftGlove = Meta->GetBoolField("hasLeftGlove");
	hasRightGlove = Meta->GetBoolField("hasRightGlove");
	hasBody = Meta->GetBoolField("hasBody");
	hasFace = Meta->GetBoolField("hasFace");

	TSharedPtr<FJsonObject> BodyObj = jsonObject->GetObjectField("body");

	ParseBone(BodyObj, SmartsuitBones::hip.ToString());
	ParseBone(BodyObj, SmartsuitBones::spine.ToString());
	ParseBone(BodyObj, SmartsuitBones::chest.ToString());
	ParseBone(BodyObj, SmartsuitBones::neck.ToString());
	ParseBone(BodyObj, SmartsuitBones::head.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftShoulder.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftUpperArm.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftLowerArm.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftHand.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightShoulder.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightUpperArm.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightLowerArm.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightHand.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftUpLeg.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftLeg.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftFoot.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftToe.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftToeEnd.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightUpLeg.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightLeg.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightFoot.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightToe.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightToeEnd.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftThumbProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftThumbMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftThumbDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftThumbTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftIndexProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftIndexMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftIndexDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftIndexTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftMiddleProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftMiddleMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftMiddleDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftMiddleTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftRingProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftRingMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftRingDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftRingTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftLittleProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftLittleMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftLittleDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::leftLittleTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightThumbProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightThumbMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightThumbDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightThumbTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightIndexProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightIndexMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightIndexDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightIndexTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightMiddleProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightMiddleMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightMiddleDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightMiddleTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightRingProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightRingMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightRingDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightRingTip.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightLittleProximal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightLittleMedial.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightLittleDistal.ToString());
	ParseBone(BodyObj, SmartsuitBones::rightLittleTip.ToString());
}

void FSuitData::ParseBone(TSharedPtr<FJsonObject> jsonObject, const FString& BoneName)
{
	if (jsonObject->HasField(BoneName))
	{
		TSharedPtr<FJsonObject> BoneObject = jsonObject->GetObjectField(BoneName);
		FVector SensorPosition = USmartsuitBlueprintLibrary::GetVectorField(BoneObject->GetObjectField("position"));
		FQuat SensorRotation = USmartsuitBlueprintLibrary::GetQuaternionField(BoneObject->GetObjectField("rotation"));

		bones.Add(FName(*BoneName), FSmartsuitBone(FName(*BoneName), SensorPosition, SensorRotation));
	}
}
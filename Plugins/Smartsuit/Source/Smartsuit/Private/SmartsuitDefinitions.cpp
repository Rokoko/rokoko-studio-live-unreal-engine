// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitDefinitions.h"
#include "Smartsuit.h"
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

FSuitData::FSuitData(TSharedPtr<FJsonObject> jsonObject)
{
	suitname = jsonObject->GetStringField("name");
	timestamp = jsonObject->GetNumberField("timestamp");
	isLive = jsonObject->GetBoolField("isLive");
	profileName = jsonObject->GetStringField("profileName");
	color = USmartsuitBlueprintLibrary::GetFLinearColorField(jsonObject->GetObjectField("color"));

	ParseBone(jsonObject, SmartsuitBones::hip.ToString());
	ParseBone(jsonObject, SmartsuitBones::spine.ToString());
	ParseBone(jsonObject, SmartsuitBones::chest.ToString());
	ParseBone(jsonObject, SmartsuitBones::neck.ToString());
	ParseBone(jsonObject, SmartsuitBones::head.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftShoulder.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftUpperArm.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftLowerArm.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftHand.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightShoulder.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightUpperArm.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightLowerArm.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightHand.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftUpLeg.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftLeg.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftFoot.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftToe.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftToeEnd.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightUpLeg.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightLeg.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightFoot.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightToe.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightToeEnd.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftThumbProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftThumbMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftThumbDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftThumbTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftIndexProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftIndexMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftIndexDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftIndexTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftMiddleProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftMiddleMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftMiddleDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftMiddleTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftRingProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftRingMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftRingDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftRingTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftLittleProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftLittleMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftLittleDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::leftLittleTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightThumbProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightThumbMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightThumbDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightThumbTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightIndexProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightIndexMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightIndexDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightIndexTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightMiddleProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightMiddleMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightMiddleDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightMiddleTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightRingProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightRingMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightRingDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightRingTip.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightLittleProximal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightLittleMedial.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightLittleDistal.ToString());
	ParseBone(jsonObject, SmartsuitBones::rightLittleTip.ToString());
}

void FSuitData::ParseBone(TSharedPtr<FJsonObject> jsonObject, const FString& BoneName)
{
	if (jsonObject->HasField(BoneName))
	{
		TSharedPtr<FJsonObject> BoneObject = jsonObject->GetObjectField(BoneName);
		FVector SensorPosition = USmartsuitBlueprintLibrary::GetVectorField(BoneObject->GetObjectField("position"));
		FQuat SensorRotation = USmartsuitBlueprintLibrary::GetQuaternionField(BoneObject->GetObjectField("rotation"));
		SmartsuitBones.Add(FName(*BoneName), FSmartsuitBone(FName(*BoneName), SensorPosition, SensorRotation));
	}
}
// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitDefinitions.h"
#include "Smartsuit.h"
#include "SmartsuitBlueprintLibrary.h"

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

	ParseBone(jsonObject, "hip");
	ParseBone(jsonObject, "spine");
	ParseBone(jsonObject, "chest");
	ParseBone(jsonObject, "neck");
	ParseBone(jsonObject, "head");
	ParseBone(jsonObject, "leftShoulder");
	ParseBone(jsonObject, "leftUpperArm");
	ParseBone(jsonObject, "leftLowerArm");
	ParseBone(jsonObject, "leftHand");
	ParseBone(jsonObject, "rightShoulder");
	ParseBone(jsonObject, "rightUpperArm");
	ParseBone(jsonObject, "rightLowerArm");
	ParseBone(jsonObject, "rightHand");
	ParseBone(jsonObject, "leftUpLeg");
	ParseBone(jsonObject, "leftLeg");
	ParseBone(jsonObject, "leftFoot");
	ParseBone(jsonObject, "leftToe");
	ParseBone(jsonObject, "leftToeEnd");
	ParseBone(jsonObject, "rightUpLeg");
	ParseBone(jsonObject, "rightLeg");
	ParseBone(jsonObject, "rightFoot");
	ParseBone(jsonObject, "rightToe");
	ParseBone(jsonObject, "rightToeEnd");
	ParseBone(jsonObject, "leftThumbProximal");
	ParseBone(jsonObject, "leftThumbMedial");
	ParseBone(jsonObject, "leftThumbDistal");
	ParseBone(jsonObject, "leftThumbTip");
	ParseBone(jsonObject, "leftIndexProximal");
	ParseBone(jsonObject, "leftIndexMedial");
	ParseBone(jsonObject, "leftIndexDistal");
	ParseBone(jsonObject, "leftIndexTip");
	ParseBone(jsonObject, "leftMiddleProximal");
	ParseBone(jsonObject, "leftMiddleMedial");
	ParseBone(jsonObject, "leftMiddleDistal");
	ParseBone(jsonObject, "leftMiddleTip");
	ParseBone(jsonObject, "leftRingProximal");
	ParseBone(jsonObject, "leftRingMedial");
	ParseBone(jsonObject, "leftRingDistal");
	ParseBone(jsonObject, "leftRingTip");
	ParseBone(jsonObject, "leftLittleProximal");
	ParseBone(jsonObject, "leftLittleMedial");
	ParseBone(jsonObject, "leftLittleDistal");
	ParseBone(jsonObject, "leftLittleTip");
	ParseBone(jsonObject, "rightThumbProximal");
	ParseBone(jsonObject, "rightThumbMedial");
	ParseBone(jsonObject, "rightThumbDistal");
	ParseBone(jsonObject, "rightThumbTip");
	ParseBone(jsonObject, "rightIndexProximal");
	ParseBone(jsonObject, "rightIndexMedial");
	ParseBone(jsonObject, "rightIndexDistal");
	ParseBone(jsonObject, "rightIndexTip");
	ParseBone(jsonObject, "rightMiddleProximal");
	ParseBone(jsonObject, "rightMiddleMedial");
	ParseBone(jsonObject, "rightMiddleDistal");
	ParseBone(jsonObject, "rightMiddleTip");
	ParseBone(jsonObject, "rightRingProximal");
	ParseBone(jsonObject, "rightRingMedial");
	ParseBone(jsonObject, "rightRingDistal");
	ParseBone(jsonObject, "rightRingTip");
	ParseBone(jsonObject, "rightLittleProximal");
	ParseBone(jsonObject, "rightLittleMedial");
	ParseBone(jsonObject, "rightLittleDistal");
	ParseBone(jsonObject, "rightLittleTip");
}

void FSuitData::ParseBone(TSharedPtr<FJsonObject> jsonObject, const FString& BoneName)
{
	if (jsonObject->HasField(BoneName))
	{
		TSharedPtr<FJsonObject> BoneObject = jsonObject->GetObjectField(BoneName);
		FVector SensorPosition = USmartsuitBlueprintLibrary::GetVectorField(BoneObject->GetObjectField("position"));
		FQuat SensorRotation = USmartsuitBlueprintLibrary::GetQuaternionField(BoneObject->GetObjectField("rotation"));
		SmartsuitBones.Add(FName(*BoneName), FSmartsuitBone(BoneName, SensorPosition, SensorRotation));
	}
}
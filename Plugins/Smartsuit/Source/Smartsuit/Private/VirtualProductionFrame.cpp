// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionFrame.h"
#include "SmartsuitBlueprintLibrary.h"

VirtualProductionFrame::VirtualProductionFrame()
{
}

VirtualProductionFrame::~VirtualProductionFrame()
{
}

FProp::FProp(TSharedPtr<FJsonObject> jsonObject)
{
	name = jsonObject->GetStringField("name");
	id = jsonObject->GetStringField("id");
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("position"));
	rotation = USmartsuitBlueprintLibrary::GetQuaternionField(jsonObject->GetObjectField("rotation"));
	isLive = jsonObject->GetBoolField("isLive");
	profile = FProfile(jsonObject->GetObjectField("profile"));
}

FProfile::FProfile(TSharedPtr<FJsonObject> jsonObject)
{
	name = jsonObject->GetStringField("name");
	uuid = jsonObject->GetStringField("uuid");
	dimensions = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("dimensions"));
	color = USmartsuitBlueprintLibrary::GetFColorField(jsonObject->GetObjectField("color"));
	trackerOffset = FReferencePoint(jsonObject->GetObjectField("trackeroffset"));
	pivot = FReferencePoint(jsonObject->GetObjectField("pivot"));

	TArray<TSharedPtr<FJsonValue>> gripsarray = jsonObject->GetArrayField("grips");

	for (auto currentgrip : gripsarray)
	{
		grips.Add(FRadiusReferencePoint(currentgrip->AsObject()));
	}

	propType = jsonObject->GetIntegerField("propType");
}

FReferencePoint::FReferencePoint(TSharedPtr<FJsonObject> jsonObject)
{
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("position"));

	FVector TempVector = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("rotation"));
	rotation = TempVector.Rotation().Quaternion();
}

FRadiusReferencePoint::FRadiusReferencePoint(TSharedPtr<FJsonObject> jsonObject)
{
	radius = jsonObject->GetNumberField("radius");
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("position"));
	rotation = USmartsuitBlueprintLibrary::GetQuaternionField(jsonObject->GetObjectField("rotation"));
}

FTracker::FTracker(TSharedPtr<FJsonObject> jsonObject)
{
	name = jsonObject->GetStringField("name");
	connectionId = jsonObject->GetStringField("connectionId");
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("position"));
	rotation = USmartsuitBlueprintLibrary::GetQuaternionField(jsonObject->GetObjectField("rotation"));
	isLive = jsonObject->GetBoolField("isLive");
	trackingResult = jsonObject->GetIntegerField("trackingResult");
	trackerType = jsonObject->GetIntegerField("trackerType");
	RenderModelName = jsonObject->GetStringField("RenderModelName");
	battery = jsonObject->GetNumberField("battery");
}

FFace::FFace(TSharedPtr<FJsonObject> jsonObject)
{
	version = jsonObject->GetIntegerField("version");
	provider = jsonObject->GetStringField("provider");
	faceId = jsonObject->GetStringField("faceId");

	//eyeBlinkLeft = jsonObject->GetNumberField("eyeBlinkLeft");
	//eyeLookDownLeft = jsonObject->GetNumberField("eyeLookDownLeft");
	//eyeLookInLeft = jsonObject->GetNumberField("eyeLookInLeft");
	//eyeLookOutLeft = jsonObject->GetNumberField("eyeLookOutLeft");
	//eyeLookUpLeft = jsonObject->GetNumberField("eyeLookUpLeft");
	//eyeSquintLeft = jsonObject->GetNumberField("eyeSquintLeft");
	//eyeWideLeft = jsonObject->GetNumberField("eyeWideLeft");
	//eyeBlinkRight = jsonObject->GetNumberField("eyeBlinkRight");
	//eyeLookDownRight = jsonObject->GetNumberField("eyeLookDownRight");
	//eyeLookInRight = jsonObject->GetNumberField("eyeLookInRight");
	//eyeLookOutRight = jsonObject->GetNumberField("eyeLookOutRight");
	//eyeLookUpRight = jsonObject->GetNumberField("eyeLookUpRight");
	//eyeSquintRight = jsonObject->GetNumberField("eyeSquintRight");
	//eyeWideRight = jsonObject->GetNumberField("eyeWideRight");
	//jawForward = jsonObject->GetNumberField("jawForward");
	//jawLeft = jsonObject->GetNumberField("jawLeft");
	//jawRight = jsonObject->GetNumberField("jawRight");
	//jawOpen = jsonObject->GetNumberField("jawOpen");
	//mouthClose = jsonObject->GetNumberField("mouthClose");
	//mouthFunnel = jsonObject->GetNumberField("mouthFunnel");
	//mouthPucker = jsonObject->GetNumberField("mouthPucker");
	//mouthLeft = jsonObject->GetNumberField("mouthLeft");
	//mouthRight = jsonObject->GetNumberField("mouthRight");
	//mouthSmileLeft = jsonObject->GetNumberField("mouthSmileLeft");
	//mouthSmileRight = jsonObject->GetNumberField("mouthSmileRight");
	//mouthFrownLeft = jsonObject->GetNumberField("mouthFrownLeft");
	//mouthFrownRight = jsonObject->GetNumberField("mouthFrownRight");
	//mouthDimpleLeft = jsonObject->GetNumberField("mouthDimpleLeft");
	//mouthDimpleRight = jsonObject->GetNumberField("mouthDimpleRight");
	//mouthStretchLeft = jsonObject->GetNumberField("mouthStretchLeft");
	//mouthStretchRight = jsonObject->GetNumberField("mouthStretchRight");
	//mouthRollLower = jsonObject->GetNumberField("mouthRollLower");
	//mouthRollUpper = jsonObject->GetNumberField("mouthRollUpper");
	//mouthShrugLower = jsonObject->GetNumberField("mouthShrugLower");
	//mouthShrugUpper = jsonObject->GetNumberField("mouthShrugUpper");
	//mouthPressLeft = jsonObject->GetNumberField("mouthPressLeft");
	//mouthPressRight = jsonObject->GetNumberField("mouthPressRight");
	//mouthLowerDownLeft = jsonObject->GetNumberField("mouthLowerDownLeft");
	//mouthLowerDownRight = jsonObject->GetNumberField("mouthLowerDownRight");
	//mouthUpperUpLeft = jsonObject->GetNumberField("mouthUpperUpLeft");
	//mouthUpperUpRight = jsonObject->GetNumberField("mouthUpperUpRight");
	//browDownLeft = jsonObject->GetNumberField("browDownLeft");
	//browDownRight = jsonObject->GetNumberField("browDownRight");
	//browInnerUp = jsonObject->GetNumberField("browInnerUp");
	//browOuterUpLeft = jsonObject->GetNumberField("browOuterUpLeft");
	//browOuterUpRight = jsonObject->GetNumberField("browOuterUpRight");
	//cheekPuff = jsonObject->GetNumberField("cheekPuff");
	//cheekSquintLeft = jsonObject->GetNumberField("cheekSquintLeft");
	//cheekSquintRight = jsonObject->GetNumberField("cheekSquintRight");
	//noseSneerLeft = jsonObject->GetNumberField("noseSneerLeft");
	//noseSneerRight = jsonObject->GetNumberField("noseSneerRight");
	//tongueOut = jsonObject->GetNumberField("tongueOut");

	eyeBlinkLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeBlinkLeft");
	eyeLookDownLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookDownLeft");
	eyeLookInLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookInLeft");
	eyeLookOutLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookOutLeft");
	eyeLookUpLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookUpLeft");
	eyeSquintLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeSquintLeft");
	eyeWideLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeWideLeft");
	eyeBlinkRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeBlinkRight");
	eyeLookDownRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookDownRight");
	eyeLookInRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookInRight");
	eyeLookOutRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookOutRight");
	eyeLookUpRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeLookUpRight");
	eyeSquintRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeSquintRight");
	eyeWideRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "eyeWideRight");
	jawForward = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "jawForward");
	jawLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "jawLeft");
	jawRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "jawRight");
	jawOpen = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "jawOpen");
	mouthClose = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthClose");
	mouthFunnel = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthFunnel");
	mouthPucker = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthPucker");
	mouthLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthLeft");
	mouthRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthRight");
	mouthSmileLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthSmileLeft");
	mouthSmileRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthSmileRight");
	mouthFrownLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthFrownLeft");
	mouthFrownRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthFrownRight");
	mouthDimpleLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthDimpleLeft");
	mouthDimpleRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthDimpleRight");
	mouthStretchLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthStretchLeft");
	mouthStretchRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthStretchRight");
	mouthRollLower = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthRollLower");
	mouthRollUpper = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthRollUpper");
	mouthShrugLower = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthShrugLower");
	mouthShrugUpper = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthShrugUpper");
	mouthPressLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthPressLeft");
	mouthPressRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthPressRight");
	mouthLowerDownLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthLowerDownLeft");
	mouthLowerDownRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthLowerDownRight");
	mouthUpperUpLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthUpperUpLeft");
	mouthUpperUpRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "mouthUpperUpRight");
	browDownLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "browDownLeft");
	browDownRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "browDownRight");
	browInnerUp = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "browInnerUp");
	browOuterUpLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "browOuterUpLeft");
	browOuterUpRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "browOuterUpRight");
	cheekPuff = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "cheekPuff");
	cheekSquintLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "cheekSquintLeft");
	cheekSquintRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "cheekSquintRight");
	noseSneerLeft = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "noseSneerLeft");
	noseSneerRight = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "noseSneerRight");
	tongueOut = USmartsuitBlueprintLibrary::ParseMorphTargetValue(jsonObject, "tongueOut");
}

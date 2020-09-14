// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionFrame.h"
#include "SmartsuitBlueprintLibrary.h"

VirtualProductionFrame::VirtualProductionFrame()
{
}

VirtualProductionFrame::~VirtualProductionFrame()
{
}

FProp::FProp(bool InIsLive, TSharedPtr<FJsonObject> jsonObject)
{
	name = jsonObject->GetStringField("name");
	//id = jsonObject->GetStringField("id");
	color = USmartsuitBlueprintLibrary::GetColorField(jsonObject);
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("position"));
	rotation = USmartsuitBlueprintLibrary::GetQuaternionField(jsonObject->GetObjectField("rotation"));
	//isLive = jsonObject->GetBoolField("isLive");
	isLive = InIsLive;
	//profile = FProfile(jsonObject->GetObjectField("profile"));
}

FProfile::FProfile(TSharedPtr<FJsonObject> jsonObject)
{
	name = jsonObject->GetStringField("name");
	uuid = jsonObject->GetStringField("uuid");
	dimensions = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField("dimensions"));
	color = USmartsuitBlueprintLibrary::GetFLinearColorField(jsonObject->GetObjectField("color"));
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

	//profileName = jsonObject->GetStringField("profileName");
	//version = jsonObject->GetIntegerField("version");
	//provider = jsonObject->GetStringField("provider");
	faceId = jsonObject->GetStringField("faceId");
	if (!jsonObject->TryGetStringField("profileName", profileName))
	{
		profileName = "NOPROFILENAME for " + faceId;
	}

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


	eyeBlinkLeft = jsonObject->GetNumberField("eyeBlinkLeft") / 100.f;
	eyeLookDownLeft = jsonObject->GetNumberField("eyeLookDownLeft") / 100.f;
	eyeLookInLeft = jsonObject->GetNumberField("eyeLookInLeft") / 100.f;
	eyeLookOutLeft = jsonObject->GetNumberField("eyeLookOutLeft") / 100.f;
	eyeLookUpLeft = jsonObject->GetNumberField("eyeLookUpLeft") / 100.f;
	eyeSquintLeft = jsonObject->GetNumberField("eyeSquintLeft") / 100.f;
	eyeWideLeft = jsonObject->GetNumberField("eyeWideLeft") / 100.f;
	eyeBlinkRight = jsonObject->GetNumberField("eyeBlinkRight") / 100.f;
	eyeLookDownRight = jsonObject->GetNumberField("eyeLookDownRight") / 100.f;
	eyeLookInRight = jsonObject->GetNumberField("eyeLookInRight") / 100.f;
	eyeLookOutRight = jsonObject->GetNumberField("eyeLookOutRight") / 100.f;
	eyeLookUpRight = jsonObject->GetNumberField("eyeLookUpRight") / 100.f;
	eyeSquintRight = jsonObject->GetNumberField("eyeSquintRight") / 100.f;
	eyeWideRight = jsonObject->GetNumberField("eyeWideRight") / 100.f;
	jawForward = jsonObject->GetNumberField("jawForward") / 100.f;
	jawLeft = jsonObject->GetNumberField("jawLeft") / 100.f;
	jawRight = jsonObject->GetNumberField("jawRight") / 100.f;
	jawOpen = jsonObject->GetNumberField("jawOpen") / 100.f;
	mouthClose = jsonObject->GetNumberField("mouthClose") / 100.f;
	mouthFunnel = jsonObject->GetNumberField("mouthFunnel") / 100.f;
	mouthPucker = jsonObject->GetNumberField("mouthPucker") / 100.f;
	mouthLeft = jsonObject->GetNumberField("mouthLeft") / 100.f;
	mouthRight = jsonObject->GetNumberField("mouthRight") / 100.f;
	mouthSmileLeft = jsonObject->GetNumberField("mouthSmileLeft") / 100.f;
	mouthSmileRight = jsonObject->GetNumberField("mouthSmileRight") / 100.f;
	mouthFrownLeft = jsonObject->GetNumberField("mouthFrownLeft") / 100.f;
	mouthFrownRight = jsonObject->GetNumberField("mouthFrownRight") / 100.f;
	mouthDimpleLeft = jsonObject->GetNumberField("mouthDimpleLeft") / 100.f;
	mouthDimpleRight = jsonObject->GetNumberField("mouthDimpleRight") / 100.f;
	mouthStretchLeft = jsonObject->GetNumberField("mouthStretchLeft") / 100.f;
	mouthStretchRight = jsonObject->GetNumberField("mouthStretchRight") / 100.f;
	mouthRollLower = jsonObject->GetNumberField("mouthRollLower") / 100.f;
	mouthRollUpper = jsonObject->GetNumberField("mouthRollUpper") / 100.f;
	mouthShrugLower = jsonObject->GetNumberField("mouthShrugLower") / 100.f;
	mouthShrugUpper = jsonObject->GetNumberField("mouthShrugUpper") / 100.f;
	mouthPressLeft = jsonObject->GetNumberField("mouthPressLeft") / 100.f;
	mouthPressRight = jsonObject->GetNumberField("mouthPressRight") / 100.f;
	mouthLowerDownLeft = jsonObject->GetNumberField("mouthLowerDownLeft") / 100.f;
	mouthLowerDownRight = jsonObject->GetNumberField("mouthLowerDownRight") / 100.f;
	mouthUpperUpLeft = jsonObject->GetNumberField("mouthUpperUpLeft") / 100.f;
	mouthUpperUpRight = jsonObject->GetNumberField("mouthUpperUpRight") / 100.f;
	browDownLeft = jsonObject->GetNumberField("browDownLeft") / 100.f;
	browDownRight = jsonObject->GetNumberField("browDownRight") / 100.f;
	browInnerUp = jsonObject->GetNumberField("browInnerUp") / 100.f;
	browOuterUpLeft = jsonObject->GetNumberField("browOuterUpLeft") / 100.f;
	browOuterUpRight = jsonObject->GetNumberField("browOuterUpRight") / 100.f;
	cheekPuff = jsonObject->GetNumberField("cheekPuff") / 100.f;
	cheekSquintLeft = jsonObject->GetNumberField("cheekSquintLeft") / 100.f;
	cheekSquintRight = jsonObject->GetNumberField("cheekSquintRight") / 100.f;
	noseSneerLeft = jsonObject->GetNumberField("noseSneerLeft") / 100.f;
	noseSneerRight = jsonObject->GetNumberField("noseSneerRight") / 100.f;
	tongueOut = jsonObject->GetNumberField("tongueOut") / 100.f;
}

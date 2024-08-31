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
	name = jsonObject->GetStringField(TEXT("name"));
	color = USmartsuitBlueprintLibrary::GetColorField(jsonObject);
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField(TEXT("position")));
	rotation = USmartsuitBlueprintLibrary::GetQuaternionField(jsonObject->GetObjectField(TEXT("rotation")));
	
	isLive = InIsLive;
}

FProfile::FProfile(TSharedPtr<FJsonObject> jsonObject)
{
	name = jsonObject->GetStringField(TEXT("name"));
	uuid = jsonObject->GetStringField(TEXT("uuid"));
	dimensions = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField(TEXT("dimensions")));
	color = USmartsuitBlueprintLibrary::GetFLinearColorField(jsonObject->GetObjectField(TEXT("color")));
	trackerOffset = FReferencePoint(jsonObject->GetObjectField(TEXT("trackeroffset")));
	pivot = FReferencePoint(jsonObject->GetObjectField(TEXT("pivot")));

	TArray<TSharedPtr<FJsonValue>> gripsarray = jsonObject->GetArrayField(TEXT("grips"));

	for (auto currentgrip : gripsarray)
	{
		grips.Add(FRadiusReferencePoint(currentgrip->AsObject()));
	}

	propType = jsonObject->GetIntegerField(TEXT("propType"));
}

FReferencePoint::FReferencePoint(TSharedPtr<FJsonObject> jsonObject)
{
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField(TEXT("position")));

	FVector TempVector = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField(TEXT("rotation")));
	rotation = TempVector.Rotation().Quaternion();
}

FRadiusReferencePoint::FRadiusReferencePoint(TSharedPtr<FJsonObject> jsonObject)
{
	radius = jsonObject->GetNumberField(TEXT("radius"));
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField(TEXT("position")));
	rotation = USmartsuitBlueprintLibrary::GetQuaternionField(jsonObject->GetObjectField(TEXT("rotation")));
}

FTracker::FTracker(TSharedPtr<FJsonObject> jsonObject)
{
	name = jsonObject->GetStringField(TEXT("name"));
	connectionId = jsonObject->GetStringField(TEXT("connectionId"));
	position = USmartsuitBlueprintLibrary::GetVectorField(jsonObject->GetObjectField(TEXT("position")));
	rotation = USmartsuitBlueprintLibrary::GetQuaternionField(jsonObject->GetObjectField(TEXT("rotation")));
	isLive = jsonObject->GetBoolField(TEXT("isLive"));
	trackingResult = jsonObject->GetIntegerField(TEXT("trackingResult"));
	trackerType = jsonObject->GetIntegerField(TEXT("trackerType"));
	RenderModelName = jsonObject->GetStringField(TEXT("RenderModelName"));
	battery = jsonObject->GetNumberField(TEXT("battery"));
}


FFace::FFace(const FFace& otherFace)
	: profileName(otherFace.profileName)
	, version(otherFace.version)
	, provider(otherFace.provider)
	, faceId(otherFace.faceId)
	, actorName(otherFace.actorName)
	, eyeBlinkLeft(otherFace.eyeBlinkLeft)
	, eyeLookDownLeft(otherFace.eyeLookDownLeft)
	, eyeLookInLeft(otherFace.eyeLookInLeft)
	, eyeLookOutLeft(otherFace.eyeLookOutLeft)
	, eyeLookUpLeft(otherFace.eyeLookUpLeft)
	, eyeSquintLeft(otherFace.eyeSquintLeft)
	, eyeWideLeft(otherFace.eyeWideLeft)
	, eyeBlinkRight(otherFace.eyeBlinkRight)
	, eyeLookDownRight(otherFace.eyeLookDownRight)
	, eyeLookInRight(otherFace.eyeLookInRight)
	, eyeLookOutRight(otherFace.eyeLookOutRight)
	, eyeLookUpRight(otherFace.eyeLookUpRight)
	, eyeSquintRight(otherFace.eyeSquintRight)
	, eyeWideRight(otherFace.eyeWideRight)
	, jawForward(otherFace.jawForward)
	, jawLeft(otherFace.jawLeft)
	, jawRight(otherFace.jawRight)
	, jawOpen(otherFace.jawOpen)
	, mouthClose(otherFace.mouthClose)
	, mouthFunnel(otherFace.mouthFunnel)
	, mouthPucker(otherFace.mouthPucker)
	, mouthLeft(otherFace.mouthLeft)
	, mouthRight(otherFace.mouthRight)
	, mouthSmileLeft(otherFace.mouthSmileLeft)
	, mouthSmileRight(otherFace.mouthSmileRight)
	, mouthFrownLeft(otherFace.mouthFrownLeft)
	, mouthFrownRight(otherFace.mouthFrownRight)
	, mouthDimpleLeft(otherFace.mouthDimpleLeft)
	, mouthDimpleRight(otherFace.mouthDimpleRight)
	, mouthStretchLeft(otherFace.mouthStretchLeft)
	, mouthStretchRight(otherFace.mouthStretchRight)
	, mouthRollLower(otherFace.mouthRollLower)
	, mouthRollUpper(otherFace.mouthRollUpper)
	, mouthShrugLower(otherFace.mouthShrugLower)
	, mouthShrugUpper(otherFace.mouthShrugUpper)
	, mouthPressLeft(otherFace.mouthPressLeft)
	, mouthPressRight(otherFace.mouthPressRight)
	, mouthLowerDownLeft(otherFace.mouthLowerDownLeft)
	, mouthLowerDownRight(otherFace.mouthLowerDownRight)
	, mouthUpperUpLeft(otherFace.mouthUpperUpLeft)
	, mouthUpperUpRight(otherFace.mouthUpperUpRight)
	, browDownLeft(otherFace.browDownLeft)
	, browDownRight(otherFace.browDownRight)
	, browInnerUp(otherFace.browInnerUp)
	, browOuterUpLeft(otherFace.browOuterUpLeft)
	, browOuterUpRight(otherFace.browOuterUpRight)
	, cheekPuff(otherFace.cheekPuff)
	, cheekSquintLeft(otherFace.cheekSquintLeft)
	, cheekSquintRight(otherFace.cheekSquintRight)
	, noseSneerLeft(otherFace.noseSneerLeft)
	, noseSneerRight(otherFace.noseSneerRight)
	, tongueOut(otherFace.tongueOut)
{
}

FFace::FFace(FFace&& otherFace)
	: profileName(MoveTemp(otherFace.profileName))
	, version(otherFace.version)
	, provider(MoveTemp(otherFace.provider))	
	, faceId(MoveTemp(otherFace.faceId))
	, actorName(MoveTemp(otherFace.actorName))
	, eyeBlinkLeft(otherFace.eyeBlinkLeft)
	, eyeLookDownLeft(otherFace.eyeLookDownLeft)
	, eyeLookInLeft(otherFace.eyeLookInLeft)
	, eyeLookOutLeft(otherFace.eyeLookOutLeft)
	, eyeLookUpLeft(otherFace.eyeLookUpLeft)
	, eyeSquintLeft(otherFace.eyeSquintLeft)
	, eyeWideLeft(otherFace.eyeWideLeft)
	, eyeBlinkRight(otherFace.eyeBlinkRight)
	, eyeLookDownRight(otherFace.eyeLookDownRight)
	, eyeLookInRight(otherFace.eyeLookInRight)
	, eyeLookOutRight(otherFace.eyeLookOutRight)
	, eyeLookUpRight(otherFace.eyeLookUpRight)
	, eyeSquintRight(otherFace.eyeSquintRight)
	, eyeWideRight(otherFace.eyeWideRight)
	, jawForward(otherFace.jawForward)
	, jawLeft(otherFace.jawLeft)
	, jawRight(otherFace.jawRight)
	, jawOpen(otherFace.jawOpen)
	, mouthClose(otherFace.mouthClose)
	, mouthFunnel(otherFace.mouthFunnel)
	, mouthPucker(otherFace.mouthPucker)
	, mouthLeft(otherFace.mouthLeft)
	, mouthRight(otherFace.mouthRight)
	, mouthSmileLeft(otherFace.mouthSmileLeft)
	, mouthSmileRight(otherFace.mouthSmileRight)
	, mouthFrownLeft(otherFace.mouthFrownLeft)
	, mouthFrownRight(otherFace.mouthFrownRight)
	, mouthDimpleLeft(otherFace.mouthDimpleLeft)
	, mouthDimpleRight(otherFace.mouthDimpleRight)
	, mouthStretchLeft(otherFace.mouthStretchLeft)
	, mouthStretchRight(otherFace.mouthStretchRight)
	, mouthRollLower(otherFace.mouthRollLower)
	, mouthRollUpper(otherFace.mouthRollUpper)
	, mouthShrugLower(otherFace.mouthShrugLower)
	, mouthShrugUpper(otherFace.mouthShrugUpper)
	, mouthPressLeft(otherFace.mouthPressLeft)
	, mouthPressRight(otherFace.mouthPressRight)
	, mouthLowerDownLeft(otherFace.mouthLowerDownLeft)
	, mouthLowerDownRight(otherFace.mouthLowerDownRight)
	, mouthUpperUpLeft(otherFace.mouthUpperUpLeft)
	, mouthUpperUpRight(otherFace.mouthUpperUpRight)
	, browDownLeft(otherFace.browDownLeft)
	, browDownRight(otherFace.browDownRight)
	, browInnerUp(otherFace.browInnerUp)
	, browOuterUpLeft(otherFace.browOuterUpLeft)
	, browOuterUpRight(otherFace.browOuterUpRight)
	, cheekPuff(otherFace.cheekPuff)
	, cheekSquintLeft(otherFace.cheekSquintLeft)
	, cheekSquintRight(otherFace.cheekSquintRight)
	, noseSneerLeft(otherFace.noseSneerLeft)
	, noseSneerRight(otherFace.noseSneerRight)
	, tongueOut(otherFace.tongueOut)
{
}

FFace::FFace(TSharedPtr<FJsonObject> jsonObject, const FString& InActorName)
{
	faceId = InActorName;
	actorName = InActorName;
	if (!jsonObject->TryGetStringField(TEXT("profileName"), profileName))
	{
		profileName = "NOPROFILENAME for " + faceId;
	}

	constexpr float scale{ 0.01f };
	
	eyeBlinkLeft = jsonObject->GetNumberField(TEXT("eyeBlinkLeft")) * scale;
	eyeLookDownLeft = jsonObject->GetNumberField(TEXT("eyeLookDownLeft")) * scale;
	eyeLookInLeft = jsonObject->GetNumberField(TEXT("eyeLookInLeft")) * scale;
	eyeLookOutLeft = jsonObject->GetNumberField(TEXT("eyeLookOutLeft")) * scale;
	eyeLookUpLeft = jsonObject->GetNumberField(TEXT("eyeLookUpLeft")) * scale;
	eyeSquintLeft = jsonObject->GetNumberField(TEXT("eyeSquintLeft")) * scale;
	eyeWideLeft = jsonObject->GetNumberField(TEXT("eyeWideLeft")) * scale;
	eyeBlinkRight = jsonObject->GetNumberField(TEXT("eyeBlinkRight")) * scale;
	eyeLookDownRight = jsonObject->GetNumberField(TEXT("eyeLookDownRight")) * scale;
	eyeLookInRight = jsonObject->GetNumberField(TEXT("eyeLookInRight")) * scale;
	eyeLookOutRight = jsonObject->GetNumberField(TEXT("eyeLookOutRight")) * scale;
	eyeLookUpRight = jsonObject->GetNumberField(TEXT("eyeLookUpRight")) * scale;
	eyeSquintRight = jsonObject->GetNumberField(TEXT("eyeSquintRight")) * scale;
	eyeWideRight = jsonObject->GetNumberField(TEXT("eyeWideRight")) * scale;
	jawForward = jsonObject->GetNumberField(TEXT("jawForward")) * scale;
	jawLeft = jsonObject->GetNumberField(TEXT("jawLeft")) * scale;
	jawRight = jsonObject->GetNumberField(TEXT("jawRight")) * scale;
	jawOpen = jsonObject->GetNumberField(TEXT("jawOpen")) * scale;
	mouthClose = jsonObject->GetNumberField(TEXT("mouthClose")) * scale;
	mouthFunnel = jsonObject->GetNumberField(TEXT("mouthFunnel")) * scale;
	mouthPucker = jsonObject->GetNumberField(TEXT("mouthPucker")) * scale;
	mouthLeft = jsonObject->GetNumberField(TEXT("mouthLeft")) * scale;
	mouthRight = jsonObject->GetNumberField(TEXT("mouthRight")) * scale;
	mouthSmileLeft = jsonObject->GetNumberField(TEXT("mouthSmileLeft")) * scale;
	mouthSmileRight = jsonObject->GetNumberField(TEXT("mouthSmileRight")) * scale;
	mouthFrownLeft = jsonObject->GetNumberField(TEXT("mouthFrownLeft")) * scale;
	mouthFrownRight = jsonObject->GetNumberField(TEXT("mouthFrownRight")) * scale;
	mouthDimpleLeft = jsonObject->GetNumberField(TEXT("mouthDimpleLeft")) * scale;
	mouthDimpleRight = jsonObject->GetNumberField(TEXT("mouthDimpleRight")) * scale;
	mouthStretchLeft = jsonObject->GetNumberField(TEXT("mouthStretchLeft")) * scale;
	mouthStretchRight = jsonObject->GetNumberField(TEXT("mouthStretchRight")) * scale;
	mouthRollLower = jsonObject->GetNumberField(TEXT("mouthRollLower")) * scale;
	mouthRollUpper = jsonObject->GetNumberField(TEXT("mouthRollUpper")) * scale;
	mouthShrugLower = jsonObject->GetNumberField(TEXT("mouthShrugLower")) * scale;
	mouthShrugUpper = jsonObject->GetNumberField(TEXT("mouthShrugUpper")) * scale;
	mouthPressLeft = jsonObject->GetNumberField(TEXT("mouthPressLeft")) * scale;
	mouthPressRight = jsonObject->GetNumberField(TEXT("mouthPressRight")) * scale;
	mouthLowerDownLeft = jsonObject->GetNumberField(TEXT("mouthLowerDownLeft")) * scale;
	mouthLowerDownRight = jsonObject->GetNumberField(TEXT("mouthLowerDownRight")) * scale;
	mouthUpperUpLeft = jsonObject->GetNumberField(TEXT("mouthUpperUpLeft")) * scale;
	mouthUpperUpRight = jsonObject->GetNumberField(TEXT("mouthUpperUpRight")) * scale;
	browDownLeft = jsonObject->GetNumberField(TEXT("browDownLeft")) * scale;
	browDownRight = jsonObject->GetNumberField(TEXT("browDownRight")) * scale;
	browInnerUp = jsonObject->GetNumberField(TEXT("browInnerUp")) * scale;
	browOuterUpLeft = jsonObject->GetNumberField(TEXT("browOuterUpLeft")) * scale;
	browOuterUpRight = jsonObject->GetNumberField(TEXT("browOuterUpRight")) * scale;
	cheekPuff = jsonObject->GetNumberField(TEXT("cheekPuff")) * scale;
	cheekSquintLeft = jsonObject->GetNumberField(TEXT("cheekSquintLeft")) * scale;
	cheekSquintRight = jsonObject->GetNumberField(TEXT("cheekSquintRight")) * scale;
	noseSneerLeft = jsonObject->GetNumberField(TEXT("noseSneerLeft")) * scale;
	noseSneerRight = jsonObject->GetNumberField(TEXT("noseSneerRight")) * scale;
	tongueOut = jsonObject->GetNumberField(TEXT("tongueOut")) * scale;
}

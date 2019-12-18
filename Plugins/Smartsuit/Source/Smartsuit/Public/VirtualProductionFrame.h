// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
//#include "SmartsuitBlueprintLibrary.h"
#include "VirtualProductionFrame.generated.h"


//#include "JsonUtilities.h"

class VirtualProductionFrame
{
public:
	VirtualProductionFrame();
	~VirtualProductionFrame();
};

USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about a grip."))
struct FRadiusReferencePoint {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Grip radius."))
	/**Holds information about the radius.*/
	float radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Grip position."))
	/**Holds information about the position during the last frame.*/
	FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Grip rotation."))
	/**Holds information about the rotation during the last frame.*/
	FQuat rotation;

	FRadiusReferencePoint() {}
	FRadiusReferencePoint(TSharedPtr<FJsonObject> jsonObject);
};

USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about the pivot."))
struct FReferencePoint {

	GENERATED_USTRUCT_BODY()

	/**Holds information about the position during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Pivot position."))
	FVector position;

	/**Holds information about the rotation during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Pivot rotation."))
	FQuat rotation;

	FReferencePoint() {}
	FReferencePoint(TSharedPtr<FJsonObject> jsonObject);
};

USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about a prop's profile."))
struct FProfile {

	GENERATED_USTRUCT_BODY()

	/** Name of the public profile. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's name."))
	FString name;

	/** Name of the uuid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's uuid."))
	FString uuid;

	/** Holds information about the dimensions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's dimensions."))
	FVector dimensions;

	/** Holds information about the color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's color."))
	FColor color;

	/** Tracker offset position and rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Tracker Offset."))
	FReferencePoint trackerOffset;

	/** Pivot position and rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Pivot."))
	FReferencePoint pivot;

	/** List of Grips. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "List of grips."))
	TArray<FRadiusReferencePoint> grips;

	/** Prop type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's prop type."))
	int propType;

	FProfile() {}

	FProfile(TSharedPtr<FJsonObject> jsonObject);
};

struct FVirtualProductionSubject {
	FVector position;
	FQuat rotation;
	FName name;

	FVirtualProductionSubject(FVector in_position, FQuat in_rotation, FName in_name) {
		position = in_position;
		rotation = in_rotation;
		name = in_name;
	}
};

/*! \brief Information about a specific props, including its status, rotation, position.*/
USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about a prop."))
struct FProp {

	GENERATED_USTRUCT_BODY()

	/** Name of the prop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Prop's name."))
	FString name;

	/** ID of the prop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Prop's id."))
	FString id;

	/**Holds information about the position during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Position information."))
	FVector position;

	/**Holds information about the rotation during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Rotation information."))
	FQuat rotation;

	/**Indicates wether the prop is live.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about whether a prop is live or not."))
	bool isLive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about prop's profile."))
	FProfile profile;

	FProp() {}

	FProp(TSharedPtr<FJsonObject> jsonObject);

	///**
	//* Get sensor position in Unreal coordinate system.
	//*
	//* @return Sensor position.
	//*/
	FVector UPosition() {
		return FVector(100.0f*position.Z, 100.0f*position.X, 100.0f*position.Y);
	}

	FRotator FQuatToRotator() {
		FQuat result(rotation.Z, rotation.X, rotation.Y, rotation.W);
		FRotator Rotator = result.Rotator();
		return Rotator;
	}

	FVirtualProductionSubject GetSubject() {
		FQuat quat = FQuat(rotation.Z, rotation.X, rotation.Y, rotation.W);
		FVector pos = UPosition();
		return FVirtualProductionSubject(pos, quat, FName(*(FString("prop:") + FString(isLive ? "L:" : "P:") + FString(name))));
	}
};


/*! \brief Information about a specific tracker, including its status, rotation, position.*/
USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about a tracker."))
struct FTracker {

	GENERATED_USTRUCT_BODY()

	FTracker() {}
	FTracker(TSharedPtr<FJsonObject> jsonObject);

	/** Name of the tracker. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Tracker's name."))
	FString name;

	/** ID of the tracker. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Tracker's connection id."))
	FString connectionId;

	/**Holds information about the position during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Position information."))
	FVector position;

	/**Holds information about the rotation during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Rotation information."))
	FQuat rotation;

	/**Indicates whether the tracker is live.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about whether a tracker is live or not."))
	bool isLive;

	/**Holds the tracking result.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about the result of the tracking."))
	int trackingResult;

	/**Indicates the type of the tracker.*/
	UPROPERTY()
	int trackerType;

	/**.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Render Model Name."))
	FString RenderModelName;

	/**.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Battery information."))
	float battery;

	///**
	//* Get sensor position in Unreal coordinate system.
	//*
	//* @return Sensor position.
	//*/
	FVector UPosition() {
		return FVector(100.0f*position.Z, 100.0f*position.X, 100.0f*position.Y);
	}

	FRotator FQuatToRotator() {
		FQuat result(rotation.Z, rotation.X, rotation.Y, rotation.W);
		FRotator Rotator = result.Rotator();
		return Rotator;
	}

	FVirtualProductionSubject GetSubject() {

		FQuat quat = FQuat(rotation.Z, rotation.X, rotation.Y, rotation.W);
		FVector pos = UPosition();

		return FVirtualProductionSubject(pos, quat, FName(*(FString("tracker:") + FString(isLive ? "L:" : "P:") + FString(name))));
	}

};

USTRUCT(BlueprintType, meta = (ToolTip = "Contains all the information for face."))
struct FFace
{
	GENERATED_USTRUCT_BODY()

	FFace() {}
	FFace(TSharedPtr<FJsonObject> jsonObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Face's version."))
	int version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Face's provider."))
	FString provider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Face's ID."))
	FString faceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeBlinkLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookDownLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookInLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookOutLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookUpLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeSquintLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeWideLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeBlinkRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookDownRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookInRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookOutRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookUpRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeSquintRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeWideRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthClose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthFunnel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthPucker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthSmileLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthSmileRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthFrownLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthFrownRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthDimpleLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthDimpleRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthStretchLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthStretchRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthRollLower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthRollUpper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthShrugLower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthShrugUpper;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthPressLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthPressRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthLowerDownLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthLowerDownRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthUpperUpLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthUpperUpRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browDownLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browDownRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browInnerUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browOuterUpLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browOuterUpRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float cheekPuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float cheekSquintLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float cheekSquintRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float noseSneerLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float noseSneerRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float tongueOut;

	FName GetSubjectName() 
	{
		return FName(*faceId);
	}
};

/*! \brief Information about a specific tracker, including its status, rotation, position.*/
USTRUCT(BlueprintType, meta = (ToolTip = "The Virtual Producion frame contains all props' and trackers' information."))
struct FVirtualProductionFrame {

	GENERATED_USTRUCT_BODY()

	/**Indicates the VP frame version.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Virtual Production version."))
	int version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "List of props."))
	/** Array of props. */
	TArray<FProp> props;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "List of trackers."))
	/** Array of trackers. */
	TArray<FTracker> trackers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "List of faces."))
	TArray<FFace> faces;
	//FVirtualProductionFrame (){}
};

struct LiveLinkSuit {

};
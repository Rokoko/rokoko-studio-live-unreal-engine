// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	//FRadiusReferencePoint() {}
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

	//FReferencePoint() {}
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

	//FProfile() {}
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

	//FProp() {}

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
		return FVirtualProductionSubject(UPosition(), FQuatToRotator().Quaternion(), FName(*(FString("prop:") + FString(name))));
	}
};


/*! \brief Information about a specific tracker, including its status, rotation, position.*/
USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about a tracker."))
struct FTracker {

	GENERATED_USTRUCT_BODY()

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
		return FVirtualProductionSubject(UPosition(), FQuatToRotator().Quaternion(), FName(*(FString("tracker:") + FString(name))));
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

	//FVirtualProductionFrame (){}
};
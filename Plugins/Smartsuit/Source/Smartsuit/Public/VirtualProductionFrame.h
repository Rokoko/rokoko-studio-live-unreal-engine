// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
#include "Runtime/Launch/Resources/Version.h"
#include "RokokoSkeletonData.h"
#include "VirtualProductionFrame.generated.h"


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
	float radius{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Grip position."))
	/**Holds information about the position during the last frame.*/
	FVector position{ FVector::ZeroVector };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Grip rotation."))
	/**Holds information about the rotation during the last frame.*/
	FQuat rotation{ FQuat::Identity };

	FRadiusReferencePoint() {}
	FRadiusReferencePoint(TSharedPtr<FJsonObject> jsonObject);
};

USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about the pivot."))
struct FReferencePoint {

	GENERATED_USTRUCT_BODY()

	/**Holds information about the position during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Pivot position."))
	FVector position{ FVector::ZeroVector };

	/**Holds information about the rotation during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Pivot rotation."))
	FQuat rotation{ FQuat::Identity };

	FReferencePoint() {}
	FReferencePoint(TSharedPtr<FJsonObject> jsonObject);
};

USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about a prop's profile."))
struct FProfile 
{

	GENERATED_USTRUCT_BODY()

	/** Name of the public profile. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's name."))
	FString name;

	/** Name of the uuid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's uuid."))
	FString uuid;

	/** Holds information about the dimensions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's dimensions."))
	FVector dimensions{ FVector::OneVector };

	/** Holds information about the color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Profile's color."))
	FLinearColor color{ FLinearColor::White };

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
	int propType{ 0 };

	FProfile() {}

	FProfile(TSharedPtr<FJsonObject> jsonObject);
};

/// <summary>
/// One tracker or prop in the scene
/// </summary>
struct FVirtualProductionSubject {
	FVector Position;
	FQuat Rotation;
	FName Name;

	FVirtualProductionSubject(FVector in_position, FQuat in_rotation, FName in_name) {
		Position = in_position;
		Rotation = in_rotation;
		Name = in_name;
	}
};

/*! \brief Information about a specific props, including its status, rotation, position.*/
USTRUCT(BlueprintType, meta = (ToolTip = "Contains all information about a prop."))
struct FProp {

	GENERATED_USTRUCT_BODY()

	/** Name of the prop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Prop's name."))
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Rotation information."))
	FColor color{ FColor::White };

	/** ID of the prop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Prop's id."))
	FString id;

	/**Holds information about the position during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Position information."))
	FVector position{ FVector::ZeroVector };

	/**Holds information about the rotation during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Rotation information."))
	FQuat rotation{ FQuat::Identity };

	/**Indicates wether the prop is live.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about whether a prop is live or not."))
	bool isLive{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about prop's profile."))
	FProfile profile;

	FProp() {}

	FProp(bool InIsLive, TSharedPtr<FJsonObject> jsonObject);

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
		return FVirtualProductionSubject(pos, quat, FName(*(FString("prop:") + /*FString(isLive ? "L:" : "P:") +*/ FString(name))));
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
	FVector position{ FVector::ZeroVector };

	/**Holds information about the rotation during the last frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Rotation information."))
	FQuat rotation{ FQuat::Identity };

	/**Indicates whether the tracker is live.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about whether a tracker is live or not."))
	bool isLive{ false };

	/**Holds the tracking result.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Information about the result of the tracking."))
	int trackingResult{ 0 };

	/**Indicates the type of the tracker.*/
	UPROPERTY()
	int trackerType{ 0 };

	/**.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Render Model Name."))
	FString RenderModelName;

	/**.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Battery information."))
	float battery{ 0.0f };

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
	FFace(const FFace& otherFace);
	FFace(FFace&& otherFace);
	FFace(TSharedPtr<FJsonObject> jsonObject, const FString& InActorName);

	FFace& operator = (const FFace& otherFace) = default;

	UPROPERTY(BlueprintReadOnly, Category = "Virtual Production")
	FString profileName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Face's version."))
	int version{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Face's provider."))
	FString provider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Face's ID."))
	FString faceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Face's ID."))
	FString actorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeBlinkLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookDownLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookInLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookOutLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookUpLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeSquintLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeWideLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeBlinkRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookDownRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookInRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookOutRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeLookUpRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeSquintRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float eyeWideRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawForward{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float jawOpen{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthClose{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthFunnel{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthPucker{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthSmileLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthSmileRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthFrownLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthFrownRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthDimpleLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthDimpleRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthStretchLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthStretchRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthRollLower{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthRollUpper{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthShrugLower{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthShrugUpper{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthPressLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthPressRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthLowerDownLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthLowerDownRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthUpperUpLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float mouthUpperUpRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browDownLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browDownRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browInnerUp{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browOuterUpLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float browOuterUpRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float cheekPuff{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float cheekSquintLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float cheekSquintRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float noseSneerLeft{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float noseSneerRight{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Faces blendshape"))
	float tongueOut{ 0.0f };

	FName GetSubjectName() const
	{
		FString TempSubjectName = "actor:" + actorName + ":face";
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
		return FName(TempSubjectName);
#else
		return FName(*TempSubjectName);
#endif
	}
};

/*! \brief Information about a specific tracker, including its status, rotation, position.*/
USTRUCT(BlueprintType, meta = (ToolTip = "The Virtual Production frame contains all props' and trackers' information."))
struct FVirtualProductionFrame {

	GENERATED_USTRUCT_BODY()

	/**Indicates the VP frame version.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "Virtual Production version."))
	FString Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "List of props."))
	/** Array of props. */
	TArray<FProp> Props;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "List of trackers."))
	/** Array of trackers. */
	TArray<FTracker> Trackers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production", meta = (ToolTip = "List of faces."))
	TArray<FFace> Faces;

	UPROPERTY()
	TArray<FSuitData> Actors;

	UPROPERTY()
	TArray<FCharacterData> Characters;

	UPROPERTY()
	TArray<FNewtonData> Newtons;

};

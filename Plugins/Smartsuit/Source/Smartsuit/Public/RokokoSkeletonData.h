// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "Runtime/Launch/Resources/Version.h"

#include "RokokoSkeletonData.generated.h"

/*! \file SmartsuitDefinitions.h
\brief This file includes all definitions used by Smartsuit plugin.
*
* The main structs, command definitions, sensor mapping and rotation mappings are defined in this class.
*/


/*! \brief Expected rotation of the hip in T-Pose. */
#define SMARTSUIT_TPOSE_HIP					FQuat::MakeFromEuler(FVector(89.999954, 0.000055, -0.000005))
/*! \brief Expected rotation of the stomach in T-Pose. */
#define SMARTSUIT_TPOSE_STOMACH				FQuat::MakeFromEuler(FVector(-89.999954, -0.000171, 180.000000)) 
/*! \brief Expected rotation of the chest in T-Pose. */
#define SMARTSUIT_TPOSE_CHEST				FQuat::MakeFromEuler(FVector(-89.999985, -0.000171, 180.000000)) 
/*! \brief Expected rotation of the neck in T-Pose. */
#define SMARTSUIT_TPOSE_NECK				FQuat::MakeFromEuler(FVector(-90.000053, -0.000191, 180.000000))
/*! \brief Expected rotation of the head in T-Pose. */
#define SMARTSUIT_TPOSE_HEAD				FQuat::MakeFromEuler(FVector(-89.999893,-0.000137,180.000000))
/*! \brief Expected rotation of the left shoulder in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_SHOULDER		FQuat::MakeFromEuler(FVector(-75.371887, 90.000000,-170.371826))
/*! \brief Expected rotation of the left arm in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_ARM			FQuat::MakeFromEuler(FVector(179.999969, 0.000014,89.999954))
/*! \brief Expected rotation of the left forearm in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_FOREARM		FQuat::MakeFromEuler(FVector(179.999969, 0.000014,89.999908))
/*! \brief Expected rotation of the left hand in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_HAND			FQuat::MakeFromEuler(FVector(179.999969, 0.000055,90.000099))
/*! \brief Expected rotation of the right shoulder in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_SHOULDER		FQuat::MakeFromEuler(FVector(87.917419, -90.000000,2.082565))
/*! \brief Expected rotation of the right arm in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_ARM			FQuat::MakeFromEuler(FVector(179.999817, -0.000075,-90.000023))
/*! \brief Expected rotation of the right forearm in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_FOREARM		FQuat::MakeFromEuler(FVector(179.999847,-0.000075,-90.000053))
/*! \brief Expected rotation of the right hand in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_HAND			FQuat::MakeFromEuler(FVector(-179.999969,-0.000075,-90.000046))
/*! \brief Expected rotation of the left up leg in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_UP_LEG			FQuat::MakeFromEuler(FVector(90.000145,0.000041,89.999908))
/*! \brief Expected rotation of the left leg in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_LEG			FQuat::MakeFromEuler(FVector(90.000107,0.000041,89.999878))
/*! \brief Expected rotation of the left foot in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_FOOT			FQuat::MakeFromEuler(FVector(179.999863,-0.000109,179.999802))

#define SMARTSUIT_TPOSE_LEFT_TOE			FQuat::MakeFromEuler(FVector(179.999863,-0.000109,179.999802))

/*! \brief Expected rotation of the right up leg in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_UP_LEG		FQuat::MakeFromEuler(FVector(89.999855,-0.000041,-89.999947))
/*! \brief Expected rotation of the right leg in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_LEG			FQuat::MakeFromEuler(FVector(89.999825,-0.000041,-89.999962))
/*! \brief Expected rotation of the right foot in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_FOOT			FQuat::MakeFromEuler(FVector(-179.999893,-0.000171,-179.999939))

#define SMARTSUIT_TPOSE_RIGHT_TOE			FQuat::MakeFromEuler(FVector(-179.999893,-0.000171,-179.999939))

#define SMARTSUIT_TPOSE_LEFT_THUMB_PROXIMAL		FQuat::MakeFromEuler(FVector(-179.999985,-44.999752,119.999985))		//(Pitch=-44.999752,Yaw=119.999985,Roll=-179.999985)
#define SMARTSUIT_TPOSE_LEFT_THUMB_MEDIAL		FQuat::MakeFromEuler(FVector(179.999863,-44.999660,90.000114))			//(Pitch=-44.999660,Yaw=90.000114,Roll=179.999863)
#define SMARTSUIT_TPOSE_LEFT_THUMB_DISTAL		FQuat::MakeFromEuler(FVector(179.999847,-44.999660,90.000107))			//(Pitch=-44.999660,Yaw=90.000107,Roll=179.999847)
#define SMARTSUIT_TPOSE_LEFT_THUMB_TIP			FQuat::MakeFromEuler(FVector(179.999817,-44.999516,90.000259))			//(Pitch=-44.999516,Yaw=90.000259,Roll=-179.999817)

#define SMARTSUIT_TPOSE_LEFT_INDEX_PROXIMAL		FQuat::MakeFromEuler(FVector(179.999954,0.000096,89.999992))			//(Pitch=0.000096,Yaw=89.999992,Roll=179.999954)
#define SMARTSUIT_TPOSE_LEFT_INDEX_MEDIAL		FQuat::MakeFromEuler(FVector(180.000000,0.000096,89.999985))			//(Pitch=0.000096,Yaw=89.999985,Roll=180.000000)
#define SMARTSUIT_TPOSE_LEFT_INDEX_DISTAL		FQuat::MakeFromEuler(FVector(-179.999985,0.000096,89.999977))			//(Pitch=0.000096,Yaw=89.999977,Roll=-179.999985)
#define SMARTSUIT_TPOSE_LEFT_INDEX_TIP			FQuat::MakeFromEuler(FVector(-179.999954,0.000362,90.000183))			//(Pitch=0.000362,Yaw=90.000183,Roll=-179.999954)

#define SMARTSUIT_TPOSE_LEFT_MIDDLE_PROXIMAL	FQuat::MakeFromEuler(FVector(179.999954,0.000096,90.000069))			//(Pitch=0.000096,Yaw=90.000069,Roll=179.999954)
#define SMARTSUIT_TPOSE_LEFT_MIDDLE_MEDIAL		FQuat::MakeFromEuler(FVector(-179.999985,0.000096,90.000046))			//(Pitch=0.000096,Yaw=90.000046,Roll=-179.999985)
#define SMARTSUIT_TPOSE_LEFT_MIDDLE_DISTAL		FQuat::MakeFromEuler(FVector(-179.999985,0.000096,90.000069))			//(Pitch=0.000096,Yaw=90.000069,Roll=-179.999985)
#define SMARTSUIT_TPOSE_LEFT_MIDDLE_TIP			FQuat::MakeFromEuler(FVector(-179.999969,0.000280,90.000168))			//(Pitch=0.000280,Yaw=90.000168,Roll=-179.999969)

#define SMARTSUIT_TPOSE_LEFT_RING_PROXIMAL		FQuat::MakeFromEuler(FVector(179.999969,0.000096,89.999954))			//(Pitch=0.000096,Yaw=89.999954,Roll=179.999969)
#define SMARTSUIT_TPOSE_LEFT_RING_MEDIAL		FQuat::MakeFromEuler(FVector(179.999954,0.000096,89.999954))			//(Pitch=0.000096,Yaw=89.999954,Roll=179.999954)
#define SMARTSUIT_TPOSE_LEFT_RING_DISTAL		FQuat::MakeFromEuler(FVector(179.999954,0.000096,89.999962))			//(Pitch=0.000096,Yaw=89.999962,Roll=179.999954)
#define SMARTSUIT_TPOSE_LEFT_RING_TIP			FQuat::MakeFromEuler(FVector(179.999985,0.000137,90.000168))			//(Pitch=0.000137,Yaw=90.000168,Roll=179.999985)

#define SMARTSUIT_TPOSE_LEFT_LITTLE_PROXIMAL	FQuat::MakeFromEuler(FVector(180.000000,0.000096,89.999947))			//(Pitch=0.000096,Yaw=89.999947,Roll=180.000000)
#define SMARTSUIT_TPOSE_LEFT_LITTLE_MEDIAL		FQuat::MakeFromEuler(FVector(-179.999985,0.000096,89.999931))			//(Pitch=0.000096,Yaw=89.999931,Roll=-179.999985)
#define SMARTSUIT_TPOSE_LEFT_LITTLE_DISTAL		FQuat::MakeFromEuler(FVector(-179.999969,0.000096,89.999939))			//(Pitch=0.000096,Yaw=89.999939,Roll=-179.999969)
#define SMARTSUIT_TPOSE_LEFT_LITTLE_TIP			FQuat::MakeFromEuler(FVector(179.999985,0.000116,90.000145))			//(Pitch=0.000116,Yaw=90.000145,Roll=179.999985)



#define SMARTSUIT_TPOSE_RIGHT_THUMB_PROXIMAL	FQuat::MakeFromEuler(FVector(179.999893,44.999851,-120.000008))			//(Pitch=44.999851,Yaw=-120.000008,Roll=179.999893)
#define SMARTSUIT_TPOSE_RIGHT_THUMB_MEDIAL		FQuat::MakeFromEuler(FVector(179.999817,44.999809,-90.000053))			//(Pitch=44.999809,Yaw=-90.000053,Roll=179.999817)
#define SMARTSUIT_TPOSE_RIGHT_THUMB_DISTAL		FQuat::MakeFromEuler(FVector(179.999786,44.999809,-90.000092))			//(Pitch=44.999809,Yaw=-90.000092,Roll=179.999786)
#define SMARTSUIT_TPOSE_RIGHT_THUMB_TIP			FQuat::MakeFromEuler(FVector(-179.999878,44.999706,-90.000023))			//(Pitch=44.999706,Yaw=-90.000023,Roll=-179.999878)

#define SMARTSUIT_TPOSE_RIGHT_INDEX_PROXIMAL	FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000046))			//(Pitch=-0.000082,Yaw=-90.000046,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_INDEX_MEDIAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000031))			//(Pitch=-0.000082,Yaw=-90.000031,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_INDEX_DISTAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000046))			//(Pitch=-0.000082,Yaw=-90.000046,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_INDEX_TIP			FQuat::MakeFromEuler(FVector(179.999969,-0.000219,-89.999985))			//(Pitch=-0.000219,Yaw=-89.999985,Roll=179.999969)

#define SMARTSUIT_TPOSE_RIGHT_MIDDLE_PROXIMAL	FQuat::MakeFromEuler(FVector(179.999939,-0.000082,-90.000053))			//(Pitch=-0.000082,Yaw=-90.000053,Roll=179.999939)
#define SMARTSUIT_TPOSE_RIGHT_MIDDLE_MEDIAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000069))			//(Pitch=-0.000082,Yaw=-90.000069,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_MIDDLE_DISTAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000076))			//(Pitch=-0.000082,Yaw=-90.000076,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_MIDDLE_TIP		FQuat::MakeFromEuler(FVector(-180.000000,-0.000171,-90.000008))			//(Pitch=-0.000171,Yaw=-90.000008,Roll=-180.000000)

#define SMARTSUIT_TPOSE_RIGHT_RING_PROXIMAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000053))			//(Pitch=-0.000082,Yaw=-90.000053,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_RING_MEDIAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000069))			//(Pitch=-0.000082,Yaw=-90.000069,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_RING_DISTAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000076))			//(Pitch=-0.000082,Yaw=-90.000076,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_RING_TIP			FQuat::MakeFromEuler(FVector(179.999969,-0.000191,-90.000008))			//(Pitch=-0.000191,Yaw=-90.000008,Roll=179.999969)

#define SMARTSUIT_TPOSE_RIGHT_LITTLE_PROXIMAL	FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000053))			//(Pitch=-0.000082,Yaw=-90.000053,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_LITTLE_MEDIAL		FQuat::MakeFromEuler(FVector(179.999954,-0.000082,-90.000069))			//(Pitch=-0.000082,Yaw=-90.000069,Roll=179.999954)
#define SMARTSUIT_TPOSE_RIGHT_LITTLE_DISTAL		FQuat::MakeFromEuler(FVector(179.999985,-0.000082,-90.000076))			//(Pitch=-0.000082,Yaw=-90.000076,Roll=179.999985)
#define SMARTSUIT_TPOSE_RIGHT_LITTLE_TIP		FQuat::MakeFromEuler(FVector(179.999985,-0.000171,-90.000008))			//(Pitch=-0.000171,Yaw=-90.000008,Roll=179.999985)


#define SMARTSUIT_TPOSE_FINGERJOINT_TEST	FQuat::MakeFromEuler(FVector(0.0,0.0,0.0))

namespace SmartsuitBones
{
	extern const FName hip;
	extern const FName spine;
	extern const FName chest;
	extern const FName neck;
	extern const FName head;
	extern const FName leftShoulder;
	extern const FName leftUpperArm;
	extern const FName leftLowerArm;
	extern const FName leftHand;
	extern const FName rightShoulder;
	extern const FName rightUpperArm;
	extern const FName rightLowerArm;
	extern const FName rightHand;
	extern const FName leftUpLeg;
	extern const FName leftLeg;
	extern const FName leftFoot;
	extern const FName leftToe;
	extern const FName leftToeEnd;
	extern const FName rightUpLeg;
	extern const FName rightLeg;
	extern const FName rightFoot;
	extern const FName rightToe;
	extern const FName rightToeEnd;
	extern const FName leftThumbProximal;
	extern const FName leftThumbMedial;
	extern const FName leftThumbDistal;
	extern const FName leftThumbTip;
	extern const FName leftIndexProximal;
	extern const FName leftIndexMedial;
	extern const FName leftIndexDistal;
	extern const FName leftIndexTip;
	extern const FName leftMiddleProximal;
	extern const FName leftMiddleMedial;
	extern const FName leftMiddleDistal;
	extern const FName leftMiddleTip;
	extern const FName leftRingProximal;
	extern const FName leftRingMedial;
	extern const FName leftRingDistal;
	extern const FName leftRingTip;
	extern const FName leftLittleProximal;
	extern const FName leftLittleMedial;
	extern const FName leftLittleDistal;
	extern const FName leftLittleTip;
	extern const FName rightThumbProximal;
	extern const FName rightThumbMedial;
	extern const FName rightThumbDistal;
	extern const FName rightThumbTip;
	extern const FName rightIndexProximal;
	extern const FName rightIndexMedial;
	extern const FName rightIndexDistal;
	extern const FName rightIndexTip;
	extern const FName rightMiddleProximal;
	extern const FName rightMiddleMedial;
	extern const FName rightMiddleDistal;
	extern const FName rightMiddleTip;
	extern const FName rightRingProximal;
	extern const FName rightRingMedial;
	extern const FName rightRingDistal;
	extern const FName rightRingTip;
	extern const FName rightLittleProximal;
	extern const FName rightLittleMedial;
	extern const FName rightLittleDistal;
	extern const FName rightLittleTip;
}

USTRUCT()
struct FRokokoCharacterJoint
{
	GENERATED_BODY()
	
	FRokokoCharacterJoint() {}
	FRokokoCharacterJoint(FName Name, int32 ParentIndex, const FTransform& Transform)
		: name(Name)
		, parentIndex(ParentIndex)
		, transform(Transform)
	{}
	FRokokoCharacterJoint(FName Name, int32 ParentIndex, const FTransform& Transform, FVector& Position, FQuat& Rotation)
		: name(Name)
		, parentIndex(ParentIndex)
		, transform(Transform)
		, position(Position)
		, rotation(Rotation)
	{}

	FName name;
	int32 parentIndex;
	FTransform transform;
	FVector position;
	FQuat rotation;
};

USTRUCT()
struct FSmartsuitBone
{
	GENERATED_BODY()

	FSmartsuitBone() {}
	FSmartsuitBone(FName Name, FVector Position, FQuat Rotation)
		: name(Name)
		, position(Position)
		, rotation(Rotation)
	{}

	FQuat NED2Unreal(FQuat InRotation)
	{
		FQuat result(InRotation.X, InRotation.Y, InRotation.Z, InRotation.W);
		result.Z = -result.Z;
		result.Y = -result.Y;

		static FQuat modifier = FQuat::MakeFromEuler(FVector(180, 0, 90));
		static FQuat postModifier = FQuat::MakeFromEuler(FVector(0, 0, 180));
		const FQuat finalResult = modifier * result * postModifier;

		return FQuat(finalResult.X, finalResult.Y, finalResult.Z, finalResult.W);
	}

	FQuat Uquaternion() const
	{
		FQuat result(rotation.Z, rotation.X, rotation.Y, rotation.W);

		static FQuat modifier = FQuat::MakeFromEuler(FVector(90, 0, -90));
		result *= modifier;

		return result;
	}

	FVector UPosition() const
	{
		return FVector(WORLD_SCALE * position.Z, WORLD_SCALE * position.X, WORLD_SCALE * position.Y);
	}

	FName name;
	FVector position;
	FQuat rotation;

	constexpr static float WORLD_SCALE{ 100.0f };
};

/*! \brief Contains data that represent the last frame received from the Rokoko Actor.
*
* This struct represents a skeleton data frame as received from the Studio Actor.
* It also includes meta variables used to manage the state of the Actor in Unreal.
*/
USTRUCT(BlueprintType)
struct FSuitData
{
	GENERATED_BODY()

	FSuitData() {}
	
	/** The name of the Smartsuit. */
	UPROPERTY(BlueprintReadOnly, Category=Default)
	FString suitname;

	uint32_t timestamp{ 0 };
	
	UPROPERTY(BlueprintReadOnly, Category=Default)
	FString id;

	UPROPERTY(BlueprintReadOnly, Category=Default)
	bool isLive{ false };

	/** The name of the profile. */
	UPROPERTY(BlueprintReadOnly, Category = Default)
	FString profileName;

	UPROPERTY(BlueprintReadOnly, Category = Default)
	bool hasGloves{ false };

	UPROPERTY(BlueprintReadOnly, Category = Default)
	bool hasLeftGlove{ false };

	UPROPERTY(BlueprintReadOnly, Category = Default)
	bool hasRightGlove{ false };

	UPROPERTY(BlueprintReadOnly, Category = Default)
	bool hasBody{ false };

	UPROPERTY(BlueprintReadOnly, Category = Default)
	bool hasFace{ false };

	UPROPERTY(BlueprintReadOnly, Category = Default)
	FString faceId;

	UPROPERTY(BlueprintReadOnly, Category = Default)
	FLinearColor color{ FLinearColor::White };

	TMap<FName, FSmartsuitBone> bones;

	const FSmartsuitBone* Hip() const
	{
		return GetBoneByName("hip");
	}

	const FSmartsuitBone* GetBoneByName(const FName& BoneName) const
	{
		return bones.Find(BoneName);
	}

	FName GetSubjectName() const
	{
		FString TempSubjectName = "actor:" + suitname + ":body";
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
		return FName(TempSubjectName);
		#else
		return FName(*TempSubjectName);
		#endif
	}
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY()

	FCharacterData(){}

	/** The name of the Smartsuit. */
	UPROPERTY(BlueprintReadOnly, Category=Default)
	FString CharacterName;

	uint32_t Timestamp{ 0 };
	
	UPROPERTY(BlueprintReadOnly, Category=Default)
	FString Id;

	UPROPERTY(BlueprintReadOnly, Category=Default)
	bool IsLive{ false };


	TArray<FRokokoCharacterJoint> joints;


	FName GetSubjectName() const
	{
		FString TempSubjectName = "character:" + CharacterName + ":body";
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
		return FName(TempSubjectName);
#else
		return FName(*TempSubjectName);
#endif
	}
};

USTRUCT(BlueprintType)
struct FNewtonData
{
	GENERATED_BODY()

	FNewtonData() {}

	/** The name of the Actor. */
	UPROPERTY(BlueprintReadOnly, Category = Default)
	FString NewtonName;

	uint32_t Timestamp{ 0 };

	UPROPERTY(BlueprintReadOnly, Category = Default)
	FString Id;

	UPROPERTY(BlueprintReadOnly, Category = Default)
	bool IsLive{ false };

	UPROPERTY(BlueprintReadOnly, Category = Default)
	bool HasFace{ false };

	TArray<FRokokoCharacterJoint> Joints;


	FName GetSubjectName() const
	{
		FString TempSubjectName = "newton:" + NewtonName + ":body";
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
		return FName(TempSubjectName);
#else
		return FName(*TempSubjectName);
#endif
	}
};

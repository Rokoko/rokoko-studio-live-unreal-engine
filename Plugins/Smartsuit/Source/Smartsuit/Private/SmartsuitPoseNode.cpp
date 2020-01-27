// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitPoseNode.h"
#include "Smartsuit.h"
#include "AnimationRuntime.h"
#include "SmartsuitDefinitions.h"
#include "Animation/AnimInstanceProxy.h"




/////////////////////////////////////////////////////
// SmartsuitPoseNode

FSmartsuitPoseNode::FSmartsuitPoseNode()
{
	//UE_LOG(LogTemp, Warning, TEXT("Smartsuit Pose Node: Initializing..."));
	//TPose = new SmartsuitTPose();

	Controller = nullptr;
	RelativeToStart = false;
	ScaleBones = false;

	//if (BoneMapOverride->IsValidLowLevel())
	//{
	//	BoneMap.hip = BoneMapOverride->hip;
	//	BoneMap.stomach = BoneMapOverride->stomach;
	//	BoneMap.chest = BoneMapOverride->chest;
	//	BoneMap.neck = BoneMapOverride->neck;
	//	BoneMap.head = BoneMapOverride->head;
	//	BoneMap.headTop = BoneMapOverride->headTop;
	//	BoneMap.leftShoulder = BoneMapOverride->leftShoulder;
	//	BoneMap.leftArm = BoneMapOverride->leftArm;
	//	BoneMap.leftForearm = BoneMapOverride->leftForearm;
	//	BoneMap.leftHand = BoneMapOverride->leftHand;
	//	BoneMap.leftFingerTip = BoneMapOverride->leftFingerTip;
	//	BoneMap.rightShoulder = BoneMapOverride->rightShoulder;
	//	BoneMap.rightArm = BoneMapOverride->rightArm;
	//	BoneMap.rightForearm = BoneMapOverride->rightForearm;
	//	BoneMap.rightHand = BoneMapOverride->rightHand;
	//	BoneMap.rightFingerTip = BoneMapOverride->rightFingerTip;
	//	BoneMap.leftUpleg = BoneMapOverride->leftUpleg;
	//	BoneMap.leftLeg = BoneMapOverride->leftLeg;
	//	BoneMap.leftFoot = BoneMapOverride->leftFoot;
	//	BoneMap.leftToe = BoneMapOverride->leftToe;
	//	BoneMap.rightUpleg = BoneMapOverride->rightUpleg;
	//	BoneMap.rightLeg = BoneMapOverride->rightLeg;
	//	BoneMap.rightFoot = BoneMapOverride->rightFoot;
	//	BoneMap.rightToe = BoneMapOverride->rightToe;
	//}
}

FSmartsuitPoseNode::~FSmartsuitPoseNode()
{
	//UE_LOG(LogTemp, Warning, TEXT("Smartsuit Pose Node: Cleaning up..."));
	/*if (TPose) {
	delete TPose;
	}*/
}

void FSmartsuitPoseNode::GatherDebugData(FNodeDebugData& DebugData)
{
	FString DebugLine = DebugData.GetNodeName(this);

	//DebugLine += "(";
	//AddDebugNodeData(DebugLine);
	//DebugLine += FString::Printf(TEXT(" Will affect the whole body)"));
	DebugData.AddDebugItem(DebugLine);

	ComponentPose.GatherDebugData(DebugData);
}

void FSmartsuitPoseNode::ApplySmartsuitRotation(FBoneReference bone, FQuat quat, FQuat dir, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();

	FCompactPoseBoneIndex CompactPoseBoneToModify = bone.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	const FQuat BoneQuat(quat);
	NewBoneTM.SetRotation(BoneQuat);
	//FTransform relativeTransform = MeshBases.GetComponentSpaceTransform(BoneMap.hipBone.GetCompactPoseIndex(BoneContainer));
	////NewBoneTM.SetTranslation(dir)
	//NewBoneTM = NewBoneTM * relativeTransform;
	//UE_LOG(LogTemp, Warning, TEXT("position is : %f, %f, %f"), NewBoneTM.GetTranslation().X, NewBoneTM.GetTranslation().Y, NewBoneTM.GetTranslation().Z);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
	//OutBoneTransforms.Add(FBoneTransform(bone.GetCompactPoseIndex(BoneContainer), NewBoneTM));
}

void FSmartsuitPoseNode::ApplySmartsuitPosition(FBoneReference bone, FVector position, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();

	FCompactPoseBoneIndex CompactPoseBoneToModify = bone.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	NewBoneTM.SetTranslation(position);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
}

void FSmartsuitPoseNode::ApplySmartsuitScale(FBoneReference bone, FVector scale, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	FCompactPoseBoneIndex CompactPoseBoneToModify = bone.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	NewBoneTM.SetScale3D(scale);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
}

void FSmartsuitPoseNode::ApplySmartsuitTransform(FBoneReference bone, FQuat quat, FVector position, FVector scale, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	FCompactPoseBoneIndex CompactPoseBoneToModify = bone.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	//NewBoneTM.SetScale3D(scale);
	const FQuat BoneQuat(quat);
	NewBoneTM.SetRotation(BoneQuat);
	NewBoneTM.SetTranslation(position);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
	//OutBoneTransforms.Add(FBoneTransform(bone.GetCompactPoseIndex(BoneContainer), NewBoneTM));
}

void FSmartsuitPoseNode::ApplySmartsuitRotationScale(FBoneReference bone, FQuat quat, FVector scale, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	FCompactPoseBoneIndex CompactPoseBoneToModify = bone.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	NewBoneTM.SetScale3D(scale);
	const FQuat BoneQuat(quat);
	NewBoneTM.SetRotation(BoneQuat);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
	//OutBoneTransforms.Add(FBoneTransform(bone.GetCompactPoseIndex(BoneContainer), NewBoneTM));
}

float FSmartsuitPoseNode::DistanceBetweenTwoBones(FBoneReference bone1, FBoneReference bone2, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	
	FCompactPoseBoneIndex Modify1 = bone1.GetCompactPoseIndex(BoneContainer);
	FTransform bone1TM = MeshBases.GetComponentSpaceTransform(Modify1);
	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, bone1TM, Modify1, space);

	FCompactPoseBoneIndex Modify2 = bone2.GetCompactPoseIndex(BoneContainer);
	FTransform bone2TM = MeshBases.GetComponentSpaceTransform(Modify2);
	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, bone2TM, Modify2, space);

	FVector diff = bone1TM.GetTranslation() - bone2TM.GetTranslation();
	float length;
	FVector dir;
	diff.ToDirectionAndLength(dir, length);
	return length;
}


FTransform OriginalTransform(FBoneReference bone, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	FCompactPoseBoneIndex CompactPoseBoneToModify = bone.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	//UE_LOG(LogTemp, Warning, TEXT("%s: %f, %f, %f, %f"), *name, NewBoneTM.GetRotation().X, NewBoneTM.GetRotation().Y, NewBoneTM.GetRotation().Z, NewBoneTM.GetRotation().W);
	return NewBoneTM;
	//OutBoneTransforms.Add(FBoneTransform(bone.GetCompactPoseIndex(BoneContainer), NewBoneTM));
}

TArray<FTransform> FSmartsuitPoseNode::GetAllBoneTransforms(FBoneReference bone, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	

	TArray<FTransform> result;
	for (int i = 0; i < BoneContainer.GetNumBones(); i++) 
	{
		if (i <= bone.BoneIndex) 
		{
			continue;
		}
		FCompactPoseBoneIndex CompactPoseBoneToModify(i);
		FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);
		FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
		result.Add(FTransform(NewBoneTM));
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s: %f, %f, %f, %f"), *name, NewBoneTM.GetRotation().X, NewBoneTM.GetRotation().Y, NewBoneTM.GetRotation().Z, NewBoneTM.GetRotation().W);
	return result;
	//OutBoneTransforms.Add(FBoneTransform(bone.GetCompactPoseIndex(BoneContainer), NewBoneTM));
}

void FSmartsuitPoseNode::ApplyAllBonePositions(FBoneReference bone, float hipWidth, TArray<FTransform> transforms, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	int index = 0;
	for (int i = bone.BoneIndex + 1; i < BoneContainer.GetNumBones(); i++) 
	{
		if (i == BoneMap.leftUpleg.BoneIndex || i == BoneMap.rightUpleg.BoneIndex) 
		{
			FVector hipWPos = OriginalTransform(BoneMap.hip, space, SkelComp, MeshBases).GetTranslation();
			
			FVector upLegWPos;
			if (i == BoneMap.leftUpleg.BoneIndex) 
			{
				upLegWPos = OriginalTransform(BoneMap.leftUpleg, space, SkelComp, MeshBases).GetTranslation();
			}
			else 
			{
				upLegWPos = OriginalTransform(BoneMap.rightUpleg, space, SkelComp, MeshBases).GetTranslation();
			}
			FVector diff = upLegWPos - hipWPos;
			FVector dir;
			float length;
			diff.ToDirectionAndLength(dir, length);
			FCompactPoseBoneIndex CompactPoseBoneToModify(i);
			FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);
			//UE_LOG(LogTemp, Warning, TEXT("UpLeg found, will use world lenght: %f, with dir %f, %f, %f"), hipWidth / 2, diff.X, diff.Y, diff.Z);

			//if (i == BoneMap.)
			FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
			NewBoneTM.SetTranslation(hipWPos + (dir * (hipWidth / 2)));
			FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
			MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
		}
		else 
		{
			FCompactPoseBoneIndex CompactPoseBoneToModify(i);
			FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);
			//if (i == BoneMap.)
			FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
			NewBoneTM.SetTranslation(transforms[index].GetTranslation());
			FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
			MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
		}
		index++;
	}
}

FVector FSmartsuitPoseNode::GetBoneScale(FBoneReference scaleBone, FBoneReference bone1, FBoneReference bone2, float desiredDistance, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	float currBoneDistance = DistanceBetweenTwoBones(bone1, bone2, space, SkelComp, MeshBases);
	FVector currScale = OriginalTransform(scaleBone, space, SkelComp, MeshBases).GetTranslation();
	float boneScale = desiredDistance / currBoneDistance;
	return FVector(boneScale, boneScale, boneScale);
}

float FSmartsuitPoseNode::ScaleBonesToDistance(FBoneReference scaleBone, FBoneReference bone1, FBoneReference bone2, float desiredDistance, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
	//scale bones...
	//FVector nextBonePosition = OriginalTransform(nextBone, EBoneControlSpace::BCS_ParentBoneSpace, SkelComp, MeshBases).GetTranslation();

	float currBoneDistance = DistanceBetweenTwoBones(bone1, bone2, space, SkelComp, MeshBases);
	FVector currScale = OriginalTransform(scaleBone, space, SkelComp, MeshBases).GetTranslation();
	float boneScale = desiredDistance / currBoneDistance;
	//float boneScale = 10.f;
	ApplySmartsuitScale(scaleBone, FVector(boneScale, boneScale, boneScale), space, SkelComp, MeshBases);

	//ApplySmartsuitPosition(nextBone, nextBonePosition, EBoneControlSpace::BCS_ParentBoneSpace, SkelComp, MeshBases);
	return boneScale;
}

FQuat GetRotation(uint8 sensor, Sensor *sensors, int numOfSensors) 
{
	for (int s = 0; s < numOfSensors; s++) 
	{
		if (sensors[s].addr == sensor) 
		{
			return sensors[s].Uquaternion();
		}
	}
	return FQuat::Identity;
}

FVector GetPosition(uint8 sensor, Sensor *sensors, int numOfSensors) 
{
	for (int s = 0; s < numOfSensors; s++) 
	{
		if (sensors[s].addr == sensor) 
		{
			return sensors[s].UPosition();
		}
	}
	return FVector::ZeroVector;
}

void FSmartsuitPoseNode::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	USkeletalMeshComponent* SkelComp = Output.AnimInstanceProxy->GetSkelMeshComponent();
	FCSPose<FCompactPose>& MeshBases = Output.Pose;

	check(OutBoneTransforms.Num() == 0);
	if (!Controller/* || !Controller->SupportsWiFi()*/) 
	{
		return;
	}

	ASmartsuitReceiver *receiver = GetReceiver();
	if (!receiver) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("No receiver"));
		return;
	}
	SuitData* data = receiver->GetSmartsuit(Controller->suitname);
	if (!data) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("No data for %s"), *Controller->suitname);
		return;
	}

	EBoneControlSpace TestBoneControlSpace = BCS_ComponentSpace;

	if (!TPose.StoredTPose) 
	{
		if (RelativeToStart) 
		{
			//if (data->Hip()) 
			//{
			//	TPose.startPos = SkelComp->GetComponentLocation();//FVector(OriginalTransform(BoneMap.hip, EBoneControlSpace::BCS_WorldSpace, SkelComp, MeshBases).GetTranslation()) - data->Hip()->UPosition();
			//}
			//else 
			{
				//TPose.startPos = FVector::ZeroVector;
			}
			//use always Z from the smartsuit.
			//TPose.startPos.Z = 0;
		}
		else 
		{
			//TPose.startPos = FVector::ZeroVector;
		}


		//TPose.startPos = FVector(OriginalTransform(BoneMap.hip, EBoneControlSpace::BCS_WorldSpace, SkelComp, MeshBases).GetTranslation());

		TPose.Pose.hip = FTransform(OriginalTransform(BoneMap.hip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.stomach = FTransform(OriginalTransform(BoneMap.stomach, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.chest = FTransform(OriginalTransform(BoneMap.chest, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.neck = FTransform(OriginalTransform(BoneMap.neck, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.head = FTransform(OriginalTransform(BoneMap.head, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftShoulder = FTransform(OriginalTransform(BoneMap.leftShoulder, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftArm = FTransform(OriginalTransform(BoneMap.leftArm, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftForearm = FTransform(OriginalTransform(BoneMap.leftForearm, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftHand = FTransform(OriginalTransform(BoneMap.leftHand, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightShoulder = FTransform(OriginalTransform(BoneMap.rightShoulder, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightArm = FTransform(OriginalTransform(BoneMap.rightArm, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightForearm = FTransform(OriginalTransform(BoneMap.rightForearm, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightHand = FTransform(OriginalTransform(BoneMap.rightHand, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftUpLeg = FTransform(OriginalTransform(BoneMap.leftUpleg, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftLeg = FTransform(OriginalTransform(BoneMap.leftLeg, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftFoot = FTransform(OriginalTransform(BoneMap.leftFoot, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightUpLeg = FTransform(OriginalTransform(BoneMap.rightUpleg, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightLeg = FTransform(OriginalTransform(BoneMap.rightLeg, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightFoot = FTransform(OriginalTransform(BoneMap.rightFoot, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.StoredTPose = true;
		return;
	}
	if (!TPose.StoredTPose) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("No tpose"));
		return;
	}
	FQuat modifier = FQuat::MakeFromEuler(FVector(0, 0, 180));
	FQuat forwardModifier = FQuat::MakeFromEuler(FVector(0, 0, 90));

	FQuat hipQuat = GetRotation(SENSOR_HIP, data->sensors, 19) * modifier;
	FVector hipPosition = GetPosition(SENSOR_HIP, data->sensors, 19);


	//UE_LOG(LogTemp, Warning, TEXT("hip position: %s"), *hipPosition.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("hip quat: %s"), *hipQuat.ToString());

	FQuat unmodifiedHipQuat = GetRotation(SENSOR_HIP, data->sensors, 19);
	//UE_LOG(LogTemp, Warning, TEXT("unmodified hip quat: %s"), *unmodifiedHipQuat.ToString());

	FRotator rotatortest = GetRotation(SENSOR_HIP, data->sensors, 19).Rotator();
	//UE_LOG(LogTemp, Warning, TEXT("rotator: %s"), *rotatortest.ToString());


	for (int s = 0; s < 19; s++)
	{
		if (data->sensors[s].addr == SENSOR_HIP)
		{
			FQuat rawQuat = data->sensors[s].quaternion;
			//UE_LOG(LogTemp, Warning, TEXT("raw quat: %s"), *rawQuat.ToString());

			FRotator rawRotator = rawQuat.Rotator();
			//UE_LOG(LogTemp, Warning, TEXT("raw rotator: %s"), *rawRotator.ToString());
		}
	}




	FQuat stomachQuat = GetRotation(SENSOR_STOMACH, data->sensors, 19);
	FQuat chestQuat = GetRotation(SENSOR_CHEST, data->sensors, 19);
	FQuat neckQuat = GetRotation(SENSOR_NECK, data->sensors, 19);
	FQuat headQuat = GetRotation(SENSOR_HEAD, data->sensors, 19);
	FQuat leftShoulderQuat = GetRotation(SENSOR_LEFT_SHOULDER, data->sensors, 19);
	FQuat leftArmQuat = GetRotation(SENSOR_LEFT_UPPER_ARM, data->sensors, 19);
	FQuat leftForearmQuat = GetRotation(SENSOR_LEFT_LOWER_ARM, data->sensors, 19);
	FQuat leftHandQuat = GetRotation(SENSOR_LEFT_HAND, data->sensors, 19);
	FQuat rightShoulderQuat = GetRotation(SENSOR_RIGHT_SHOULDER, data->sensors, 19);
	FQuat rightArmQuat = GetRotation(SENSOR_RIGHT_UPPER_ARM, data->sensors, 19);
	FQuat rightForearmQuat = GetRotation(SENSOR_RIGHT_LOWER_ARM, data->sensors, 19);
	FQuat rightHandQuat = GetRotation(SENSOR_RIGHT_HAND, data->sensors, 19);
	FQuat leftUpLegQuat = GetRotation(SENSOR_LEFT_UPPER_LEG, data->sensors, 19);
	FQuat leftLegQuat = GetRotation(SENSOR_LEFT_LOWER_LEG, data->sensors, 19);
	FQuat leftFootQuat = GetRotation(SENSOR_LEFT_FOOT, data->sensors, 19);
	FQuat rightUpLegQuat = GetRotation(SENSOR_RIGHT_UPPER_LEG, data->sensors, 19);
	FQuat rightLegQuat = GetRotation(SENSOR_RIGHT_LOWER_LEG, data->sensors, 19);
	FQuat rightFootQuat = GetRotation(SENSOR_RIGHT_FOOT, data->sensors, 19);

	FQuat hipExpected = SMARTSUIT_TPOSE_HIP.Inverse() * TPose.Pose.hip.GetRotation();
	FQuat stomachExpected = SMARTSUIT_TPOSE_STOMACH.Inverse() * TPose.Pose.stomach.GetRotation();
	FQuat chestExpected = SMARTSUIT_TPOSE_CHEST.Inverse() * TPose.Pose.chest.GetRotation();
	FQuat neckExpected = SMARTSUIT_TPOSE_NECK.Inverse() * TPose.Pose.neck.GetRotation();
	FQuat headExpected = SMARTSUIT_TPOSE_HEAD.Inverse() * TPose.Pose.head.GetRotation();
	FQuat leftShoulderExpected = SMARTSUIT_TPOSE_LEFT_SHOULDER.Inverse() * TPose.Pose.leftShoulder.GetRotation();
	FQuat leftArmExpected = SMARTSUIT_TPOSE_LEFT_ARM.Inverse() * TPose.Pose.leftArm.GetRotation();
	FQuat leftForearmExpected = SMARTSUIT_TPOSE_LEFT_FOREARM.Inverse() * TPose.Pose.leftForearm.GetRotation();
	FQuat leftHandExpected = SMARTSUIT_TPOSE_LEFT_HAND.Inverse() * TPose.Pose.leftHand.GetRotation();
	FQuat rightShoulderExpected = SMARTSUIT_TPOSE_RIGHT_SHOULDER.Inverse() * TPose.Pose.rightShoulder.GetRotation();
	FQuat rightArmExpected = SMARTSUIT_TPOSE_RIGHT_ARM.Inverse() * TPose.Pose.rightArm.GetRotation();
	FQuat rightForearmExpected = SMARTSUIT_TPOSE_RIGHT_FOREARM.Inverse() * TPose.Pose.rightForearm.GetRotation();
	FQuat rightHandExpected = SMARTSUIT_TPOSE_RIGHT_HAND.Inverse() * TPose.Pose.rightHand.GetRotation();
	FQuat leftUpLegExpected = SMARTSUIT_TPOSE_LEFT_UP_LEG.Inverse() * TPose.Pose.leftUpLeg.GetRotation();
	FQuat leftLegExpected = SMARTSUIT_TPOSE_LEFT_LEG.Inverse() * TPose.Pose.leftLeg.GetRotation();
	FQuat leftFootExpected = SMARTSUIT_TPOSE_LEFT_FOOT.Inverse() * TPose.Pose.leftFoot.GetRotation();
	FQuat rightUpLegExpected = SMARTSUIT_TPOSE_RIGHT_UP_LEG.Inverse() * TPose.Pose.rightUpLeg.GetRotation();
	FQuat rightLegExpected = SMARTSUIT_TPOSE_RIGHT_LEG.Inverse() * TPose.Pose.rightLeg.GetRotation();
	FQuat rightFootExpected = SMARTSUIT_TPOSE_RIGHT_FOOT.Inverse() * TPose.Pose.rightFoot.GetRotation();

	//float ImportInformScale = SkelComp->SkeletalMesh->AssetImportData->

	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	FCompactPoseBoneIndex CompactPoseBoneToModify = BoneMap.hip.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetLocalSpaceTransform(CompactPoseBoneToModify);//.GetComponentSpaceTransform(CompactPoseBoneToModify);

	float testval1 = NewBoneTM.GetLocation().Size();
	float testval2 = TPose.Pose.hip.GetLocation().Size();

	float scale = testval2 / testval1;

	FString teststring1 = FString::SanitizeFloat(testval1);
	FString teststring2 = FString::SanitizeFloat(testval2);

	FString teststring3 = FString::SanitizeFloat(scale);

	//UE_LOG(LogTemp, Warning, TEXT("size 1: %s    size 2: %s"), *teststring1, *teststring2);
	//UE_LOG(LogTemp, Warning, TEXT("scale: %s"), *teststring3);

	if (RelativeToStart) 
	{
		ApplySmartsuitTransform(BoneMap.hip, hipQuat*hipExpected, (hipPosition*scale) + TPose.startPos, FVector(1, 1, 1), TestBoneControlSpace, SkelComp, MeshBases);
	}
	else 
	{
		ApplySmartsuitTransform(BoneMap.hip, hipQuat*hipExpected, hipPosition * scale, FVector(1, 1, 1), TestBoneControlSpace, SkelComp, MeshBases);
	}
	ApplySmartsuitRotation(BoneMap.stomach, stomachQuat * stomachExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.chest, chestQuat * chestExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.neck, neckQuat * neckExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.head, headQuat * headExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftShoulder, leftShoulderQuat * leftShoulderExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftArm, leftArmQuat * leftArmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftForearm, leftForearmQuat * leftForearmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftHand, leftHandQuat * leftHandExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightShoulder, rightShoulderQuat * rightShoulderExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightArm, rightArmQuat * rightArmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightForearm, rightForearmQuat * rightForearmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightHand, rightHandQuat * rightHandExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftUpleg, leftUpLegQuat * leftUpLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftLeg, leftLegQuat * leftLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftFoot, leftFootQuat * leftFootExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightUpleg, rightUpLegQuat * rightUpLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightLeg, rightLegQuat * rightLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightFoot, rightFootQuat * rightFootExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);


	if (ScaleBones) 
	{
		Body body = Controller->bodyModel.GetBody();

		TArray<FTransform> relativePositions = GetAllBoneTransforms(BoneMap.hip, BCS_ParentBoneSpace, SkelComp, MeshBases);
		//base body
		float hipScale = ScaleBonesToDistance(BoneMap.hip, BoneMap.hip, BoneMap.stomach, body._hip * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float stomachScale = ScaleBonesToDistance(BoneMap.stomach, BoneMap.stomach, BoneMap.chest, body._low_back * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float chestScale = ScaleBonesToDistance(BoneMap.chest, BoneMap.chest, BoneMap.neck, body._middle_back * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float neckScale =  ScaleBonesToDistance(BoneMap.neck, BoneMap.neck, BoneMap.head, body._neck * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float headScale = ScaleBonesToDistance(BoneMap.head, BoneMap.head, BoneMap.headTop, body._head * 100, TestBoneControlSpace, SkelComp, MeshBases);
		//arms
		float leftShoulderScale = ScaleBonesToDistance(BoneMap.leftShoulder, BoneMap.leftShoulder, BoneMap.leftArm, body._shoulder_blade * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float leftArmScale = ScaleBonesToDistance(BoneMap.leftArm, BoneMap.leftArm, BoneMap.leftForearm, body._upper_arm * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float leftForearmScale = ScaleBonesToDistance(BoneMap.leftForearm, BoneMap.leftForearm, BoneMap.leftHand, body._forearm * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float leftHandScale = ScaleBonesToDistance(BoneMap.leftHand, BoneMap.leftHand, BoneMap.leftFingerTip, body._hand * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float rightShoulderScale = ScaleBonesToDistance(BoneMap.rightShoulder, BoneMap.rightShoulder, BoneMap.rightArm, body._shoulder_blade * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float rightArmScale = ScaleBonesToDistance(BoneMap.rightArm, BoneMap.rightArm, BoneMap.rightForearm, body._upper_arm * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float rightForearmScale = ScaleBonesToDistance(BoneMap.rightForearm, BoneMap.rightForearm, BoneMap.rightHand, body._forearm * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float rightHandScale = ScaleBonesToDistance(BoneMap.rightHand, BoneMap.rightHand, BoneMap.rightFingerTip, body._hand * 100, TestBoneControlSpace, SkelComp, MeshBases);
		//legs
		float leftUpLegScale = ScaleBonesToDistance(BoneMap.leftUpleg, BoneMap.leftUpleg, BoneMap.leftLeg, body._thigh * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float leftLegScale = ScaleBonesToDistance(BoneMap.leftLeg, BoneMap.leftLeg, BoneMap.leftFoot, body._leg * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float leftFootScale = ScaleBonesToDistance(BoneMap.leftFoot, BoneMap.leftFoot, BoneMap.leftToe, (body._foot_length - body._foot_heel_offset) * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float rightUpLegScale = ScaleBonesToDistance(BoneMap.rightUpleg, BoneMap.rightUpleg, BoneMap.rightLeg, body._thigh * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float rightLegScale = ScaleBonesToDistance(BoneMap.rightLeg, BoneMap.rightLeg, BoneMap.rightFoot, body._leg * 100, TestBoneControlSpace, SkelComp, MeshBases);
		float rightFootScale = ScaleBonesToDistance(BoneMap.rightFoot, BoneMap.rightFoot, BoneMap.rightToe, (body._foot_length - body._foot_heel_offset) * 100, TestBoneControlSpace, SkelComp, MeshBases);

		ApplyAllBonePositions(BoneMap.hip, body._hip_width * 100, relativePositions, BCS_ParentBoneSpace, SkelComp, MeshBases);
	}
}

bool FSmartsuitPoseNode::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// if both bones are valid
#if BUILT_MINOR_VERSION >= 17
	return (BoneMap.hip.IsValidToEvaluate(RequiredBones) && BoneMap.chest.IsValidToEvaluate(RequiredBones) && BoneMap.stomach.IsValidToEvaluate(RequiredBones) &&
		BoneMap.neck.IsValidToEvaluate(RequiredBones) && BoneMap.head.IsValidToEvaluate(RequiredBones) && BoneMap.headTop.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftShoulder.IsValidToEvaluate(RequiredBones) && BoneMap.leftArm.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftForearm.IsValidToEvaluate(RequiredBones) && BoneMap.leftHand.IsValidToEvaluate(RequiredBones) && BoneMap.leftFingerTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightShoulder.IsValidToEvaluate(RequiredBones) && BoneMap.rightArm.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightForearm.IsValidToEvaluate(RequiredBones) && BoneMap.rightHand.IsValidToEvaluate(RequiredBones) && BoneMap.rightFingerTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftUpleg.IsValidToEvaluate(RequiredBones) && BoneMap.leftLeg.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftFoot.IsValidToEvaluate(RequiredBones) && BoneMap.rightUpleg.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightLeg.IsValidToEvaluate(RequiredBones) && BoneMap.rightFoot.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftToe.IsValidToEvaluate(RequiredBones) && BoneMap.rightToe.IsValidToEvaluate(RequiredBones));
#else
	return (BoneMap.hip.IsValid(RequiredBones) && BoneMap.chest.IsValid(RequiredBones) && BoneMap.stomach.IsValid(RequiredBones) &&
		BoneMap.neck.IsValid(RequiredBones) && BoneMap.head.IsValid(RequiredBones) && BoneMap.headTop.IsValid(RequiredBones) &&
		BoneMap.leftShoulder.IsValid(RequiredBones) && BoneMap.leftArm.IsValid(RequiredBones) &&
		BoneMap.leftForearm.IsValid(RequiredBones) && BoneMap.leftHand.IsValid(RequiredBones) && BoneMap.leftFingerTip.IsValid(RequiredBones) &&
		BoneMap.rightShoulder.IsValid(RequiredBones) && BoneMap.rightArm.IsValid(RequiredBones) && 
		BoneMap.rightForearm.IsValid(RequiredBones) && BoneMap.rightHand.IsValid(RequiredBones) && BoneMap.rightFingerTip.IsValid(RequiredBones) &&
		BoneMap.leftUpleg.IsValid(RequiredBones) && BoneMap.leftLeg.IsValid(RequiredBones) &&
		BoneMap.leftFoot.IsValid(RequiredBones) && BoneMap.rightUpleg.IsValid(RequiredBones) &&
		BoneMap.rightLeg.IsValid(RequiredBones) && BoneMap.rightFoot.IsValid(RequiredBones) &&
		BoneMap.leftToe.IsValid(RequiredBones) && BoneMap.rightToe.IsValid(RequiredBones));
#endif
}

void FSmartsuitPoseNode::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	if (BoneMapOverride->IsValidLowLevel())
	{
		BoneMap.hip = BoneMapOverride->hip;
		BoneMap.stomach = BoneMapOverride->stomach;
		BoneMap.chest = BoneMapOverride->chest;
		BoneMap.neck = BoneMapOverride->neck;
		BoneMap.head = BoneMapOverride->head;
		BoneMap.headTop = BoneMapOverride->headTop;
		BoneMap.leftShoulder = BoneMapOverride->leftShoulder;
		BoneMap.leftArm = BoneMapOverride->leftArm;
		BoneMap.leftForearm = BoneMapOverride->leftForearm;
		BoneMap.leftHand = BoneMapOverride->leftHand;
		BoneMap.leftFingerTip = BoneMapOverride->leftFingerTip;
		BoneMap.rightShoulder = BoneMapOverride->rightShoulder;
		BoneMap.rightArm = BoneMapOverride->rightArm;
		BoneMap.rightForearm = BoneMapOverride->rightForearm;
		BoneMap.rightHand = BoneMapOverride->rightHand;
		BoneMap.rightFingerTip = BoneMapOverride->rightFingerTip;
		BoneMap.leftUpleg = BoneMapOverride->leftUpleg;
		BoneMap.leftLeg = BoneMapOverride->leftLeg;
		BoneMap.leftFoot = BoneMapOverride->leftFoot;
		BoneMap.leftToe = BoneMapOverride->leftToe;
		BoneMap.rightUpleg = BoneMapOverride->rightUpleg;
		BoneMap.rightLeg = BoneMapOverride->rightLeg;
		BoneMap.rightFoot = BoneMapOverride->rightFoot;
		BoneMap.rightToe = BoneMapOverride->rightToe;
	}

	BoneMap.hip.Initialize(RequiredBones);
	BoneMap.stomach.Initialize(RequiredBones);
	BoneMap.chest.Initialize(RequiredBones);
	BoneMap.neck.Initialize(RequiredBones);
	BoneMap.head.Initialize(RequiredBones);
	BoneMap.headTop.Initialize(RequiredBones);
	BoneMap.leftShoulder.Initialize(RequiredBones);
	BoneMap.leftArm.Initialize(RequiredBones);
	BoneMap.leftForearm.Initialize(RequiredBones);
	BoneMap.leftHand.Initialize(RequiredBones);
	BoneMap.leftFingerTip.Initialize(RequiredBones);
	BoneMap.rightShoulder.Initialize(RequiredBones);
	BoneMap.rightArm.Initialize(RequiredBones);
	BoneMap.rightForearm.Initialize(RequiredBones);
	BoneMap.rightHand.Initialize(RequiredBones);
	BoneMap.rightFingerTip.Initialize(RequiredBones);
	BoneMap.leftUpleg.Initialize(RequiredBones);
	BoneMap.leftLeg.Initialize(RequiredBones);
	BoneMap.leftFoot.Initialize(RequiredBones);
	BoneMap.leftToe.Initialize(RequiredBones);
	BoneMap.rightUpleg.Initialize(RequiredBones);
	BoneMap.rightLeg.Initialize(RequiredBones);
	BoneMap.rightFoot.Initialize(RequiredBones);
	BoneMap.rightToe.Initialize(RequiredBones);
}

USmartsuitBodyMapData::USmartsuitBodyMapData()
{
	hip = "hip";
	stomach = "stomach";
	chest = "chest";
	neck = "neck";
	head = "head";
	headTop = "headTop";
	leftShoulder = "leftShoulder";
	leftArm = "leftArm";
	leftForearm = "leftForearm";
	leftHand = "leftHand";
	leftFingerTip = "leftFingerTip";
	rightShoulder = "rightShoulder";
	rightArm = "rightArm";
	rightForearm = "rightForearm";
	rightHand = "rightHand";
	rightFingerTip = "rightFingerTip";
	leftUpleg = "leftUpleg";
	leftLeg = "leftLeg";
	leftFoot = "leftFoot";
	leftToe = "leftToe";
	rightUpleg = "rightUpleg";
	rightLeg = "rightLeg";
	rightFoot = "rightFoot";
	rightToe = "rightToe";
}

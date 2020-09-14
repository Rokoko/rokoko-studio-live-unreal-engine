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
	//RelativeToStart = false;
	//ScaleBones = false;

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

//FQuat GetRotation(uint8 sensor, Sensor *sensors, int numOfSensors) 
//{
//	for (int s = 0; s < numOfSensors; s++) 
//	{
//		if (sensors[s].addr == sensor) 
//		{
//			return sensors[s].Uquaternion();
//		}
//	}
//	return FQuat::Identity;
//}
//
//FVector GetPosition(uint8 sensor, Sensor *sensors, int numOfSensors) 
//{
//	for (int s = 0; s < numOfSensors; s++) 
//	{
//		if (sensors[s].addr == sensor) 
//		{
//			return sensors[s].UPosition();
//		}
//	}
//	return FVector::ZeroVector;
//}

FQuat GetRotation2(const FName& BoneName, FSuitData* suitdata)
{
	if (auto SmartsuitBone = suitdata->SmartsuitBones.Find(BoneName))
	{
		return SmartsuitBone->Uquaternion();
	}

	return FQuat::Identity;
}

FVector GetPosition2(const FName& BoneName, FSuitData* suitdata)
{
	if (auto SmartsuitBone = suitdata->SmartsuitBones.Find(BoneName))
	{
		return SmartsuitBone->UPosition();
	}

	return FVector::ZeroVector;
}
PRAGMA_DISABLE_OPTIMIZATION
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
	FSuitData* data = receiver->GetSmartsuit(Controller->suitname);
	if (!data) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("No data for %s"), *Controller->suitname);
		return;
	}

	EBoneControlSpace TestBoneControlSpace = BCS_ComponentSpace;

	if (!TPose.StoredTPose) 
	{
		
		//if (RelativeToStart)
		//{
		//	if (data->Hip())
		//	{
		//		TPose.startPos = FVector(OriginalTransform(BoneMap.hip, TestBoneControlSpace, SkelComp, MeshBases).GetTranslation()) - data->Hip()->UPosition();//SkelComp->GetComponentLocation();

		//		//TPose.startPos.X = 0;
		//		//TPose.startPos.Y = 0;
		//		//TPose.startPos.Z = 0;
		//	}
		//	else
		//	{
		//		TPose.startPos = FVector::ZeroVector;
		//	}
		//}
		//else
		//{
		//	TPose.startPos = FVector::ZeroVector;
		//	//SkelComp->SetWorldRotation(FRotator::ZeroRotator);
		//}
		


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

		TPose.Pose.leftThumbProximal = FTransform(OriginalTransform(BoneMap.leftThumbProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftThumbMedial = FTransform(OriginalTransform(BoneMap.leftThumbMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftThumbDistal = FTransform(OriginalTransform(BoneMap.leftThumbDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftThumbTip = FTransform(OriginalTransform(BoneMap.leftThumbTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftIndexProximal = FTransform(OriginalTransform(BoneMap.leftIndexProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftIndexMedial = FTransform(OriginalTransform(BoneMap.leftIndexMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftIndexDistal = FTransform(OriginalTransform(BoneMap.leftIndexDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftIndexTip = FTransform(OriginalTransform(BoneMap.leftIndexTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftMiddleProximal = FTransform(OriginalTransform(BoneMap.leftMiddleProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftMiddleMedial = FTransform(OriginalTransform(BoneMap.leftMiddleMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftMiddleDistal = FTransform(OriginalTransform(BoneMap.leftMiddleDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftMiddleTip = FTransform(OriginalTransform(BoneMap.leftMiddleTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftRingProximal = FTransform(OriginalTransform(BoneMap.leftRingProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftRingMedial = FTransform(OriginalTransform(BoneMap.leftRingMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftRingDistal = FTransform(OriginalTransform(BoneMap.leftRingDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftRingTip = FTransform(OriginalTransform(BoneMap.leftRingTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftLittleProximal = FTransform(OriginalTransform(BoneMap.leftLittleProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftLittleMedial = FTransform(OriginalTransform(BoneMap.leftLittleMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftLittleDistal = FTransform(OriginalTransform(BoneMap.leftLittleDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.leftLittleTip = FTransform(OriginalTransform(BoneMap.leftLittleTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightThumbProximal = FTransform(OriginalTransform(BoneMap.rightThumbProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightThumbMedial = FTransform(OriginalTransform(BoneMap.rightThumbMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightThumbDistal = FTransform(OriginalTransform(BoneMap.rightThumbDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightThumbTip = FTransform(OriginalTransform(BoneMap.rightThumbTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightIndexProximal = FTransform(OriginalTransform(BoneMap.rightIndexProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightIndexMedial = FTransform(OriginalTransform(BoneMap.rightIndexMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightIndexDistal = FTransform(OriginalTransform(BoneMap.rightIndexDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightIndexTip = FTransform(OriginalTransform(BoneMap.rightIndexTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightMiddleProximal = FTransform(OriginalTransform(BoneMap.rightMiddleProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightMiddleMedial = FTransform(OriginalTransform(BoneMap.rightMiddleMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightMiddleDistal = FTransform(OriginalTransform(BoneMap.rightMiddleDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightMiddleTip = FTransform(OriginalTransform(BoneMap.rightMiddleTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightRingProximal = FTransform(OriginalTransform(BoneMap.rightRingProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightRingMedial = FTransform(OriginalTransform(BoneMap.rightRingMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightRingDistal = FTransform(OriginalTransform(BoneMap.rightRingDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightRingTip = FTransform(OriginalTransform(BoneMap.rightRingTip, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightLittleProximal = FTransform(OriginalTransform(BoneMap.rightLittleProximal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightLittleMedial = FTransform(OriginalTransform(BoneMap.rightLittleMedial, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightLittleDistal = FTransform(OriginalTransform(BoneMap.rightLittleDistal, TestBoneControlSpace, SkelComp, MeshBases));
		TPose.Pose.rightLittleTip = FTransform(OriginalTransform(BoneMap.rightLittleTip, TestBoneControlSpace, SkelComp, MeshBases));
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

	//FQuat hipQuat = GetRotation(SENSOR_HIP, data->sensors, 19) * modifier;
	//FVector hipPosition = GetPosition(SENSOR_HIP, data->sensors, 19);
	//FQuat stomachQuat = GetRotation(SENSOR_STOMACH, data->sensors, 19);
	//FQuat chestQuat = GetRotation(SENSOR_CHEST, data->sensors, 19);
	//FQuat neckQuat = GetRotation(SENSOR_NECK, data->sensors, 19);
	//FQuat headQuat = GetRotation(SENSOR_HEAD, data->sensors, 19);
	//FQuat leftShoulderQuat = GetRotation(SENSOR_LEFT_SHOULDER, data->sensors, 19);
	//FQuat leftArmQuat = GetRotation(SENSOR_LEFT_UPPER_ARM, data->sensors, 19);
	//FQuat leftForearmQuat = GetRotation(SENSOR_LEFT_LOWER_ARM, data->sensors, 19);
	//FQuat leftHandQuat = GetRotation(SENSOR_LEFT_HAND, data->sensors, 19);
	//FQuat rightShoulderQuat = GetRotation(SENSOR_RIGHT_SHOULDER, data->sensors, 19);
	//FQuat rightArmQuat = GetRotation(SENSOR_RIGHT_UPPER_ARM, data->sensors, 19);
	//FQuat rightForearmQuat = GetRotation(SENSOR_RIGHT_LOWER_ARM, data->sensors, 19);
	//FQuat rightHandQuat = GetRotation(SENSOR_RIGHT_HAND, data->sensors, 19);
	//FQuat leftUpLegQuat = GetRotation(SENSOR_LEFT_UPPER_LEG, data->sensors, 19);
	//FQuat leftLegQuat = GetRotation(SENSOR_LEFT_LOWER_LEG, data->sensors, 19);
	//FQuat leftFootQuat = GetRotation(SENSOR_LEFT_FOOT, data->sensors, 19);
	//FQuat rightUpLegQuat = GetRotation(SENSOR_RIGHT_UPPER_LEG, data->sensors, 19);
	//FQuat rightLegQuat = GetRotation(SENSOR_RIGHT_LOWER_LEG, data->sensors, 19);
	//FQuat rightFootQuat = GetRotation(SENSOR_RIGHT_FOOT, data->sensors, 19);

	FQuat hipQuat = GetRotation2(SmartsuitBones::hip, data);// *modifier;
	FVector hipPosition = GetPosition2(SmartsuitBones::hip, data);
	FQuat stomachQuat = GetRotation2(SmartsuitBones::spine, data);
	FQuat chestQuat = GetRotation2(SmartsuitBones::chest, data);
	FQuat neckQuat = GetRotation2(SmartsuitBones::neck, data);
	FQuat headQuat = GetRotation2(SmartsuitBones::head, data);
	FQuat leftShoulderQuat = GetRotation2(SmartsuitBones::leftShoulder, data);
	FQuat leftArmQuat = GetRotation2(SmartsuitBones::leftUpperArm, data);
	FQuat leftForearmQuat = GetRotation2(SmartsuitBones::leftLowerArm, data);
	FQuat leftHandQuat = GetRotation2(SmartsuitBones::leftHand, data);
	FQuat rightShoulderQuat = GetRotation2(SmartsuitBones::rightShoulder, data);
	FQuat rightArmQuat = GetRotation2(SmartsuitBones::rightUpperArm, data);
	FQuat rightForearmQuat = GetRotation2(SmartsuitBones::rightLowerArm, data);
	FQuat rightHandQuat = GetRotation2(SmartsuitBones::rightHand, data);
	FQuat leftUpLegQuat = GetRotation2(SmartsuitBones::leftUpLeg, data);
	FQuat leftLegQuat = GetRotation2(SmartsuitBones::leftLeg, data);
	FQuat leftFootQuat = GetRotation2(SmartsuitBones::leftFoot, data);
	FQuat rightUpLegQuat = GetRotation2(SmartsuitBones::rightUpLeg, data);
	FQuat rightLegQuat = GetRotation2(SmartsuitBones::rightLeg, data);
	FQuat rightFootQuat = GetRotation2(SmartsuitBones::rightFoot, data);

	FQuat leftThumbProximalQuat = GetRotation2(SmartsuitBones::leftThumbProximal, data);
	FQuat leftThumbMedialQuat = GetRotation2(SmartsuitBones::leftThumbMedial, data);
	FQuat leftThumbDistalQuat = GetRotation2(SmartsuitBones::leftThumbDistal, data);
	FQuat leftThumbTipQuat = GetRotation2(SmartsuitBones::leftThumbTip, data);
	FQuat leftIndexProximalQuat = GetRotation2(SmartsuitBones::leftIndexProximal, data);
	FQuat leftIndexMedialQuat = GetRotation2(SmartsuitBones::leftIndexMedial, data);
	FQuat leftIndexDistalQuat = GetRotation2(SmartsuitBones::leftIndexDistal, data);
	FQuat leftIndexTipQuat = GetRotation2(SmartsuitBones::leftIndexTip, data);
	FQuat leftMiddleProximalQuat = GetRotation2(SmartsuitBones::leftMiddleProximal, data);
	FQuat leftMiddleMedialQuat = GetRotation2(SmartsuitBones::leftMiddleMedial, data);
	FQuat leftMiddleDistalQuat = GetRotation2(SmartsuitBones::leftMiddleDistal, data);
	FQuat leftMiddleTipQuat = GetRotation2(SmartsuitBones::leftMiddleTip, data);
	FQuat leftRingProximalQuat = GetRotation2(SmartsuitBones::leftRingProximal, data);
	FQuat leftRingMedialQuat = GetRotation2(SmartsuitBones::leftRingMedial, data);
	FQuat leftRingDistalQuat = GetRotation2(SmartsuitBones::leftRingDistal, data);
	FQuat leftRingTipQuat = GetRotation2(SmartsuitBones::leftRingTip, data);
	FQuat leftLittleProximalQuat = GetRotation2(SmartsuitBones::leftLittleProximal, data);
	FQuat leftLittleMedialQuat = GetRotation2(SmartsuitBones::leftLittleMedial, data);
	FQuat leftLittleDistalQuat = GetRotation2(SmartsuitBones::leftLittleDistal, data);
	FQuat leftLittleTipQuat = GetRotation2(SmartsuitBones::leftLittleTip, data);
	FQuat rightThumbProximalQuat = GetRotation2(SmartsuitBones::rightThumbProximal, data);
	FQuat rightThumbMedialQuat = GetRotation2(SmartsuitBones::rightThumbMedial, data);
	FQuat rightThumbDistalQuat = GetRotation2(SmartsuitBones::rightThumbDistal, data);
	FQuat rightThumbTipQuat = GetRotation2(SmartsuitBones::rightThumbTip, data);
	FQuat rightIndexProximalQuat = GetRotation2(SmartsuitBones::rightIndexProximal, data);
	FQuat rightIndexMedialQuat = GetRotation2(SmartsuitBones::rightIndexMedial, data);
	FQuat rightIndexDistalQuat = GetRotation2(SmartsuitBones::rightIndexDistal, data);
	FQuat rightIndexTipQuat = GetRotation2(SmartsuitBones::rightIndexTip, data);
	FQuat rightMiddleProximalQuat = GetRotation2(SmartsuitBones::rightMiddleProximal, data);
	FQuat rightMiddleMedialQuat = GetRotation2(SmartsuitBones::rightMiddleMedial, data);
	FQuat rightMiddleDistalQuat = GetRotation2(SmartsuitBones::rightMiddleDistal, data);
	FQuat rightMiddleTipQuat = GetRotation2(SmartsuitBones::rightMiddleTip, data);
	FQuat rightRingProximalQuat = GetRotation2(SmartsuitBones::rightRingProximal, data);
	FQuat rightRingMedialQuat = GetRotation2(SmartsuitBones::rightRingMedial, data);
	FQuat rightRingDistalQuat = GetRotation2(SmartsuitBones::rightRingDistal, data);
	FQuat rightRingTipQuat = GetRotation2(SmartsuitBones::rightRingTip, data);
	FQuat rightLittleProximalQuat = GetRotation2(SmartsuitBones::rightLittleProximal, data);
	FQuat rightLittleMedialQuat = GetRotation2(SmartsuitBones::rightLittleMedial, data);
	FQuat rightLittleDistalQuat = GetRotation2(SmartsuitBones::rightLittleDistal, data);
	FQuat rightLittleTipQuat = GetRotation2(SmartsuitBones::rightLittleTip, data);



	/*
	UE_LOG(LogTemp, Warning, TEXT("hip position: %s"), *hipPosition.ToString());
	UE_LOG(LogTemp, Warning, TEXT("hip quat: %s"), *hipQuat.ToString());

	FQuat unmodifiedHipQuat = GetRotation2("hip", data->SmartsuitBones);
	UE_LOG(LogTemp, Warning, TEXT("unmodified hip quat: %s"), *unmodifiedHipQuat.ToString());

	FRotator rotatortest = GetRotation2("hip", data->SmartsuitBones).Rotator();
	UE_LOG(LogTemp, Warning, TEXT("rotator: %s"), *rotatortest.ToString());

	for (int s = 0; s < data->SmartsuitBones.Num(); s++)
	{
		if (data->SmartsuitBones[s].name == "hip")
		{
			FQuat rawQuat = data->SmartsuitBones[s].rotation;
			UE_LOG(LogTemp, Warning, TEXT("raw quat: %s"), *rawQuat.ToString());

			FRotator rawRotator = rawQuat.Rotator();
			UE_LOG(LogTemp, Warning, TEXT("raw rotator: %s"), *rawRotator.ToString());
		}
	}
	*/





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

	FQuat leftThumbProximalExpected = SMARTSUIT_TPOSE_LEFT_THUMB_PROXIMAL.Inverse() * TPose.Pose.leftThumbProximal.GetRotation();
	FQuat leftThumbMedialExpected = SMARTSUIT_TPOSE_LEFT_THUMB_MEDIAL.Inverse() * TPose.Pose.leftThumbMedial.GetRotation();
	FQuat leftThumbDistalExpected = SMARTSUIT_TPOSE_LEFT_THUMB_DISTAL.Inverse() * TPose.Pose.leftThumbDistal.GetRotation();
	FQuat leftThumbTipExpected = SMARTSUIT_TPOSE_LEFT_THUMB_TIP.Inverse() * TPose.Pose.leftThumbTip.GetRotation();

	FQuat leftIndexProximalExpected = SMARTSUIT_TPOSE_LEFT_INDEX_PROXIMAL.Inverse() * TPose.Pose.leftIndexProximal.GetRotation();
	FQuat leftIndexMedialExpected = SMARTSUIT_TPOSE_LEFT_INDEX_MEDIAL.Inverse() * TPose.Pose.leftIndexMedial.GetRotation();
	FQuat leftIndexDistalExpected = SMARTSUIT_TPOSE_LEFT_INDEX_DISTAL.Inverse() * TPose.Pose.leftIndexDistal.GetRotation();
	FQuat leftIndexTipExpected = SMARTSUIT_TPOSE_LEFT_INDEX_TIP.Inverse() * TPose.Pose.leftIndexTip.GetRotation();

	FQuat leftMiddleProximalExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_PROXIMAL.Inverse() * TPose.Pose.leftMiddleProximal.GetRotation();
	FQuat leftMiddleMedialExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_MEDIAL.Inverse() * TPose.Pose.leftMiddleMedial.GetRotation();
	FQuat leftMiddleDistalExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_DISTAL.Inverse() * TPose.Pose.leftMiddleDistal.GetRotation();
	FQuat leftMiddleTipExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_TIP.Inverse() * TPose.Pose.leftMiddleTip.GetRotation();

	FQuat leftRingProximalExpected = SMARTSUIT_TPOSE_LEFT_RING_PROXIMAL.Inverse() * TPose.Pose.leftRingProximal.GetRotation();
	FQuat leftRingMedialExpected = SMARTSUIT_TPOSE_LEFT_RING_MEDIAL.Inverse() * TPose.Pose.leftRingMedial.GetRotation();
	FQuat leftRingDistalExpected = SMARTSUIT_TPOSE_LEFT_RING_DISTAL.Inverse() * TPose.Pose.leftRingDistal.GetRotation();
	FQuat leftRingTipExpected = SMARTSUIT_TPOSE_LEFT_RING_TIP.Inverse() * TPose.Pose.leftRingTip.GetRotation();

	FQuat leftLittleProximalExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_PROXIMAL.Inverse() * TPose.Pose.leftLittleProximal.GetRotation();
	FQuat leftLittleMedialExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_MEDIAL.Inverse() * TPose.Pose.leftLittleMedial.GetRotation();
	FQuat leftLittleDistalExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_DISTAL.Inverse() * TPose.Pose.leftLittleDistal.GetRotation();
	FQuat leftLittleTipExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_TIP.Inverse() * TPose.Pose.leftLittleTip.GetRotation();


	FQuat rightThumbProximalExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_PROXIMAL.Inverse() * TPose.Pose.rightThumbProximal.GetRotation();
	FQuat rightThumbMedialExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_MEDIAL.Inverse() * TPose.Pose.rightThumbMedial.GetRotation();
	FQuat rightThumbDistalExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_DISTAL.Inverse() * TPose.Pose.rightThumbDistal.GetRotation();
	FQuat rightThumbTipExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_TIP.Inverse() * TPose.Pose.rightThumbTip.GetRotation();

	FQuat rightIndexProximalExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_PROXIMAL.Inverse() * TPose.Pose.rightIndexProximal.GetRotation();
	FQuat rightIndexMedialExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_MEDIAL.Inverse() * TPose.Pose.rightIndexMedial.GetRotation();
	FQuat rightIndexDistalExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_DISTAL.Inverse() * TPose.Pose.rightIndexDistal.GetRotation();
	FQuat rightIndexTipExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_TIP.Inverse() * TPose.Pose.rightIndexTip.GetRotation();

	FQuat rightMiddleProximalExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_PROXIMAL.Inverse() * TPose.Pose.rightMiddleProximal.GetRotation();
	FQuat rightMiddleMedialExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_MEDIAL.Inverse() * TPose.Pose.rightMiddleMedial.GetRotation();
	FQuat rightMiddleDistalExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_DISTAL.Inverse() * TPose.Pose.rightMiddleDistal.GetRotation();
	FQuat rightMiddleTipExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_TIP.Inverse() * TPose.Pose.rightMiddleTip.GetRotation();

	FQuat rightRingProximalExpected = SMARTSUIT_TPOSE_RIGHT_RING_PROXIMAL.Inverse() * TPose.Pose.rightRingProximal.GetRotation();
	FQuat rightRingMedialExpected = SMARTSUIT_TPOSE_RIGHT_RING_MEDIAL.Inverse() * TPose.Pose.rightRingMedial.GetRotation();
	FQuat rightRingDistalExpected = SMARTSUIT_TPOSE_RIGHT_RING_DISTAL.Inverse() * TPose.Pose.rightRingDistal.GetRotation();
	FQuat rightRingTipExpected = SMARTSUIT_TPOSE_RIGHT_RING_TIP.Inverse() * TPose.Pose.rightRingTip.GetRotation();

	FQuat rightLittleProximalExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_PROXIMAL.Inverse() * TPose.Pose.rightLittleProximal.GetRotation();
	FQuat rightLittleMedialExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_MEDIAL.Inverse() * TPose.Pose.rightLittleMedial.GetRotation();
	FQuat rightLittleDistalExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_DISTAL.Inverse() * TPose.Pose.rightLittleDistal.GetRotation();
	FQuat rightLittleTipExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_TIP.Inverse() * TPose.Pose.rightLittleTip.GetRotation();


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

	float ScaleTest = testval1 * .01f;


	FQuat SuitRotation = FQuat();
	FVector SuitLocation = FVector::ZeroVector;
	EBoneControlSpace SuitTransformBoneControlSpace = EBoneControlSpace::BCS_WorldSpace;//TestBoneControlSpace;
	//if (RelativeToStart) 
	{
		SuitRotation = /*SkelComp->GetComponentRotation().Quaternion() * */hipQuat * hipExpected;

		SuitLocation = (hipPosition * ScaleTest) /*+ TPose.startPos*/;
		ApplySmartsuitTransform(BoneMap.hip, SuitRotation, SuitLocation, FVector(1, 1, 1), TestBoneControlSpace, SkelComp, MeshBases);
	}
	//else 
	//{
	//	SuitRotation = /*SkelComp->GetComponentRotation().Quaternion().Inverse() **/ hipQuat * hipExpected;

	//	SuitLocation = hipPosition * ScaleTest;
	//	ApplySmartsuitTransform(BoneMap.hip, SuitRotation, SuitLocation, FVector(1, 1, 1), SuitTransformBoneControlSpace, SkelComp, MeshBases);
	//}




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

	ApplySmartsuitRotation(BoneMap.leftThumbProximal, leftThumbProximalQuat * leftThumbProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftThumbMedial, leftThumbMedialQuat * leftThumbMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftThumbDistal, leftThumbDistalQuat * leftThumbDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftThumbTip, leftThumbTipQuat * leftThumbTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftIndexProximal, leftIndexProximalQuat * leftIndexProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftIndexMedial, leftIndexMedialQuat * leftIndexMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftIndexDistal, leftIndexDistalQuat * leftIndexDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftIndexTip, leftIndexTipQuat * leftIndexTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftMiddleProximal, leftMiddleProximalQuat * leftMiddleProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftMiddleMedial, leftMiddleMedialQuat * leftMiddleMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftMiddleDistal, leftMiddleDistalQuat * leftMiddleDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftMiddleTip, leftMiddleTipQuat * leftMiddleTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftRingProximal, leftRingProximalQuat * leftRingProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftRingMedial, leftRingMedialQuat * leftRingMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftRingDistal, leftRingDistalQuat * leftRingDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftRingTip, leftRingTipQuat * leftRingTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftLittleProximal, leftLittleProximalQuat * leftLittleProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftLittleMedial, leftLittleMedialQuat * leftLittleMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftLittleDistal, leftLittleDistalQuat * leftLittleDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftLittleTip, leftLittleTipQuat * leftLittleTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightThumbProximal, rightThumbProximalQuat * rightThumbProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightThumbMedial, rightThumbMedialQuat * rightThumbMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightThumbDistal, rightThumbDistalQuat * rightThumbDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightThumbTip, rightThumbTipQuat * rightThumbTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightIndexProximal, rightIndexProximalQuat * rightIndexProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightIndexMedial, rightIndexMedialQuat * rightIndexMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightIndexDistal, rightIndexDistalQuat * rightIndexDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightIndexTip, rightIndexTipQuat * rightIndexTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightMiddleProximal, rightMiddleProximalQuat * rightMiddleProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightMiddleMedial, rightMiddleMedialQuat * rightMiddleMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightMiddleDistal, rightMiddleDistalQuat * rightMiddleDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightMiddleTip, rightMiddleTipQuat * rightMiddleTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightRingProximal, rightRingProximalQuat * rightRingProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightRingMedial, rightRingMedialQuat * rightRingMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightRingDistal, rightRingDistalQuat * rightRingDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightRingTip, rightRingTipQuat * rightRingTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightLittleProximal, rightLittleProximalQuat * rightLittleProximalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightLittleMedial, rightLittleMedialQuat * rightLittleMedialExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightLittleDistal, rightLittleDistalQuat * rightLittleDistalExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightLittleTip, rightLittleTipQuat * rightLittleTipExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
}
PRAGMA_ENABLE_OPTIMIZATION
bool FSmartsuitPoseNode::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// if both bones are valid
#if BUILT_MINOR_VERSION >= 17
	return (BoneMap.hip.IsValidToEvaluate(RequiredBones) && BoneMap.chest.IsValidToEvaluate(RequiredBones) && BoneMap.stomach.IsValidToEvaluate(RequiredBones) &&
		BoneMap.neck.IsValidToEvaluate(RequiredBones) && BoneMap.head.IsValidToEvaluate(RequiredBones) /*&& BoneMap.headTop.IsValidToEvaluate(RequiredBones)*/ &&
		BoneMap.leftShoulder.IsValidToEvaluate(RequiredBones) && BoneMap.leftArm.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftForearm.IsValidToEvaluate(RequiredBones) && BoneMap.leftHand.IsValidToEvaluate(RequiredBones) /*&& BoneMap.leftFingerTip.IsValidToEvaluate(RequiredBones)*/ &&
		BoneMap.rightShoulder.IsValidToEvaluate(RequiredBones) && BoneMap.rightArm.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightForearm.IsValidToEvaluate(RequiredBones) && BoneMap.rightHand.IsValidToEvaluate(RequiredBones) /*&& BoneMap.rightFingerTip.IsValidToEvaluate(RequiredBones)*/ &&
		BoneMap.leftUpleg.IsValidToEvaluate(RequiredBones) && BoneMap.leftLeg.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftFoot.IsValidToEvaluate(RequiredBones) && BoneMap.rightUpleg.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightLeg.IsValidToEvaluate(RequiredBones) && BoneMap.rightFoot.IsValidToEvaluate(RequiredBones) &&
		/*BoneMap.leftToe.IsValidToEvaluate(RequiredBones)*/ /*&& BoneMap.rightToe.IsValidToEvaluate(RequiredBones)*/
		BoneMap.leftThumbProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftThumbMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftThumbDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftThumbTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftIndexProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftIndexMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftIndexDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftIndexTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftMiddleProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftMiddleMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftMiddleDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftMiddleTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftRingProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftRingMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftRingDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftRingTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftLittleProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftLittleMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftLittleDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.leftLittleTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightThumbProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightThumbMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightThumbDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightThumbTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightIndexProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightIndexMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightIndexDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightIndexTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightMiddleProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightMiddleMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightMiddleDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightMiddleTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightRingProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightRingMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightRingDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightRingTip.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightLittleProximal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightLittleMedial.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightLittleDistal.IsValidToEvaluate(RequiredBones) &&
		BoneMap.rightLittleTip.IsValidToEvaluate(RequiredBones)
		);
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
		//BoneMap.headTop = BoneMapOverride->headTop;
		BoneMap.leftShoulder = BoneMapOverride->leftShoulder;
		BoneMap.leftArm = BoneMapOverride->leftArm;
		BoneMap.leftForearm = BoneMapOverride->leftForearm;
		BoneMap.leftHand = BoneMapOverride->leftHand;
		//BoneMap.leftFingerTip = BoneMapOverride->leftFingerTip;
		BoneMap.rightShoulder = BoneMapOverride->rightShoulder;
		BoneMap.rightArm = BoneMapOverride->rightArm;
		BoneMap.rightForearm = BoneMapOverride->rightForearm;
		BoneMap.rightHand = BoneMapOverride->rightHand;
		//BoneMap.rightFingerTip = BoneMapOverride->rightFingerTip;
		BoneMap.leftUpleg = BoneMapOverride->leftUpleg;
		BoneMap.leftLeg = BoneMapOverride->leftLeg;
		BoneMap.leftFoot = BoneMapOverride->leftFoot;
		//BoneMap.leftToe = BoneMapOverride->leftToe;
		BoneMap.rightUpleg = BoneMapOverride->rightUpleg;
		BoneMap.rightLeg = BoneMapOverride->rightLeg;
		BoneMap.rightFoot = BoneMapOverride->rightFoot;
		//BoneMap.rightToe = BoneMapOverride->rightToe;
		BoneMap.leftThumbProximal = BoneMapOverride->leftThumbProximal;
		BoneMap.leftThumbMedial = BoneMapOverride->leftThumbMedial;
		BoneMap.leftThumbDistal = BoneMapOverride->leftThumbDistal;
		BoneMap.leftThumbTip = BoneMapOverride->leftThumbTip;
		BoneMap.leftIndexProximal = BoneMapOverride->leftIndexProximal;
		BoneMap.leftIndexMedial = BoneMapOverride->leftIndexMedial;
		BoneMap.leftIndexDistal = BoneMapOverride->leftIndexDistal;
		BoneMap.leftIndexTip = BoneMapOverride->leftIndexTip;
		BoneMap.leftMiddleProximal = BoneMapOverride->leftMiddleProximal;
		BoneMap.leftMiddleMedial = BoneMapOverride->leftMiddleMedial;
		BoneMap.leftMiddleDistal = BoneMapOverride->leftMiddleDistal;
		BoneMap.leftMiddleTip = BoneMapOverride->leftMiddleTip;
		BoneMap.leftRingProximal = BoneMapOverride->leftRingProximal;
		BoneMap.leftRingMedial = BoneMapOverride->leftRingMedial;
		BoneMap.leftRingDistal = BoneMapOverride->leftRingDistal;
		BoneMap.leftRingTip = BoneMapOverride->leftRingTip;
		BoneMap.leftLittleProximal = BoneMapOverride->leftLittleProximal;
		BoneMap.leftLittleMedial = BoneMapOverride->leftLittleMedial;
		BoneMap.leftLittleDistal = BoneMapOverride->leftLittleDistal;
		BoneMap.leftLittleTip = BoneMapOverride->leftLittleTip;
		BoneMap.rightThumbProximal = BoneMapOverride->rightThumbProximal;
		BoneMap.rightThumbMedial = BoneMapOverride->rightThumbMedial;
		BoneMap.rightThumbDistal = BoneMapOverride->rightThumbDistal;
		BoneMap.rightThumbTip = BoneMapOverride->rightThumbTip;
		BoneMap.rightIndexProximal = BoneMapOverride->rightIndexProximal;
		BoneMap.rightIndexMedial = BoneMapOverride->rightIndexMedial;
		BoneMap.rightIndexDistal = BoneMapOverride->rightIndexDistal;
		BoneMap.rightIndexTip = BoneMapOverride->rightIndexTip;
		BoneMap.rightMiddleProximal = BoneMapOverride->rightMiddleProximal;
		BoneMap.rightMiddleMedial = BoneMapOverride->rightMiddleMedial;
		BoneMap.rightMiddleDistal = BoneMapOverride->rightMiddleDistal;
		BoneMap.rightMiddleTip = BoneMapOverride->rightMiddleTip;
		BoneMap.rightRingProximal = BoneMapOverride->rightRingProximal;
		BoneMap.rightRingMedial = BoneMapOverride->rightRingMedial;
		BoneMap.rightRingDistal = BoneMapOverride->rightRingDistal;
		BoneMap.rightRingTip = BoneMapOverride->rightRingTip;
		BoneMap.rightLittleProximal = BoneMapOverride->rightLittleProximal;
		BoneMap.rightLittleMedial = BoneMapOverride->rightLittleMedial;
		BoneMap.rightLittleDistal = BoneMapOverride->rightLittleDistal;
		BoneMap.rightLittleTip = BoneMapOverride->rightLittleTip;
	}

	BoneMap.hip.Initialize(RequiredBones);
	BoneMap.stomach.Initialize(RequiredBones);
	BoneMap.chest.Initialize(RequiredBones);
	BoneMap.neck.Initialize(RequiredBones);
	BoneMap.head.Initialize(RequiredBones);
	//BoneMap.headTop.Initialize(RequiredBones);
	BoneMap.leftShoulder.Initialize(RequiredBones);
	BoneMap.leftArm.Initialize(RequiredBones);
	BoneMap.leftForearm.Initialize(RequiredBones);
	BoneMap.leftHand.Initialize(RequiredBones);
	//BoneMap.leftFingerTip.Initialize(RequiredBones);
	BoneMap.rightShoulder.Initialize(RequiredBones);
	BoneMap.rightArm.Initialize(RequiredBones);
	BoneMap.rightForearm.Initialize(RequiredBones);
	BoneMap.rightHand.Initialize(RequiredBones);
	//BoneMap.rightFingerTip.Initialize(RequiredBones);
	BoneMap.leftUpleg.Initialize(RequiredBones);
	BoneMap.leftLeg.Initialize(RequiredBones);
	BoneMap.leftFoot.Initialize(RequiredBones);
	//BoneMap.leftToe.Initialize(RequiredBones);
	BoneMap.rightUpleg.Initialize(RequiredBones);
	BoneMap.rightLeg.Initialize(RequiredBones);
	BoneMap.rightFoot.Initialize(RequiredBones);
	//BoneMap.rightToe.Initialize(RequiredBones);

	BoneMap.leftThumbProximal.Initialize(RequiredBones);
	BoneMap.leftThumbMedial.Initialize(RequiredBones);
	BoneMap.leftThumbDistal.Initialize(RequiredBones);
	BoneMap.leftThumbTip.Initialize(RequiredBones);
	BoneMap.leftIndexProximal.Initialize(RequiredBones);
	BoneMap.leftIndexMedial.Initialize(RequiredBones);
	BoneMap.leftIndexDistal.Initialize(RequiredBones);
	BoneMap.leftIndexTip.Initialize(RequiredBones);
	BoneMap.leftMiddleProximal.Initialize(RequiredBones);
	BoneMap.leftMiddleMedial.Initialize(RequiredBones);
	BoneMap.leftMiddleDistal.Initialize(RequiredBones);
	BoneMap.leftMiddleTip.Initialize(RequiredBones);
	BoneMap.leftRingProximal.Initialize(RequiredBones);
	BoneMap.leftRingMedial.Initialize(RequiredBones);
	BoneMap.leftRingDistal.Initialize(RequiredBones);
	BoneMap.leftRingTip.Initialize(RequiredBones);
	BoneMap.leftLittleProximal.Initialize(RequiredBones);
	BoneMap.leftLittleMedial.Initialize(RequiredBones);
	BoneMap.leftLittleDistal.Initialize(RequiredBones);
	BoneMap.leftLittleTip.Initialize(RequiredBones);
	BoneMap.rightThumbProximal.Initialize(RequiredBones);
	BoneMap.rightThumbMedial.Initialize(RequiredBones);
	BoneMap.rightThumbDistal.Initialize(RequiredBones);
	BoneMap.rightThumbTip.Initialize(RequiredBones);
	BoneMap.rightIndexProximal.Initialize(RequiredBones);
	BoneMap.rightIndexMedial.Initialize(RequiredBones);
	BoneMap.rightIndexDistal.Initialize(RequiredBones);
	BoneMap.rightIndexTip.Initialize(RequiredBones);
	BoneMap.rightMiddleProximal.Initialize(RequiredBones);
	BoneMap.rightMiddleMedial.Initialize(RequiredBones);
	BoneMap.rightMiddleDistal.Initialize(RequiredBones);
	BoneMap.rightMiddleTip.Initialize(RequiredBones);
	BoneMap.rightRingProximal.Initialize(RequiredBones);
	BoneMap.rightRingMedial.Initialize(RequiredBones);
	BoneMap.rightRingDistal.Initialize(RequiredBones);
	BoneMap.rightRingTip.Initialize(RequiredBones);
	BoneMap.rightLittleProximal.Initialize(RequiredBones);
	BoneMap.rightLittleMedial.Initialize(RequiredBones);
	BoneMap.rightLittleDistal.Initialize(RequiredBones);
	BoneMap.rightLittleTip.Initialize(RequiredBones);
}

USmartsuitBodyMapData::USmartsuitBodyMapData()
{
	hip = "hip";
	stomach = "stomach";
	chest = "chest";
	neck = "neck";
	head = "head";
	//headTop = "headTop";
	leftShoulder = "leftShoulder";
	leftArm = "leftArm";
	leftForearm = "leftForearm";
	leftHand = "leftHand";
	//leftFingerTip = "leftFingerTip";
	rightShoulder = "rightShoulder";
	rightArm = "rightArm";
	rightForearm = "rightForearm";
	rightHand = "rightHand";
	//rightFingerTip = "rightFingerTip";
	leftUpleg = "leftUpleg";
	leftLeg = "leftLeg";
	leftFoot = "leftFoot";
	//leftToe = "leftToe";
	rightUpleg = "rightUpleg";
	rightLeg = "rightLeg";
	rightFoot = "rightFoot";
	//rightToe = "rightToe";
	leftThumbProximal = "leftThumbProximal";
	leftThumbMedial = "leftThumbMedial";
	leftThumbDistal = "leftThumbDistal";
	leftThumbTip = "leftThumbTip";
	leftIndexProximal = "leftIndexProximal";
	leftIndexMedial = "leftIndexMedial";
	leftIndexDistal = "leftIndexDistal";
	leftIndexTip = "leftIndexTip";
	leftMiddleProximal = "leftMiddleProximal";
	leftMiddleMedial = "leftMiddleMedial";
	leftMiddleDistal = "leftMiddleDistal";
	leftMiddleTip = "leftMiddleTip";
	leftRingProximal = "leftRingProximal";
	leftRingMedial = "leftRingMedial";
	leftRingDistal = "leftRingDistal";
	leftRingTip = "leftRingTip";
	leftLittleProximal = "leftLittleProximal";
	leftLittleMedial = "leftLittleMedial";
	leftLittleDistal = "leftLittleDistal";
	leftLittleTip = "leftLittleTip";
	rightThumbProximal = "rightThumbProximal";
	rightThumbMedial = "rightThumbMedial";
	rightThumbDistal = "rightThumbDistal";
	rightThumbTip = "rightThumbTip";
	rightIndexProximal = "rightIndexProximal";
	rightIndexMedial = "rightIndexMedial";
	rightIndexDistal = "rightIndexDistal";
	rightIndexTip = "rightIndexTip";
	rightMiddleProximal = "rightMiddleProximal";
	rightMiddleMedial = "rightMiddleMedial";
	rightMiddleDistal = "rightMiddleDistal";
	rightMiddleTip = "rightMiddleTip";
	rightRingProximal = "rightRingProximal";
	rightRingMedial = "rightRingMedial";
	rightRingDistal = "rightRingDistal";
	rightRingTip = "rightRingTip";
	rightLittleProximal = "rightLittleProximal";
	rightLittleMedial = "rightLittleMedial";
	rightLittleDistal = "rightLittleDistal";
	rightLittleTip = "rightLittleTip";
}

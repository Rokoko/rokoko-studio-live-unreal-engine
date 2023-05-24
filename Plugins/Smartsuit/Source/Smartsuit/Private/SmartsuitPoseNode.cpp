// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitPoseNode.h"
#include "AnimationRuntime.h"
#include "LiveLinkCustomVersion.h"
#include "LiveLinkClient.h"
#include "RokokoSkeletonData.h"
#include "Animation/AnimInstanceProxy.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "SmartsuitBlueprintLibrary.h"
#include "Roles/LiveLinkSmartsuitRole.h"
#include "Roles/LiveLinkSmartsuitTypes.h"



/////////////////////////////////////////////////////
// SmartsuitPoseNode

FSmartsuitPoseNode::FSmartsuitPoseNode()
{
	//UE_LOG(LogTemp, Warning, TEXT("Smartsuit Pose Node: Initializing..."));
	//TPose = new SmartsuitTPose();

	//Controller = nullptr;
	//RelativeToStart = false;
	//ScaleBones = false;

	//if (Bone_Map_Override_OLD->IsValidLowLevel())
	//{
	//	BoneMap.hip = Bone_Map_Override_OLD->hip;
	//	BoneMap.stomach = Bone_Map_Override_OLD->stomach;
	//	BoneMap.chest = Bone_Map_Override_OLD->chest;
	//	BoneMap.neck = Bone_Map_Override_OLD->neck;
	//	BoneMap.head = Bone_Map_Override_OLD->head;
	//	BoneMap.headTop = Bone_Map_Override_OLD->headTop;
	//	BoneMap.leftShoulder = Bone_Map_Override_OLD->leftShoulder;
	//	BoneMap.leftArm = Bone_Map_Override_OLD->leftArm;
	//	BoneMap.leftForearm = Bone_Map_Override_OLD->leftForearm;
	//	BoneMap.leftHand = Bone_Map_Override_OLD->leftHand;
	//	BoneMap.leftFingerTip = Bone_Map_Override_OLD->leftFingerTip;
	//	BoneMap.rightShoulder = Bone_Map_Override_OLD->rightShoulder;
	//	BoneMap.rightArm = Bone_Map_Override_OLD->rightArm;
	//	BoneMap.rightForearm = Bone_Map_Override_OLD->rightForearm;
	//	BoneMap.rightHand = Bone_Map_Override_OLD->rightHand;
	//	BoneMap.rightFingerTip = Bone_Map_Override_OLD->rightFingerTip;
	//	BoneMap.leftUpleg = Bone_Map_Override_OLD->leftUpleg;
	//	BoneMap.leftLeg = Bone_Map_Override_OLD->leftLeg;
	//	BoneMap.leftFoot = Bone_Map_Override_OLD->leftFoot;
	//	BoneMap.leftToe = Bone_Map_Override_OLD->leftToe;
	//	BoneMap.rightUpleg = Bone_Map_Override_OLD->rightUpleg;
	//	BoneMap.rightLeg = Bone_Map_Override_OLD->rightLeg;
	//	BoneMap.rightFoot = Bone_Map_Override_OLD->rightFoot;
	//	BoneMap.rightToe = Bone_Map_Override_OLD->rightToe;
	//}

	//CurrentRetargetAsset = nullptr;
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

	if (CompactPoseBoneToModify == -1)
	{
		return;
	}

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

	if (CompactPoseBoneToModify == -1)
	{
		return FTransform();
	}

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
	//const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	//int index = 0;
	//for (int i = bone.BoneIndex + 1; i < BoneContainer.GetNumBones(); i++) 
	//{
	//	if (i == BoneMap.leftUpleg.BoneIndex || i == BoneMap.rightUpleg.BoneIndex) 
	//	{
	//		FVector hipWPos = OriginalTransform(BoneMap.hip, space, SkelComp, MeshBases).GetTranslation();
	//		
	//		FVector upLegWPos;
	//		if (i == BoneMap.leftUpleg.BoneIndex) 
	//		{
	//			upLegWPos = OriginalTransform(BoneMap.leftUpleg, space, SkelComp, MeshBases).GetTranslation();
	//		}
	//		else 
	//		{
	//			upLegWPos = OriginalTransform(BoneMap.rightUpleg, space, SkelComp, MeshBases).GetTranslation();
	//		}
	//		FVector diff = upLegWPos - hipWPos;
	//		FVector dir;
	//		float length;
	//		diff.ToDirectionAndLength(dir, length);
	//		FCompactPoseBoneIndex CompactPoseBoneToModify(i);
	//		FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);
	//		//UE_LOG(LogTemp, Warning, TEXT("UpLeg found, will use world lenght: %f, with dir %f, %f, %f"), hipWidth / 2, diff.X, diff.Y, diff.Z);

	//		//if (i == BoneMap.)
	//		FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	//		NewBoneTM.SetTranslation(hipWPos + (dir * (hipWidth / 2)));
	//		FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	//		MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
	//	}
	//	else 
	//	{
	//		FCompactPoseBoneIndex CompactPoseBoneToModify(i);
	//		FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);
	//		//if (i == BoneMap.)
	//		FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	//		NewBoneTM.SetTranslation(transforms[index].GetTranslation());
	//		FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	//		MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
	//	}
	//	index++;
	//}
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
	if (auto SmartsuitBone = suitdata->bones.Find(BoneName))
	{
		return SmartsuitBone->Uquaternion();
	}

	return FQuat::Identity;
}

FVector GetPosition2(const FName& BoneName, FSuitData* suitdata)
{
	if (auto SmartsuitBone = suitdata->bones.Find(BoneName))
	{
		return SmartsuitBone->UPosition();
	}

	return FVector::ZeroVector;
}

FQuat GetRotation3(const FName& BoneName, FLiveLinkSubjectFrameData &InSubjectFrameData)
{
	FLiveLinkSkeletonStaticData* SkeletonData = InSubjectFrameData.StaticData.Cast<FLiveLinkSkeletonStaticData>();
	FLiveLinkAnimationFrameData* FrameData = InSubjectFrameData.FrameData.Cast<FLiveLinkAnimationFrameData>();

	int32 index = -1;
	if (auto SmartsuitBone = SkeletonData->BoneNames.Find(BoneName, index))
	{
		return FrameData->Transforms[index].GetRotation();
	}

	return FQuat::Identity;
}

FVector GetPosition3(const FName& BoneName, FLiveLinkSubjectFrameData &InSubjectFrameData)
{
	FLiveLinkSkeletonStaticData* SkeletonData = InSubjectFrameData.StaticData.Cast<FLiveLinkSkeletonStaticData>();
	FLiveLinkAnimationFrameData* FrameData = InSubjectFrameData.FrameData.Cast<FLiveLinkAnimationFrameData>();

	int32 index = -1;
	if (auto SmartsuitBone = SkeletonData->BoneNames.Find(BoneName, index))
	{
		return FrameData->Transforms[index].GetLocation();
	}

	return FVector::ZeroVector;
}

PRAGMA_DISABLE_OPTIMIZATION
void FSmartsuitPoseNode::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	USkeletalMeshComponent* SkelComp = Output.AnimInstanceProxy->GetSkelMeshComponent();
	FCSPose<FCompactPose>& MeshBases = Output.Pose;

	check(OutBoneTransforms.Num() == 0);
	//if (!Controller/* || !Controller->SupportsWiFi()*/) 
	//{
	//	return;
	//}

	ARokokoReceiver *receiver = GetReceiver();
	if (!receiver) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("No receiver"));
		//return;
	}
	FSuitData* data = nullptr;//receiver->GetSmartsuit(Controller->suitname);
	if (!data) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("No data for %s"), *Controller->suitname);
		//return;
	}

	if (!LiveLinkClient_AnyThread /*|| !CurrentRetargetAsset*/)
	{
		return;
	}


	//if (!CurrentRetargetAsset->IsA(URokokoBodyMapData::StaticClass()))
	//{
	//	return;
	//}

	FLiveLinkSubjectFrameData SubjectFrameData;

	FLiveLinkSubjectName LiveLinkSubjectName = GetLiveLinkSubjectName();

	TSubclassOf<ULiveLinkRole> SubjectRole = LiveLinkClient_AnyThread->GetSubjectRole_AnyThread(LiveLinkSubjectName);
	if (SubjectRole)
	{
#ifdef USE_SMARTSUIT_ANIMATION_ROLE
		if (SubjectRole->IsChildOf(ULiveLinkSmartsuitRole::StaticClass()))
		{
			//Process animation data if the subject is from that type
			if (LiveLinkClient_AnyThread->EvaluateFrame_AnyThread(LiveLinkSubjectName, ULiveLinkSmartsuitRole::StaticClass(), SubjectFrameData))
			{

			}
			else
			{
				return;
			}


		}
		else
		{
			return;
		}
#else
		if (SubjectRole->IsChildOf(ULiveLinkAnimationRole::StaticClass()))
		{
			//Process animation data if the subject is from that type
			if (LiveLinkClient_AnyThread->EvaluateFrame_AnyThread(LiveLinkSubjectName, ULiveLinkAnimationRole::StaticClass(), SubjectFrameData))
			{

			}
			else
			{
				return;
			}


	}
		else
		{
			return;
		}
#endif
	}
	else
	{
		return;
	}

#ifdef USE_SMARTSUIT_ANIMATION_ROLE
	FLiveLinkSmartsuitStaticData* SkeletonData = SubjectFrameData.StaticData.Cast<FLiveLinkSmartsuitStaticData>();
	FLiveLinkSmartsuitFrameData* FrameData = SubjectFrameData.FrameData.Cast<FLiveLinkSmartsuitFrameData>();
#else
	FLiveLinkSkeletonStaticData* SkeletonData = SubjectFrameData.StaticData.Cast<FLiveLinkSkeletonStaticData>();
	FLiveLinkAnimationFrameData* FrameData = SubjectFrameData.FrameData.Cast<FLiveLinkAnimationFrameData>();
#endif

	check(SkeletonData);
	check(FrameData);



	//SkeletonData->PropertyNames.Find()


	EBoneControlSpace TestBoneControlSpace = BCS_ComponentSpace;

	if (!TPose.StoredTPose /*&& CurrentRetargetAsset*/)
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

		TPose.Pose.hip =					OriginalTransform(BoneMap.hip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.stomach =				OriginalTransform(BoneMap.stomach, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.chest =					OriginalTransform(BoneMap.chest, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.neck =					OriginalTransform(BoneMap.neck, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.head =					OriginalTransform(BoneMap.head, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftShoulder =			OriginalTransform(BoneMap.leftShoulder, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftArm =				OriginalTransform(BoneMap.leftArm, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftForearm =			OriginalTransform(BoneMap.leftForearm, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftHand =				OriginalTransform(BoneMap.leftHand, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightShoulder =			OriginalTransform(BoneMap.rightShoulder, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightArm =				OriginalTransform(BoneMap.rightArm, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightForearm =			OriginalTransform(BoneMap.rightForearm, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightHand =				OriginalTransform(BoneMap.rightHand, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftUpLeg =				OriginalTransform(BoneMap.leftUpleg, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftLeg =				OriginalTransform(BoneMap.leftLeg, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftFoot =				OriginalTransform(BoneMap.leftFoot, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftToe =				OriginalTransform(BoneMap.leftToe, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightUpLeg =				OriginalTransform(BoneMap.rightUpleg, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightLeg =				OriginalTransform(BoneMap.rightLeg, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightFoot =				OriginalTransform(BoneMap.rightFoot, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightToe =				OriginalTransform(BoneMap.rightToe, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftThumbProximal =		OriginalTransform(BoneMap.leftThumbProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftThumbMedial =		OriginalTransform(BoneMap.leftThumbMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftThumbDistal =		OriginalTransform(BoneMap.leftThumbDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftThumbTip =			OriginalTransform(BoneMap.leftThumbTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftIndexProximal =		OriginalTransform(BoneMap.leftIndexProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftIndexMedial =		OriginalTransform(BoneMap.leftIndexMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftIndexDistal =		OriginalTransform(BoneMap.leftIndexDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftIndexTip =			OriginalTransform(BoneMap.leftIndexTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftMiddleProximal =		OriginalTransform(BoneMap.leftMiddleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftMiddleMedial =		OriginalTransform(BoneMap.leftMiddleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftMiddleDistal =		OriginalTransform(BoneMap.leftMiddleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftMiddleTip =			OriginalTransform(BoneMap.leftMiddleTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftRingProximal =		OriginalTransform(BoneMap.leftRingProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftRingMedial =			OriginalTransform(BoneMap.leftRingMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftRingDistal =			OriginalTransform(BoneMap.leftRingDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftRingTip =			OriginalTransform(BoneMap.leftRingTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftLittleProximal =		OriginalTransform(BoneMap.leftLittleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftLittleMedial =		OriginalTransform(BoneMap.leftLittleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftLittleDistal =		OriginalTransform(BoneMap.leftLittleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.leftLittleTip =			OriginalTransform(BoneMap.leftLittleTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightThumbProximal =		OriginalTransform(BoneMap.rightThumbProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightThumbMedial =		OriginalTransform(BoneMap.rightThumbMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightThumbDistal =		OriginalTransform(BoneMap.rightThumbDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightThumbTip =			OriginalTransform(BoneMap.rightThumbTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightIndexProximal =		OriginalTransform(BoneMap.rightIndexProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightIndexMedial =		OriginalTransform(BoneMap.rightIndexMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightIndexDistal =		OriginalTransform(BoneMap.rightIndexDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightIndexTip =			OriginalTransform(BoneMap.rightIndexTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightMiddleProximal =	OriginalTransform(BoneMap.rightMiddleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightMiddleMedial =		OriginalTransform(BoneMap.rightMiddleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightMiddleDistal =		OriginalTransform(BoneMap.rightMiddleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightMiddleTip =			OriginalTransform(BoneMap.rightMiddleTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightRingProximal =		OriginalTransform(BoneMap.rightRingProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightRingMedial =		OriginalTransform(BoneMap.rightRingMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightRingDistal =		OriginalTransform(BoneMap.rightRingDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightRingTip =			OriginalTransform(BoneMap.rightRingTip, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightLittleProximal =	OriginalTransform(BoneMap.rightLittleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightLittleMedial =		OriginalTransform(BoneMap.rightLittleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightLittleDistal =		OriginalTransform(BoneMap.rightLittleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		TPose.Pose.rightLittleTip =			OriginalTransform(BoneMap.rightLittleTip, TestBoneControlSpace, SkelComp, MeshBases);
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


	FQuat hipQuat =						GetRotation3(SmartsuitBones::hip, SubjectFrameData);// *modifier;
	FVector hipPosition =				GetPosition3(SmartsuitBones::hip, SubjectFrameData);
	FQuat stomachQuat =					GetRotation3(SmartsuitBones::spine, SubjectFrameData);
	FQuat chestQuat =					GetRotation3(SmartsuitBones::chest, SubjectFrameData);
	FQuat neckQuat =					GetRotation3(SmartsuitBones::neck, SubjectFrameData);
	FQuat headQuat =					GetRotation3(SmartsuitBones::head, SubjectFrameData);
	FQuat leftShoulderQuat =			GetRotation3(SmartsuitBones::leftShoulder, SubjectFrameData);
	FQuat leftArmQuat =					GetRotation3(SmartsuitBones::leftUpperArm, SubjectFrameData);
	FQuat leftForearmQuat =				GetRotation3(SmartsuitBones::leftLowerArm, SubjectFrameData);
	FQuat leftHandQuat =				GetRotation3(SmartsuitBones::leftHand, SubjectFrameData);
	FQuat rightShoulderQuat =			GetRotation3(SmartsuitBones::rightShoulder, SubjectFrameData);
	FQuat rightArmQuat =				GetRotation3(SmartsuitBones::rightUpperArm, SubjectFrameData);
	FQuat rightForearmQuat =			GetRotation3(SmartsuitBones::rightLowerArm, SubjectFrameData);
	FQuat rightHandQuat =				GetRotation3(SmartsuitBones::rightHand, SubjectFrameData);
	FQuat leftUpLegQuat =				GetRotation3(SmartsuitBones::leftUpLeg, SubjectFrameData);
	FQuat leftLegQuat =					GetRotation3(SmartsuitBones::leftLeg, SubjectFrameData);
	FQuat leftFootQuat =				GetRotation3(SmartsuitBones::leftFoot, SubjectFrameData);
	FQuat leftToeQuat =					GetRotation3(SmartsuitBones::leftToe, SubjectFrameData);
	FQuat rightUpLegQuat =				GetRotation3(SmartsuitBones::rightUpLeg, SubjectFrameData);
	FQuat rightLegQuat =				GetRotation3(SmartsuitBones::rightLeg, SubjectFrameData);
	FQuat rightFootQuat =				GetRotation3(SmartsuitBones::rightFoot, SubjectFrameData);
	FQuat rightToeQuat =				GetRotation3(SmartsuitBones::rightToe, SubjectFrameData);

	FQuat leftThumbProximalQuat =		GetRotation3(SmartsuitBones::leftThumbProximal, SubjectFrameData);
	FQuat leftThumbMedialQuat =			GetRotation3(SmartsuitBones::leftThumbMedial, SubjectFrameData);
	FQuat leftThumbDistalQuat =			GetRotation3(SmartsuitBones::leftThumbDistal, SubjectFrameData);
	FQuat leftThumbTipQuat =			GetRotation3(SmartsuitBones::leftThumbTip, SubjectFrameData);
	FQuat leftIndexProximalQuat =		GetRotation3(SmartsuitBones::leftIndexProximal, SubjectFrameData);
	FQuat leftIndexMedialQuat =			GetRotation3(SmartsuitBones::leftIndexMedial, SubjectFrameData);
	FQuat leftIndexDistalQuat =			GetRotation3(SmartsuitBones::leftIndexDistal, SubjectFrameData);
	FQuat leftIndexTipQuat =			GetRotation3(SmartsuitBones::leftIndexTip, SubjectFrameData);
	FQuat leftMiddleProximalQuat =		GetRotation3(SmartsuitBones::leftMiddleProximal, SubjectFrameData);
	FQuat leftMiddleMedialQuat =		GetRotation3(SmartsuitBones::leftMiddleMedial, SubjectFrameData);
	FQuat leftMiddleDistalQuat =		GetRotation3(SmartsuitBones::leftMiddleDistal, SubjectFrameData);
	FQuat leftMiddleTipQuat =			GetRotation3(SmartsuitBones::leftMiddleTip, SubjectFrameData);
	FQuat leftRingProximalQuat =		GetRotation3(SmartsuitBones::leftRingProximal, SubjectFrameData);
	FQuat leftRingMedialQuat =			GetRotation3(SmartsuitBones::leftRingMedial, SubjectFrameData);
	FQuat leftRingDistalQuat =			GetRotation3(SmartsuitBones::leftRingDistal, SubjectFrameData);
	FQuat leftRingTipQuat =				GetRotation3(SmartsuitBones::leftRingTip, SubjectFrameData);
	FQuat leftLittleProximalQuat =		GetRotation3(SmartsuitBones::leftLittleProximal, SubjectFrameData);
	FQuat leftLittleMedialQuat =		GetRotation3(SmartsuitBones::leftLittleMedial, SubjectFrameData);
	FQuat leftLittleDistalQuat =		GetRotation3(SmartsuitBones::leftLittleDistal, SubjectFrameData);
	FQuat leftLittleTipQuat =			GetRotation3(SmartsuitBones::leftLittleTip, SubjectFrameData);
	FQuat rightThumbProximalQuat =		GetRotation3(SmartsuitBones::rightThumbProximal, SubjectFrameData);
	FQuat rightThumbMedialQuat =		GetRotation3(SmartsuitBones::rightThumbMedial, SubjectFrameData);
	FQuat rightThumbDistalQuat =		GetRotation3(SmartsuitBones::rightThumbDistal, SubjectFrameData);
	FQuat rightThumbTipQuat =			GetRotation3(SmartsuitBones::rightThumbTip, SubjectFrameData);
	FQuat rightIndexProximalQuat =		GetRotation3(SmartsuitBones::rightIndexProximal, SubjectFrameData);
	FQuat rightIndexMedialQuat =		GetRotation3(SmartsuitBones::rightIndexMedial, SubjectFrameData);
	FQuat rightIndexDistalQuat =		GetRotation3(SmartsuitBones::rightIndexDistal, SubjectFrameData);
	FQuat rightIndexTipQuat =			GetRotation3(SmartsuitBones::rightIndexTip, SubjectFrameData);
	FQuat rightMiddleProximalQuat =		GetRotation3(SmartsuitBones::rightMiddleProximal, SubjectFrameData);
	FQuat rightMiddleMedialQuat =		GetRotation3(SmartsuitBones::rightMiddleMedial, SubjectFrameData);
	FQuat rightMiddleDistalQuat =		GetRotation3(SmartsuitBones::rightMiddleDistal, SubjectFrameData);
	FQuat rightMiddleTipQuat =			GetRotation3(SmartsuitBones::rightMiddleTip, SubjectFrameData);
	FQuat rightRingProximalQuat =		GetRotation3(SmartsuitBones::rightRingProximal, SubjectFrameData);
	FQuat rightRingMedialQuat =			GetRotation3(SmartsuitBones::rightRingMedial, SubjectFrameData);
	FQuat rightRingDistalQuat =			GetRotation3(SmartsuitBones::rightRingDistal, SubjectFrameData);
	FQuat rightRingTipQuat =			GetRotation3(SmartsuitBones::rightRingTip, SubjectFrameData);
	FQuat rightLittleProximalQuat =		GetRotation3(SmartsuitBones::rightLittleProximal, SubjectFrameData);
	FQuat rightLittleMedialQuat =		GetRotation3(SmartsuitBones::rightLittleMedial, SubjectFrameData);
	FQuat rightLittleDistalQuat =		GetRotation3(SmartsuitBones::rightLittleDistal, SubjectFrameData);
	FQuat rightLittleTipQuat =			GetRotation3(SmartsuitBones::rightLittleTip, SubjectFrameData);




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
	FQuat leftToeExpected = SMARTSUIT_TPOSE_LEFT_TOE.Inverse() * TPose.Pose.leftToe.GetRotation();

	FQuat rightUpLegExpected = SMARTSUIT_TPOSE_RIGHT_UP_LEG.Inverse() * TPose.Pose.rightUpLeg.GetRotation();
	FQuat rightLegExpected = SMARTSUIT_TPOSE_RIGHT_LEG.Inverse() * TPose.Pose.rightLeg.GetRotation();
	FQuat rightFootExpected = SMARTSUIT_TPOSE_RIGHT_FOOT.Inverse() * TPose.Pose.rightFoot.GetRotation();
	FQuat rightToeExpected = SMARTSUIT_TPOSE_RIGHT_TOE.Inverse() * TPose.Pose.rightToe.GetRotation();

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

	if (CompactPoseBoneToModify == -1)
	{
		return;
	}

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

	// Apply root motion, if relevant
	if (bApplyRootMotion)
	{
		FVector NewRootPosition = hipPosition;
		NewRootPosition.Z = 0.0f;

		// Make sure to initialize root motion position, otherwise we will get erronous results
		if (!bInitializedRootPosition)
		{
			OldRootPosition = NewRootPosition;
			bInitializedRootPosition = true;
		}

		FVector RootDelta = NewRootPosition - OldRootPosition;
		OldRootPosition = NewRootPosition;

		FTransform NewRootTransform;
		NewRootTransform.SetRotation(FQuat::Identity);
		NewRootTransform.SetScale3D(FVector::OneVector);
		NewRootTransform.SetTranslation(RootDelta);

		Output.AnimInstanceProxy->GetExtractedRootMotion().Accumulate(NewRootTransform);
	}

	ApplySmartsuitRotation(BoneMap.stomach, stomachQuat* stomachExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.chest, chestQuat* chestExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.neck, neckQuat* neckExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.head, headQuat* headExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftShoulder, leftShoulderQuat* leftShoulderExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftArm, leftArmQuat* leftArmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftForearm, leftForearmQuat* leftForearmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftHand, leftHandQuat* leftHandExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightShoulder, rightShoulderQuat* rightShoulderExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightArm, rightArmQuat* rightArmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightForearm, rightForearmQuat* rightForearmExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightHand, rightHandQuat* rightHandExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftUpleg, leftUpLegQuat* leftUpLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftLeg, leftLegQuat* leftLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftFoot, leftFootQuat* leftFootExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.leftToe, leftToeQuat* leftToeExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightUpleg, rightUpLegQuat* rightUpLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightLeg, rightLegQuat* rightLegExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightFoot, rightFootQuat* rightFootExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);
	ApplySmartsuitRotation(BoneMap.rightToe, rightToeQuat* rightToeExpected, hipQuat, TestBoneControlSpace, SkelComp, MeshBases);

	bool HasLeftGlove = true;
	bool HasRightGlove = true;

	#ifdef USE_SMARTSUIT_ANIMATION_ROLE
	HasLeftGlove = FrameData->HasLeftGlove;
	HasRightGlove = FrameData->HasRightGlove;
	#endif

	if(HasLeftGlove)
	{
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
	}

	if (HasRightGlove)
	{
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
}
PRAGMA_ENABLE_OPTIMIZATION

bool FSmartsuitPoseNode::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return true;
//
//	// if both bones are valid
//	return (BoneMap.hip.IsValidToEvaluate(RequiredBones) && /*BoneMap.chest.IsValidToEvaluate(RequiredBones) &&*/ BoneMap.stomach.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.neck.IsValidToEvaluate(RequiredBones) && BoneMap.head.IsValidToEvaluate(RequiredBones) /*&& BoneMap.headTop.IsValidToEvaluate(RequiredBones)*/ &&
//		BoneMap.leftShoulder.IsValidToEvaluate(RequiredBones) && BoneMap.leftArm.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftForearm.IsValidToEvaluate(RequiredBones) && BoneMap.leftHand.IsValidToEvaluate(RequiredBones) /*&& BoneMap.leftFingerTip.IsValidToEvaluate(RequiredBones)*/ &&
//		BoneMap.rightShoulder.IsValidToEvaluate(RequiredBones) && BoneMap.rightArm.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightForearm.IsValidToEvaluate(RequiredBones) && BoneMap.rightHand.IsValidToEvaluate(RequiredBones) /*&& BoneMap.rightFingerTip.IsValidToEvaluate(RequiredBones)*/ &&
//		BoneMap.leftUpleg.IsValidToEvaluate(RequiredBones) && BoneMap.leftLeg.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftFoot.IsValidToEvaluate(RequiredBones) && BoneMap.rightUpleg.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightLeg.IsValidToEvaluate(RequiredBones) && BoneMap.rightFoot.IsValidToEvaluate(RequiredBones) &&
//		/*BoneMap.leftToe.IsValidToEvaluate(RequiredBones)*/ /*&& BoneMap.rightToe.IsValidToEvaluate(RequiredBones)*/
//		BoneMap.leftThumbProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftThumbMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftThumbDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftThumbTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftIndexProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftIndexMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftIndexDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftIndexTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftMiddleProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftMiddleMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftMiddleDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftMiddleTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftRingProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftRingMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftRingDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftRingTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftLittleProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftLittleMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftLittleDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.leftLittleTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightThumbProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightThumbMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightThumbDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightThumbTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightIndexProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightIndexMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightIndexDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightIndexTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightMiddleProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightMiddleMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightMiddleDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightMiddleTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightRingProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightRingMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightRingDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightRingTip.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightLittleProximal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightLittleMedial.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightLittleDistal.IsValidToEvaluate(RequiredBones) &&
//		BoneMap.rightLittleTip.IsValidToEvaluate(RequiredBones)
//		);

}

void FSmartsuitPoseNode::PreUpdate(const UAnimInstance* InAnimInstance)
{
	Super::PreUpdate(InAnimInstance);
	//
	// LiveLinkClient_AnyThread = LiveLinkClient_GameThread.GetClient();


	ILiveLinkClient* ThisFrameClient = nullptr;
	IModularFeatures& ModularFeatures = IModularFeatures::Get();
	if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ThisFrameClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
	}
	LiveLinkClient_AnyThread = ThisFrameClient;

	CreateRetargetAsset(InAnimInstance);
}

void FSmartsuitPoseNode::CreateRetargetAsset(const UAnimInstance* InAnimInstance)
{
	// Protection as a class graph pin does not honor rules on abstract classes and NoClear
	UClass* RetargetAssetPtr = RetargetAsset.Get();
	if (!RetargetAssetPtr || RetargetAssetPtr->HasAnyClassFlags(CLASS_Abstract))
	{
		RetargetAssetPtr = ULiveLinkRemapAsset::StaticClass();
		RetargetAsset = RetargetAssetPtr;
	}

	if (!CurrentRetargetAsset || RetargetAssetPtr != CurrentRetargetAsset->GetClass())
	{
		CurrentRetargetAsset = NewObject<ULiveLinkRemapAsset>(const_cast<UAnimInstance*>(InAnimInstance), RetargetAssetPtr);
		CurrentRetargetAsset->Initialize();
	}
}

void FSmartsuitPoseNode::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	if (CurrentRetargetAsset->IsValidLowLevel())
	{
		//BoneMap.hip =					Bone_Map_Override_OLD->hip;
		//BoneMap.stomach =				Bone_Map_Override_OLD->stomach;
		//BoneMap.chest =					Bone_Map_Override_OLD->chest;
		//BoneMap.neck =					Bone_Map_Override_OLD->neck;
		//BoneMap.head =					Bone_Map_Override_OLD->head;
		//BoneMap.leftShoulder =			Bone_Map_Override_OLD->leftShoulder;
		//BoneMap.leftArm =				Bone_Map_Override_OLD->leftArm;
		//BoneMap.leftForearm =			Bone_Map_Override_OLD->leftForearm;
		//BoneMap.leftHand =				Bone_Map_Override_OLD->leftHand;
		//BoneMap.rightShoulder =			Bone_Map_Override_OLD->rightShoulder;
		//BoneMap.rightArm =				Bone_Map_Override_OLD->rightArm;
		//BoneMap.rightForearm =			Bone_Map_Override_OLD->rightForearm;
		//BoneMap.rightHand =				Bone_Map_Override_OLD->rightHand;
		//BoneMap.leftUpleg =				Bone_Map_Override_OLD->leftUpleg;
		//BoneMap.leftLeg =				Bone_Map_Override_OLD->leftLeg;
		//BoneMap.leftFoot =				Bone_Map_Override_OLD->leftFoot;
		//BoneMap.leftToe =				Bone_Map_Override_OLD->leftToe;
		//BoneMap.rightUpleg =			Bone_Map_Override_OLD->rightUpleg;
		//BoneMap.rightLeg =				Bone_Map_Override_OLD->rightLeg;
		//BoneMap.rightFoot =				Bone_Map_Override_OLD->rightFoot;
		//BoneMap.rightToe =				Bone_Map_Override_OLD->rightToe;

		//BoneMap.leftThumbProximal =		Bone_Map_Override_OLD->leftThumbProximal;
		//BoneMap.leftThumbMedial =		Bone_Map_Override_OLD->leftThumbMedial;
		//BoneMap.leftThumbDistal =		Bone_Map_Override_OLD->leftThumbDistal;
		//BoneMap.leftThumbTip =			Bone_Map_Override_OLD->leftThumbTip;
		//BoneMap.leftIndexProximal =		Bone_Map_Override_OLD->leftIndexProximal;
		//BoneMap.leftIndexMedial =		Bone_Map_Override_OLD->leftIndexMedial;
		//BoneMap.leftIndexDistal =		Bone_Map_Override_OLD->leftIndexDistal;
		//BoneMap.leftIndexTip =			Bone_Map_Override_OLD->leftIndexTip;
		//BoneMap.leftMiddleProximal =	Bone_Map_Override_OLD->leftMiddleProximal;
		//BoneMap.leftMiddleMedial =		Bone_Map_Override_OLD->leftMiddleMedial;
		//BoneMap.leftMiddleDistal =		Bone_Map_Override_OLD->leftMiddleDistal;
		//BoneMap.leftMiddleTip =			Bone_Map_Override_OLD->leftMiddleTip;
		//BoneMap.leftRingProximal =		Bone_Map_Override_OLD->leftRingProximal;
		//BoneMap.leftRingMedial =		Bone_Map_Override_OLD->leftRingMedial;
		//BoneMap.leftRingDistal =		Bone_Map_Override_OLD->leftRingDistal;
		//BoneMap.leftRingTip =			Bone_Map_Override_OLD->leftRingTip;
		//BoneMap.leftLittleProximal =	Bone_Map_Override_OLD->leftLittleProximal;
		//BoneMap.leftLittleMedial =		Bone_Map_Override_OLD->leftLittleMedial;
		//BoneMap.leftLittleDistal =		Bone_Map_Override_OLD->leftLittleDistal;
		//BoneMap.leftLittleTip =			Bone_Map_Override_OLD->leftLittleTip;
		//BoneMap.rightThumbProximal =	Bone_Map_Override_OLD->rightThumbProximal;
		//BoneMap.rightThumbMedial =		Bone_Map_Override_OLD->rightThumbMedial;
		//BoneMap.rightThumbDistal =		Bone_Map_Override_OLD->rightThumbDistal;
		//BoneMap.rightThumbTip =			Bone_Map_Override_OLD->rightThumbTip;
		//BoneMap.rightIndexProximal =	Bone_Map_Override_OLD->rightIndexProximal;
		//BoneMap.rightIndexMedial =		Bone_Map_Override_OLD->rightIndexMedial;
		//BoneMap.rightIndexDistal =		Bone_Map_Override_OLD->rightIndexDistal;
		//BoneMap.rightIndexTip =			Bone_Map_Override_OLD->rightIndexTip;
		//BoneMap.rightMiddleProximal =	Bone_Map_Override_OLD->rightMiddleProximal;
		//BoneMap.rightMiddleMedial =		Bone_Map_Override_OLD->rightMiddleMedial;
		//BoneMap.rightMiddleDistal =		Bone_Map_Override_OLD->rightMiddleDistal;
		//BoneMap.rightMiddleTip =		Bone_Map_Override_OLD->rightMiddleTip;
		//BoneMap.rightRingProximal =		Bone_Map_Override_OLD->rightRingProximal;
		//BoneMap.rightRingMedial =		Bone_Map_Override_OLD->rightRingMedial;
		//BoneMap.rightRingDistal =		Bone_Map_Override_OLD->rightRingDistal;
		//BoneMap.rightRingTip =			Bone_Map_Override_OLD->rightRingTip;
		//BoneMap.rightLittleProximal =	Bone_Map_Override_OLD->rightLittleProximal;
		//BoneMap.rightLittleMedial =		Bone_Map_Override_OLD->rightLittleMedial;
		//BoneMap.rightLittleDistal =		Bone_Map_Override_OLD->rightLittleDistal;
		//BoneMap.rightLittleTip =		Bone_Map_Override_OLD->rightLittleTip;

		BoneMap.hip = CurrentRetargetAsset->GetRemappedBoneName("hip");
		BoneMap.stomach = CurrentRetargetAsset->GetRemappedBoneName("stomach");
		BoneMap.chest = CurrentRetargetAsset->GetRemappedBoneName("chest");
		BoneMap.neck = CurrentRetargetAsset->GetRemappedBoneName("neck");
		BoneMap.head = CurrentRetargetAsset->GetRemappedBoneName("head");
		BoneMap.leftShoulder = CurrentRetargetAsset->GetRemappedBoneName("leftShoulder");
		BoneMap.leftArm = CurrentRetargetAsset->GetRemappedBoneName("leftArm");
		BoneMap.leftForearm = CurrentRetargetAsset->GetRemappedBoneName("leftForearm");
		BoneMap.leftHand = CurrentRetargetAsset->GetRemappedBoneName("leftHand");
		BoneMap.rightShoulder = CurrentRetargetAsset->GetRemappedBoneName("rightShoulder");
		BoneMap.rightArm = CurrentRetargetAsset->GetRemappedBoneName("rightArm");
		BoneMap.rightForearm = CurrentRetargetAsset->GetRemappedBoneName("rightForearm");
		BoneMap.rightHand = CurrentRetargetAsset->GetRemappedBoneName("rightHand");
		BoneMap.leftUpleg = CurrentRetargetAsset->GetRemappedBoneName("leftUpleg");
		BoneMap.leftLeg = CurrentRetargetAsset->GetRemappedBoneName("leftLeg");
		BoneMap.leftFoot = CurrentRetargetAsset->GetRemappedBoneName("leftFoot");
		BoneMap.leftToe = CurrentRetargetAsset->GetRemappedBoneName("leftToe");
		BoneMap.rightUpleg = CurrentRetargetAsset->GetRemappedBoneName("rightUpleg");
		BoneMap.rightLeg = CurrentRetargetAsset->GetRemappedBoneName("rightLeg");
		BoneMap.rightFoot = CurrentRetargetAsset->GetRemappedBoneName("rightFoot");
		BoneMap.rightToe = CurrentRetargetAsset->GetRemappedBoneName("rightToe");
		BoneMap.leftThumbProximal = CurrentRetargetAsset->GetRemappedBoneName("leftThumbProximal");
		BoneMap.leftThumbMedial = CurrentRetargetAsset->GetRemappedBoneName("leftThumbMedial");
		BoneMap.leftThumbDistal = CurrentRetargetAsset->GetRemappedBoneName("leftThumbDistal");
		BoneMap.leftThumbTip = CurrentRetargetAsset->GetRemappedBoneName("leftThumbTip");
		BoneMap.leftIndexProximal = CurrentRetargetAsset->GetRemappedBoneName("leftIndexProximal");
		BoneMap.leftIndexMedial = CurrentRetargetAsset->GetRemappedBoneName("leftIndexMedial");
		BoneMap.leftIndexDistal = CurrentRetargetAsset->GetRemappedBoneName("leftIndexDistal");
		BoneMap.leftIndexTip = CurrentRetargetAsset->GetRemappedBoneName("leftIndexTip");
		BoneMap.leftMiddleProximal = CurrentRetargetAsset->GetRemappedBoneName("leftMiddleProximal");
		BoneMap.leftMiddleMedial = CurrentRetargetAsset->GetRemappedBoneName("leftMiddleMedial");
		BoneMap.leftMiddleDistal = CurrentRetargetAsset->GetRemappedBoneName("leftMiddleDistal");
		BoneMap.leftMiddleTip = CurrentRetargetAsset->GetRemappedBoneName("leftMiddleTip");
		BoneMap.leftRingProximal = CurrentRetargetAsset->GetRemappedBoneName("leftRingProximal");
		BoneMap.leftRingMedial = CurrentRetargetAsset->GetRemappedBoneName("leftRingMedial");
		BoneMap.leftRingDistal = CurrentRetargetAsset->GetRemappedBoneName("leftRingDistal");
		BoneMap.leftRingTip = CurrentRetargetAsset->GetRemappedBoneName("leftRingTip");
		BoneMap.leftLittleProximal = CurrentRetargetAsset->GetRemappedBoneName("leftLittleProximal");
		BoneMap.leftLittleMedial = CurrentRetargetAsset->GetRemappedBoneName("leftLittleMedial");
		BoneMap.leftLittleDistal = CurrentRetargetAsset->GetRemappedBoneName("leftLittleDistal");
		BoneMap.leftLittleTip = CurrentRetargetAsset->GetRemappedBoneName("leftLittleTip");
		BoneMap.rightThumbProximal = CurrentRetargetAsset->GetRemappedBoneName("rightThumbProximal");
		BoneMap.rightThumbMedial = CurrentRetargetAsset->GetRemappedBoneName("rightThumbMedial");
		BoneMap.rightThumbDistal = CurrentRetargetAsset->GetRemappedBoneName("rightThumbDistal");
		BoneMap.rightThumbTip = CurrentRetargetAsset->GetRemappedBoneName("rightThumbTip");
		BoneMap.rightIndexProximal = CurrentRetargetAsset->GetRemappedBoneName("rightIndexProximal");
		BoneMap.rightIndexMedial = CurrentRetargetAsset->GetRemappedBoneName("rightIndexMedial");
		BoneMap.rightIndexDistal = CurrentRetargetAsset->GetRemappedBoneName("rightIndexDistal");
		BoneMap.rightIndexTip = CurrentRetargetAsset->GetRemappedBoneName("rightIndexTip");
		BoneMap.rightMiddleProximal = CurrentRetargetAsset->GetRemappedBoneName("rightMiddleProximal");
		BoneMap.rightMiddleMedial = CurrentRetargetAsset->GetRemappedBoneName("rightMiddleMedial");
		BoneMap.rightMiddleDistal = CurrentRetargetAsset->GetRemappedBoneName("rightMiddleDistal");
		BoneMap.rightMiddleTip = CurrentRetargetAsset->GetRemappedBoneName("rightMiddleTip");
		BoneMap.rightRingProximal = CurrentRetargetAsset->GetRemappedBoneName("rightRingProximal");
		BoneMap.rightRingMedial = CurrentRetargetAsset->GetRemappedBoneName("rightRingMedial");
		BoneMap.rightRingDistal = CurrentRetargetAsset->GetRemappedBoneName("rightRingDistal");
		BoneMap.rightRingTip = CurrentRetargetAsset->GetRemappedBoneName("rightRingTip");
		BoneMap.rightLittleProximal = CurrentRetargetAsset->GetRemappedBoneName("rightLittleProximal");
		BoneMap.rightLittleMedial = CurrentRetargetAsset->GetRemappedBoneName("rightLittleMedial");
		BoneMap.rightLittleDistal = CurrentRetargetAsset->GetRemappedBoneName("rightLittleDistal");
		BoneMap.rightLittleTip = CurrentRetargetAsset->GetRemappedBoneName("rightLittleTip");
	}

	BoneMap.hip.Initialize(RequiredBones);
	BoneMap.stomach.Initialize(RequiredBones);
	BoneMap.chest.Initialize(RequiredBones);
	BoneMap.neck.Initialize(RequiredBones);
	BoneMap.head.Initialize(RequiredBones);
	BoneMap.leftShoulder.Initialize(RequiredBones);
	BoneMap.leftArm.Initialize(RequiredBones);
	BoneMap.leftForearm.Initialize(RequiredBones);
	BoneMap.leftHand.Initialize(RequiredBones);
	BoneMap.rightShoulder.Initialize(RequiredBones);
	BoneMap.rightArm.Initialize(RequiredBones);
	BoneMap.rightForearm.Initialize(RequiredBones);
	BoneMap.rightHand.Initialize(RequiredBones);
	BoneMap.leftUpleg.Initialize(RequiredBones);
	BoneMap.leftLeg.Initialize(RequiredBones);
	BoneMap.leftFoot.Initialize(RequiredBones);
	BoneMap.leftToe.Initialize(RequiredBones);
	BoneMap.rightUpleg.Initialize(RequiredBones);
	BoneMap.rightLeg.Initialize(RequiredBones);
	BoneMap.rightFoot.Initialize(RequiredBones);
	BoneMap.rightToe.Initialize(RequiredBones);

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

FLiveLinkSubjectName FSmartsuitPoseNode::GetLiveLinkSubjectName()
{
	FString TempSubjectName = "actor:" + RokokoActorName.ToString() + ":body";
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
	return FName(TempSubjectName);
#else
	return FName(*TempSubjectName);
#endif
}


void FSmartsuitPoseNode::OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance)
{
	//CurrentRetargetAsset = nullptr;

	Super::OnInitializeAnimInstance(InProxy, InAnimInstance);
}

void FSmartsuitPoseNode::UpdateComponentPose_AnyThread(const FAnimationUpdateContext& Context)
{
	// Evaluate any BP logic plugged into this node
	GetEvaluateGraphExposedInputs().Execute(Context);

	Super::UpdateComponentPose_AnyThread(Context);
}

void FSmartsuitPoseNode::UpdateInternal(const FAnimationUpdateContext & Context)
{
	GetEvaluateGraphExposedInputs().Execute(Context);

	Super::UpdateInternal(Context);
}

void FSmartsuitPoseNode::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	GetEvaluateGraphExposedInputs().Execute(Context);
	
	Super::Initialize_AnyThread(Context);
}

// bool FSmartsuitPoseNode::Serialize(FArchive& Ar)
// {
// 	Ar.UsingCustomVersion(FLiveLinkCustomVersion::GUID);
// 	
// 	UScriptStruct* Struct = FSmartsuitPoseNode::StaticStruct();
// 	
// 	if (Ar.IsLoading() || Ar.IsSaving())
// 	{
// 		Struct->SerializeTaggedProperties(Ar, (uint8*)this, Struct, nullptr);
// 	}
//
// #if WITH_EDITORONLY_DATA
// 	//Take old data and put it in new data structure
// 	if (Ar.IsLoading())
// 	{
// 		if (Ar.CustomVer(FLiveLinkCustomVersion::GUID) < FLiveLinkCustomVersion::NewLiveLinkRoleSystem)
// 		{
// 			PRAGMA_DISABLE_DEPRECATION_WARNINGS
// 			//LiveLinkSubjectName.Name = SubjectName_DEPRECATED;
// 			PRAGMA_ENABLE_DEPRECATION_WARNINGS
// 		}
// 	}
// #endif
//
// 	return true;
// }
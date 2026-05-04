// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitPoseNode.h"
#include "AnimationRuntime.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "Components/SkeletalMeshComponent.h"
#include "LiveLinkCustomVersion.h"
#include "LiveLinkClient.h"
#include "RokokoSkeletonData.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Runtime/Core/Public/Features/IModularFeatures.h"
#include "SmartsuitBlueprintLibrary.h"

/**Holds information a Smartsuit character pose.*/
struct TPose {
	FTransform hip;
	FTransform stomach;
	FTransform chest;
	FTransform neck;
	FTransform head;
	FTransform leftShoulder;
	FTransform leftArm;
	FTransform leftForearm;
	FTransform leftHand;
	FTransform rightShoulder;
	FTransform rightArm;
	FTransform rightForearm;
	FTransform rightHand;
	FTransform leftUpLeg;
	FTransform leftLeg;
	FTransform leftFoot;
	FTransform leftToe;
	FTransform rightUpLeg;
	FTransform rightLeg;
	FTransform rightFoot;
	FTransform rightToe;
	FTransform leftThumbProximal;
	FTransform leftThumbMedial;
	FTransform leftThumbDistal;
	FTransform leftThumbTip;
	FTransform leftIndexProximal;
	FTransform leftIndexMedial;
	FTransform leftIndexDistal;
	FTransform leftIndexTip;
	FTransform leftMiddleProximal;
	FTransform leftMiddleMedial;
	FTransform leftMiddleDistal;
	FTransform leftMiddleTip;
	FTransform leftRingProximal;
	FTransform leftRingMedial;
	FTransform leftRingDistal;
	FTransform leftRingTip;
	FTransform leftLittleProximal;
	FTransform leftLittleMedial;
	FTransform leftLittleDistal;
	FTransform leftLittleTip;
	FTransform rightThumbProximal;
	FTransform rightThumbMedial;
	FTransform rightThumbDistal;
	FTransform rightThumbTip;
	FTransform rightIndexProximal;
	FTransform rightIndexMedial;
	FTransform rightIndexDistal;
	FTransform rightIndexTip;
	FTransform rightMiddleProximal;
	FTransform rightMiddleMedial;
	FTransform rightMiddleDistal;
	FTransform rightMiddleTip;
	FTransform rightRingProximal;
	FTransform rightRingMedial;
	FTransform rightRingDistal;
	FTransform rightRingTip;
	FTransform rightLittleProximal;
	FTransform rightLittleMedial;
	FTransform rightLittleDistal;
	FTransform rightLittleTip;
};

/**
 * Helper function that stores the T Pose for a Smartsuit character.
 */
class SMARTSUIT_API SmartsuitTPose
{
public:
	
	/**The T-pose transforms.*/
	TPose Pose;
	/**The start position of the character, used to offset the character if it is set.*/
	FVector startPos;
	/**Indicates if the pose has been stored.*/
	bool StoredTPose;
};


/////////////////////////////////////////////////////
// SmartsuitPoseNode

FSmartsuitPoseNode::FSmartsuitPoseNode()
	: SSPTPose(new SmartsuitTPose())
{
	SSPTPose->StoredTPose = false;
}

void FSmartsuitPoseNode::GatherDebugData(FNodeDebugData& DebugData)
{
	FString DebugLine = DebugData.GetNodeName(this);
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
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
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
	
	const FQuat BoneQuat(quat);
	NewBoneTM.SetRotation(BoneQuat);
	NewBoneTM.SetTranslation(position);
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp->GetComponentTransform(), MeshBases, NewBoneTM, CompactPoseBoneToModify, space);
	MeshBases.SetComponentSpaceTransform(CompactPoseBoneToModify, NewBoneTM);
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
	return NewBoneTM;
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
	return result;
}

void FSmartsuitPoseNode::ApplyAllBonePositions(FBoneReference bone, float hipWidth, TArray<FTransform> transforms, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases) 
{
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
	float currBoneDistance = DistanceBetweenTwoBones(bone1, bone2, space, SkelComp, MeshBases);
	FVector currScale = OriginalTransform(scaleBone, space, SkelComp, MeshBases).GetTranslation();
	float boneScale = desiredDistance / currBoneDistance;
	
	ApplySmartsuitScale(scaleBone, FVector(boneScale, boneScale, boneScale), space, SkelComp, MeshBases);
	return boneScale;
}

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

FQuat GetCachedRotation(const FName& BoneName, const TMap<FName, FTransform>& CachedTransforms)
{
	if (const FTransform* CachedTransform = CachedTransforms.Find(BoneName))
	{
		return CachedTransform->GetRotation();
	}

	return FQuat::Identity;
}

FVector GetCachedPosition(const FName& BoneName, const TMap<FName, FTransform>& CachedTransforms)
{
	if (const FTransform* CachedTransform = CachedTransforms.Find(BoneName))
	{
		return CachedTransform->GetLocation();
	}

	return FVector::ZeroVector;
}

//PRAGMA_DISABLE_OPTIMIZATION
void FSmartsuitPoseNode::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	USkeletalMeshComponent* SkelComp = Output.AnimInstanceProxy->GetSkelMeshComponent();
	FCSPose<FCompactPose>& MeshBases = Output.Pose;

	check(OutBoneTransforms.Num() == 0);
	
	
	if (!LiveLinkClient_AnyThread /*|| !CurrentRetargetAsset*/)
	{
		return;
	}

	FLiveLinkSubjectFrameData SubjectFrameData;

	bool bHasValidLiveFrame = false;
	bool bTriedBoundSubjectEvaluation = false;
	if (bHasBoundSubjectKey_AnyThread)
	{
		bTriedBoundSubjectEvaluation = true;
		TSubclassOf<ULiveLinkRole> SubjectRole = LiveLinkClient_AnyThread->GetSubjectRole_AnyThread(BoundSubjectKey_AnyThread);
		if (SubjectRole && SubjectRole->IsChildOf(ULiveLinkAnimationRole::StaticClass()))
		{
			bHasValidLiveFrame = LiveLinkClient_AnyThread->EvaluateFrameFromSource_AnyThread(BoundSubjectKey_AnyThread, ULiveLinkAnimationRole::StaticClass(), SubjectFrameData);
			if (!bHasValidLiveFrame)
			{
				++ConsecutiveKeyedFrameMisses_AnyThread;
				if (ConsecutiveKeyedFrameMisses_AnyThread == 1 || (ConsecutiveKeyedFrameMisses_AnyThread % 60) == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("[Rokoko][PoseNode] Keyed frame miss Actor=%s Source=%s Subject=%s Misses=%d"), *RokokoActorName.ToString(), *BoundSubjectKey_AnyThread.Source.ToString(), *BoundSubjectKey_AnyThread.SubjectName.ToString(), ConsecutiveKeyedFrameMisses_AnyThread);
				}
			}
			else
			{
				ConsecutiveKeyedFrameMisses_AnyThread = 0;
			}
		}
	}

	if (!bHasValidLiveFrame)
	{
		FLiveLinkSubjectName LiveLinkSubjectName = GetLiveLinkSubjectName();
		TSubclassOf<ULiveLinkRole> SubjectRole = LiveLinkClient_AnyThread->GetSubjectRole_AnyThread(LiveLinkSubjectName);
		if (SubjectRole)
		{
//#ifdef USE_SMARTSUIT_ANIMATION_ROLE
//		if (SubjectRole->IsChildOf(ULiveLinkSmartsuitRole::StaticClass()))
//		{
//			//Process animation data if the subject is from that type
//			if (LiveLinkClient_AnyThread->EvaluateFrame_AnyThread(LiveLinkSubjectName, ULiveLinkSmartsuitRole::StaticClass(), SubjectFrameData))
//			{
//
//			}
//			else
//			{
//				return;
//			}
//
//
//		}
//		else
//		{
//			return;
//		}
//#else
			if (SubjectRole->IsChildOf(ULiveLinkAnimationRole::StaticClass()))
			{
				bHasValidLiveFrame = LiveLinkClient_AnyThread->EvaluateFrame_AnyThread(LiveLinkSubjectName, ULiveLinkAnimationRole::StaticClass(), SubjectFrameData);
			}
//#endif
		}
	}

	if (bHasValidLiveFrame)
	{
		if (ConsecutiveLiveFrameMisses_AnyThread > 0)
		{
			UE_LOG(LogTemp, Display, TEXT("[Rokoko][PoseNode] Live frame recovered Actor=%s MissesBeforeRecovery=%d"), *RokokoActorName.ToString(), ConsecutiveLiveFrameMisses_AnyThread);
			ConsecutiveLiveFrameMisses_AnyThread = 0;
		}

		FLiveLinkSkeletonStaticData* SkeletonData = SubjectFrameData.StaticData.Cast<FLiveLinkSkeletonStaticData>();
		FLiveLinkAnimationFrameData* FrameData = SubjectFrameData.FrameData.Cast<FLiveLinkAnimationFrameData>();

		if (SkeletonData != nullptr && FrameData != nullptr)
		{
			CachedSmartsuitTransforms.Reset();
			const int32 NumTransforms = FMath::Min(SkeletonData->BoneNames.Num(), FrameData->Transforms.Num());
			for (int32 Index = 0; Index < NumTransforms; ++Index)
			{
				CachedSmartsuitTransforms.Add(SkeletonData->BoneNames[Index], FrameData->Transforms[Index]);
			}
			bHasCachedSmartsuitFrame = CachedSmartsuitTransforms.Num() > 0;
		}
	}
	else if (!bHasCachedSmartsuitFrame)
	{
		++ConsecutiveLiveFrameMisses_AnyThread;
		if (ConsecutiveLiveFrameMisses_AnyThread == 1 || (ConsecutiveLiveFrameMisses_AnyThread % 60) == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Rokoko][PoseNode] No live frame and no cache Actor=%s Subject=%s Bound=%s TriedBound=%d Misses=%d"), *RokokoActorName.ToString(), *GetLiveLinkSubjectName().ToString(), bHasBoundSubjectKey_AnyThread ? TEXT("true") : TEXT("false"), bTriedBoundSubjectEvaluation ? 1 : 0, ConsecutiveLiveFrameMisses_AnyThread);
		}
		return;
	}
	else
	{
		++ConsecutiveLiveFrameMisses_AnyThread;
		if (ConsecutiveLiveFrameMisses_AnyThread == 1 || (ConsecutiveLiveFrameMisses_AnyThread % 60) == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("[Rokoko][PoseNode] Using cached transforms Actor=%s Subject=%s Misses=%d"), *RokokoActorName.ToString(), *GetLiveLinkSubjectName().ToString(), ConsecutiveLiveFrameMisses_AnyThread);
		}
	}

//#ifdef USE_SMARTSUIT_ANIMATION_ROLE
//	FLiveLinkSmartsuitStaticData* SkeletonData = SubjectFrameData.StaticData.Cast<FLiveLinkSmartsuitStaticData>();
//	FLiveLinkSmartsuitFrameData* FrameData = SubjectFrameData.FrameData.Cast<FLiveLinkSmartsuitFrameData>();
//#else
	FLiveLinkSkeletonStaticData* SkeletonData = bHasValidLiveFrame ? SubjectFrameData.StaticData.Cast<FLiveLinkSkeletonStaticData>() : nullptr;
	FLiveLinkAnimationFrameData* FrameData = bHasValidLiveFrame ? SubjectFrameData.FrameData.Cast<FLiveLinkAnimationFrameData>() : nullptr;
//#endif

	if (bHasValidLiveFrame)
	{
		check(SkeletonData);
		check(FrameData);
	}


	EBoneControlSpace TestBoneControlSpace = BCS_ComponentSpace;

	if (!SSPTPose->StoredTPose /*&& CurrentRetargetAsset*/)
	{
		TPose& pose = SSPTPose->Pose;
		pose.hip =					OriginalTransform(BoneMap.hip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.stomach =				OriginalTransform(BoneMap.stomach, TestBoneControlSpace, SkelComp, MeshBases);
		pose.chest =					OriginalTransform(BoneMap.chest, TestBoneControlSpace, SkelComp, MeshBases);
		pose.neck =					OriginalTransform(BoneMap.neck, TestBoneControlSpace, SkelComp, MeshBases);
		pose.head =					OriginalTransform(BoneMap.head, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftShoulder =			OriginalTransform(BoneMap.leftShoulder, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftArm =				OriginalTransform(BoneMap.leftArm, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftForearm =			OriginalTransform(BoneMap.leftForearm, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftHand =				OriginalTransform(BoneMap.leftHand, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightShoulder =			OriginalTransform(BoneMap.rightShoulder, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightArm =				OriginalTransform(BoneMap.rightArm, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightForearm =			OriginalTransform(BoneMap.rightForearm, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightHand =				OriginalTransform(BoneMap.rightHand, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftUpLeg =				OriginalTransform(BoneMap.leftUpleg, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftLeg =				OriginalTransform(BoneMap.leftLeg, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftFoot =				OriginalTransform(BoneMap.leftFoot, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftToe =				OriginalTransform(BoneMap.leftToe, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightUpLeg =				OriginalTransform(BoneMap.rightUpleg, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightLeg =				OriginalTransform(BoneMap.rightLeg, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightFoot =				OriginalTransform(BoneMap.rightFoot, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightToe =				OriginalTransform(BoneMap.rightToe, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftThumbProximal =		OriginalTransform(BoneMap.leftThumbProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftThumbMedial =		OriginalTransform(BoneMap.leftThumbMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftThumbDistal =		OriginalTransform(BoneMap.leftThumbDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftThumbTip =			OriginalTransform(BoneMap.leftThumbTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftIndexProximal =		OriginalTransform(BoneMap.leftIndexProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftIndexMedial =		OriginalTransform(BoneMap.leftIndexMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftIndexDistal =		OriginalTransform(BoneMap.leftIndexDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftIndexTip =			OriginalTransform(BoneMap.leftIndexTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftMiddleProximal =		OriginalTransform(BoneMap.leftMiddleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftMiddleMedial =		OriginalTransform(BoneMap.leftMiddleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftMiddleDistal =		OriginalTransform(BoneMap.leftMiddleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftMiddleTip =			OriginalTransform(BoneMap.leftMiddleTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftRingProximal =		OriginalTransform(BoneMap.leftRingProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftRingMedial =			OriginalTransform(BoneMap.leftRingMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftRingDistal =			OriginalTransform(BoneMap.leftRingDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftRingTip =			OriginalTransform(BoneMap.leftRingTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftLittleProximal =		OriginalTransform(BoneMap.leftLittleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftLittleMedial =		OriginalTransform(BoneMap.leftLittleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftLittleDistal =		OriginalTransform(BoneMap.leftLittleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.leftLittleTip =			OriginalTransform(BoneMap.leftLittleTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightThumbProximal =		OriginalTransform(BoneMap.rightThumbProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightThumbMedial =		OriginalTransform(BoneMap.rightThumbMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightThumbDistal =		OriginalTransform(BoneMap.rightThumbDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightThumbTip =			OriginalTransform(BoneMap.rightThumbTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightIndexProximal =		OriginalTransform(BoneMap.rightIndexProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightIndexMedial =		OriginalTransform(BoneMap.rightIndexMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightIndexDistal =		OriginalTransform(BoneMap.rightIndexDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightIndexTip =			OriginalTransform(BoneMap.rightIndexTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightMiddleProximal =	OriginalTransform(BoneMap.rightMiddleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightMiddleMedial =		OriginalTransform(BoneMap.rightMiddleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightMiddleDistal =		OriginalTransform(BoneMap.rightMiddleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightMiddleTip =			OriginalTransform(BoneMap.rightMiddleTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightRingProximal =		OriginalTransform(BoneMap.rightRingProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightRingMedial =		OriginalTransform(BoneMap.rightRingMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightRingDistal =		OriginalTransform(BoneMap.rightRingDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightRingTip =			OriginalTransform(BoneMap.rightRingTip, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightLittleProximal =	OriginalTransform(BoneMap.rightLittleProximal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightLittleMedial =		OriginalTransform(BoneMap.rightLittleMedial, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightLittleDistal =		OriginalTransform(BoneMap.rightLittleDistal, TestBoneControlSpace, SkelComp, MeshBases);
		pose.rightLittleTip =			OriginalTransform(BoneMap.rightLittleTip, TestBoneControlSpace, SkelComp, MeshBases);
		SSPTPose->StoredTPose = true;
		return;
	}
	if (!SSPTPose->StoredTPose)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No tpose"));
		return;
	}
	FQuat modifier = FQuat::MakeFromEuler(FVector(0, 0, 180));
	FQuat forwardModifier = FQuat::MakeFromEuler(FVector(0, 0, 90));

	// These duplicate variables are a bit redundant in their current state, however lets keep these around in case we need to negate an axis for example
	FQuat LeftShoulderSpace = FQuat::MakeFromEuler(FVector(ShoulderSpace, 0.0f, 0.0f));
	FQuat RightShoulderSpace = FQuat::MakeFromEuler(FVector(ShoulderSpace, 0.0f, 0.0f));
	FQuat LeftArmSpace = FQuat::MakeFromEuler(FVector(ArmSpace, 0.0f, 0.0f));
	FQuat RightArmSpace = FQuat::MakeFromEuler(FVector(ArmSpace, 0.0f, 0.0f));

	FQuat hipQuat =						bHasValidLiveFrame ? GetRotation3(SmartsuitBones::hip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::hip, CachedSmartsuitTransforms);// *modifier;
	FVector hipPosition =				bHasValidLiveFrame ? GetPosition3(SmartsuitBones::hip, SubjectFrameData) : GetCachedPosition(SmartsuitBones::hip, CachedSmartsuitTransforms);
	FQuat stomachQuat =					bHasValidLiveFrame ? GetRotation3(SmartsuitBones::spine, SubjectFrameData) : GetCachedRotation(SmartsuitBones::spine, CachedSmartsuitTransforms);
	FQuat chestQuat =					bHasValidLiveFrame ? GetRotation3(SmartsuitBones::chest, SubjectFrameData) : GetCachedRotation(SmartsuitBones::chest, CachedSmartsuitTransforms);
	FQuat neckQuat =					bHasValidLiveFrame ? GetRotation3(SmartsuitBones::neck, SubjectFrameData) : GetCachedRotation(SmartsuitBones::neck, CachedSmartsuitTransforms);
	FQuat headQuat =					bHasValidLiveFrame ? GetRotation3(SmartsuitBones::head, SubjectFrameData) : GetCachedRotation(SmartsuitBones::head, CachedSmartsuitTransforms);
	FQuat leftShoulderQuat =			(bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftShoulder, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftShoulder, CachedSmartsuitTransforms)) * LeftShoulderSpace;
	FQuat leftArmQuat =					(bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftUpperArm, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftUpperArm, CachedSmartsuitTransforms)) * LeftArmSpace;
	FQuat leftForearmQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftLowerArm, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftLowerArm, CachedSmartsuitTransforms);
	FQuat leftHandQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftHand, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftHand, CachedSmartsuitTransforms);
	FQuat rightShoulderQuat =			(bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightShoulder, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightShoulder, CachedSmartsuitTransforms)) * RightShoulderSpace;
	FQuat rightArmQuat =				(bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightUpperArm, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightUpperArm, CachedSmartsuitTransforms)) * RightArmSpace;
	FQuat rightForearmQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightLowerArm, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightLowerArm, CachedSmartsuitTransforms);
	FQuat rightHandQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightHand, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightHand, CachedSmartsuitTransforms);
	FQuat leftUpLegQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftUpLeg, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftUpLeg, CachedSmartsuitTransforms);
	FQuat leftLegQuat =					bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftLeg, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftLeg, CachedSmartsuitTransforms);
	FQuat leftFootQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftFoot, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftFoot, CachedSmartsuitTransforms);
	FQuat leftToeQuat =					bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftToe, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftToe, CachedSmartsuitTransforms);
	FQuat rightUpLegQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightUpLeg, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightUpLeg, CachedSmartsuitTransforms);
	FQuat rightLegQuat =					bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightLeg, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightLeg, CachedSmartsuitTransforms);
	FQuat rightFootQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightFoot, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightFoot, CachedSmartsuitTransforms);
	FQuat rightToeQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightToe, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightToe, CachedSmartsuitTransforms);

	FQuat leftThumbProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftThumbProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftThumbProximal, CachedSmartsuitTransforms);
	FQuat leftThumbMedialQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftThumbMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftThumbMedial, CachedSmartsuitTransforms);
	FQuat leftThumbDistalQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftThumbDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftThumbDistal, CachedSmartsuitTransforms);
	FQuat leftThumbTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftThumbTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftThumbTip, CachedSmartsuitTransforms);
	FQuat leftIndexProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftIndexProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftIndexProximal, CachedSmartsuitTransforms);
	FQuat leftIndexMedialQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftIndexMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftIndexMedial, CachedSmartsuitTransforms);
	FQuat leftIndexDistalQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftIndexDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftIndexDistal, CachedSmartsuitTransforms);
	FQuat leftIndexTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftIndexTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftIndexTip, CachedSmartsuitTransforms);
	FQuat leftMiddleProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftMiddleProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftMiddleProximal, CachedSmartsuitTransforms);
	FQuat leftMiddleMedialQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftMiddleMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftMiddleMedial, CachedSmartsuitTransforms);
	FQuat leftMiddleDistalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftMiddleDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftMiddleDistal, CachedSmartsuitTransforms);
	FQuat leftMiddleTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftMiddleTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftMiddleTip, CachedSmartsuitTransforms);
	FQuat leftRingProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftRingProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftRingProximal, CachedSmartsuitTransforms);
	FQuat leftRingMedialQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftRingMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftRingMedial, CachedSmartsuitTransforms);
	FQuat leftRingDistalQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftRingDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftRingDistal, CachedSmartsuitTransforms);
	FQuat leftRingTipQuat =				bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftRingTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftRingTip, CachedSmartsuitTransforms);
	FQuat leftLittleProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftLittleProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftLittleProximal, CachedSmartsuitTransforms);
	FQuat leftLittleMedialQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftLittleMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftLittleMedial, CachedSmartsuitTransforms);
	FQuat leftLittleDistalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftLittleDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftLittleDistal, CachedSmartsuitTransforms);
	FQuat leftLittleTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::leftLittleTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::leftLittleTip, CachedSmartsuitTransforms);
	FQuat rightThumbProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightThumbProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightThumbProximal, CachedSmartsuitTransforms);
	FQuat rightThumbMedialQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightThumbMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightThumbMedial, CachedSmartsuitTransforms);
	FQuat rightThumbDistalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightThumbDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightThumbDistal, CachedSmartsuitTransforms);
	FQuat rightThumbTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightThumbTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightThumbTip, CachedSmartsuitTransforms);
	FQuat rightIndexProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightIndexProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightIndexProximal, CachedSmartsuitTransforms);
	FQuat rightIndexMedialQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightIndexMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightIndexMedial, CachedSmartsuitTransforms);
	FQuat rightIndexDistalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightIndexDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightIndexDistal, CachedSmartsuitTransforms);
	FQuat rightIndexTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightIndexTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightIndexTip, CachedSmartsuitTransforms);
	FQuat rightMiddleProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightMiddleProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightMiddleProximal, CachedSmartsuitTransforms);
	FQuat rightMiddleMedialQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightMiddleMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightMiddleMedial, CachedSmartsuitTransforms);
	FQuat rightMiddleDistalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightMiddleDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightMiddleDistal, CachedSmartsuitTransforms);
	FQuat rightMiddleTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightMiddleTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightMiddleTip, CachedSmartsuitTransforms);
	FQuat rightRingProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightRingProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightRingProximal, CachedSmartsuitTransforms);
	FQuat rightRingMedialQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightRingMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightRingMedial, CachedSmartsuitTransforms);
	FQuat rightRingDistalQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightRingDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightRingDistal, CachedSmartsuitTransforms);
	FQuat rightRingTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightRingTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightRingTip, CachedSmartsuitTransforms);
	FQuat rightLittleProximalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightLittleProximal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightLittleProximal, CachedSmartsuitTransforms);
	FQuat rightLittleMedialQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightLittleMedial, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightLittleMedial, CachedSmartsuitTransforms);
	FQuat rightLittleDistalQuat =		bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightLittleDistal, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightLittleDistal, CachedSmartsuitTransforms);
	FQuat rightLittleTipQuat =			bHasValidLiveFrame ? GetRotation3(SmartsuitBones::rightLittleTip, SubjectFrameData) : GetCachedRotation(SmartsuitBones::rightLittleTip, CachedSmartsuitTransforms);



	TPose& pose = SSPTPose->Pose;
	FQuat hipExpected = SMARTSUIT_TPOSE_HIP.Inverse() * pose.hip.GetRotation();
	FQuat stomachExpected = SMARTSUIT_TPOSE_STOMACH.Inverse() * pose.stomach.GetRotation();
	FQuat chestExpected = SMARTSUIT_TPOSE_CHEST.Inverse() * pose.chest.GetRotation();
	FQuat neckExpected = SMARTSUIT_TPOSE_NECK.Inverse() * pose.neck.GetRotation();
	FQuat headExpected = SMARTSUIT_TPOSE_HEAD.Inverse() * pose.head.GetRotation();
	FQuat leftShoulderExpected = SMARTSUIT_TPOSE_LEFT_SHOULDER.Inverse() * pose.leftShoulder.GetRotation();
	FQuat leftArmExpected = SMARTSUIT_TPOSE_LEFT_ARM.Inverse() * pose.leftArm.GetRotation();
	FQuat leftForearmExpected = SMARTSUIT_TPOSE_LEFT_FOREARM.Inverse() * pose.leftForearm.GetRotation();
	FQuat leftHandExpected = SMARTSUIT_TPOSE_LEFT_HAND.Inverse() * pose.leftHand.GetRotation();
	FQuat rightShoulderExpected = SMARTSUIT_TPOSE_RIGHT_SHOULDER.Inverse() * pose.rightShoulder.GetRotation();
	FQuat rightArmExpected = SMARTSUIT_TPOSE_RIGHT_ARM.Inverse() * pose.rightArm.GetRotation();
	FQuat rightForearmExpected = SMARTSUIT_TPOSE_RIGHT_FOREARM.Inverse() * pose.rightForearm.GetRotation();
	FQuat rightHandExpected = SMARTSUIT_TPOSE_RIGHT_HAND.Inverse() * pose.rightHand.GetRotation();
	FQuat leftUpLegExpected = SMARTSUIT_TPOSE_LEFT_UP_LEG.Inverse() * pose.leftUpLeg.GetRotation();
	FQuat leftLegExpected = SMARTSUIT_TPOSE_LEFT_LEG.Inverse() * pose.leftLeg.GetRotation();
	FQuat leftFootExpected = SMARTSUIT_TPOSE_LEFT_FOOT.Inverse() * pose.leftFoot.GetRotation();
	FQuat leftToeExpected = SMARTSUIT_TPOSE_LEFT_TOE.Inverse() * pose.leftToe.GetRotation();

	FQuat rightUpLegExpected = SMARTSUIT_TPOSE_RIGHT_UP_LEG.Inverse() * pose.rightUpLeg.GetRotation();
	FQuat rightLegExpected = SMARTSUIT_TPOSE_RIGHT_LEG.Inverse() * pose.rightLeg.GetRotation();
	FQuat rightFootExpected = SMARTSUIT_TPOSE_RIGHT_FOOT.Inverse() * pose.rightFoot.GetRotation();
	FQuat rightToeExpected = SMARTSUIT_TPOSE_RIGHT_TOE.Inverse() * pose.rightToe.GetRotation();

	FQuat leftThumbProximalExpected = SMARTSUIT_TPOSE_LEFT_THUMB_PROXIMAL.Inverse() * pose.leftThumbProximal.GetRotation();
	FQuat leftThumbMedialExpected = SMARTSUIT_TPOSE_LEFT_THUMB_MEDIAL.Inverse() * pose.leftThumbMedial.GetRotation();
	FQuat leftThumbDistalExpected = SMARTSUIT_TPOSE_LEFT_THUMB_DISTAL.Inverse() * pose.leftThumbDistal.GetRotation();
	FQuat leftThumbTipExpected = SMARTSUIT_TPOSE_LEFT_THUMB_TIP.Inverse() * pose.leftThumbTip.GetRotation();

	FQuat leftIndexProximalExpected = SMARTSUIT_TPOSE_LEFT_INDEX_PROXIMAL.Inverse() * pose.leftIndexProximal.GetRotation();
	FQuat leftIndexMedialExpected = SMARTSUIT_TPOSE_LEFT_INDEX_MEDIAL.Inverse() * pose.leftIndexMedial.GetRotation();
	FQuat leftIndexDistalExpected = SMARTSUIT_TPOSE_LEFT_INDEX_DISTAL.Inverse() * pose.leftIndexDistal.GetRotation();
	FQuat leftIndexTipExpected = SMARTSUIT_TPOSE_LEFT_INDEX_TIP.Inverse() * pose.leftIndexTip.GetRotation();

	FQuat leftMiddleProximalExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_PROXIMAL.Inverse() * pose.leftMiddleProximal.GetRotation();
	FQuat leftMiddleMedialExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_MEDIAL.Inverse() * pose.leftMiddleMedial.GetRotation();
	FQuat leftMiddleDistalExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_DISTAL.Inverse() * pose.leftMiddleDistal.GetRotation();
	FQuat leftMiddleTipExpected = SMARTSUIT_TPOSE_LEFT_MIDDLE_TIP.Inverse() * pose.leftMiddleTip.GetRotation();

	FQuat leftRingProximalExpected = SMARTSUIT_TPOSE_LEFT_RING_PROXIMAL.Inverse() * pose.leftRingProximal.GetRotation();
	FQuat leftRingMedialExpected = SMARTSUIT_TPOSE_LEFT_RING_MEDIAL.Inverse() * pose.leftRingMedial.GetRotation();
	FQuat leftRingDistalExpected = SMARTSUIT_TPOSE_LEFT_RING_DISTAL.Inverse() * pose.leftRingDistal.GetRotation();
	FQuat leftRingTipExpected = SMARTSUIT_TPOSE_LEFT_RING_TIP.Inverse() * pose.leftRingTip.GetRotation();

	FQuat leftLittleProximalExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_PROXIMAL.Inverse() * pose.leftLittleProximal.GetRotation();
	FQuat leftLittleMedialExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_MEDIAL.Inverse() * pose.leftLittleMedial.GetRotation();
	FQuat leftLittleDistalExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_DISTAL.Inverse() * pose.leftLittleDistal.GetRotation();
	FQuat leftLittleTipExpected = SMARTSUIT_TPOSE_LEFT_LITTLE_TIP.Inverse() * pose.leftLittleTip.GetRotation();


	FQuat rightThumbProximalExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_PROXIMAL.Inverse() * pose.rightThumbProximal.GetRotation();
	FQuat rightThumbMedialExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_MEDIAL.Inverse() * pose.rightThumbMedial.GetRotation();
	FQuat rightThumbDistalExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_DISTAL.Inverse() * pose.rightThumbDistal.GetRotation();
	FQuat rightThumbTipExpected = SMARTSUIT_TPOSE_RIGHT_THUMB_TIP.Inverse() * pose.rightThumbTip.GetRotation();

	FQuat rightIndexProximalExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_PROXIMAL.Inverse() * pose.rightIndexProximal.GetRotation();
	FQuat rightIndexMedialExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_MEDIAL.Inverse() * pose.rightIndexMedial.GetRotation();
	FQuat rightIndexDistalExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_DISTAL.Inverse() * pose.rightIndexDistal.GetRotation();
	FQuat rightIndexTipExpected = SMARTSUIT_TPOSE_RIGHT_INDEX_TIP.Inverse() * pose.rightIndexTip.GetRotation();

	FQuat rightMiddleProximalExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_PROXIMAL.Inverse() * pose.rightMiddleProximal.GetRotation();
	FQuat rightMiddleMedialExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_MEDIAL.Inverse() * pose.rightMiddleMedial.GetRotation();
	FQuat rightMiddleDistalExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_DISTAL.Inverse() * pose.rightMiddleDistal.GetRotation();
	FQuat rightMiddleTipExpected = SMARTSUIT_TPOSE_RIGHT_MIDDLE_TIP.Inverse() * pose.rightMiddleTip.GetRotation();

	FQuat rightRingProximalExpected = SMARTSUIT_TPOSE_RIGHT_RING_PROXIMAL.Inverse() * pose.rightRingProximal.GetRotation();
	FQuat rightRingMedialExpected = SMARTSUIT_TPOSE_RIGHT_RING_MEDIAL.Inverse() * pose.rightRingMedial.GetRotation();
	FQuat rightRingDistalExpected = SMARTSUIT_TPOSE_RIGHT_RING_DISTAL.Inverse() * pose.rightRingDistal.GetRotation();
	FQuat rightRingTipExpected = SMARTSUIT_TPOSE_RIGHT_RING_TIP.Inverse() * pose.rightRingTip.GetRotation();

	FQuat rightLittleProximalExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_PROXIMAL.Inverse() * pose.rightLittleProximal.GetRotation();
	FQuat rightLittleMedialExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_MEDIAL.Inverse() * pose.rightLittleMedial.GetRotation();
	FQuat rightLittleDistalExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_DISTAL.Inverse() * pose.rightLittleDistal.GetRotation();
	FQuat rightLittleTipExpected = SMARTSUIT_TPOSE_RIGHT_LITTLE_TIP.Inverse() * pose.rightLittleTip.GetRotation();


	const FBoneContainer& BoneContainer = MeshBases.GetPose().GetBoneContainer();
	FCompactPoseBoneIndex CompactPoseBoneToModify = BoneMap.hip.GetCompactPoseIndex(BoneContainer);

	if (CompactPoseBoneToModify == -1)
	{
		return;
	}

	FTransform NewBoneTM = MeshBases.GetLocalSpaceTransform(CompactPoseBoneToModify);//.GetComponentSpaceTransform(CompactPoseBoneToModify);

	float testval1 = NewBoneTM.GetLocation().Size();
	float testval2 = SSPTPose->Pose.hip.GetLocation().Size();

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

		// Make sure to initialize root motion position, otherwise we will get erroneous results
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

	//#ifdef USE_SMARTSUIT_ANIMATION_ROLE
	//HasLeftGlove = FrameData->HasLeftGlove;
	//HasRightGlove = FrameData->HasRightGlove;
	//#endif

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
//PRAGMA_ENABLE_OPTIMIZATION

bool FSmartsuitPoseNode::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return true;
}

void FSmartsuitPoseNode::PreUpdate(const UAnimInstance* InAnimInstance)
{
	Super::PreUpdate(InAnimInstance);
	
	ILiveLinkClient* ThisFrameClient = nullptr;
	IModularFeatures& ModularFeatures = IModularFeatures::Get();
	if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ThisFrameClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
	}
	LiveLinkClient_AnyThread = ThisFrameClient;
	bHasBoundSubjectKey_AnyThread = false;

	if (ThisFrameClient)
	{
		const FLiveLinkSubjectName LiveLinkSubjectName = GetLiveLinkSubjectName();

		if (BoundSubjectKey_AnyThread.SubjectName == LiveLinkSubjectName && ThisFrameClient->IsSubjectValid(BoundSubjectKey_AnyThread))
		{
			bHasBoundSubjectKey_AnyThread = true;
		}
		else
		{
			FLiveLinkSubjectKey FallbackSubjectKey;
			bool bPickedBestKey = false;
			for (const FLiveLinkSubjectKey& SubjectKey : ThisFrameClient->GetSubjects(true, false))
			{
				if (SubjectKey.SubjectName != LiveLinkSubjectName)
				{
					continue;
				}

				if (!ThisFrameClient->IsSubjectValid(SubjectKey) || !ThisFrameClient->DoesSubjectSupportsRole_AnyThread(SubjectKey, ULiveLinkAnimationRole::StaticClass()))
				{
					continue;
				}

				if (!bHasBoundSubjectKey_AnyThread)
				{
					FallbackSubjectKey = SubjectKey;
					bHasBoundSubjectKey_AnyThread = true;
				}

				if (ThisFrameClient->IsSubjectEnabled(SubjectKey, false))
				{
					BoundSubjectKey_AnyThread = SubjectKey;
					bPickedBestKey = true;
					break;
				}
			}

			if (!bPickedBestKey && bHasBoundSubjectKey_AnyThread)
			{
				BoundSubjectKey_AnyThread = FallbackSubjectKey;
			}
		}
	}

	if (bHasBoundSubjectKey_AnyThread)
	{
		if (!bHasLastLoggedBoundSubjectKey_AnyThread || LastLoggedBoundSubjectKey_AnyThread != BoundSubjectKey_AnyThread)
		{
			UE_LOG(LogTemp, Display, TEXT("[Rokoko][PoseNode] Bound Actor=%s Source=%s Subject=%s"), *RokokoActorName.ToString(), *BoundSubjectKey_AnyThread.Source.ToString(), *BoundSubjectKey_AnyThread.SubjectName.ToString());
			LastLoggedBoundSubjectKey_AnyThread = BoundSubjectKey_AnyThread;
			bHasLastLoggedBoundSubjectKey_AnyThread = true;
		}
	}
	else if (bHasLastLoggedBoundSubjectKey_AnyThread)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Rokoko][PoseNode] Lost bound subject Actor=%s Subject=%s"), *RokokoActorName.ToString(), *GetLiveLinkSubjectName().ToString());
		bHasLastLoggedBoundSubjectKey_AnyThread = false;
	}

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
	if (CurrentRetargetAsset != nullptr && CurrentRetargetAsset->IsValidLowLevel())
	{
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
// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "AnimNode_RokokoFacePose.h"
#include "ILiveLinkClient.h"

#include "Features/IModularFeatures.h"

#include "Animation/AnimInstanceProxy.h"
#include "LiveLinkCustomVersion.h"
#include "LiveLinkRemapAsset.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"

#include "VirtualProductionSource.h"

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
#include "Animation/AnimTrace.h"
#endif

FAnimNode_RokokoFacePose::FAnimNode_RokokoFacePose()
	: RetargetAsset(ULiveLinkRemapAsset::StaticClass())
	, CurrentRetargetAsset(nullptr)
	, LiveLinkClient_AnyThread(nullptr)
	, CachedDeltaTime(0.0f)
{
}

void FAnimNode_RokokoFacePose::OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance)
{
	CurrentRetargetAsset = nullptr;

	Super::OnInitializeAnimInstance(InProxy, InAnimInstance);
}

void FAnimNode_RokokoFacePose::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	InputPose.Initialize(Context);
}

void FAnimNode_RokokoFacePose::PreUpdate(const UAnimInstance* InAnimInstance)
{
	LiveLinkClient_AnyThread = LiveLinkClient_GameThread.GetClient();

	// Protection as a class graph pin does not honor rules on abstract classes and NoClear
	UClass* RetargetAssetPtr = RetargetAsset.Get();
	if (!RetargetAssetPtr || RetargetAssetPtr->HasAnyClassFlags(CLASS_Abstract))
	{
		RetargetAssetPtr = ULiveLinkRemapAsset::StaticClass();
		RetargetAsset = RetargetAssetPtr;
	}

	if (!CurrentRetargetAsset || RetargetAssetPtr != CurrentRetargetAsset->GetClass())
	{
		CurrentRetargetAsset = NewObject<ULiveLinkRetargetAsset>(const_cast<UAnimInstance*>(InAnimInstance), RetargetAssetPtr);
		CurrentRetargetAsset->Initialize();
	}
}

void FAnimNode_RokokoFacePose::Update_AnyThread(const FAnimationUpdateContext & Context)
{
	InputPose.Update(Context);

	GetEvaluateGraphExposedInputs().Execute(Context);

	// Accumulate Delta time from update
	CachedDeltaTime += Context.GetDeltaTime();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
	TRACE_ANIM_NODE_VALUE(Context, TEXT("SubjectName"), GetLiveLinkSubjectName().Name);
#endif
}

void FAnimNode_RokokoFacePose::Evaluate_AnyThread(FPoseContext& Output)
{
	InputPose.Evaluate(Output);

	if (!LiveLinkClient_AnyThread || !CurrentRetargetAsset)
	{
		return;
	}

	FLiveLinkSubjectFrameData SubjectFrameData;

	FLiveLinkSubjectName LiveLinkSubjectName = GetLiveLinkSubjectName();

	TSubclassOf<ULiveLinkRole> SubjectRole = LiveLinkClient_AnyThread->GetSubjectRole_AnyThread(LiveLinkSubjectName);
	if (SubjectRole)
	{
		if (SubjectRole->IsChildOf(ULiveLinkAnimationRole::StaticClass()))
		{
			//Process animation data if the subject is from that type
			if (LiveLinkClient_AnyThread->EvaluateFrame_AnyThread(LiveLinkSubjectName, ULiveLinkAnimationRole::StaticClass(), SubjectFrameData))
			{
				FLiveLinkSkeletonStaticData* SkeletonData = SubjectFrameData.StaticData.Cast<FLiveLinkSkeletonStaticData>();
				FLiveLinkAnimationFrameData* FrameData = SubjectFrameData.FrameData.Cast<FLiveLinkAnimationFrameData>();
				check(SkeletonData);
				check(FrameData);

				CurrentRetargetAsset->BuildPoseFromAnimationData(CachedDeltaTime, SkeletonData, FrameData, Output.Pose);
				CurrentRetargetAsset->BuildPoseAndCurveFromBaseData(CachedDeltaTime, SkeletonData, FrameData, Output.Pose, Output.Curve);
				CachedDeltaTime = 0.f; // Reset so that if we evaluate again we don't "create" time inside of the retargeter
			}
		}
		else
		{
			//Otherwise, fetch basic data that contains property / curve data
			if (LiveLinkClient_AnyThread->EvaluateFrame_AnyThread(LiveLinkSubjectName, ULiveLinkBasicRole::StaticClass(), SubjectFrameData))
			{
				FLiveLinkBaseStaticData* BaseStaticData = SubjectFrameData.StaticData.Cast<FLiveLinkBaseStaticData>();
				FLiveLinkBaseFrameData* BaseFrameData = SubjectFrameData.FrameData.Cast<FLiveLinkBaseFrameData>();
				check(BaseStaticData);
				check(BaseFrameData);

				CurrentRetargetAsset->BuildPoseAndCurveFromBaseData(CachedDeltaTime, BaseStaticData, BaseFrameData, Output.Pose, Output.Curve);
				CachedDeltaTime = 0.f; // Reset so that if we evaluate again we don't "create" time inside of the retargeter
			}
		}
	}
}

void FAnimNode_RokokoFacePose::CacheBones_AnyThread(const FAnimationCacheBonesContext & Context)
{
	Super::CacheBones_AnyThread(Context);
	InputPose.CacheBones(Context);
}

void FAnimNode_RokokoFacePose::GatherDebugData(FNodeDebugData& DebugData)
{
	FString DebugLine = FString::Printf(TEXT("LiveLink - SubjectName: %s"), *GetLiveLinkSubjectName().ToString());

	DebugData.AddDebugItem(DebugLine);
	InputPose.GatherDebugData(DebugData);
}

FLiveLinkSubjectName FAnimNode_RokokoFacePose::GetLiveLinkSubjectName()
{
	FString TempSubjectName = "actor:" + RokokoActorName.ToString() + ":face";
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25)
	return FName(TempSubjectName);
#else
	return FName(*TempSubjectName);
#endif
}

bool FAnimNode_RokokoFacePose::Serialize(FArchive& Ar)
{
	Ar.UsingCustomVersion(FLiveLinkCustomVersion::GUID);
	
	UScriptStruct* Struct = FAnimNode_RokokoFacePose::StaticStruct();
	
	if (Ar.IsLoading() || Ar.IsSaving())
	{
		Struct->SerializeTaggedProperties(Ar, (uint8*)this, Struct, nullptr);
	}

#if WITH_EDITORONLY_DATA
	//Take old data and put it in new data structure
	if (Ar.IsLoading())
	{
		if (Ar.CustomVer(FLiveLinkCustomVersion::GUID) < FLiveLinkCustomVersion::NewLiveLinkRoleSystem)
		{
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
			//LiveLinkSubjectName.Name = SubjectName_DEPRECATED;
			PRAGMA_ENABLE_DEPRECATION_WARNINGS
		}
	}
#endif

	return true;
}


// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "Runtime/AnimGraphRuntime/Public/BoneControllers/AnimNode_SkeletalControlBase.h"
#include "LiveLinkClientReference.h"
#include "LiveLinkCustomVersion.h"
#include "LiveLinkRemapAsset.h"
#include "LiveLinkTypes.h"

#include "SmartsuitController.h"
//#include "SmartsuitTPose.h"
#include "SmartsuitPoseNode.generated.h"

class ILiveLinkClient;
class SmartsuitTPose;

/*! \brief Bone reference mapping that is used to map bones between Smartsuit sensors and skeleton.*/
USTRUCT()
struct SMARTSUIT_API FSmartsuitBodyMap 
{
	GENERATED_USTRUCT_BODY()

	/**Hip bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The hip bone to control."))
		FBoneReference hip;

	/**Stomach bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The stomach bone to control."))
		FBoneReference stomach;
	/**Chest bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The chest bone to control."))
		FBoneReference chest;
	/**Neck bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The neck bone to control."))
		FBoneReference neck;
	/**Head bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The head bone to control."))
		FBoneReference head;
	/**Left shoulder bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left shoulder bone to control."))
		FBoneReference leftShoulder;
	/**Left arm reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left upper arm bone to control."))
		FBoneReference leftArm;
	/**Left forearm reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left lower arm bone to control."))
		FBoneReference leftForearm;
	/**Left hand reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left hand bone to control."))
		FBoneReference leftHand;
	/**Right shoulder bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right shoulder bone to control."))
		FBoneReference rightShoulder;
	/**Right arm bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right upper arm bone to control."))
		FBoneReference rightArm;
	/**Right forearm bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right lower arm bone to control."))
		FBoneReference rightForearm;
	/**Right hand bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right hand bone to control."))
		FBoneReference rightHand;
	/**Left upper leg bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left upper leg bone to control."))
		FBoneReference leftUpleg;
	/**Left leg bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left lower leg bone to control."))
		FBoneReference leftLeg;
	/**Left foot bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left foot bone to control."))
		FBoneReference leftFoot;
	/**Left toe bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left toe bone to control."))
		FBoneReference leftToe;
	/**Right upper leg bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right upper leg bone to control."))
		FBoneReference rightUpleg;
	/**Right leg bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right lower leg bone to control."))
		FBoneReference rightLeg;
	/**Right foot bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right foot bone to control."))
		FBoneReference rightFoot;
	/**Right toe bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The right toe bone to control."))
		FBoneReference rightToe;


	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftThumbProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftThumbMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftThumbDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftThumbTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftIndexProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftIndexMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftIndexDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftIndexTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftMiddleProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftMiddleMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftMiddleDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftMiddleTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftRingProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftRingMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftRingDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftRingTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftLittleProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftLittleMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftLittleDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference leftLittleTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightThumbProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightThumbMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightThumbDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightThumbTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightIndexProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightIndexMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightIndexDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightIndexTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightMiddleProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightMiddleMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightMiddleDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightMiddleTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightRingProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightRingMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightRingDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightRingTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightLittleProximal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightLittleMedial;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightLittleDistal;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FBoneReference rightLittleTip;
};


/*! \brief An animation controller which poses a humanoid character to the pose as received from the Smartsuit.
*
* This is the main animation blueprint node for Smartsuit. It takes as input a character in T-pose along with a mapping of the bones necessary to drive the animation.
* Then it rotates the bones and translates the hip so the character behaves as the player in the Smartsuit.
*/
USTRUCT()
struct SMARTSUIT_API FSmartsuitPoseNode : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()

	/** Bone mapping between Smartsuit sensors and character's humanoid bones. */
	UPROPERTY(/*EditAnywhere, BlueprintReadWrite, Category = SmartsuitAnimationSetup, meta = (NeverAsPin, ToolTip = "A mapping between the Smartsuit expected bone names and the bones that will animate from the Smartsuit component. Every bone is required for the animation to work properly.")*/)
	FSmartsuitBodyMap BoneMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = Retarget, meta = (PinShownByDefault))
	TSubclassOf<ULiveLinkRemapAsset> RetargetAsset;

	UPROPERTY(transient)
	ULiveLinkRemapAsset* CurrentRetargetAsset;

	void OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SourceData, meta = (PinShownByDefault))
	FName RokokoActorName;

	/** If enabled, root motion will be applied from this pose. This can help prevent characters from moving through walls etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RootMotion, meta = (PinShownByDefault))
	bool bApplyRootMotion{ false };

	/** Use to shoulder space. Tweaks the clavicle/shoulder rotation, around approx. the character up axis. Make sure that skeleton axis are imported correctly into Unreal! Defaults to 0.0f */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Space, meta = (PinShownByDefault))
	float ShoulderSpace{0.0f};

	/** Use to tweak arm space. Tweaks the upper arm rotation around approx. the character forward axis. Make sure that skeleton axis are imported correctly into Unreal! Defaults to 0.0f */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Space, meta = (PinShownByDefault))
	float ArmSpace{ 0.0f };

	/// @private
	TSharedPtr<SmartsuitTPose> SSPTPose;
	/// @private
	FVector initialPos;

	FSmartsuitPoseNode();
	
	/// @private
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	
	/// @private
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	
	/// @private
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;

	virtual bool HasPreUpdate() const { return true; }

	virtual void PreUpdate(const UAnimInstance* InAnimInstance) override;

	FLiveLinkSubjectName GetLiveLinkSubjectName();

	void CreateRetargetAsset(const UAnimInstance* InAnimInstance);

	void UpdateComponentPose_AnyThread(const FAnimationUpdateContext& Context) override;
	
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
	
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	
	//virtual bool Serialize(FArchive& Ar);

private:
	//bool firstTime = true;
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	void ApplySmartsuitRotation(FBoneReference bone, FQuat quat, FQuat dir, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	void ApplySmartsuitPosition(FBoneReference bone, FVector position, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	void ApplySmartsuitScale(FBoneReference bone, FVector scale, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	void ApplySmartsuitTransform(FBoneReference bone, FQuat quat, FVector position, FVector scale, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	float DistanceBetweenTwoBones(FBoneReference bone1, FBoneReference bone2, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	float ScaleBonesToDistance(FBoneReference scaleBone, FBoneReference bone1, FBoneReference bone2, float desiredDistance, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	FVector GetBoneScale(FBoneReference scaleBone, FBoneReference bone1, FBoneReference bone2, float desiredDistance, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	void ApplySmartsuitRotationScale(FBoneReference bone, FQuat quat, FVector scale, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	TArray<FTransform> GetAllBoneTransforms(FBoneReference bone, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	void ApplyAllBonePositions(FBoneReference bone, float hipWidth, TArray<FTransform> transforms, EBoneControlSpace space, USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases);
	// End of FAnimNode_SkeletalControlBase interface


	FLiveLinkClientReference LiveLinkClient_GameThread;
	ILiveLinkClient* LiveLinkClient_AnyThread;

	// Root motion support 
	bool bInitializedRootPosition{};
	FVector OldRootPosition{};

};

// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

//#include "UObject/NoExportTypes.h"
//#include "Engine.h"
//#include "AnimNode_SkeletalControlBase.h"
#include "Runtime/AnimGraphRuntime/Public/BoneControllers/AnimNode_SkeletalControlBase.h"
#include "SmartsuitReceiver.h"
#include "SmartsuitController.h"
#include "SmartsuitTPose.h"
#include "SmartsuitPoseNode.generated.h"


/*! \brief Bone reference mapping that is used to map bones between Smartsuit sensors and skeleton.*/
USTRUCT(BlueprintType)
struct SMARTSUIT_API FSmartsuitBodyMap {
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
	/**Head top bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The head bone to control."))
		FBoneReference headTop;
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
	/**Left finger tip bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left hand bone to control."))
		FBoneReference leftFingerTip;
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
	/**Right finger tip bone reference.*/
	UPROPERTY(EditAnywhere, Category = SkeletalControl, meta = (ToolTip = "The left hand bone to control."))
		FBoneReference rightFingerTip;
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
};

UCLASS(BlueprintType)
class SMARTSUIT_API USmartsuitBodyMapData : public UDataAsset
{
	GENERATED_BODY()

	USmartsuitBodyMapData();
public:

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName hip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName stomach;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName chest;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName neck;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName head;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName headTop;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftShoulder;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftArm;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftForearm;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftHand;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftFingerTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightShoulder;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightArm;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightForearm;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightHand;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightFingerTip;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftUpleg;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftLeg;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftFoot;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName leftToe;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightUpleg;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightLeg;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightFoot;

	UPROPERTY(EditAnywhere, Category = SkeletalControl)
	FName rightToe;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartsuitAnimationSetup, meta = (ToolTip = "A mapping between the Smartsuit expected bone names and the bones that will animate from the Smartsuit component. Every bone is required for the animation to work properly."))
	FSmartsuitBodyMap BoneMap;

	UPROPERTY(EditAnywhere, Category = SmartsuitAnimationSetup)
	USmartsuitBodyMapData* BoneMapOverride;

	/** The smartsuit controller to use. This defines which Smartsuit to use for the animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartsuitAnimationSetup, meta = (AlwaysAsPin, ToolTip = "The SmartsuitController that will be used to animate this character. This is required for the animation to work."))
		ASmartsuitController *Controller;

	/** Indicates weither the character should maintain it's XY position or override it from the position of the Smartsuit. If this is true then the character will have an offset from the actual Smartsuit position so he remains in the same position when the game begins. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartsuitAnimationSetup, meta = (NeverAsPin, ToolTip = "When this is True the animation will keep the starting position in the XY-plane of the character when the game starts. This way the character will not jump out of his initial position. Its Z position however will be identical to the one received from the Smartsuit."))
		bool RelativeToStart;

	/** Indicates if the character should scale to match the Smartsuit dimensions. This makes the character motion to be 1-1 with the player, but it may cause skinning issues with the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartsuitAnimationSetup, meta = (NeverAsPin, ToolTip = "Scale bones to match the BodyModel dimensions. This will make the animation look more natural on the character but may cause unusual effects on the mesh."))
		bool ScaleBones;

	/// @private
	SmartsuitTPose TPose;
	/// @private
	FVector initialPos;

	FSmartsuitPoseNode();
	~FSmartsuitPoseNode();

	/// @private
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	
	/// @private
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	
	/// @private
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;

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

	ASmartsuitReceiver* GetReceiver() {
		ASmartsuitReceiver * listener = nullptr;
		// Find UObjects by type
		for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
		{
			if (It->enabled) {
				listener = *It;
				break;
			}
			// ...
		}
		return listener;
	}



};

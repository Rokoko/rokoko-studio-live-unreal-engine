// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitPose.h"
#include "SmartsuitEditor.h"
#include "LiveLinkRemapAsset.h"
#include "SmartsuitEditorPrivatePCH.h"

#define LOCTEXT_NAMESPACE "SmartsuitEditor"


void USmartsuitPose::CheckForWarnings(FString name, FBoneReference bone, USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog) {
	if (ForSkeleton->GetReferenceSkeleton().FindBoneIndex(bone.BoneName) == INDEX_NONE)
	{
		if (bone.BoneName == NAME_None)
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("BoneName"), FText::FromName(*name));
			FText Msg = FText::Format(LOCTEXT("NoBoneFoundToModify", "@@ - Bone {BoneName} override in the bone override map is not set"), Args);
			MessageLog.Warning(*Msg.ToString(), this);
		}
		else
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("BoneName"), FText::FromName(*name));
			Args.Add(TEXT("OverrideName"), FText::FromName(bone.BoneName));
			FText Msg = FText::Format(LOCTEXT("NoBoneFoundToModify", "@@ - Bone override of \"{OverrideName}\" for \"{BoneName}\" in the bone override map wasn't found in the skeleton"), Args);
			MessageLog.Warning(*Msg.ToString(), this);
		}
	}
}
//PRAGMA_DISABLE_OPTIMIZATION
void USmartsuitPose::ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog)
{
	
	Super::ValidateAnimNodeDuringCompilation(ForSkeleton, MessageLog);
	
	//if (Node.Bone_Map_Override_OLD->IsValidLowLevel())
	//{
	//	Node.BoneMap.hip =					Node.Bone_Map_Override_OLD->hip;
	//	Node.BoneMap.stomach =				Node.Bone_Map_Override_OLD->stomach;
	//	Node.BoneMap.chest =				Node.Bone_Map_Override_OLD->chest;
	//	Node.BoneMap.neck =					Node.Bone_Map_Override_OLD->neck;
	//	Node.BoneMap.head =					Node.Bone_Map_Override_OLD->head;
	//	Node.BoneMap.leftShoulder =			Node.Bone_Map_Override_OLD->leftShoulder;
	//	Node.BoneMap.leftArm =				Node.Bone_Map_Override_OLD->leftArm;
	//	Node.BoneMap.leftForearm =			Node.Bone_Map_Override_OLD->leftForearm;
	//	Node.BoneMap.leftHand =				Node.Bone_Map_Override_OLD->leftHand;
	//	Node.BoneMap.rightShoulder =		Node.Bone_Map_Override_OLD->rightShoulder;
	//	Node.BoneMap.rightArm =				Node.Bone_Map_Override_OLD->rightArm;
	//	Node.BoneMap.rightForearm =			Node.Bone_Map_Override_OLD->rightForearm;
	//	Node.BoneMap.rightHand =			Node.Bone_Map_Override_OLD->rightHand;
	//	Node.BoneMap.leftUpleg =			Node.Bone_Map_Override_OLD->leftUpleg;
	//	Node.BoneMap.leftLeg =				Node.Bone_Map_Override_OLD->leftLeg;
	//	Node.BoneMap.leftFoot =				Node.Bone_Map_Override_OLD->leftFoot;
	//	Node.BoneMap.leftToe =				Node.Bone_Map_Override_OLD->leftToe;
	//	Node.BoneMap.rightUpleg =			Node.Bone_Map_Override_OLD->rightUpleg;
	//	Node.BoneMap.rightLeg =				Node.Bone_Map_Override_OLD->rightLeg;
	//	Node.BoneMap.rightFoot =			Node.Bone_Map_Override_OLD->rightFoot;
	//	Node.BoneMap.rightToe =				Node.Bone_Map_Override_OLD->rightToe;
	//	Node.BoneMap.leftThumbProximal =	Node.Bone_Map_Override_OLD->leftThumbProximal;
	//	Node.BoneMap.leftThumbMedial =		Node.Bone_Map_Override_OLD->leftThumbMedial;
	//	Node.BoneMap.leftThumbDistal =		Node.Bone_Map_Override_OLD->leftThumbDistal;
	//	Node.BoneMap.leftThumbTip =			Node.Bone_Map_Override_OLD->leftThumbTip;
	//	Node.BoneMap.leftIndexProximal =	Node.Bone_Map_Override_OLD->leftIndexProximal;
	//	Node.BoneMap.leftIndexMedial =		Node.Bone_Map_Override_OLD->leftIndexMedial;
	//	Node.BoneMap.leftIndexDistal =		Node.Bone_Map_Override_OLD->leftIndexDistal;
	//	Node.BoneMap.leftIndexTip =			Node.Bone_Map_Override_OLD->leftIndexTip;
	//	Node.BoneMap.leftMiddleProximal =	Node.Bone_Map_Override_OLD->leftMiddleProximal;
	//	Node.BoneMap.leftMiddleMedial =		Node.Bone_Map_Override_OLD->leftMiddleMedial;
	//	Node.BoneMap.leftMiddleDistal =		Node.Bone_Map_Override_OLD->leftMiddleDistal;
	//	Node.BoneMap.leftMiddleTip =		Node.Bone_Map_Override_OLD->leftMiddleTip;
	//	Node.BoneMap.leftRingProximal =		Node.Bone_Map_Override_OLD->leftRingProximal;
	//	Node.BoneMap.leftRingMedial =		Node.Bone_Map_Override_OLD->leftRingMedial;
	//	Node.BoneMap.leftRingDistal =		Node.Bone_Map_Override_OLD->leftRingDistal;
	//	Node.BoneMap.leftRingTip =			Node.Bone_Map_Override_OLD->leftRingTip;
	//	Node.BoneMap.leftLittleProximal =	Node.Bone_Map_Override_OLD->leftLittleProximal;
	//	Node.BoneMap.leftLittleMedial =		Node.Bone_Map_Override_OLD->leftLittleMedial;
	//	Node.BoneMap.leftLittleDistal =		Node.Bone_Map_Override_OLD->leftLittleDistal;
	//	Node.BoneMap.leftLittleTip =		Node.Bone_Map_Override_OLD->leftLittleTip;
	//	Node.BoneMap.rightThumbProximal =	Node.Bone_Map_Override_OLD->rightThumbProximal;
	//	Node.BoneMap.rightThumbMedial =		Node.Bone_Map_Override_OLD->rightThumbMedial;
	//	Node.BoneMap.rightThumbDistal =		Node.Bone_Map_Override_OLD->rightThumbDistal;
	//	Node.BoneMap.rightThumbTip =		Node.Bone_Map_Override_OLD->rightThumbTip;
	//	Node.BoneMap.rightIndexProximal =	Node.Bone_Map_Override_OLD->rightIndexProximal;
	//	Node.BoneMap.rightIndexMedial =		Node.Bone_Map_Override_OLD->rightIndexMedial;
	//	Node.BoneMap.rightIndexDistal =		Node.Bone_Map_Override_OLD->rightIndexDistal;
	//	Node.BoneMap.rightIndexTip =		Node.Bone_Map_Override_OLD->rightIndexTip;
	//	Node.BoneMap.rightMiddleProximal =	Node.Bone_Map_Override_OLD->rightMiddleProximal;
	//	Node.BoneMap.rightMiddleMedial =	Node.Bone_Map_Override_OLD->rightMiddleMedial;
	//	Node.BoneMap.rightMiddleDistal =	Node.Bone_Map_Override_OLD->rightMiddleDistal;
	//	Node.BoneMap.rightMiddleTip =		Node.Bone_Map_Override_OLD->rightMiddleTip;
	//	Node.BoneMap.rightRingProximal =	Node.Bone_Map_Override_OLD->rightRingProximal;
	//	Node.BoneMap.rightRingMedial =		Node.Bone_Map_Override_OLD->rightRingMedial;
	//	Node.BoneMap.rightRingDistal =		Node.Bone_Map_Override_OLD->rightRingDistal;
	//	Node.BoneMap.rightRingTip =			Node.Bone_Map_Override_OLD->rightRingTip;
	//	Node.BoneMap.rightLittleProximal =	Node.Bone_Map_Override_OLD->rightLittleProximal;
	//	Node.BoneMap.rightLittleMedial =	Node.Bone_Map_Override_OLD->rightLittleMedial;
	//	Node.BoneMap.rightLittleDistal =	Node.Bone_Map_Override_OLD->rightLittleDistal;
	//	Node.BoneMap.rightLittleTip =		Node.Bone_Map_Override_OLD->rightLittleTip;

	UClass* RetargetAssetPtr = Node.RetargetAsset.Get();
	bool assetIsRetargetted = false;

	if(IsValid(RetargetAssetPtr))
	{
		FString Msg = "retarget asset: " + Node.RetargetAsset->GetName();
		MessageLog.Note(*Msg, this);


		//ForSkeleton
		Node.CurrentRetargetAsset = NewObject<ULiveLinkRemapAsset>(GetTransientPackage(), RetargetAssetPtr);
		Node.CurrentRetargetAsset->Initialize();
		assetIsRetargetted = true;
	}
	else
	{
		FString Msg = "could not get retarget asset!";
		MessageLog.Warning(*Msg, this);
	}

	bool CheckBoneOverrides = true;
	
	if (assetIsRetargetted && Node.CurrentRetargetAsset->IsValidLowLevel())
	{ 
		Node.BoneMap.hip = Node.CurrentRetargetAsset->GetRemappedBoneName("hip");
		Node.BoneMap.stomach = Node.CurrentRetargetAsset->GetRemappedBoneName("stomach");
		Node.BoneMap.chest = Node.CurrentRetargetAsset->GetRemappedBoneName("chest");
		Node.BoneMap.neck = Node.CurrentRetargetAsset->GetRemappedBoneName("neck");
		Node.BoneMap.head = Node.CurrentRetargetAsset->GetRemappedBoneName("head");
		Node.BoneMap.leftShoulder = Node.CurrentRetargetAsset->GetRemappedBoneName("leftShoulder");
		Node.BoneMap.leftArm = Node.CurrentRetargetAsset->GetRemappedBoneName("leftArm");
		Node.BoneMap.leftForearm = Node.CurrentRetargetAsset->GetRemappedBoneName("leftForearm");
		Node.BoneMap.leftHand = Node.CurrentRetargetAsset->GetRemappedBoneName("leftHand");
		Node.BoneMap.rightShoulder = Node.CurrentRetargetAsset->GetRemappedBoneName("rightShoulder");
		Node.BoneMap.rightArm = Node.CurrentRetargetAsset->GetRemappedBoneName("rightArm");
		Node.BoneMap.rightForearm = Node.CurrentRetargetAsset->GetRemappedBoneName("rightForearm");
		Node.BoneMap.rightHand = Node.CurrentRetargetAsset->GetRemappedBoneName("rightHand");
		Node.BoneMap.leftUpleg = Node.CurrentRetargetAsset->GetRemappedBoneName("leftUpleg");
		Node.BoneMap.leftLeg = Node.CurrentRetargetAsset->GetRemappedBoneName("leftLeg");
		Node.BoneMap.leftFoot = Node.CurrentRetargetAsset->GetRemappedBoneName("leftFoot");
		Node.BoneMap.leftToe = Node.CurrentRetargetAsset->GetRemappedBoneName("leftToe");
		Node.BoneMap.rightUpleg = Node.CurrentRetargetAsset->GetRemappedBoneName("rightUpleg");
		Node.BoneMap.rightLeg = Node.CurrentRetargetAsset->GetRemappedBoneName("rightLeg");
		Node.BoneMap.rightFoot = Node.CurrentRetargetAsset->GetRemappedBoneName("rightFoot");
		Node.BoneMap.rightToe = Node.CurrentRetargetAsset->GetRemappedBoneName("rightToe");
		Node.BoneMap.leftThumbProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftThumbProximal");
		Node.BoneMap.leftThumbMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("leftThumbMedial");
		Node.BoneMap.leftThumbDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftThumbDistal");
		Node.BoneMap.leftThumbTip = Node.CurrentRetargetAsset->GetRemappedBoneName("leftThumbTip");
		Node.BoneMap.leftIndexProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftIndexProximal");
		Node.BoneMap.leftIndexMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("leftIndexMedial");
		Node.BoneMap.leftIndexDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftIndexDistal");
		Node.BoneMap.leftIndexTip = Node.CurrentRetargetAsset->GetRemappedBoneName("leftIndexTip");
		Node.BoneMap.leftMiddleProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftMiddleProximal");
		Node.BoneMap.leftMiddleMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("leftMiddleMedial");
		Node.BoneMap.leftMiddleDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftMiddleDistal");
		Node.BoneMap.leftMiddleTip = Node.CurrentRetargetAsset->GetRemappedBoneName("leftMiddleTip");
		Node.BoneMap.leftRingProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftRingProximal");
		Node.BoneMap.leftRingMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("leftRingMedial");
		Node.BoneMap.leftRingDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftRingDistal");
		Node.BoneMap.leftRingTip = Node.CurrentRetargetAsset->GetRemappedBoneName("leftRingTip");
		Node.BoneMap.leftLittleProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftLittleProximal");
		Node.BoneMap.leftLittleMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("leftLittleMedial");
		Node.BoneMap.leftLittleDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("leftLittleDistal");
		Node.BoneMap.leftLittleTip = Node.CurrentRetargetAsset->GetRemappedBoneName("leftLittleTip");
		Node.BoneMap.rightThumbProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightThumbProximal");
		Node.BoneMap.rightThumbMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("rightThumbMedial");
		Node.BoneMap.rightThumbDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightThumbDistal");
		Node.BoneMap.rightThumbTip = Node.CurrentRetargetAsset->GetRemappedBoneName("rightThumbTip");
		Node.BoneMap.rightIndexProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightIndexProximal");
		Node.BoneMap.rightIndexMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("rightIndexMedial");
		Node.BoneMap.rightIndexDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightIndexDistal");
		Node.BoneMap.rightIndexTip = Node.CurrentRetargetAsset->GetRemappedBoneName("rightIndexTip");
		Node.BoneMap.rightMiddleProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightMiddleProximal");
		Node.BoneMap.rightMiddleMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("rightMiddleMedial");
		Node.BoneMap.rightMiddleDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightMiddleDistal");
		Node.BoneMap.rightMiddleTip = Node.CurrentRetargetAsset->GetRemappedBoneName("rightMiddleTip");
		Node.BoneMap.rightRingProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightRingProximal");
		Node.BoneMap.rightRingMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("rightRingMedial");
		Node.BoneMap.rightRingDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightRingDistal");
		Node.BoneMap.rightRingTip = Node.CurrentRetargetAsset->GetRemappedBoneName("rightRingTip");
		Node.BoneMap.rightLittleProximal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightLittleProximal");
		Node.BoneMap.rightLittleMedial = Node.CurrentRetargetAsset->GetRemappedBoneName("rightLittleMedial");
		Node.BoneMap.rightLittleDistal = Node.CurrentRetargetAsset->GetRemappedBoneName("rightLittleDistal");
		Node.BoneMap.rightLittleTip = Node.CurrentRetargetAsset->GetRemappedBoneName("rightLittleTip");
	}
	else
	{
		FFormatNamedArguments Args;
		FString Msg = "Bone map override appears to not be set.  If is set, try recompiling";
		MessageLog.Warning(*Msg, this);

		CheckBoneOverrides = false;
	}

	if(CheckBoneOverrides && ForSkeleton && !ForSkeleton->HasAnyFlags(RF_NeedPostLoad))
	{
		CheckForWarnings("Hip", Node.BoneMap.hip, ForSkeleton, MessageLog);
		CheckForWarnings("Stomach", Node.BoneMap.stomach, ForSkeleton, MessageLog);
		CheckForWarnings("Chest", Node.BoneMap.chest, ForSkeleton, MessageLog);
		CheckForWarnings("Neck", Node.BoneMap.neck, ForSkeleton, MessageLog);
		CheckForWarnings("Head", Node.BoneMap.head, ForSkeleton, MessageLog);
		CheckForWarnings("Left Shoulder", Node.BoneMap.leftShoulder, ForSkeleton, MessageLog);
		CheckForWarnings("Left Arm", Node.BoneMap.leftArm, ForSkeleton, MessageLog);
		CheckForWarnings("Left Forearm", Node.BoneMap.leftForearm, ForSkeleton, MessageLog);
		CheckForWarnings("Left Hand", Node.BoneMap.leftHand, ForSkeleton, MessageLog);
		CheckForWarnings("RightShoulder", Node.BoneMap.rightShoulder, ForSkeleton, MessageLog);
		CheckForWarnings("Right Arm", Node.BoneMap.rightArm, ForSkeleton, MessageLog);
		CheckForWarnings("Right Forearm", Node.BoneMap.rightForearm, ForSkeleton, MessageLog);
		CheckForWarnings("Right Hand", Node.BoneMap.rightHand, ForSkeleton, MessageLog);
		CheckForWarnings("Left Up Leg", Node.BoneMap.leftUpleg, ForSkeleton, MessageLog);
		CheckForWarnings("Left Leg", Node.BoneMap.leftLeg, ForSkeleton, MessageLog);
		CheckForWarnings("Left Foot", Node.BoneMap.leftFoot, ForSkeleton, MessageLog);
		CheckForWarnings("Left Toe", Node.BoneMap.leftToe, ForSkeleton, MessageLog);
		CheckForWarnings("Right Up Leg", Node.BoneMap.rightUpleg, ForSkeleton, MessageLog);
		CheckForWarnings("Right Leg", Node.BoneMap.rightLeg, ForSkeleton, MessageLog);
		CheckForWarnings("Right Foot", Node.BoneMap.rightFoot, ForSkeleton, MessageLog);
		CheckForWarnings("Right Toe", Node.BoneMap.rightToe, ForSkeleton, MessageLog);
		CheckForWarnings("Left Thumb Proximal", Node.BoneMap.leftThumbProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Thumb Medial", Node.BoneMap.leftThumbMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Left Thumb Distal", Node.BoneMap.leftThumbDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Thumb Tip", Node.BoneMap.leftThumbTip, ForSkeleton, MessageLog);
		CheckForWarnings("Left Index Proximal", Node.BoneMap.leftIndexProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Index Medial", Node.BoneMap.leftIndexMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Left Index Distal", Node.BoneMap.leftIndexDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Index Tip", Node.BoneMap.leftIndexTip, ForSkeleton, MessageLog);
		CheckForWarnings("Left Middle Proximal", Node.BoneMap.leftMiddleProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Middle Medial", Node.BoneMap.leftMiddleMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Left Middle Distal", Node.BoneMap.leftMiddleDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Middle Tip", Node.BoneMap.leftMiddleTip, ForSkeleton, MessageLog);
		CheckForWarnings("Left Ring Proximal", Node.BoneMap.leftRingProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Ring Medial", Node.BoneMap.leftRingMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Left Ring Distal", Node.BoneMap.leftRingDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Ring Tip", Node.BoneMap.leftRingTip, ForSkeleton, MessageLog);
		CheckForWarnings("Left Little Proximal", Node.BoneMap.leftLittleProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Little Medial", Node.BoneMap.leftLittleMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Left Little Distal", Node.BoneMap.leftLittleDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Left Little Tip", Node.BoneMap.leftLittleTip, ForSkeleton, MessageLog);
		CheckForWarnings("Right Thumb Proximal", Node.BoneMap.rightThumbProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Thumb Medial", Node.BoneMap.rightThumbMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Right Thumb Distal", Node.BoneMap.rightThumbDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Thumb Tip", Node.BoneMap.rightThumbTip, ForSkeleton, MessageLog);
		CheckForWarnings("Right Index Proximal", Node.BoneMap.rightIndexProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Index Medial", Node.BoneMap.rightIndexMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Right Index Distal", Node.BoneMap.rightIndexDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Index Tip", Node.BoneMap.rightIndexTip, ForSkeleton, MessageLog);
		CheckForWarnings("Right Middle Proximal", Node.BoneMap.rightMiddleProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Middle Medial", Node.BoneMap.rightMiddleMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Right Middle Distal", Node.BoneMap.rightMiddleDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Middle Tip", Node.BoneMap.rightMiddleTip, ForSkeleton, MessageLog);
		CheckForWarnings("Right Ring Proximal", Node.BoneMap.rightRingProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Ring Medial", Node.BoneMap.rightRingMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Right Ring Distal", Node.BoneMap.rightRingDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Ring Tip", Node.BoneMap.rightRingTip, ForSkeleton, MessageLog);
		CheckForWarnings("Right Little Proximal", Node.BoneMap.rightLittleProximal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Little Medial", Node.BoneMap.rightLittleMedial, ForSkeleton, MessageLog);
		CheckForWarnings("Right Little Distal", Node.BoneMap.rightLittleDistal, ForSkeleton, MessageLog);
		CheckForWarnings("Right Little Tip", Node.BoneMap.rightLittleTip, ForSkeleton, MessageLog);
	}


	MessageLog.Note(*LOCTEXT("UsageNote", "@@ - For this animation node to work properly the character needs to be in T-pose when this node is evaluating the pose and all bones in Bone Map need to be specified. This node will change the body position of the character to match that of the Smartsuit, assuming the character is in TPose, which means it will set an absolute position of the character. If you want to change the position of the character is better to do it after this node. The alpha value of this node is ignored at the moment.").ToString(), this);
	//if (Node.Controller == NULL) {
	//	MessageLog.Warning(*LOCTEXT("NoSmartsuit", "@@ - Smartsuit Controller is not set.").ToString(), this);
	//}
	
}

FText USmartsuitPose::GetControllerDescription() const
{
	return LOCTEXT("SmartsuitPose", "Rokoko Body Pose");
}

FText USmartsuitPose::GetTooltipText() const
{
	return LOCTEXT("SmartsuitPose_Tooltip", "This node will make the character pose the same as the live feed from the defined Smartsuit.");
}

FText USmartsuitPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GetControllerDescription();
}

void USmartsuitPose::CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode)
{
	FSmartsuitPoseNode* ModifyBone = static_cast<FSmartsuitPoseNode*>(InPreviewNode);
	Node.RetargetAsset = ModifyBone->RetargetAsset;
}

void USmartsuitPose::CopyPinDefaultsToNodeData(UEdGraphPin* InPin)
{
}

void USmartsuitPose::PreloadRequiredAssets()
{
	Super::PreloadRequiredAssets();
	PreloadObject(Node.RetargetAsset);
}

//PRAGMA_ENABLE_OPTIMIZATION
#undef LOCTEXT_NAMESPACE

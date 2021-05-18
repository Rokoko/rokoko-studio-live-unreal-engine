// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitPose.h"
#include "SmartsuitEditor.h"
#include "SmartsuitEditorPrivatePCH.h"
//#include "CompilerResultsLog.h"





#define LOCTEXT_NAMESPACE "SmartsuitEditor"


//USmartsuitPose::USmartsuitPose(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	//CurWidgetMode = (int32)FWidget::WM_Rotate;
//}

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

void USmartsuitPose::ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog)
{
	if (Node.BoneMapOverride->IsValidLowLevel())
	{
		Node.BoneMap.hip =				Node.BoneMapOverride->hip;
		Node.BoneMap.stomach =			Node.BoneMapOverride->stomach;
		Node.BoneMap.chest =			Node.BoneMapOverride->chest;
		Node.BoneMap.neck =				Node.BoneMapOverride->neck;
		Node.BoneMap.head =				Node.BoneMapOverride->head;
		//Node.BoneMap.headTop =		Node.BoneMapOverride->headTop;
		Node.BoneMap.leftShoulder =		Node.BoneMapOverride->leftShoulder;
		Node.BoneMap.leftArm =			Node.BoneMapOverride->leftArm;
		Node.BoneMap.leftForearm =		Node.BoneMapOverride->leftForearm;
		Node.BoneMap.leftHand =			Node.BoneMapOverride->leftHand;
		//Node.BoneMap.leftFingerTip =	Node.BoneMapOverride->leftFingerTip;
		Node.BoneMap.rightShoulder =	Node.BoneMapOverride->rightShoulder;
		Node.BoneMap.rightArm =			Node.BoneMapOverride->rightArm;
		Node.BoneMap.rightForearm =		Node.BoneMapOverride->rightForearm;
		Node.BoneMap.rightHand =		Node.BoneMapOverride->rightHand;
		//Node.BoneMap.rightFingerTip =	Node.BoneMapOverride->rightFingerTip;
		Node.BoneMap.leftUpleg =		Node.BoneMapOverride->leftUpleg;
		Node.BoneMap.leftLeg =			Node.BoneMapOverride->leftLeg;
		Node.BoneMap.leftFoot =			Node.BoneMapOverride->leftFoot;
		Node.BoneMap.leftToe =		Node.BoneMapOverride->leftToe;
		Node.BoneMap.rightUpleg =		Node.BoneMapOverride->rightUpleg;
		Node.BoneMap.rightLeg =			Node.BoneMapOverride->rightLeg;
		Node.BoneMap.rightFoot =		Node.BoneMapOverride->rightFoot;
		Node.BoneMap.rightToe =		Node.BoneMapOverride->rightToe;

		Node.BoneMap.leftThumbProximal = Node.BoneMapOverride->leftThumbProximal;
		Node.BoneMap.leftThumbMedial = Node.BoneMapOverride->leftThumbMedial;
		Node.BoneMap.leftThumbDistal = Node.BoneMapOverride->leftThumbDistal;
		Node.BoneMap.leftThumbTip = Node.BoneMapOverride->leftThumbTip;
		Node.BoneMap.leftIndexProximal = Node.BoneMapOverride->leftIndexProximal;
		Node.BoneMap.leftIndexMedial = Node.BoneMapOverride->leftIndexMedial;
		Node.BoneMap.leftIndexDistal = Node.BoneMapOverride->leftIndexDistal;
		Node.BoneMap.leftIndexTip = Node.BoneMapOverride->leftIndexTip;
		Node.BoneMap.leftMiddleProximal = Node.BoneMapOverride->leftMiddleProximal;
		Node.BoneMap.leftMiddleMedial = Node.BoneMapOverride->leftMiddleMedial;
		Node.BoneMap.leftMiddleDistal = Node.BoneMapOverride->leftMiddleDistal;
		Node.BoneMap.leftMiddleTip = Node.BoneMapOverride->leftMiddleTip;
		Node.BoneMap.leftRingProximal = Node.BoneMapOverride->leftRingProximal;
		Node.BoneMap.leftRingMedial = Node.BoneMapOverride->leftRingMedial;
		Node.BoneMap.leftRingDistal = Node.BoneMapOverride->leftRingDistal;
		Node.BoneMap.leftRingTip = Node.BoneMapOverride->leftRingTip;
		Node.BoneMap.leftLittleProximal = Node.BoneMapOverride->leftLittleProximal;
		Node.BoneMap.leftLittleMedial = Node.BoneMapOverride->leftLittleMedial;
		Node.BoneMap.leftLittleDistal = Node.BoneMapOverride->leftLittleDistal;
		Node.BoneMap.leftLittleTip = Node.BoneMapOverride->leftLittleTip;
		Node.BoneMap.rightThumbProximal = Node.BoneMapOverride->rightThumbProximal;
		Node.BoneMap.rightThumbMedial = Node.BoneMapOverride->rightThumbMedial;
		Node.BoneMap.rightThumbDistal = Node.BoneMapOverride->rightThumbDistal;
		Node.BoneMap.rightThumbTip = Node.BoneMapOverride->rightThumbTip;
		Node.BoneMap.rightIndexProximal = Node.BoneMapOverride->rightIndexProximal;
		Node.BoneMap.rightIndexMedial = Node.BoneMapOverride->rightIndexMedial;
		Node.BoneMap.rightIndexDistal = Node.BoneMapOverride->rightIndexDistal;
		Node.BoneMap.rightIndexTip = Node.BoneMapOverride->rightIndexTip;
		Node.BoneMap.rightMiddleProximal = Node.BoneMapOverride->rightMiddleProximal;
		Node.BoneMap.rightMiddleMedial = Node.BoneMapOverride->rightMiddleMedial;
		Node.BoneMap.rightMiddleDistal = Node.BoneMapOverride->rightMiddleDistal;
		Node.BoneMap.rightMiddleTip = Node.BoneMapOverride->rightMiddleTip;
		Node.BoneMap.rightRingProximal = Node.BoneMapOverride->rightRingProximal;
		Node.BoneMap.rightRingMedial = Node.BoneMapOverride->rightRingMedial;
		Node.BoneMap.rightRingDistal = Node.BoneMapOverride->rightRingDistal;
		Node.BoneMap.rightRingTip = Node.BoneMapOverride->rightRingTip;
		Node.BoneMap.rightLittleProximal = Node.BoneMapOverride->rightLittleProximal;
		Node.BoneMap.rightLittleMedial = Node.BoneMapOverride->rightLittleMedial;
		Node.BoneMap.rightLittleDistal = Node.BoneMapOverride->rightLittleDistal;
		Node.BoneMap.rightLittleTip = Node.BoneMapOverride->rightLittleTip;
	}
	else
	{
		FFormatNamedArguments Args;
		FString Msg = "Bone map override not set!";
		MessageLog.Warning(*Msg, this);
	}

	if(ForSkeleton && !ForSkeleton->HasAnyFlags(RF_NeedPostLoad))
	{
		CheckForWarnings("Hip", Node.BoneMap.hip, ForSkeleton, MessageLog);
		CheckForWarnings("Stomach", Node.BoneMap.stomach, ForSkeleton, MessageLog);
		CheckForWarnings("Chest", Node.BoneMap.chest, ForSkeleton, MessageLog);
		CheckForWarnings("Neck", Node.BoneMap.neck, ForSkeleton, MessageLog);
		CheckForWarnings("Head", Node.BoneMap.head, ForSkeleton, MessageLog);
		//CheckForWarnings("HeadTop", Node.BoneMap.headTop, ForSkeleton, MessageLog);
		CheckForWarnings("Left Shoulder", Node.BoneMap.leftShoulder, ForSkeleton, MessageLog);
		CheckForWarnings("Left Arm", Node.BoneMap.leftArm, ForSkeleton, MessageLog);
		CheckForWarnings("Left Forearm", Node.BoneMap.leftForearm, ForSkeleton, MessageLog);
		CheckForWarnings("Left Hand", Node.BoneMap.leftHand, ForSkeleton, MessageLog);
		//CheckForWarnings("Left Finger Tip", Node.BoneMap.leftFingerTip, ForSkeleton, MessageLog);
		CheckForWarnings("RightShoulder", Node.BoneMap.rightShoulder, ForSkeleton, MessageLog);
		CheckForWarnings("Right Arm", Node.BoneMap.rightArm, ForSkeleton, MessageLog);
		CheckForWarnings("Right Forearm", Node.BoneMap.rightForearm, ForSkeleton, MessageLog);
		CheckForWarnings("Right Hand", Node.BoneMap.rightHand, ForSkeleton, MessageLog);
		//CheckForWarnings("Right Finger Tip", Node.BoneMap.rightFingerTip, ForSkeleton, MessageLog);
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
	Super::ValidateAnimNodeDuringCompilation(ForSkeleton, MessageLog);
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
	//if ((TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle) && (Node.BoneToModify.BoneName == NAME_None))
	//{
	return GetControllerDescription();
	//}
	// @TODO: the bone can be altered in the property editor, so we have to 
	//        choose to mark this dirty when that happens for this to properly work
	//else //if (!CachedNodeTitles.IsTitleCached(TitleType, this))
	//{
	//	FFormatNamedArguments Args;
	//	Args.Add(TEXT("ControllerDescription"), GetControllerDescription());
	//	Args.Add(TEXT("BoneName"), FText::FromName(Node.BoneToModify.BoneName));

	//	// FText::Format() is slow, so we cache this to save on performance
	//	if (TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle)
	//	{
	//		CachedNodeTitles.SetCachedTitle(TitleType, FText::Format(LOCTEXT("AnimGraphNode_ModifyBone_ListTitle", "{ControllerDescription} - Bone: {BoneName}"), Args), this);
	//	}
	//	else
	//	{
	//		CachedNodeTitles.SetCachedTitle(TitleType, FText::Format(LOCTEXT("AnimGraphNode_ModifyBone_Title", "{ControllerDescription}\nBone: {BoneName}"), Args), this);
	//	}
	//}
	//return CachedNodeTitles[TitleType];
}

void USmartsuitPose::CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode)
{
	FSmartsuitPoseNode* ModifyBone = static_cast<FSmartsuitPoseNode*>(InPreviewNode);

	//// copies Pin values from the internal node to get data which are not compiled yet
	//ModifyBone->Translation = Node.Translation;
	//ModifyBone->Rotation = Node.Rotation;
	//ModifyBone->Scale = Node.Scale;

	//// copies Modes
	////ModifyBone->TranslationMode = Node.TranslationMode;
	////ModifyBone->RotationMode = Node.RotationMode;
	////ModifyBone->ScaleMode = Node.ScaleMode;

	//// copies Spaces
	//ModifyBone->TranslationSpace = Node.TranslationSpace;
	//ModifyBone->RotationSpace = Node.RotationSpace;
	//ModifyBone->ScaleSpace = Node.ScaleSpace;
}

//FEditorModeID USmartsuitPose::GetEditorMode() const
//{
//	return FEditorModeID("AnimGraph.SkeletalControl.ModifyBone");
//	//return  AnimNodeEditModes::ModifyBone;
//}

void USmartsuitPose::CopyPinDefaultsToNodeData(UEdGraphPin* InPin)
{
	//if (InPin->GetName() == GET_MEMBER_NAME_STRING_CHECKED(FSmartsuitPoseNode, Translation))
	//{
	//	GetDefaultValue(GET_MEMBER_NAME_STRING_CHECKED(FSmartsuitPoseNode, Translation), Node.Translation);
	//}
	//else if (InPin->GetName() == GET_MEMBER_NAME_STRING_CHECKED(FSmartsuitPoseNode, Rotation))
	//{
	//	GetDefaultValue(GET_MEMBER_NAME_STRING_CHECKED(FSmartsuitPoseNode, Rotation), Node.Rotation);
	//}
	//else if (InPin->GetName() == GET_MEMBER_NAME_STRING_CHECKED(FSmartsuitPoseNode, Scale))
	//{
	//	GetDefaultValue(GET_MEMBER_NAME_STRING_CHECKED(FSmartsuitPoseNode, Scale), Node.Scale);
	//}
}

#undef LOCTEXT_NAMESPACE

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
			FText Msg = FText::Format(LOCTEXT("NoBoneFoundToModify", "@@ - Bone {BoneName} is not set"), Args);
			MessageLog.Warning(*Msg.ToString(), this);
		}
		else
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("BoneName"), FText::FromName(*name));
			FText Msg = FText::Format(LOCTEXT("NoBoneFoundToModify", "@@ - Bone {BoneName} wasn't found in the skeleton"), Args);
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
		Node.BoneMap.headTop =			Node.BoneMapOverride->headTop;
		Node.BoneMap.leftShoulder =		Node.BoneMapOverride->leftShoulder;
		Node.BoneMap.leftArm =			Node.BoneMapOverride->leftArm;
		Node.BoneMap.leftForearm =		Node.BoneMapOverride->leftForearm;
		Node.BoneMap.leftHand =			Node.BoneMapOverride->leftHand;
		Node.BoneMap.leftFingerTip =	Node.BoneMapOverride->leftFingerTip;
		Node.BoneMap.rightShoulder =	Node.BoneMapOverride->rightShoulder;
		Node.BoneMap.rightArm =			Node.BoneMapOverride->rightArm;
		Node.BoneMap.rightForearm =		Node.BoneMapOverride->rightForearm;
		Node.BoneMap.rightHand =		Node.BoneMapOverride->rightHand;
		Node.BoneMap.rightFingerTip =	Node.BoneMapOverride->rightFingerTip;
		Node.BoneMap.leftUpleg =		Node.BoneMapOverride->leftUpleg;
		Node.BoneMap.leftLeg =			Node.BoneMapOverride->leftLeg;
		Node.BoneMap.leftFoot =			Node.BoneMapOverride->leftFoot;
		Node.BoneMap.leftToe =			Node.BoneMapOverride->leftToe;
		Node.BoneMap.rightUpleg =		Node.BoneMapOverride->rightUpleg;
		Node.BoneMap.rightLeg =			Node.BoneMapOverride->rightLeg;
		Node.BoneMap.rightFoot =		Node.BoneMapOverride->rightFoot;
		Node.BoneMap.rightToe =			Node.BoneMapOverride->rightToe;
	}

	CheckForWarnings("Hip", Node.BoneMap.hip, ForSkeleton, MessageLog);
	CheckForWarnings("Stomach", Node.BoneMap.stomach, ForSkeleton, MessageLog);
	CheckForWarnings("Chest", Node.BoneMap.chest, ForSkeleton, MessageLog);
	CheckForWarnings("Neck", Node.BoneMap.neck, ForSkeleton, MessageLog);
	CheckForWarnings("Head", Node.BoneMap.head, ForSkeleton, MessageLog);
	CheckForWarnings("HeadTop", Node.BoneMap.headTop, ForSkeleton, MessageLog);
	CheckForWarnings("Left Shoulder", Node.BoneMap.leftShoulder, ForSkeleton, MessageLog);
	CheckForWarnings("Left Arm", Node.BoneMap.leftArm, ForSkeleton, MessageLog);
	CheckForWarnings("Left Forearm", Node.BoneMap.leftForearm, ForSkeleton, MessageLog);
	CheckForWarnings("Left Hand", Node.BoneMap.leftHand, ForSkeleton, MessageLog);
	CheckForWarnings("Left Finger Tip", Node.BoneMap.leftFingerTip, ForSkeleton, MessageLog);
	CheckForWarnings("RightShoulder", Node.BoneMap.rightShoulder, ForSkeleton, MessageLog);
	CheckForWarnings("Right Arm", Node.BoneMap.rightArm, ForSkeleton, MessageLog);
	CheckForWarnings("Right Forearm", Node.BoneMap.rightForearm, ForSkeleton, MessageLog);
	CheckForWarnings("Right Hand", Node.BoneMap.rightHand, ForSkeleton, MessageLog);
	CheckForWarnings("Right Finger Tip", Node.BoneMap.rightFingerTip, ForSkeleton, MessageLog);
	CheckForWarnings("Left Up Leg", Node.BoneMap.leftUpleg, ForSkeleton, MessageLog);
	CheckForWarnings("Left Leg", Node.BoneMap.leftLeg, ForSkeleton, MessageLog);
	CheckForWarnings("Left Foot", Node.BoneMap.leftFoot, ForSkeleton, MessageLog);
	CheckForWarnings("Left Toe", Node.BoneMap.leftToe, ForSkeleton, MessageLog);
	CheckForWarnings("Right Up Leg", Node.BoneMap.rightUpleg, ForSkeleton, MessageLog);
	CheckForWarnings("Right Leg", Node.BoneMap.rightLeg, ForSkeleton, MessageLog);
	CheckForWarnings("Right Foot", Node.BoneMap.rightFoot, ForSkeleton, MessageLog);
	CheckForWarnings("Right Toe", Node.BoneMap.rightToe, ForSkeleton, MessageLog);
	MessageLog.Note(*LOCTEXT("UsageNote", "@@ - For this animation node to work properly the character needs to be in T-pose when this node is evaluating the pose and all bones in Bone Map need to be specified. This node will change the body position of the character to match that of the Smartsuit, assuming the character is in TPose, which means it will set an absolute position of the character. If you want to change the position of the character is better to do it after this node. The alpha value of this node is ignored at the moment.").ToString(), this);
	//if (Node.Controller == NULL) {
	//	MessageLog.Warning(*LOCTEXT("NoSmartsuit", "@@ - Smartsuit Controller is not set.").ToString(), this);
	//}
	Super::ValidateAnimNodeDuringCompilation(ForSkeleton, MessageLog);
}

FText USmartsuitPose::GetControllerDescription() const
{
	return LOCTEXT("SmartsuitPose", "Smartsuit body pose");
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

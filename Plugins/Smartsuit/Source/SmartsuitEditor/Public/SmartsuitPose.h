// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "AnimGraphNode_SkeletalControlBase.h"
#include "SmartsuitPoseNode.h"

#include "EdGraph/EdGraphNodeUtils.h" // for FNodeTitleTextTable
#include "SmartsuitPose.generated.h"

/// @cond no_doc
UCLASS(meta = (Keywords = "Smartsuit Pose"))
class SMARTSUITEDITOR_API USmartsuitPose : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()
	
	
		UPROPERTY(EditAnywhere, Category = Settings)
		FSmartsuitPoseNode Node;

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

protected:
	// UAnimGraphNode_Base interface
	virtual void ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog) override;
	//virtual FEditorModeID GetEditorMode() const override;
	virtual void CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode) override;
	virtual void CopyPinDefaultsToNodeData(UEdGraphPin* InPin) override;
	// End of UAnimGraphNode_Base interface

	// UAnimGraphNode_SkeletalControlBase interface
	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }
	// End of UAnimGraphNode_SkeletalControlBase interface

private:
	void CheckForWarnings(FString name, FBoneReference bone, USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog);
	/** Constructing FText strings can be costly, so we cache the node's title */
	FNodeTitleTextTable CachedNodeTitles;

	// storing current widget mode 
	int32 CurWidgetMode;
	
};
/// @endcond
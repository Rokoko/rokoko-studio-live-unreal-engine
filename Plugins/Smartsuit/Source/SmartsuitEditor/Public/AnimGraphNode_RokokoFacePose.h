// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "AnimGraphNode_Base.h"
#include "AnimNode_RokokoFacePose.h"

#include "AnimGraphNode_RokokoFacePose.generated.h"



UCLASS()
class UAnimGraphNode_RokokoFacePose : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:

	//~ UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const;
	//~ End of UEdGraphNode

	//~ Begin UK2Node interface
	virtual void ConvertDeprecatedNode(UEdGraph* Graph, bool bOnlySafeChanges) override;
	//~ End UK2Node interface

public:

	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_RokokoFacePose Node;

};

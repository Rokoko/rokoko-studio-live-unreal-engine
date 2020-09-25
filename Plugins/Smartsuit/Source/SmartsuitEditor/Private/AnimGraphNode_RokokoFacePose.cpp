// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "AnimGraphNode_RokokoFacePose.h"

#include "EdGraph/EdGraphSchema.h"

#define LOCTEXT_NAMESPACE "RokokoFaceAnimNode"

FText UAnimGraphNode_RokokoFacePose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("NodeTitle", "Rokoko Face Pose");
}

FText UAnimGraphNode_RokokoFacePose::GetTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Retrieves the current pose associated with the supplied subject");
}

FText UAnimGraphNode_RokokoFacePose::GetMenuCategory() const
{
	return LOCTEXT("NodeCategory", "Live Link");
}

void UAnimGraphNode_RokokoFacePose::ConvertDeprecatedNode(UEdGraph* Graph, bool bOnlySafeChanges)
{
	////Find deprecated SubjectName pin and set new pin with its value
	//const FName OldPinName = TEXT("SubjectName"); //Variable now has _DEPRECATED appended so can't use GET_MEMBER_NAME_CHECKED
	//const FName NewPinName = GET_MEMBER_NAME_CHECKED(FAnimNode_RokokoFacePose, LiveLinkSubjectName);
	//UEdGraphPin** FoundPinPtr = Pins.FindByPredicate([OldPinName](const UEdGraphPin* Other) { return Other->PinName == OldPinName; });
	//if (FoundPinPtr != nullptr)
	//{
	//	UEdGraphPin** FoundNewPinPtr = Pins.FindByPredicate([NewPinName](const UEdGraphPin* Other) { return Other->PinName == NewPinName; });
	//	if (FoundNewPinPtr != nullptr)
	//	{
	//		UScriptStruct* StructType = FLiveLinkSubjectName::StaticStruct();
	//		UEdGraphPin* OldPin = *FoundPinPtr;
	//		UEdGraphPin* NewPin = *FoundNewPinPtr;

	//		//Create new structure from old data
	//		FLiveLinkSubjectName NewName;
	//		NewName.Name = *OldPin->DefaultValue;

	//		//Apply new name structure 
	//		FString StringValue;
	//		StructType->ExportText(StringValue, &NewName, nullptr, nullptr, EPropertyPortFlags::PPF_None, nullptr);
	//		NewPin->GetSchema()->TrySetDefaultValue(*NewPin, StringValue);

	//		//Update node data with graph data
	//		Node.LiveLinkSubjectName = NewName;
	//	}
	//}
}

#undef LOCTEXT_NAMESPACE
//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#include "Roles/LiveLinkSmartsuitRole.h"
//#include "Roles/LiveLinkSmartsuitTypes.h"
//#include "Roles/LiveLinkAnimationBlueprintStructs.h"
//
//#define LOCTEXT_NAMESPACE "LiveLinkRole"
//
//UScriptStruct* ULiveLinkSmartsuitRole::GetStaticDataStruct() const
//{
//	return FLiveLinkSmartsuitStaticData::StaticStruct();
//}
//
//UScriptStruct* ULiveLinkSmartsuitRole::GetFrameDataStruct() const
//{
//	return FLiveLinkSmartsuitFrameData::StaticStruct();
//}
//
//UScriptStruct* ULiveLinkSmartsuitRole::GetBlueprintDataStruct() const
//{
//	return FLiveLinkSmartsuitBlueprintData::StaticStruct();
//}
//
//bool ULiveLinkSmartsuitRole::InitializeBlueprintData(const FLiveLinkSubjectFrameData& InSourceData, FLiveLinkBlueprintDataStruct& OutBlueprintData) const
//{
//	//bool bSuccess = false;
//
//	//FLiveLinkSmartsuitBlueprintData* BlueprintData = OutBlueprintData.Cast<FLiveLinkSmartsuitBlueprintData>();
//	//const FLiveLinkSmartsuitStaticData* StaticData = InSourceData.StaticData.Cast<FLiveLinkSmartsuitStaticData>();
//	//const FLiveLinkSmartsuitFrameData* FrameData = InSourceData.FrameData.Cast<FLiveLinkSmartsuitFrameData>();
//	//if (BlueprintData && StaticData && FrameData)
//	//{
//	//	GetStaticDataStruct()->CopyScriptStruct(&BlueprintData->StaticData, StaticData);
//	//	GetFrameDataStruct()->CopyScriptStruct(&BlueprintData->FrameData, FrameData);
//	//	bSuccess = true;
//	//}
//
//	//return bSuccess;
//
//	bool bSuccess = false;
//
//	//FSubjectFrameHandle* AnimationFrameHandle = OutBlueprintData.Cast<FSubjectFrameHandle>();
//	//const FLiveLinkSmartsuitStaticData* StaticData = InSourceData.StaticData.Cast<FLiveLinkSmartsuitStaticData>();
//	//const FLiveLinkSmartsuitFrameData* FrameData = InSourceData.FrameData.Cast<FLiveLinkSmartsuitFrameData>();
//	//if (AnimationFrameHandle && StaticData && FrameData)
//	//{
//	//	AnimationFrameHandle->SetCachedFrame(MakeShared<FCachedSubjectFrame>(StaticData, FrameData));
//	//	bSuccess = true;
//	//}
//
//	bSuccess = Super::InitializeBlueprintData(InSourceData, OutBlueprintData);
//
//	return bSuccess;
//}
//
//FText ULiveLinkSmartsuitRole::GetDisplayName() const
//{
//	return LOCTEXT("SmartsuitRole", "Smartsuit");
//}
//
//#undef LOCTEXT_NAMESPACE
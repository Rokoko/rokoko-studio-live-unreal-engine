// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitBlueprintLibrary.h"
#include "VirtualProductionSource.h"
#include "Engine/SkeletalMesh.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"


FFace USmartsuitBlueprintLibrary::GetFaceByFaceID(FString faceId)
{
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		return livelink->GetFaceByFaceID(faceId);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("can not get virtual production source for FaceID!!!"));
	}
	return FFace();
}

FFace USmartsuitBlueprintLibrary::GetFaceByProfileName(const FString& faceName, bool& found)
{
	found = false;
	
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		FFace* temp = livelink->GetFaceByProfileName(faceName);
		if (temp)
		{
			found = true;
			return FFace(*temp);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("can not get virtual production source!!!"));
	}
	return FFace();
}

TArray<FFace> USmartsuitBlueprintLibrary::GetAllFaces()
{
	TArray<FFace> Faces;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		Faces = livelink->GetAllFaces();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetAllFaces] can not get virtual production source!!!"));
	}
	return Faces;
}

TArray<FFace> USmartsuitBlueprintLibrary::GetFacesNotAssociatedWithActor()
{
	TArray<FFace> FacesNotPairedWithSuit;
	for (auto& CurrentFace : GetAllFaces())
	{
		bool FoundExistingProfileForFace = false;

		for (auto& CurrentSuit : GetAllSmartsuits())
		{
			if (CurrentSuit.id == CurrentFace.profileName)
			{
				FoundExistingProfileForFace = true;
			}
		}

		if (!FoundExistingProfileForFace)
		{
			FacesNotPairedWithSuit.Add(CurrentFace);
		}
	}
	return FacesNotPairedWithSuit;
}


FSuitData* USmartsuitBlueprintLibrary::GetSmartsuit(FString suitName)
{
	FSuitData* ReturnValue = nullptr;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		ReturnValue = livelink->GetSmartsuitByName(suitName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetSmartsuit] can not get virtual production source!!!"));
	}

	return ReturnValue;
}

bool USmartsuitBlueprintLibrary::GetSmartsuitByName(const FString& suitName, FSuitData& SuitData)
{
	if (FSuitData* smartsuit = GetSmartsuit(suitName))
	{
		SuitData = *smartsuit;
		return true;
	}
	SuitData = FSuitData();
	return false;
}

TArray<FSuitData> USmartsuitBlueprintLibrary::GetAllSmartsuits()
{
	TArray<FSuitData> Smartsuits;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		Smartsuits = livelink->GetAllSmartsuits();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetAllSmartsuits] can not get virtual production source!!!"));
	}
	return Smartsuits;
}

TArray<FString> USmartsuitBlueprintLibrary::GetAvailableActorNames()
{
	TArray<FString> SmartsuitNames;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		SmartsuitNames = livelink->GetAvailableSmartsuitNames();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetAvailableActorNames] can not get virtual production source!!!"));
	}
	return SmartsuitNames;
}

ASmartsuitController* USmartsuitBlueprintLibrary::GetSmartsuitControllerByName(FString name)
{
	ASmartsuitController* actor = nullptr;

	for (TObjectIterator<ASmartsuitController> It; It; ++It)
	{
		if (It->IsRealLife()) 
		{
				ASmartsuitController* curActor = *It;
				FString mySuitName = curActor->suitname;
				if (name.Compare(mySuitName) == 0) 
				{
					actor = curActor;
					break;
				}
		}
	}
	return actor;
}

ASmartsuitController* USmartsuitBlueprintLibrary::GetSmartsuitController(int id)
{
	ASmartsuitController* actor = nullptr;

	for (TObjectIterator<ASmartsuitController> It; It; ++It)
	{
		if (It->IsRealLife())
		{
			ASmartsuitController* curActor = *It;

			if (id == curActor->IndexID)
			{
				actor = curActor;
				break;
			}
		}
	}
	return actor;
}


FProp* USmartsuitBlueprintLibrary::GetPropByNameFromVP(FString name, bool isLive)
{
	FProp* ReturnValue = nullptr;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		ReturnValue = livelink->GetPropByName(name, isLive);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetPropByNameFromVP] can not get virtual production source!!!"));
	}
	return ReturnValue;
}

TArray<FProp> USmartsuitBlueprintLibrary::GetAllProps()
{
	TArray<FProp> AllProps;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
	
		AllProps = livelink->GetAllProps();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetAllProps] can not get virtual production source!!!"));
	}
	return AllProps;
}

bool USmartsuitBlueprintLibrary::GetProp(FString name, /*bool isLive, */FProp& OutProp)
{
	if (FProp* prop = GetPropByNameFromVP(name, true))
	{
		OutProp = *prop;
		return true;
	}
	OutProp = FProp();
	return false;
}

FTracker* USmartsuitBlueprintLibrary::GetTrackerByNameFromVP(FString name, bool isLive)
{
	FTracker* Tracker = nullptr;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		Tracker = livelink->GetTrackerByName(name, isLive);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetTrackerByNameFromVP] can not get virtual production source!!!"));
	}
	return Tracker;
}

FTracker USmartsuitBlueprintLibrary::GetTracker(FString name, bool isLive)
{
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		return *livelink->GetTrackerByName(name, isLive);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[USmartsuitBlueprintLibrary::GetTracker] can not get virtual production source!!!"));
	}
	return FTracker();
}

FTracker USmartsuitBlueprintLibrary::GetTrackerByConnectionIDFromVP(const FString& name, bool isLive, bool& found)
{
	found = false;
	FTracker returnval;
	FTracker* temp = nullptr;
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		temp = livelink->GetTrackerByConnectionID(name, isLive);
	}
	if (temp)
	{
		returnval = *temp;
		found = true;
	}
	return returnval;
}


void USmartsuitBlueprintLibrary::JSONTest()
{
	FString JsonTest = "{\"version\":0,\"timestamp\":3501.165771484375,\"playbackTimestamp\":36.01746044922,\"props\":[{\"name\":\"Camera-1\",\"id\":\"764cde8d-27d9-4a3a-ac60-e53af5ddfaac\",\"position\":{\"X\":0.62,\"Y\":1.53,\"z\":-0.417694},\"rotation\":{\"x\":0.00904452,\"y\":-0.832524,\"z\":0.063621,\"w\":0.550299},\"isLive\":false,\"profile\":{\"name\":\"Camera\",\"uuid\":\"camera-1\",\"dimensions\":{\"x\":0.23999198,\"y\":0.1805738,\"z\":0.0393033},\"color\":{\"x\":0.85888933,\"y\":0.274593,\"z\":0.380505},\"trackerOffset\":{\"position\":{\"x\":-0.0599549,\"y\":0.099993,\"z\":0.0},\"rotation\":{\"x\":-90.0,\"y\":0.0,\"z\":0.0}},\"pivot\":{\"position\":{\"x\":0.0,\"y\":0.0,\"z\":0.0199165},\"rotation\":{\"x\":0.0,\"y\":0.0,\"z\":0.0}},\"grips\":[],\"propType\":0}},{\"name\":\"Paddle-1\",\"id\":\"3d39d613-db56-49e1-923a-15a43ead2c09\",\"position\":{\"x\":-0.2098624,\"y\":0.34978352,\"z\":-0.2011109},\"rotation\":{\"x\":0.1669078,\"y\":0.8222,\"z\":0.478498,\"w\":-0.303668},\"isLive\":false,\"profile\":{\"name\":\"Stick\",\"uuid\":\"stick-1\",\"dimensions\":{\"x\":0.03033,\"y\":0.699071,\"z\":0.0399033},\"color\":{\"x\":0.6629,\"y\":0.239045,\"z\":0.9814636},\"trackerOffset\":{\"position\":{\"x\":0.0,\"y\":0.156449,\"z\":0.01165},\"rotation\":{\"x\":0.0,\"y\":0.0,\"z\":0.0}},\"pivot\":{\"position\":{\"x\":0.0,\"y\":0.3499355,\"z\":0.0},\"rotation\":{\"x\":0.0,\"y\":0.0,\"z\":180.0}},\"grips\":[],\"propType\":0}}],\"trackers\":[{\"name\":\"2\",\"connectionId\":\"H23\",\"position\":{\"x\":-0.865737,\"y\":1.471664,\"z\":-1.65637984},\"rotation\":{\"x\":0.65916397,\"y\":-0.088037,\"z\":-0.7399105,\"w\":-0.11258},\"isLive\":false,\"trackingResult\":200,\"trackerType\":1,\"RenderModelName\":\"\",\"battery\":0.5931},{\"name\":\"3\",\"connectionId\":\"764cde8d-27d9-4a3a-ac60-e53af5ddfaac\",\"position\":{\"x\":0.663784485,\"y\":1.6378482,\"z\":-0.474449},\"rotation\":{\"x\":-0.3851843,\"y\":-0.6338438,\"z\":-0.551294,\"w\":0.39638153},\"isLive\":false,\"trackingResult\":200,\"trackerType\":1,\"RenderModelName\":\"\",\"battery\":0.592131},{\"name\":\"8\",\"connectionId\":\"3d39d613-db56-49e1-923a-15a43ead2c09\",\"position\":{\"x\":-0.104797,\"y\":0.464008,\"z\":-0.07374},\"rotation\":{\"x\":-0.8070222,\"y\":0.1646839,\"z\":0.3036444,\"w\":0.478498},\"isLive\":false,\"trackingResult\":101,\"trackerType\":1,\"RenderModelName\":\"\",\"battery\":0.939142}],\"faces\":[]}";

	FVirtualProductionFrame VPFrame;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonTest, &VPFrame, 0, 0);
}

FVector USmartsuitBlueprintLibrary::GetVectorField(TSharedPtr<FJsonObject> jsonObject)
{
	FVector ReturnVal;
	ReturnVal.X = jsonObject->GetNumberField(TEXT("x"));
	ReturnVal.Y = jsonObject->GetNumberField(TEXT("y"));
	ReturnVal.Z = jsonObject->GetNumberField(TEXT("z"));
	return ReturnVal;
}

FColor USmartsuitBlueprintLibrary::GetColorField(TSharedPtr<FJsonObject> jsonObject)
{
	//TArray<TSharedPtr<FJsonValue>> ColorArray = jsonObject->GetArrayField("color");
	const TArray<TSharedPtr<FJsonValue>>* ColorArray = nullptr;
	
	FColor Color;
	if(jsonObject->TryGetArrayField(TEXT("color"), ColorArray))
	{
		Color.R = (*ColorArray)[0]->AsNumber();
		Color.G = (*ColorArray)[1]->AsNumber();
		Color.B = (*ColorArray)[2]->AsNumber();
	}

	return Color;
}

FLinearColor USmartsuitBlueprintLibrary::GetFLinearColorField(TSharedPtr<FJsonObject> jsonObject)
{
	FLinearColor LinearColor;
	LinearColor.R = jsonObject->GetNumberField(TEXT("x"));
	LinearColor.G = jsonObject->GetNumberField(TEXT("y"));
	LinearColor.B = jsonObject->GetNumberField(TEXT("z"));

	//this is so we can properly convert the color to srgb
	FColor NewColor = LinearColor.QuantizeRound();
	
	return FLinearColor(NewColor);
}

FQuat USmartsuitBlueprintLibrary::GetQuaternionField(TSharedPtr<FJsonObject> jsonObject)
{
	FQuat ReturnVal;
	ReturnVal.X = jsonObject->GetNumberField(TEXT("x"));
	ReturnVal.Y = jsonObject->GetNumberField(TEXT("y"));
	ReturnVal.Z = jsonObject->GetNumberField(TEXT("z"));
	ReturnVal.W = jsonObject->GetNumberField(TEXT("w"));
	return ReturnVal;
}

void USmartsuitBlueprintLibrary::CreateVirtualProductionSource()
{
	FVirtualProductionSource::CreateLiveLinkSource();
}

// .h
 // static FTransform GetWorldSpaceTransform(FReferenceSkeleton RefSkel, int32 BoneIdx);
 // .cpp
FTransform USmartsuitBlueprintLibrary::GetWorldSpaceTransform(FReferenceSkeleton RefSkel, int32 BoneIdx)
{
	FTransform BoneTransform;

	if (BoneIdx > 0)
	{
		BoneTransform = RefSkel.GetRefBonePose()[BoneIdx];

		FMeshBoneInfo BoneInfo = RefSkel.GetRefBoneInfo()[BoneIdx];
		if (BoneInfo.ParentIndex != 0)
		{
			BoneTransform *= GetWorldSpaceTransform(RefSkel, BoneInfo.ParentIndex);
		}
	}

	return BoneTransform;
}

// .h
// UFUNCTION(BlueprintCallable, Category = "BPLibrary")
// static FTransform GetRefPoseBoneTransform(USkeletalMeshComponent* SkelMesh, FName BoneName);
// .cpp
FTransform USmartsuitBlueprintLibrary::GetRefPoseBoneTransform(USkeletalMeshComponent* SkelMesh, FName BoneName)
{
	FTransform BoneTransform;

	if (SkelMesh && !BoneName.IsNone())
	{
		SkelMesh->ClearRefPoseOverride();
		FReferenceSkeleton RefSkel;
		RefSkel = SkelMesh->GetSkeletalMeshAsset()->GetRefSkeleton();

		BoneTransform = GetWorldSpaceTransform(RefSkel, RefSkel.FindBoneIndex(BoneName));
	}

	return BoneTransform;
}

// .h
// UFUNCTION(BlueprintCallable, Category = "BPLibrary")
// static FTransform GetBoneTransform(USkeletalMeshComponent* SkelMesh, FName BoneName);
// .cpp
FTransform USmartsuitBlueprintLibrary::GetBoneTransform(USkeletalMeshComponent* SkelMesh, FName BoneName)
{
	FTransform BoneTransform;

	if (SkelMesh && !BoneName.IsNone())
	{
		FReferenceSkeleton RefSkel;
		RefSkel = SkelMesh->GetSkeletalMeshAsset()->GetRefSkeleton();
		BoneTransform = SkelMesh->GetBoneTransform(RefSkel.FindBoneIndex(BoneName));
	}

	return BoneTransform;
}

FVector USmartsuitBlueprintLibrary::GetPropLocation(FProp InProp)
{
	return InProp.UPosition();
}

FRotator USmartsuitBlueprintLibrary::GetPropRotation(FProp InProp)
{
	return InProp.FQuatToRotator();
}

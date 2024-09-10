// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionSource.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "Roles/LiveLinkCameraTypes.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkLightRole.h"
#include "Roles/LiveLinkLightTypes.h"
#include "Features/IModularFeatures.h"

#include "VirtualProductionFrame.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Serialization/BufferArchive.h"
#include "lz4frame.h"
#include "SmartsuitBlueprintLibrary.h"
#include "Dom/JsonObject.h"

TSharedPtr<FVirtualProductionSource> FVirtualProductionSource::instance = nullptr;

FVirtualProductionSource::FVirtualProductionSource(FIPv4Endpoint address, const FText& InSourceType, const FText& InSourceMachineName, const FMessageAddress& InConnectionAddress)
	: SourceType(InSourceType)
	, SourceMachineName(InSourceMachineName)
{
	Client = nullptr;
	
	m_NetworkAddress = address;
	
	UE_LOG(LogTemp, Warning, TEXT("Creating Virtual production source!!!"));

	if (InitSocket())
	{
		StartRunnable();
	}
}

FVirtualProductionSource::~FVirtualProductionSource()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroying Virtual production source!!!"));

	// Stop the runnable
	Stop();
	ClearAllSubjects();

	if (Socket)
	{
		Socket->Close();
	}

	// And last but not least stop the main thread
	if (Thread != nullptr)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

void FVirtualProductionSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
}

void FVirtualProductionSource::InitializeSettings(ULiveLinkSourceSettings* Settings)
{
	// Save our source settings pointer so we can use it directly
	SavedSourceSettings = Cast<UVirtualProductionSourceSettings>(Settings);
}

bool FVirtualProductionSource::IsSourceStillValid() const
{
	return Client != nullptr;
}

void FVirtualProductionSource::HandleClearSubject(const FName subjectName)
{
	//verify(Client != nullptr);

	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	Client->RemoveSubject_AnyThread(FLiveLinkSubjectKey(SourceGuid,subjectName));
}

void FVirtualProductionSource::ClearAllSubjects() 
{
	for (int i = 0; i < subjectNames.Num(); i++) 
	{
		HandleClearSubject(subjectNames[i]);
	}
	for (int i = 0; i < faceNames.Num(); i++) 
	{
		HandleClearSubject(faceNames[i]);
	}
	for (int i = 0; i < actorNames.Num(); i++) 
	{
		HandleClearSubject(actorNames[i]);
	}

	subjectNames.Empty();
	faceNames.Empty();
	actorNames.Empty();
}

bool FVirtualProductionSource::RequestSourceShutdown()
{
	ClearAllSubjects();
	instance = nullptr;
	return true;
}

void FVirtualProductionSource::HandleFaceData(const FFace& face) 
{
	//verify(Client != nullptr);

	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Creating a new face %s - %f"), *face.GetSubjectName().ToString(), face.jawOpen);
	faceNames.Add(face.GetSubjectName());

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, face.GetSubjectName());

	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();

	SkeletonData->PropertyNames.Append(
		{ 
			"browDownLeft",
			"browDownRight",
			"browInnerUp",
			"browOuterUpLeft",
			"browOuterUpRight",
			"cheekPuff",
			"cheekSquintLeft",
			"cheekSquintRight",
			"eyeBlinkLeft",
			"eyeBlinkRight",
			"eyeLookDownLeft",
			"eyeLookDownRight",
			"eyeLookInLeft",
			"eyeLookInRight",
			"eyeLookOutLeft",
			"eyeLookOutRight",
			"eyeLookUpLeft",
			"eyeLookUpRight",
			"eyeSquintLeft",
			"eyeSquintRight",
			"eyeWideLeft",
			"eyeWideRight",
			"jawOpen",
			"jawForward",
			"jawLeft",
			"jawRight",
			"mouthClose",
			"mouthDimpleLeft",
			"mouthDimpleRight",
			"mouthFrownLeft",
			"mouthFrownRight",
			"mouthFunnel",
			"mouthLeft",
			"mouthLowerDownLeft",
			"mouthLowerDownRight",
			"mouthPressLeft",
			"mouthPressRight",
			"mouthPucker",
			"mouthRight",
			"mouthRollLower",
			"mouthRollUpper",
			"mouthShrugLower",
			"mouthShrugUpper",
			"mouthSmileLeft",
			"mouthSmileRight",
			"mouthStretchLeft",
			"mouthStretchRight",
			"mouthUpperUpLeft",
			"mouthUpperUpRight",
			"noseSneerLeft",
			"noseSneerRight",
			"tongueOut"
		});

	Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
}

void FVirtualProductionSource::HandleSubjectData(const FVirtualProductionSubject& virtualProductionObject)
{
	//verify(Client != nullptr);

	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	subjectNames.Add(virtualProductionObject.Name);

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, virtualProductionObject.Name);
	Client->RemoveSubject_AnyThread(Key);

	if (virtualProductionObject.Name.ToString().StartsWith("prop"))
	{
		FString subjectname = virtualProductionObject.Name.ToString();
		int32 indexlastchar = -1;
		subjectname.FindLastChar(':', indexlastchar);
		FString testval = subjectname.RightChop(indexlastchar + 1);

		if (testval.StartsWith("Camera"))
		{
			FLiveLinkStaticDataStruct CameraData(FLiveLinkCameraStaticData::StaticStruct());
			FLiveLinkCameraStaticData& CameraStaticData = *CameraData.Cast<FLiveLinkCameraStaticData>();
			CameraStaticData.bIsFieldOfViewSupported = true;
			CameraStaticData.bIsAspectRatioSupported = true;

			Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkCameraRole::StaticClass(), MoveTemp(CameraData));
		}
		else
		if (testval.StartsWith("Light"))
		{
			FLiveLinkStaticDataStruct LightData(FLiveLinkLightStaticData::StaticStruct());
			FLiveLinkLightStaticData& LightStaticData = *LightData.Cast<FLiveLinkLightStaticData>();

			Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkLightRole::StaticClass(), MoveTemp(LightData));
		}
		else
		{
			TArray<FName> boneNames;
			boneNames.Add("Root");
			TArray<int32> boneParents;
			boneParents.Add(0);

			FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
			FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();
			SkeletonData->SetBoneNames(boneNames);
			SkeletonData->SetBoneParents(boneParents);
			Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
		}
	}
	else
	{
		TArray<FName> boneNames;
		boneNames.Add("Root");
		TArray<int32> boneParents;
		boneParents.Add(0);

		FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
		FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();
		SkeletonData->SetBoneNames(boneNames);
		SkeletonData->SetBoneParents(boneParents);
		Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
	}
	//UE_LOG(LogTemp, Warning, TEXT("SKELETON!! "), skeleton);
}
	
void FVirtualProductionSource::HandleSuitData(const FSuitData& suit) 
{
	actorNames.Add(suit.GetSubjectName());

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, suit.GetSubjectName());

	static TArray<FName> boneNames = 
	{ 
		"Base",
		"hip",
		"spine",
		"chest",
		"neck",
		"head",

		"leftShoulder",
		"leftUpperArm",
		"leftLowerArm",
		"leftHand",

		"rightShoulder",
		"rightUpperArm",
		"rightLowerArm",
		"rightHand",

		"leftUpLeg",
		"leftLeg",
		"leftFoot",

		"rightUpLeg",
		"rightLeg",
		"rightFoot",

		"leftThumbProximal",
		"leftThumbMedial",
		"leftThumbDistal",
		"leftThumbTip",

		"leftIndexProximal",
		"leftIndexMedial",
		"leftIndexDistal",
		"leftIndexTip",

		"leftMiddleProximal",
		"leftMiddleMedial",
		"leftMiddleDistal",
		"leftMiddleTip",

		"leftRingProximal",
		"leftRingMedial",
		"leftRingDistal",
		"leftRingTip",

		"leftLittleProximal",
		"leftLittleMedial",
		"leftLittleDistal",
		"leftLittleTip",

		"rightThumbProximal",
		"rightThumbMedial",
		"rightThumbDistal",
		"rightThumbTip",

		"rightIndexProximal",
		"rightIndexMedial",
		"rightIndexDistal",
		"rightIndexTip",

		"rightMiddleProximal",
		"rightMiddleMedial",
		"rightMiddleDistal",
		"rightMiddleTip",

		"rightRingProximal",
		"rightRingMedial",
		"rightRingDistal",
		"rightRingTip",

		"rightLittleProximal",
		"rightLittleMedial",
		"rightLittleDistal",
		"rightLittleTip",

		"leftToe",
		"rightToe",
	};
	

	static TArray<int32> boneParents =
	{
		0, //0 - root
		0, //1 - hip
		1, //2 - spine
		2, //3 - spine2
		3, //4 - neck
		4, //5 - head

		3, //6 - LeftShoulder
		6, //7 - LeftArm
		7, //8 - LeftForearm
		8, //9 - LeftHand

		3, //10 - RightShoulder
		10, //11 - RightArm
		11, //12 - RightForearm
		12, //13 - RightHand

		1, //14 - LeftUpLeg
		14, //15 - LeftLeg
		15, //16 - LeftFoot

		1, //17 - RightUpLeg
		17, //18 - RightLeg
		18, //19 - RightFoot

		9, //20 - leftThumbProximal
		20, //21 - leftThumbMedial
		21, //22 - leftThumbDistal
		22, //23 - leftThumbTip

		9, //24 - leftIndexProximal
		24, //25 - leftIndexMedial
		25, //26 - leftIndexDistal
		26, //27 - leftIndexTip

		9, //28 - leftMiddleProximal
		28, //29 - leftMiddleMedial
		29, //30 - leftMiddleDistal
		30, //31 - leftMiddleTip

		9, //32 - leftRingProximal
		32, //33 - leftRingMedial
		33, //34 - leftRingDistal
		34, //35 - leftRingTip

		9, //36 - leftLittleProximal
		36, //37 - leftLittleMedial
		37, //38 - leftLittleDistal
		38, //39 - leftLittleTip

		13, //40 - rightThumbProximal
		40, //41 - rightThumbMedial
		41, //42 - rightThumbDistal
		42, //43 - rightThumbTip

		13, //44 - rightIndexProximal
		44, //45 - rightIndexMedial
		45, //46 - rightIndexDistal
		46, //47 - rightIndexTip

		13, //48 - rightMiddleProximal
		48, //49 - rightMiddleMedial
		49, //50 - rightMiddleDistal
		50, //51 - rightMiddleTip

		13, //52 - rightRingProximal
		52, //53 - rightRingMedial
		53, //54 - rightRingDistal
		54, //55 - rightRingTip

		13, //56 - rightLittleProximal
		56, //57 - rightLittleMedial
		57, //58 - rightLittleDistal
		58, //59 - rightLittleTip

		16, //60 - LeftToe
		19 //61 - RightToe
	};
	

	//#ifdef USE_SMARTSUIT_ANIMATION_ROLE
	//FLiveLinkStaticDataStruct StaticData(FLiveLinkSmartsuitStaticData::StaticStruct());
	//FLiveLinkSmartsuitStaticData* SkeletonData = StaticData.Cast<FLiveLinkSmartsuitStaticData>();
	//#else
	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();
	//#endif


	SkeletonData->SetBoneNames(boneNames);
	SkeletonData->SetBoneParents(boneParents);

	if(Client)
	{
		//#ifdef USE_SMARTSUIT_ANIMATION_ROLE
		//	Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkSmartsuitRole::StaticClass(), MoveTemp(StaticData));
		//#else
			Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
		//#endif
	}
}


void FVirtualProductionSource::CreateJoint(TArray<FTransform>& transforms, int32 index, const FSmartsuitBone* parent, const FSmartsuitBone* sensor)
{

	int32 transformIndex = transforms.AddUninitialized(1);
	if (!sensor)
	{
		transforms[transformIndex].SetLocation(FVector::ZeroVector);
		transforms[transformIndex].SetRotation(FQuat::Identity);
		transforms[transformIndex].SetScale3D(FVector::OneVector);
	}
	else
	{
		FQuat modifier = FQuat::MakeFromEuler(FVector(0, 0, 180));
		transforms[transformIndex].SetLocation(sensor->UPosition());
		transforms[transformIndex].SetRotation(sensor->Uquaternion() /** modifier*/);
		transforms[transformIndex].SetScale3D(FVector::OneVector);
	}
}


void FVirtualProductionSource::HandleSuits(const TArray<FSuitData>& suits) 
{
	//UE_LOG(LogTemp, Warning, TEXT("Handling faces %d"), faces.Num());
	existingActors.Empty();
	notExistingSubjects.Empty();
	for (int subjectIndex = 0; subjectIndex < suits.Num(); subjectIndex++) 
	{
		const FSuitData& subject = suits[subjectIndex];

		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int suitNameIndex = 0; suitNameIndex < actorNames.Num(); suitNameIndex++) 
		{
			if (subject.GetSubjectName() == actorNames[suitNameIndex]) 
			{
				nameExists = true;
				existingActors.Add(subject);
				break;
			}
		}

		if (!nameExists) 
		{
			existingActors.Add(subject);
			HandleSuitData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		if (subjectIndex == suits.Num() - 1) 
		{
			for (int i = 0; i < actorNames.Num(); i++) 
			{
				bool subjectExists = false;
				for (int j = 0; j < existingActors.Num(); j++) 
				{
					if (actorNames[i] == existingActors[j].GetSubjectName()) 
					{
						subjectExists = true;
					}
				}
				if (!subjectExists) 
				{
					notExistingSubjects.Add(actorNames[i]);
				}
			}

			for (int i = 0; i < notExistingSubjects.Num(); i++) 
			{
				//UE_LOG(LogTemp, Warning, TEXT("Removing face"));
				Client->RemoveSubject_AnyThread(FLiveLinkSubjectKey(SourceGuid, notExistingSubjects[i]));
				actorNames.RemoveSingle(notExistingSubjects[i]);
				notExistingSubjects.RemoveAt(i);
			}
		}

		//#ifdef USE_SMARTSUIT_ANIMATION_ROLE
		//FLiveLinkFrameDataStruct FrameData1(FLiveLinkSmartsuitFrameData::StaticStruct());
		//FLiveLinkSmartsuitFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkSmartsuitFrameData>();
		//#else
		FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
		FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
		//#endif
		
		AnimFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);

		TArray<FTransform> transforms;
		transforms.Reset(62);
		int32 transformIndex = transforms.AddUninitialized(1);
		
		transforms[transformIndex].SetLocation(FVector::ZeroVector);
		transforms[transformIndex].SetRotation(FQuat::Identity);
		transforms[transformIndex].SetScale3D(FVector::OneVector);

		
		CreateJoint(transforms, 0, nullptr, subject.Hip());
		CreateJoint(transforms, -1, subject.Hip(), subject.GetBoneByName(SmartsuitBones::spine));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::spine), subject.GetBoneByName(SmartsuitBones::chest));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::chest), subject.GetBoneByName(SmartsuitBones::neck));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::neck), subject.GetBoneByName(SmartsuitBones::head));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::chest), subject.GetBoneByName(SmartsuitBones::leftShoulder));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftShoulder), subject.GetBoneByName(SmartsuitBones::leftUpperArm));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftUpperArm), subject.GetBoneByName(SmartsuitBones::leftLowerArm));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftLowerArm), subject.GetBoneByName(SmartsuitBones::leftHand));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::chest), subject.GetBoneByName(SmartsuitBones::rightShoulder));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightShoulder), subject.GetBoneByName(SmartsuitBones::rightUpperArm));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightUpperArm), subject.GetBoneByName(SmartsuitBones::rightLowerArm));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightLowerArm), subject.GetBoneByName(SmartsuitBones::rightHand));

		CreateJoint(transforms, -1, subject.GetBoneByName(SmartsuitBones::hip), subject.GetBoneByName(SmartsuitBones::leftUpLeg));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftUpLeg), subject.GetBoneByName(SmartsuitBones::leftLeg));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftLeg), subject.GetBoneByName(SmartsuitBones::leftFoot));

		CreateJoint(transforms, -1, subject.GetBoneByName(SmartsuitBones::hip), subject.GetBoneByName(SmartsuitBones::rightUpLeg));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightUpLeg), subject.GetBoneByName(SmartsuitBones::rightLeg));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightLeg), subject.GetBoneByName(SmartsuitBones::rightFoot));
		
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftHand), subject.GetBoneByName(SmartsuitBones::leftThumbProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftThumbProximal), subject.GetBoneByName(SmartsuitBones::leftThumbMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftThumbMedial), subject.GetBoneByName(SmartsuitBones::leftThumbDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftThumbDistal), subject.GetBoneByName(SmartsuitBones::leftThumbTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftHand), subject.GetBoneByName(SmartsuitBones::leftIndexProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftIndexProximal), subject.GetBoneByName(SmartsuitBones::leftIndexMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftIndexMedial), subject.GetBoneByName(SmartsuitBones::leftIndexDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftIndexDistal), subject.GetBoneByName(SmartsuitBones::leftIndexTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftHand), subject.GetBoneByName(SmartsuitBones::leftMiddleProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftMiddleProximal), subject.GetBoneByName(SmartsuitBones::leftMiddleMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftMiddleMedial), subject.GetBoneByName(SmartsuitBones::leftMiddleDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftMiddleDistal), subject.GetBoneByName(SmartsuitBones::leftMiddleTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftHand), subject.GetBoneByName(SmartsuitBones::leftRingProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftRingProximal), subject.GetBoneByName(SmartsuitBones::leftRingMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftRingMedial), subject.GetBoneByName(SmartsuitBones::leftRingDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftRingDistal), subject.GetBoneByName(SmartsuitBones::leftRingTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftHand), subject.GetBoneByName(SmartsuitBones::leftLittleProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftLittleProximal), subject.GetBoneByName(SmartsuitBones::leftLittleMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftLittleMedial), subject.GetBoneByName(SmartsuitBones::leftLittleDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftLittleDistal), subject.GetBoneByName(SmartsuitBones::leftLittleTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightHand), subject.GetBoneByName(SmartsuitBones::rightThumbProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightThumbProximal), subject.GetBoneByName(SmartsuitBones::rightThumbMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightThumbMedial), subject.GetBoneByName(SmartsuitBones::rightThumbDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightThumbDistal), subject.GetBoneByName(SmartsuitBones::rightThumbTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightHand), subject.GetBoneByName(SmartsuitBones::rightIndexProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightIndexProximal), subject.GetBoneByName(SmartsuitBones::rightIndexMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightIndexMedial), subject.GetBoneByName(SmartsuitBones::rightIndexDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightIndexDistal), subject.GetBoneByName(SmartsuitBones::rightIndexTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightHand), subject.GetBoneByName(SmartsuitBones::rightMiddleProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightMiddleProximal), subject.GetBoneByName(SmartsuitBones::rightMiddleMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightMiddleMedial), subject.GetBoneByName(SmartsuitBones::rightMiddleDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightMiddleDistal), subject.GetBoneByName(SmartsuitBones::rightMiddleTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightHand), subject.GetBoneByName(SmartsuitBones::rightRingProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightRingProximal), subject.GetBoneByName(SmartsuitBones::rightRingMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightRingMedial), subject.GetBoneByName(SmartsuitBones::rightRingDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightRingDistal), subject.GetBoneByName(SmartsuitBones::rightRingTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightHand), subject.GetBoneByName(SmartsuitBones::rightLittleProximal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightLittleProximal), subject.GetBoneByName(SmartsuitBones::rightLittleMedial));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightLittleMedial), subject.GetBoneByName(SmartsuitBones::rightLittleDistal));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightLittleDistal), subject.GetBoneByName(SmartsuitBones::rightLittleTip));

		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::leftFoot), subject.GetBoneByName(SmartsuitBones::leftToe));
		CreateJoint(transforms, 0, subject.GetBoneByName(SmartsuitBones::rightFoot), subject.GetBoneByName(SmartsuitBones::rightToe));

		AnimFrameData.Transforms.Append(transforms);

		//#ifdef USE_SMARTSUIT_ANIMATION_ROLE
		//AnimFrameData.HasLeftGlove = subject.hasLeftGlove;
		//AnimFrameData.HasRightGlove = subject.hasRightGlove;
		//#endif

		if(Client)
			Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.GetSubjectName()), MoveTemp(FrameData1));
	}
}

void FVirtualProductionSource::HandleCharacters(const TArray<FCharacterData>& characters)
{
	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	existingCharacters.Empty();
	notExistingSubjects.Empty();

	for (int subjectIndex = 0; subjectIndex < characters.Num(); subjectIndex++)
	{
		const FCharacterData& subject = characters[subjectIndex];

		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int characterNameIndex = 0; characterNameIndex < characterNames.Num(); characterNameIndex++) 
		{
			if (subject.GetSubjectName() == characterNames[characterNameIndex]) 
			{
				nameExists = true;
				existingCharacters.Add(subject);
				break;
			}
		}

		if (!nameExists) 
		{
			existingCharacters.Add(subject);
			HandleCharacterData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		if (subjectIndex == characters.Num() - 1) 
		{
			for (int i = 0; i < characterNames.Num(); i++)
			{
				bool subjectExists = false;
				for (int j = 0; j < existingCharacters.Num(); j++) 
				{
					if (characterNames[i] == existingCharacters[j].GetSubjectName()) 
					{
						subjectExists = true;
					}
				}
				if (!subjectExists) 
				{
					notExistingSubjects.Add(characterNames[i]);
				}
			}

			for (int i = 0; i < notExistingSubjects.Num(); i++) 
			{
				//UE_LOG(LogTemp, Warning, TEXT("Removing face"));
				Client->RemoveSubject_AnyThread(FLiveLinkSubjectKey(SourceGuid, notExistingSubjects[i]));
				characterNames.RemoveSingle(notExistingSubjects[i]);
				notExistingSubjects.RemoveAt(i);
			}
		}
		
		FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
		FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
		
		AnimFrameData.WorldTime = FLiveLinkWorldTime();

		TArray<FTransform> transforms;
		transforms.Reset(subject.joints.Num());
		FTransform tm;

		for(int x = 0; x < subject.joints.Num(); x++)
		{
			const int32 transformIndex = transforms.AddUninitialized(1);
			const int parentIndex = subject.joints[x].parentIndex;

			if (parentIndex >= 0)
			{
				tm = subject.joints[x].transform.GetRelativeTransform(subject.joints[parentIndex].transform);
			}
			else
			{
				tm = subject.joints[x].transform;
			}
			
			const FVector JointPosition = tm.GetLocation();
			const FQuat JointRotation = tm.GetRotation();

			FQuat preRotation = FQuat::MakeFromRotator(SavedSourceSettings->HipPreRotation);
			
			FVector AdjustedJointPosition;
			FQuat qu;

			if (SavedSourceSettings != nullptr && SavedSourceSettings->bUseRotationOrderZYX)
			{
				//convert meters to centimeters since values coming from unity are in meters
				constexpr double WORLD_SCALE = 100.0;
				AdjustedJointPosition = FVector(-JointPosition.X * WORLD_SCALE, -JointPosition.Y * WORLD_SCALE, JointPosition.Z * WORLD_SCALE);

				// Quaternions - Convert Rotations from Studio to UE
				const FVector jointRotationEuler = JointRotation.Euler();
				const FQuat qx(FVector::UnitX(), FMath::DegreesToRadians(jointRotationEuler.X));
				const FQuat qz(FVector::UnitZ(), FMath::DegreesToRadians(jointRotationEuler.Z));
				const FQuat qy(FVector::UnitY(), FMath::DegreesToRadians(jointRotationEuler.Y));

				// Change Rotation Order - ZYX
				qu = qz * qy * qx;
			}
			else
			{
				//convert meters to centimeters since values coming from unity are in meters
				constexpr double WORLD_SCALE = 100.0;
				AdjustedJointPosition = FVector(-JointPosition.X * WORLD_SCALE, JointPosition.Z * WORLD_SCALE, JointPosition.Y * WORLD_SCALE);

				// Quaternions - Convert Rotations from Studio to UE
				const FVector jointRotationEuler = JointRotation.Euler();
				const FQuat qx(FVector::UnitX(), FMath::DegreesToRadians(jointRotationEuler.X));
				const FQuat qy(FVector::UnitY(), FMath::DegreesToRadians(jointRotationEuler.Z));
				const FQuat qz(FVector::UnitZ(), -FMath::DegreesToRadians(jointRotationEuler.Y));

				// Change Rotation Order - YZX
				qu = qy * qz * qx;
			}
			
			if (parentIndex < 0 && !preRotation.IsIdentity())
			{
				AdjustedJointPosition = preRotation * AdjustedJointPosition;
				qu = preRotation * qu;
			}

			transforms[transformIndex].SetComponents(qu, AdjustedJointPosition, FVector::One());
		}
		
		AnimFrameData.Transforms.Append(transforms);
		
		if(Client)
		{
			Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.GetSubjectName()), MoveTemp(FrameData1));
		}
	}
}

void FVirtualProductionSource::HandleNewtons(const TArray<FNewtonData>& newtons)
{
	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	existingNewtons.Empty();
	notExistingSubjects.Empty();

	if (newtons.Num() == 0) return;

	for (int subjectIndex = 0; subjectIndex < newtons.Num(); subjectIndex++)
	{
		const FNewtonData& subject = newtons[subjectIndex];

		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int idx = 0; idx < newtonNames.Num(); idx++)
		{
			if (subject.GetSubjectName() == newtonNames[idx])
			{
				nameExists = true;
				existingNewtons.Add(subject);
				break;
			}
		}

		if (!nameExists)
		{
			existingNewtons.Add(subject);
			HandleNewtonData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		if (subjectIndex == newtons.Num() - 1)
		{
			for (int i = 0; i < newtonNames.Num(); i++)
			{
				bool subjectExists = false;
				for (int j = 0; j < existingNewtons.Num(); j++)
				{
					if (newtonNames[i] == existingNewtons[j].GetSubjectName())
					{
						subjectExists = true;
					}
				}
				if (!subjectExists)
				{
					notExistingSubjects.Add(newtonNames[i]);
				}
			}

			for (int i = 0; i < notExistingSubjects.Num(); i++)
			{
				Client->RemoveSubject_AnyThread(FLiveLinkSubjectKey(SourceGuid, notExistingSubjects[i]));
				newtonNames.RemoveSingle(notExistingSubjects[i]);
				notExistingSubjects.RemoveAt(i);
			}
		}

		FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
		FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();

		AnimFrameData.WorldTime = FLiveLinkWorldTime();

		TArray<FTransform> transforms;
		transforms.Reset(subject.Joints.Num());
		FTransform tm;

		for (int x = 0; x < subject.Joints.Num(); x++)
		{
			const int32 transformIndex = transforms.AddUninitialized(1);
			const int parentIndex = subject.Joints[x].parentIndex;

			if (parentIndex >= 0)
			{
				tm = subject.Joints[x].transform.GetRelativeTransform(subject.Joints[parentIndex].transform);
			}
			else
			{
				tm = subject.Joints[x].transform;
			}

			const FVector JointPosition = tm.GetLocation();
			const FQuat JointRotation = tm.GetRotation();

			FVector AdjustedJointPosition;
			FQuat qu;

			//convert meters to centimeters since values coming from unity are in meters
			constexpr double WORLD_SCALE = 100.0;
			AdjustedJointPosition = FVector(-JointPosition.X * WORLD_SCALE, -JointPosition.Y * WORLD_SCALE, JointPosition.Z * WORLD_SCALE);

			// Quaternions - Convert Rotations from Studio to UE
			const FVector jointRotationEuler = JointRotation.Euler();
			const FQuat qx(FVector::UnitX(), FMath::DegreesToRadians(jointRotationEuler.X));
			const FQuat qz(FVector::UnitZ(), FMath::DegreesToRadians(jointRotationEuler.Z));
			const FQuat qy(FVector::UnitY(), FMath::DegreesToRadians(jointRotationEuler.Y));

			// Change Rotation Order - ZYX
			qu = qz * qy * qx;

			if (parentIndex < 0)
			{
				static FQuat preRotation = FQuat::MakeFromEuler(FVector(90, 0, -90));
				AdjustedJointPosition = preRotation * AdjustedJointPosition;
				qu = preRotation * qu;
			}

			transforms[transformIndex].SetComponents(qu, AdjustedJointPosition, FVector::One());
		}

		AnimFrameData.Transforms.Append(transforms);

		if (Client)
		{
			Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.GetSubjectName()), MoveTemp(FrameData1));
		}
	}
}

void FVirtualProductionSource::HandleCharacterData(const FCharacterData& character) 
{
	characterNames.Add(character.GetSubjectName());

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, character.GetSubjectName());

	TArray<FName> boneNames;
	TArray<int32> boneParents;
	
	for(int x = 0; x < character.joints.Num(); x++)
	{
		if (SavedSourceSettings != nullptr && SavedSourceSettings->bTrimNamespaces)
		{
			const FString name = character.joints[x].name.ToString();
			int32 index = 0;
			if (name.FindLastChar(':', index))
			{
				boneNames.Add(FName(name.Right(name.Len() - index - 1)));
			}
			else
			{
				boneNames.Add(character.joints[x].name);
			}
		}
		else
		{
			boneNames.Add(character.joints[x].name);
		}
		
		boneParents.Add(character.joints[x].parentIndex );
	}
	
	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();
	
	SkeletonData->SetBoneNames(boneNames);
	SkeletonData->SetBoneParents(boneParents);

	if(Client)
	{
		Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
	}
}

void FVirtualProductionSource::HandleNewtonData(const FNewtonData& newton)
{
	newtonNames.Add(newton.GetSubjectName());

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, newton.GetSubjectName());

	TArray<FName> boneNames;
	TArray<int32> boneParents;

	for (int x = 0; x < newton.Joints.Num(); x++)
	{
		boneNames.Add(newton.Joints[x].name);
		boneParents.Add(newton.Joints[x].parentIndex);
	}

	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();

	SkeletonData->SetBoneNames(boneNames);
	SkeletonData->SetBoneParents(boneParents);

	if (Client)
	{
		Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
	}
}

void FVirtualProductionSource::HandleFace(const TArray<FFace>& faces) 
{
	//verify(Client != nullptr);

	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Handling faces %d"), faces.Num());
	existingFaces.Empty();
	notExistingSubjects.Empty();
	for (int subjectIndex = 0; subjectIndex < faces.Num(); subjectIndex++) 
	{
		const FFace& subject = faces[subjectIndex];

		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int faceNameIndex = 0; faceNameIndex < faceNames.Num(); faceNameIndex++) 
		{
			if (subject.GetSubjectName() == faceNames[faceNameIndex]) 
			{
				nameExists = true;
				existingFaces.Add(subject);
				break;
			}
		}

		if (!nameExists) 
		{
			existingFaces.Add(subject);
			HandleFaceData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		if (subjectIndex == faces.Num() - 1) 
		{
			for (int i = 0; i < faceNames.Num(); i++) 
			{
				bool subjectExists = false;
				for (int j = 0; j < existingFaces.Num(); j++) 
				{
					if (faceNames[i] == existingFaces[j].GetSubjectName()) 
					{
						subjectExists = true;
					}
				}
				if (!subjectExists) 
				{
					notExistingSubjects.Add(faceNames[i]);
				}
			}

			for (int i = 0; i < notExistingSubjects.Num(); i++) 
			{
				//UE_LOG(LogTemp, Warning, TEXT("Removing face"));
				Client->RemoveSubject_AnyThread(FLiveLinkSubjectKey(SourceGuid, notExistingSubjects[i]));
				faceNames.RemoveSingle(notExistingSubjects[i]);
				notExistingSubjects.RemoveAt(i);
			}
		}

		//FTimer timer;
		FLiveLinkFrameDataStruct FrameData(FLiveLinkAnimationFrameData::StaticStruct());
		FLiveLinkAnimationFrameData& AnimFrameData = *FrameData.Cast<FLiveLinkAnimationFrameData>();
		AnimFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);

		AnimFrameData.PropertyValues.Append(
			{
				subject.browDownLeft,
				subject.browDownRight,
				subject.browInnerUp,
				subject.browOuterUpLeft,
				subject.browOuterUpRight,
				subject.cheekPuff,
				subject.cheekSquintLeft,
				subject.cheekSquintRight,
				subject.eyeBlinkLeft,
				subject.eyeBlinkRight,
				subject.eyeLookDownLeft,
				subject.eyeLookDownRight,
				subject.eyeLookInLeft,
				subject.eyeLookInRight,
				subject.eyeLookOutLeft,
				subject.eyeLookOutRight,
				subject.eyeLookUpLeft,
				subject.eyeLookUpRight,
				subject.eyeSquintLeft,
				subject.eyeSquintRight,
				subject.eyeWideLeft,
				subject.eyeWideRight,
				subject.jawOpen,
				subject.jawForward,
				subject.jawLeft,
				subject.jawRight,
				subject.mouthClose,
				subject.mouthDimpleLeft,
				subject.mouthDimpleRight,
				subject.mouthFrownLeft,
				subject.mouthFrownRight,
				subject.mouthFunnel,
				subject.mouthLeft,
				subject.mouthLowerDownLeft,
				subject.mouthLowerDownRight,
				subject.mouthPressLeft,
				subject.mouthPressRight,
				subject.mouthPucker,
				subject.mouthRight,
				subject.mouthRollLower,
				subject.mouthRollUpper,
				subject.mouthShrugLower,
				subject.mouthShrugUpper,
				subject.mouthSmileLeft,
				subject.mouthSmileRight,
				subject.mouthStretchLeft,
				subject.mouthStretchRight,
				subject.mouthUpperUpLeft,
				subject.mouthUpperUpRight,
				subject.noseSneerLeft,
				subject.noseSneerRight,
				subject.tongueOut
			});

		Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.GetSubjectName()), MoveTemp(FrameData));
	}
}


void FVirtualProductionSource::HandleSubjectFrame(const TArray<FVirtualProductionSubject>& FrameSubjects)
{
	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	existingSubjects.Empty();
	notExistingSubjects.Empty();

	for (int i = 0; i < subjectNames.Num(); i++) 
	{
		bool subjectExists = false;
		for (int j = 0; j < Subjects.Num(); j++) 
		{
			if (subjectNames[i] == Subjects[j].Name) 
			{
				subjectExists = true;
			}
		}
		if (!subjectExists) 
		{
			notExistingSubjects.Add(subjectNames[i]);
		}
	}

	for (int i = 0; i < notExistingSubjects.Num(); i++) 
	{
		Client->RemoveSubject_AnyThread(FLiveLinkSubjectKey(SourceGuid, notExistingSubjects[i]));
		subjectNames.RemoveSingle(notExistingSubjects[i]);
	}

	for (int subjectIndex = 0; subjectIndex < Subjects.Num(); subjectIndex++) 
	{
		FVirtualProductionSubject subject = Subjects[subjectIndex];
		
		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int subjectNameIndex = 0; subjectNameIndex < subjectNames.Num(); subjectNameIndex++) 
		{
			if (subject.Name == subjectNames[subjectNameIndex]) 
			{
				nameExists = true;
				existingSubjects.Add(subject);
				break;
			}
		}

		if (!nameExists) 
		{
			existingSubjects.Add(subject);
			HandleSubjectData(subject);
			existingSubjects.Add(subject);
		}


		FTransform hardCodedTransform;
		hardCodedTransform.SetTranslation(subject.Position);
		hardCodedTransform.SetRotation(subject.Rotation);
		hardCodedTransform.SetScale3D(FVector::OneVector);

		if (subject.Name.ToString().StartsWith("prop"))
		{
			FString subjectname = subject.Name.ToString();
			int32 indexlastchar = -1;
			subjectname.FindLastChar(':', indexlastchar);
			FString testval = subjectname.RightChop(indexlastchar + 1);

			if (testval.StartsWith("Camera"))
			{
				//FTimer timer;
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkCameraFrameData::StaticStruct());
				FLiveLinkCameraFrameData& CameraFrameData = *FrameData1.Cast<FLiveLinkCameraFrameData>();
				CameraFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);
				CameraFrameData.Transform = hardCodedTransform;
				CameraFrameData.AspectRatio = 1.11f;
				CameraFrameData.FieldOfView = 130.f;
				
				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.Name), MoveTemp(FrameData1));
			}
			else
			if (testval.StartsWith("light"))
			{
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkLightFrameData::StaticStruct());
				FLiveLinkLightFrameData& LightFrameData = *FrameData1.Cast<FLiveLinkLightFrameData>();
				LightFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);
				LightFrameData.Transform = hardCodedTransform;
			
				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.Name), MoveTemp(FrameData1));

			}
			else
			{
				//FTimer timer;
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
				FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
				AnimFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);
				AnimFrameData.Transforms.Add(hardCodedTransform);

				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.Name), MoveTemp(FrameData1));
			}
		}
		else
		//if (subject.name.ToString().StartsWith("tracker"))
		{
			//FTimer timer;
			FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
			FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
			AnimFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);
			AnimFrameData.Transforms.Add(hardCodedTransform);

			Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.Name), MoveTemp(FrameData1));
		}
	}
}

TSharedPtr<FVirtualProductionSource> FVirtualProductionSource::Get()
{ 
	if (!instance)
	{
		IModularFeatures& ModularFeatures = IModularFeatures::Get();

		if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
		{
			ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
			TArray<FGuid> Sources = LiveLinkClient.GetSources();
			for (FGuid SourceGuid : Sources)
			{
				if (LiveLinkClient.GetSourceType(SourceGuid).ToString() == "Studio")
				{
					UE_LOG(LogTemp, Warning, TEXT("Remove already existed LiveLink Source and replace with new one"));
					LiveLinkClient.RemoveSource(SourceGuid);
				}
			}

			FIPv4Address address(FIPv4Address::Any);

			FIPv4Endpoint Endpoint;
			Endpoint.Address = address;
			Endpoint.Port = 14043;

			TSharedPtr<FVirtualProductionSource> Source = MakeShared<FVirtualProductionSource>(Endpoint, FText::FromString("Studio"), FText::FromString(""), FMessageAddress::NewAddress());
			SetInstance(Source);
			LiveLinkClient.AddSource(Source);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[Rokoko] modular feature {0} is not available"), ILiveLinkClient::ModularFeatureName);
		}
	}

	return instance; 
}

TSharedPtr<FVirtualProductionSource> FVirtualProductionSource::CreateLiveLinkSource()
{
	IModularFeatures& ModularFeatures = IModularFeatures::Get();

	if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		bool bDoesAlreadyExist = false;
		{
			ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
			TArray<FGuid> Sources = LiveLinkClient.GetSources();
			for (FGuid SourceGuid : Sources)
			{
				if (LiveLinkClient.GetSourceType(SourceGuid).ToString() == "Studio")
				{
					//UE_LOG(LogTemp, Warning, TEXT("you can't add more than one instance of FVirtualProductionSource!!"));
					bDoesAlreadyExist = true;

					//LiveLinkClient.RemoveSource(SourceGuid);

					break;
				}
			}
		}

		if (!bDoesAlreadyExist)
		{
			// TODO: start source with a default streaming address and port
			FIPv4Address address(FIPv4Address::Any);

			FIPv4Endpoint Endpoint;
			Endpoint.Address = address;
			Endpoint.Port = 14043;
			
			ILiveLinkClient* LiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
			TSharedPtr<FVirtualProductionSource> Source = MakeShared<FVirtualProductionSource>(Endpoint, FText::FromString("Studio"), FText::FromString(""), FMessageAddress::NewAddress());
			FVirtualProductionSource::SetInstance(Source);
			LiveLinkClient->AddSource(Source);
			return Source;
		}
	}
	return TSharedPtr<FVirtualProductionSource>();
}

void FVirtualProductionSource::RemoveLiveLinkSource(TSharedPtr<FVirtualProductionSource> InSource)
{
	IModularFeatures& ModularFeatures = IModularFeatures::Get();

	if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ILiveLinkClient* LiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

		LiveLinkClient->RemoveSource(InSource);
	}
}

void FVirtualProductionSource::StartRunnable()
{
	FString ThreadName(FString::Printf(TEXT("VPStreamingNetwork%ld"), (long)(FDateTime::UtcNow().ToUnixTimestamp())));
	if (m_NetworkAddress.Port)
	{
		UE_LOG(LogTemp, Warning, TEXT("VP port... %i"), m_NetworkAddress.Port);
	}
	Thread = FRunnableThread::Create(this, *ThreadName, 512 * 1024, TPri_Normal);
}


bool FVirtualProductionSource::InitSocket()
{
	if (Socket == nullptr)
	{
		Running = false;
		Socket = FUdpSocketBuilder(TEXT("VPStreamingNetwork")).BoundToAddress(m_NetworkAddress.Address).BoundToPort(m_NetworkAddress.Port).AsReusable().Build();
		if (!Socket)
		{
			const FString addressStr = m_NetworkAddress.Address.ToString();
			UE_LOG(LogTemp, Error, TEXT("Failed to bind to address %s and port %i"), *addressStr, m_NetworkAddress.Port);
			return false;
		}
		
		//allow the socket to listen to an already bounded address.
		Socket->SetReuseAddr(true);
		Running = true;
	}
	return true;
}

FString BytesToStringFixed(const uint8* In, int32 Count)
{
	FString Text = BytesToString(In, Count);
	
	for (int i = 0; i < Text.Len(); i++)
	{
		const TCHAR c = Text[i] - 1;
		Text[i] = c;
	}

	return Text;
}

void SuitParseBone(FSuitData* suitData, TSharedPtr<FJsonObject> jsonObject, const FString& BoneName)
{
	if (jsonObject->HasField(BoneName))
	{
		TSharedPtr<FJsonObject> BoneObject = jsonObject->GetObjectField(BoneName);
		FVector SensorPosition = USmartsuitBlueprintLibrary::GetVectorField(BoneObject->GetObjectField(TEXT("position")));
		FQuat SensorRotation = USmartsuitBlueprintLibrary::GetQuaternionField(BoneObject->GetObjectField(TEXT("rotation")));

		suitData->bones.Add(FName(*BoneName), FSmartsuitBone(FName(*BoneName), SensorPosition, SensorRotation));
	}
}

void UpdateSuitFromJson(FSuitData* suitData, const TSharedPtr<FJsonObject> jsonObject)
{
	suitData->suitname = jsonObject->GetStringField(TEXT("name"));

	//temp
	suitData->profileName = suitData->suitname;

	//timestamp = jsonObject->GetNumberField("timestamp");
	//id = jsonObject->GetStringField("id");
	//isLive = jsonObject->GetBoolField("isLive");
	//isLive = InIsLive;
	//profileName = jsonObject->GetStringField("profileName");
	//color = USmartsuitBlueprintLibrary::GetFLinearColorField(jsonObject->GetObjectField("color"));
	//TArray<TSharedPtr<FJsonValue>> ColorArray = jsonObject->GetArrayField("color");
	suitData->color = USmartsuitBlueprintLibrary::GetColorField(jsonObject);

	TSharedPtr<FJsonObject> Meta = jsonObject->GetObjectField(TEXT("meta"));
	suitData->hasGloves = Meta->GetBoolField(TEXT("hasGloves"));
	suitData->hasLeftGlove = Meta->GetBoolField(TEXT("hasLeftGlove"));
	suitData->hasRightGlove = Meta->GetBoolField(TEXT("hasRightGlove"));
	suitData->hasBody = Meta->GetBoolField(TEXT("hasBody"));
	suitData->hasFace = Meta->GetBoolField(TEXT("hasFace"));

	TSharedPtr<FJsonObject> BodyObj = jsonObject->GetObjectField(TEXT("body"));

	SuitParseBone(suitData, BodyObj, SmartsuitBones::hip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::spine.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::chest.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::neck.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::head.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftShoulder.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftUpperArm.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftLowerArm.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftHand.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightShoulder.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightUpperArm.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightLowerArm.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightHand.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftUpLeg.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftLeg.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftFoot.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftToe.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftToeEnd.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightUpLeg.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightLeg.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightFoot.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightToe.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightToeEnd.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftThumbProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftThumbMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftThumbDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftThumbTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftIndexProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftIndexMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftIndexDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftIndexTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftMiddleProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftMiddleMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftMiddleDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftMiddleTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftRingProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftRingMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftRingDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftRingTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftLittleProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftLittleMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftLittleDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::leftLittleTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightThumbProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightThumbMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightThumbDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightThumbTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightIndexProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightIndexMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightIndexDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightIndexTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightMiddleProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightMiddleMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightMiddleDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightMiddleTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightRingProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightRingMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightRingDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightRingTip.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightLittleProximal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightLittleMedial.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightLittleDistal.ToString());
	SuitParseBone(suitData, BodyObj, SmartsuitBones::rightLittleTip.ToString());
}

	


void UpdateCharacterFromJson(FCharacterData* characterData, const TSharedPtr<FJsonObject> jsonObject)
{
	characterData->CharacterName = jsonObject->GetStringField(TEXT("name"));

	TArray<TSharedPtr<FJsonValue>> JointsArray = jsonObject->GetArrayField(TEXT("joints"));

	for (TArray< TSharedPtr< FJsonValue > >::TConstIterator JointsIter(JointsArray.CreateConstIterator()); JointsIter; ++JointsIter)
	{
		const TSharedPtr< FJsonValue >  JointEntry = *JointsIter;
		const TSharedPtr< FJsonObject > JoinJSONObject = JointEntry->AsObject();

		FString JointName = JoinJSONObject->GetStringField(TEXT("name"));
		int32 JointParentIndex = JoinJSONObject->GetIntegerField(TEXT("parent"));
		FVector JointPosition = USmartsuitBlueprintLibrary::GetVectorField(JoinJSONObject->GetObjectField(TEXT("position")));
		FQuat JointRotation = USmartsuitBlueprintLibrary::GetQuaternionField(JoinJSONObject->GetObjectField(TEXT("rotation")));

		FTransform JointTransform(JointRotation, JointPosition, FVector::OneVector);
		characterData->joints.Add(FRokokoCharacterJoint(*JointName, JointParentIndex, JointTransform));
	}
}

void UpdateNewtonsFromJson(FNewtonData* newtonData, const TSharedPtr<FJsonObject> jsonObject)
{
	if (jsonObject->HasField(TEXT("name")))
	{
		newtonData->NewtonName = jsonObject->GetStringField(TEXT("name"));
	}
	else
	{
		newtonData->NewtonName = "UnknownName";
	}

	if (jsonObject->HasField(TEXT("meta")))
	{
		TSharedPtr<FJsonObject> Meta = jsonObject->GetObjectField(TEXT("meta"));
		newtonData->HasFace = Meta->GetBoolField(TEXT("hasFace"));
	}

	if (jsonObject->HasField(TEXT("joints")))
	{
		TArray<TSharedPtr<FJsonValue>> JointsArray = jsonObject->GetArrayField(TEXT("joints"));

		for (const auto& JointElem : JointsArray)
		{
			const TSharedPtr< FJsonObject > JoinJSONObject = JointElem->AsObject();

			FRokokoCharacterJoint CharacterJoint;
			CharacterJoint.name = *JoinJSONObject->GetStringField(TEXT("name"));
			CharacterJoint.parentIndex = JoinJSONObject->GetIntegerField(TEXT("parent"));
			CharacterJoint.position = USmartsuitBlueprintLibrary::GetVectorField(JoinJSONObject->GetObjectField(TEXT("position")));
			CharacterJoint.rotation = USmartsuitBlueprintLibrary::GetQuaternionField(JoinJSONObject->GetObjectField(TEXT("rotation")));
			FTransform JointTransform(CharacterJoint.rotation, CharacterJoint.position, FVector::OneVector);
			CharacterJoint.transform = JointTransform;
			newtonData->Joints.Add(MoveTemp(CharacterJoint));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Newton skeleton contains no joints"));
	}
}


//PRAGMA_DISABLE_OPTIMIZATION
uint32 FVirtualProductionSource::Run()
{
	static LZ4F_decompressionContext_t g_dCtx;
	LZ4F_createDecompressionContext(&g_dCtx, LZ4F_VERSION);
	bool added = false;

	auto addr_in = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	int32 bytes_read = 0;
	FString ret;

	constexpr int32 PacketSize{ 32768 };
	constexpr int32 UncompressedSize{ 2097152 };

	TArray<uint8> PacketData;
	PacketData.Empty(PacketSize);
	PacketData.AddUninitialized(PacketSize);

	TArray<uint8> UncompressedData;
	UncompressedData.Empty(UncompressedSize);
	UncompressedData.AddUninitialized(UncompressedSize);

	while (Running)
	{
		FDateTime time = FDateTime::UtcNow();
		int seconds = time.ToUnixTimestamp();

		if (!Socket)
			break;

		if (!Socket->RecvFrom(PacketData.GetData(), PacketData.Num() * sizeof(uint8), bytes_read, *addr_in)
			|| bytes_read == 0)
		{
			continue;
		}

		if (!Running) 
			break;
				
		FVirtualProductionFrame VPFrame;
		
		size_t srcSize = static_cast<size_t>(bytes_read);
		size_t dstSize = static_cast<size_t>(UncompressedData.Num());
		size_t decompressResult;
		verify(srcSize >= 0);
		verify(dstSize >= 0);
		decompressResult = LZ4F_decompress(g_dCtx, UncompressedData.GetData(), &dstSize, PacketData.GetData(), &srcSize, nullptr);
		if (decompressResult != 0) 
		{ 
			UE_LOG(LogTemp, Error, TEXT("Error decompressing frame : unfinished frame \n")); 
			continue;
		}
		if (srcSize != (size_t)bytes_read) 
		{ 
			UE_LOG(LogTemp, Error, TEXT("Error decompressing frame : read size incorrect \n")); 
			continue;
		}

		FString result = BytesToStringFixed(UncompressedData.GetData(), static_cast<int32_t>(dstSize));
				
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(result);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{

			VPFrame.Version = JsonObject->GetStringField(TEXT("version"));
			
			//SCENE
			{
				TSharedPtr<FJsonObject> SceneObj = JsonObject-> GetObjectField(TEXT("scene"));
				if (SceneObj->HasField(TEXT("props")))
				{
					TArray<TSharedPtr<FJsonValue>> Livepropsarray = SceneObj->GetArrayField(TEXT("props"));

					for (auto& currentprop : Livepropsarray)
					{
						auto NewProp = FProp(true, currentprop->AsObject());
						VPFrame.Props.Emplace(MoveTemp(NewProp));
					}
				}

				if (SceneObj->HasField(TEXT("actors")))
				{

					TArray<TSharedPtr<FJsonValue>> LivesuitsArray = SceneObj->GetArrayField(TEXT("actors"));
					for (auto& currentsuit : LivesuitsArray)
					{
						FSuitData SuitData; // = FSuitData(true, currentsuit->AsObject());
						SuitData.isLive = true;
						UpdateSuitFromJson(&SuitData, currentsuit->AsObject());

						if (SuitData.hasFace)
						{
							auto JSONObjectface = currentsuit->AsObject()->GetObjectField(TEXT("face"));
							auto FaceData = FFace(JSONObjectface, SuitData.suitname);
							SuitData.faceId = FaceData.faceId;
							VPFrame.Faces.Emplace(MoveTemp(FaceData));
						}
						VPFrame.Actors.Emplace(MoveTemp(SuitData));
					}
				}

				if (SceneObj->HasField(TEXT("newtons")))
				{
					TArray<TSharedPtr<FJsonValue>> NewtonsArray = SceneObj->GetArrayField(TEXT("newtons"));
					for (auto& currentNewton : NewtonsArray)
					{
						FNewtonData NewtonData;
						NewtonData.IsLive = true;
						UpdateNewtonsFromJson(&NewtonData, currentNewton->AsObject());

						if (NewtonData.HasFace)
						{
							auto JSONObjectface = currentNewton->AsObject()->GetObjectField(TEXT("face"));
							auto FaceData = FFace(JSONObjectface, NewtonData.NewtonName);
							VPFrame.Faces.Emplace(MoveTemp(FaceData));
						}
						VPFrame.Newtons.Emplace(MoveTemp(NewtonData));
					}
				}

				if (SceneObj->HasField(TEXT("characters")))
				{
					TArray<TSharedPtr<FJsonValue>> CharactersArray = SceneObj->GetArrayField(TEXT("characters"));
					for (auto& currentcharacter : CharactersArray)
					{
						FCharacterData CharacterData;
						CharacterData.IsLive = true;
						UpdateCharacterFromJson(&CharacterData, currentcharacter->AsObject());
						
						VPFrame.Characters.Emplace(MoveTemp(CharacterData));
					}
				}
			}
		}

		mtx.lock();

		GlobalVPFrame.Version = VPFrame.Version;
		GlobalVPFrame.Props = MoveTemp(VPFrame.Props);
		GlobalVPFrame.Trackers = MoveTemp(VPFrame.Trackers);
		
		Subjects.Empty();
		for (int i = 0; i < VPFrame.Props.Num(); i++)
		{
			FVirtualProductionSubject subject = GlobalVPFrame.Props[i].GetSubject();
			Subjects.Add(subject);
		}
		for (int i = 0; i < VPFrame.Trackers.Num(); i++)
		{
			FVirtualProductionSubject subject = GlobalVPFrame.Trackers[i].GetSubject();
			Subjects.Add(subject);
		}
			
		GlobalVPFrame.Faces = MoveTemp(VPFrame.Faces);
		GlobalVPFrame.Actors = MoveTemp(VPFrame.Actors);
		GlobalVPFrame.Characters = MoveTemp(VPFrame.Characters);
		GlobalVPFrame.Newtons = MoveTemp(VPFrame.Newtons);
		
		mtx.unlock();

		HandleSubjectFrame(Subjects);
		HandleFace(GlobalVPFrame.Faces);
		HandleSuits(GlobalVPFrame.Actors);
		HandleCharacters(GlobalVPFrame.Characters);
		HandleNewtons(GlobalVPFrame.Newtons);
	}
	return 0;
}
//PRAGMA_ENABLE_OPTIMIZATION
FProp* FVirtualProductionSource::GetPropByName(FString name, bool isLive)
{
	FProp* result = nullptr;
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Props.Num(); i++)
		{
			if (name == GlobalVPFrame.Props[i].name && GlobalVPFrame.Props[i].isLive == isLive)
			{
				result = &GlobalVPFrame.Props[i];
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FProp> FVirtualProductionSource::GetAllProps()
{
	TArray<FProp> result;
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Props.Num(); i++)
		{
			result.Add(GlobalVPFrame.Props[i]);
		}
	}
	mtx.unlock();
	//UE_LOG(LogTemp, Display, TEXT("Yeeee3"));
	return result;
}

FTracker* FVirtualProductionSource::GetTrackerByName(FString name, bool isLive)
{
	FTracker* result = nullptr;
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Trackers.Num(); i++)
		{
			if (name == GlobalVPFrame.Trackers[i].name && GlobalVPFrame.Trackers[i].isLive == isLive)
			{
				result = &GlobalVPFrame.Trackers[i];
			}
		}
	}
	mtx.unlock();
	return result;
}

FTracker* FVirtualProductionSource::GetTrackerByConnectionID(const FString& name, bool isLive)
{
	FTracker* result = nullptr;
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Trackers.Num(); i++)
		{
			if (name == GlobalVPFrame.Trackers[i].connectionId /*&& GlobalVPFrame.trackers[i].isLive == isLive*/)
			{
				result = &GlobalVPFrame.Trackers[i];
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FTracker> FVirtualProductionSource::GetTrackersWithMatchingId(FString ConnectionId, bool isLive)
{
	TArray<FTracker> result;
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Trackers.Num(); i++)
		{
			if (ConnectionId == GlobalVPFrame.Trackers[i].connectionId /*&& GlobalVPFrame.trackers[i].isLive == isLive*/)
			{
				result.Add(GlobalVPFrame.Trackers[i]);
			}
		}
	}
	mtx.unlock();
	return result;
}

FSuitData* FVirtualProductionSource::GetSmartsuitByName(FString suitName)
{
	
	if (suitName.IsEmpty() || suitName.Compare(FString("")) == 0)
	{
		return nullptr;
	}

	FSuitData* result = nullptr;
	mtx.lock();
	{
		//should probably set the limit to the size of the suit array here?
		for (int i = 0; i < GlobalVPFrame.Actors.Num(); i++)
		{
			FString mySuitName(GlobalVPFrame.Actors[i].suitname);
			if (suitName.Compare(mySuitName) == 0 && !mySuitName.IsEmpty())
			{
				result = &(GlobalVPFrame.Actors[i]);
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FString> FVirtualProductionSource::GetAvailableSmartsuitNames()
{
	TArray<FString> result;
	//maybe we should set the limit to the size of the suits array
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Actors.Num(); i++)
		{
			if ((GlobalVPFrame.Actors[i].suitname != "\0\0\0\0") /*&& GlobalVPFrame.suits[i].fps > 0*/)
			{
				result.Add(FString(GlobalVPFrame.Actors[i].suitname));
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FSuitData> FVirtualProductionSource::GetAllSmartsuits()
{
	TArray<FSuitData> suits;
	//maybe we should set the limit to the size of the suits array
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Actors.Num(); i++)
		{
			if ((GlobalVPFrame.Actors[i].suitname != "\0\0\0\0") /*&& GlobalVPFrame.suits[i].fps > 0*/)
			{
				suits.Add(GlobalVPFrame.Actors[i]);
			}
		}
	}
	mtx.unlock();
	return suits;
}

FFace FVirtualProductionSource::GetFaceByFaceID(FString faceID)
{
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Faces.Num(); i++)
		{
			if (GlobalVPFrame.Faces[i].faceId == faceID)
			{
				return FFace(GlobalVPFrame.Faces[i]);
			}
		}
	}
	mtx.unlock();

	return FFace();
}

FFace* FVirtualProductionSource::GetFaceByProfileName(const FString& profileName)
{
	FFace* result = nullptr;
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Faces.Num(); i++)
		{
			if (GlobalVPFrame.Faces[i].profileName == profileName)
			{
				result = &GlobalVPFrame.Faces[i];
				break;
			}
		}
	}
	mtx.unlock();

	return result;
}

TArray<FFace> FVirtualProductionSource::GetAllFaces()
{
	TArray<FFace> result;
	mtx.lock();
	{
		for (int i = 0; i < GlobalVPFrame.Faces.Num(); i++)
		{
			result.Add(GlobalVPFrame.Faces[i]);
		}
	}
	mtx.unlock();

	return result;
}


void FVirtualProductionSource::OnSettingsChanged(ULiveLinkSourceSettings* Settings, const FPropertyChangedEvent& PropertyChangedEvent)
{
	ILiveLinkSource::OnSettingsChanged(Settings, PropertyChangedEvent);

	FProperty* MemberProperty = PropertyChangedEvent.MemberProperty;
	FProperty* Property = PropertyChangedEvent.Property;
	if (Property && MemberProperty && (PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive))
	{
		UVirtualProductionSourceSettings* SourceSettings = Cast<UVirtualProductionSourceSettings>(Settings);
		if (SavedSourceSettings != SourceSettings)
		{
			UE_LOG(LogTemp, Error, TEXT("FVirtualProductionSource: OnSettingsChanged pointers don't match - this should never happen!"));
			return;
		}

		existingCharacters.Empty();
		characterNames.Empty();
	}
}

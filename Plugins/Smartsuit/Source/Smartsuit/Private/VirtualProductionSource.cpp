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
#include "Smartsuit.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
//#include "Engine.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Serialization/BufferArchive.h"
#include "lz4frame.h"
#include "SmartsuitReceiver.h"

TSharedPtr<FVirtualProductionSource> FVirtualProductionSource::instance = nullptr;

FVirtualProductionSource::FVirtualProductionSource(const FText& InSourceType, const FText& InSourceMachineName, const FMessageAddress& InConnectionAddress)
	: SourceType(InSourceType)
	, SourceMachineName(InSourceMachineName)
{
	Client = nullptr;

	int32 RokokoPort;
	for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	{
		RokokoPort = It->RokokoPortNumber;
		UE_LOG(LogTemp, Warning, TEXT("setting port to... %i"), RokokoPort);
	}

	InitSocket(RokokoPort);
	StartRunnable(RokokoPort);
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
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

void FVirtualProductionSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	
	SourceGuid = InSourceGuid;
	//instance = this;
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

	//Client->ClearSubject(subjectName);
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
	for (int i = 0; i < suitNames.Num(); i++) 
	{
		HandleClearSubject(suitNames[i]);
	}

	subjectNames.Empty();
	faceNames.Empty();
	suitNames.Empty();
}

bool FVirtualProductionSource::RequestSourceShutdown()
{
	ClearAllSubjects();
	instance = nullptr;
	return true;
}

void FVirtualProductionSource::HandleFaceData(FFace face) 
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

	
	SkeletonData->PropertyNames.Add("browDownLeft");
	SkeletonData->PropertyNames.Add("browDownRight");
	SkeletonData->PropertyNames.Add("browInnerUp");
	SkeletonData->PropertyNames.Add("browOuterUpLeft");
	SkeletonData->PropertyNames.Add("browOuterUpRight");
	SkeletonData->PropertyNames.Add("cheekPuff");
	SkeletonData->PropertyNames.Add("cheekSquintLeft");
	SkeletonData->PropertyNames.Add("cheekSquintRight");
	SkeletonData->PropertyNames.Add("eyeBlinkLeft");
	SkeletonData->PropertyNames.Add("eyeBlinkRight");
	SkeletonData->PropertyNames.Add("eyeLookDownLeft");
	SkeletonData->PropertyNames.Add("eyeLookDownRight");
	SkeletonData->PropertyNames.Add("eyeLookInLeft");
	SkeletonData->PropertyNames.Add("eyeLookInRight");
	SkeletonData->PropertyNames.Add("eyeLookOutLeft");
	SkeletonData->PropertyNames.Add("eyeLookOutRight");
	SkeletonData->PropertyNames.Add("eyeLookUpLeft");
	SkeletonData->PropertyNames.Add("eyeLookUpRight");
	SkeletonData->PropertyNames.Add("eyeSquintLeft");
	SkeletonData->PropertyNames.Add("eyeSquintRight");
	SkeletonData->PropertyNames.Add("eyeWideLeft");
	SkeletonData->PropertyNames.Add("eyeWideRight");
	SkeletonData->PropertyNames.Add("jawOpen");
	SkeletonData->PropertyNames.Add("jawForward");
	SkeletonData->PropertyNames.Add("jawLeft");
	SkeletonData->PropertyNames.Add("jawRight");
	SkeletonData->PropertyNames.Add("mouthClose");
	SkeletonData->PropertyNames.Add("mouthDimpleLeft");
	SkeletonData->PropertyNames.Add("mouthDimpleRight");
	SkeletonData->PropertyNames.Add("mouthFrownLeft");
	SkeletonData->PropertyNames.Add("mouthFrownRight");
	SkeletonData->PropertyNames.Add("mouthFunnel");
	SkeletonData->PropertyNames.Add("mouthLeft");
	SkeletonData->PropertyNames.Add("mouthLowerDownLeft");
	SkeletonData->PropertyNames.Add("mouthLowerDownRight");
	SkeletonData->PropertyNames.Add("mouthPressLeft");
	SkeletonData->PropertyNames.Add("mouthPressRight");
	SkeletonData->PropertyNames.Add("mouthPucker");
	SkeletonData->PropertyNames.Add("mouthRight");
	SkeletonData->PropertyNames.Add("mouthRollLower");
	SkeletonData->PropertyNames.Add("mouthRollUpper");
	SkeletonData->PropertyNames.Add("mouthShrugLower");
	SkeletonData->PropertyNames.Add("mouthShrugUpper");
	SkeletonData->PropertyNames.Add("mouthSmileLeft");
	SkeletonData->PropertyNames.Add("mouthSmileRight");
	SkeletonData->PropertyNames.Add("mouthStretchLeft");
	SkeletonData->PropertyNames.Add("mouthStretchRight");
	SkeletonData->PropertyNames.Add("mouthUpperUpLeft");
	SkeletonData->PropertyNames.Add("mouthUpperUpRight");
	SkeletonData->PropertyNames.Add("noseSneerLeft");
	SkeletonData->PropertyNames.Add("noseSneerRight");
	SkeletonData->PropertyNames.Add("tongueOut");

	Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
}

void FVirtualProductionSource::HandleSubjectData(FVirtualProductionSubject virtualProductionObject)
{
	//verify(Client != nullptr);

	if (Client == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client was null!!!!!!"));
		return;
	}

	subjectNames.Add(virtualProductionObject.name);

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, virtualProductionObject.name);
	Client->RemoveSubject_AnyThread(Key);

	if (virtualProductionObject.name.ToString().StartsWith("prop"))
	{
		FString subjectname = virtualProductionObject.name.ToString();
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
	
void FVirtualProductionSource::HandleSuitData(FSuitData suit) 
{
	suitNames.Add(suit.GetSubjectName());

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, suit.GetSubjectName());

	TArray<FName> boneNames;
	boneNames.Add("Base");
	boneNames.Add("hip");
	boneNames.Add("spine");
	boneNames.Add("chest");
	boneNames.Add("neck");
	boneNames.Add("head");

	boneNames.Add("leftShoulder");
	boneNames.Add("leftUpperArm");
	boneNames.Add("leftLowerArm");
	boneNames.Add("leftHand");
	
	boneNames.Add("rightShoulder");
	boneNames.Add("rightUpperArm");
	boneNames.Add("rightLowerArm");
	boneNames.Add("rightHand");

	boneNames.Add("leftUpLeg");
	boneNames.Add("leftLeg");
	boneNames.Add("leftFoot");

	boneNames.Add("rightUpLeg");
	boneNames.Add("rightLeg");
	boneNames.Add("rightFoot");

	boneNames.Add("leftThumbProximal");
	boneNames.Add("leftThumbMedial");
	boneNames.Add("leftThumbDistal");
	boneNames.Add("leftThumbTip");

	boneNames.Add("leftIndexProximal");
	boneNames.Add("leftIndexMedial");
	boneNames.Add("leftIndexDistal");
	boneNames.Add("leftIndexTip");

	boneNames.Add("leftMiddleProximal");
	boneNames.Add("leftMiddleMedial");
	boneNames.Add("leftMiddleDistal");
	boneNames.Add("leftMiddleTip");

	boneNames.Add("leftRingProximal");
	boneNames.Add("leftRingMedial");
	boneNames.Add("leftRingDistal");
	boneNames.Add("leftRingTip");

	boneNames.Add("leftLittleProximal");
	boneNames.Add("leftLittleMedial");
	boneNames.Add("leftLittleDistal");
	boneNames.Add("leftLittleTip");

	boneNames.Add("rightThumbProximal");
	boneNames.Add("rightThumbMedial");
	boneNames.Add("rightThumbDistal");
	boneNames.Add("rightThumbTip");

	boneNames.Add("rightIndexProximal");
	boneNames.Add("rightIndexMedial");
	boneNames.Add("rightIndexDistal");
	boneNames.Add("rightIndexTip");

	boneNames.Add("rightMiddleProximal");
	boneNames.Add("rightMiddleMedial");
	boneNames.Add("rightMiddleDistal");
	boneNames.Add("rightMiddleTip");

	boneNames.Add("rightRingProximal");
	boneNames.Add("rightRingMedial");
	boneNames.Add("rightRingDistal");
	boneNames.Add("rightRingTip");

	boneNames.Add("rightLittleProximal");
	boneNames.Add("rightLittleMedial");
	boneNames.Add("rightLittleDistal");
	boneNames.Add("rightLittleTip");


	TArray<int32> boneParents;
	boneParents.Add(0); //0 - root
	boneParents.Add(0); //1 - hip
	boneParents.Add(1); //2 - spine
	boneParents.Add(2); //3 - spine2
	boneParents.Add(3); //4 - neck
	boneParents.Add(4); //5 - head

	boneParents.Add(3); //6 - LeftShoulder
	boneParents.Add(6); //7 - LeftArm
	boneParents.Add(7); //8 - LeftForearm
	boneParents.Add(8); //9 - LeftHand

	boneParents.Add(3); //10 - RightShoulder
	boneParents.Add(10); //11 - RightArm
	boneParents.Add(11); //12 - RightForearm
	boneParents.Add(12); //13 - RightHand

	boneParents.Add(1); //14 - LeftUpLeg
	boneParents.Add(14); //15 - LeftLeg
	boneParents.Add(15); //16 - LeftFoot

	boneParents.Add(1); //17 - RightUpLeg
	boneParents.Add(17); //18 - RightLeg
	boneParents.Add(18); //19 - RightFoot

	boneParents.Add(9); //20 - leftThumbProximal
	boneParents.Add(20); //21 - leftThumbMedial
	boneParents.Add(21); //22 - leftThumbDistal
	boneParents.Add(22); //23 - leftThumbTip

	boneParents.Add(9); //24 - leftIndexProximal
	boneParents.Add(24); //25 - leftIndexMedial
	boneParents.Add(25); //26 - leftIndexDistal
	boneParents.Add(26); //27 - leftIndexTip

	boneParents.Add(9); //28 - leftMiddleProximal
	boneParents.Add(28); //29 - leftMiddleMedial
	boneParents.Add(29); //30 - leftMiddleDistal
	boneParents.Add(30); //31 - leftMiddleTip

	boneParents.Add(9); //32 - leftRingProximal
	boneParents.Add(32); //33 - leftRingMedial
	boneParents.Add(33); //34 - leftRingDistal
	boneParents.Add(34); //35 - leftRingTip

	boneParents.Add(9); //36 - leftLittleProximal
	boneParents.Add(36); //37 - leftLittleMedial
	boneParents.Add(37); //38 - leftLittleDistal
	boneParents.Add(38); //39 - leftLittleTip

	boneParents.Add(13); //40 - rightThumbProximal
	boneParents.Add(40); //41 - rightThumbMedial
	boneParents.Add(41); //42 - rightThumbDistal
	boneParents.Add(42); //43 - rightThumbTip

	boneParents.Add(13); //44 - rightIndexProximal
	boneParents.Add(44); //45 - rightIndexMedial
	boneParents.Add(45); //46 - rightIndexDistal
	boneParents.Add(46); //47 - rightIndexTip

	boneParents.Add(13); //48 - rightMiddleProximal
	boneParents.Add(48); //49 - rightMiddleMedial
	boneParents.Add(49); //50 - rightMiddleDistal
	boneParents.Add(50); //51 - rightMiddleTip

	boneParents.Add(13); //52 - rightRingProximal
	boneParents.Add(52); //53 - rightRingMedial
	boneParents.Add(53); //54 - rightRingDistal
	boneParents.Add(54); //55 - rightRingTip

	boneParents.Add(13); //56 - rightLittleProximal
	boneParents.Add(56); //57 - rightLittleMedial
	boneParents.Add(57); //58 - rightLittleDistal
	boneParents.Add(58); //59 - rightLittleTip

	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();
	SkeletonData->SetBoneNames(boneNames);
	SkeletonData->SetBoneParents(boneParents);

	if(Client)
		Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
}


void FVirtualProductionSource::CreateJoint(TArray<FTransform>& transforms, int32 index, FSmartsuitBone* parent, FSmartsuitBone* sensor)
{

	int32 transformIndex = transforms.AddUninitialized(1);
	if (!sensor)
	{
		transforms[transformIndex].SetLocation(FVector(0, 0, 0));
		transforms[transformIndex].SetRotation(FQuat::Identity);
		transforms[transformIndex].SetScale3D(FVector(1, 1, 1));
	}
	//else if (parent)
	//{
	//	FQuat parentRealRotation;
	//	if (index == -1)
	//	{
	//		parentRealRotation = parent->Uquaternion() * FQuat::MakeFromEuler(FVector(0, 0, 180));
	//	}
	//	else
	//	{
	//		parentRealRotation = parent->Uquaternion();
	//	}

	//	//FVector realSensorPosition;
	//	//float chestOffset = 20;
	//	//if (sensor->addr == SENSOR_NECK) {
	//	//	realSensorPosition
	//	//
	//	//} else if (sensor->addr == SENSOR_LEFT_SHOULDER || sensor->addr == SENSOR_RIGHT_SHOULDER) {
	//	//	FVector direction = sensor->UPosition();
	//	//	direction.Normalize();
	//	//	realSensorPosition = sensor->UPosition() + (direction * (-chestOffset));
	//	//}
	//	//else {
	//	//	realSensorPosition = sensor->UPosition();
	//	//}


	//	//FVector realParentPosition;
	//	//if (parent->addr == SENSOR_CHEST) {

	//	//}
	//	//else {
	//	//	realParentPosition = parent->UPosition();
	//	//}
	//	if (sensor->name == SmartsuitBones::neck)
	//	{
	//		transforms[transformIndex].SetLocation(FVector(0, 20.150345, 0));
	//	}
	//	else if (sensor->name == SmartsuitBones::rightShoulder)
	//	{
	//		transforms[transformIndex].SetLocation(FVector(7, 12.368073, 1.90378));
	//	}
	//	else if (sensor->name == SmartsuitBones::leftShoulder)
	//	{
	//		transforms[transformIndex].SetLocation(FVector(-7, 12.368073, 1.90378));
	//	}
	//	else
	//	{
	//		transforms[transformIndex].SetLocation(parentRealRotation.Inverse() * (sensor->UPosition() - parent->UPosition()));
	//	}
	//	transforms[transformIndex].SetRotation(parentRealRotation.Inverse() * sensor->Uquaternion());
	//	transforms[transformIndex].SetScale3D(FVector(1, 1, 1));
	//}
	else
	{
		FQuat modifier = FQuat::MakeFromEuler(FVector(0, 0, 180));
		transforms[transformIndex].SetLocation(sensor->UPosition());
		transforms[transformIndex].SetRotation(sensor->Uquaternion() /** modifier*/);
		transforms[transformIndex].SetScale3D(FVector(1, 1, 1));
	}
}


/*
void FVirtualProductionSource::CreateJoint(TArray<FTransform>& transforms, int32 index, Sensor* parent, Sensor* sensor) {
	
	int32 transformIndex = transforms.AddUninitialized(1);
	if (!sensor) 
	{
		transforms[transformIndex].SetLocation(FVector(0, 0, 0));
		transforms[transformIndex].SetRotation(FQuat::Identity);
		transforms[transformIndex].SetScale3D(FVector(1, 1, 1));
	} 
	else if (parent) 
	{
		FQuat parentRealRotation;
		if (index == -1) 
		{
			parentRealRotation = parent->Uquaternion() * FQuat::MakeFromEuler(FVector(0, 0, 180));
		}
		else 
		{
			parentRealRotation = parent->Uquaternion();
		}

		//FVector realSensorPosition;
		//float chestOffset = 20;
		//if (sensor->addr == SENSOR_NECK) {
		//	realSensorPosition 
		//	
		//} else if (sensor->addr == SENSOR_LEFT_SHOULDER || sensor->addr == SENSOR_RIGHT_SHOULDER) {
		//	FVector direction = sensor->UPosition();
		//	direction.Normalize();
		//	realSensorPosition = sensor->UPosition() + (direction * (-chestOffset));
		//}
		//else {
		//	realSensorPosition = sensor->UPosition();
		//}

		
		//FVector realParentPosition;
		//if (parent->addr == SENSOR_CHEST) {

		//}
		//else {
		//	realParentPosition = parent->UPosition();
		//}
		if (sensor->addr == SENSOR_NECK) 
		{
			transforms[transformIndex].SetLocation(FVector(0, 20.150345, 0));
		}
		else if (sensor->addr == SENSOR_RIGHT_SHOULDER) 
		{
			transforms[transformIndex].SetLocation(FVector(7, 12.368073, 1.90378));
		}
		else if (sensor->addr == SENSOR_LEFT_SHOULDER) 
		{
			transforms[transformIndex].SetLocation(FVector(-7, 12.368073, 1.90378));
		}
		else 
		{
			transforms[transformIndex].SetLocation(parentRealRotation.Inverse() * (sensor->UPosition() - parent->UPosition()));
		}
		transforms[transformIndex].SetRotation(parentRealRotation.Inverse() * sensor->Uquaternion());
		transforms[transformIndex].SetScale3D(FVector(1, 1, 1));
	}
	else 
	{
		FQuat modifier = FQuat::MakeFromEuler(FVector(0, 0, 180));
		transforms[transformIndex].SetLocation(sensor->UPosition());
		transforms[transformIndex].SetRotation(sensor->Uquaternion() * modifier);
		transforms[transformIndex].SetScale3D(FVector(1, 1, 1));
	}
}
*/
void FVirtualProductionSource::HandleSuits(TArray<FSuitData> suits) 
{
	//UE_LOG(LogTemp, Warning, TEXT("Handling faces %d"), faces.Num());
	existingSuits.Empty();
	notExistingSubjects.Empty();
	for (int subjectIndex = 0; subjectIndex < suits.Num(); subjectIndex++) 
	{
		FSuitData subject = suits[subjectIndex];

		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int suitNameIndex = 0; suitNameIndex < suitNames.Num(); suitNameIndex++) 
		{
			if (subject.GetSubjectName() == suitNames[suitNameIndex]) 
			{
				nameExists = true;
				existingSuits.Add(subject);
				break;
			}
		}

		if (!nameExists) 
		{
			existingSuits.Add(subject);
			HandleSuitData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		if (subjectIndex == suits.Num() - 1) 
		{
			for (int i = 0; i < suitNames.Num(); i++) 
			{
				bool subjectExists = false;
				for (int j = 0; j < existingSuits.Num(); j++) 
				{
					if (suitNames[i] == existingSuits[j].GetSubjectName()) 
					{
						subjectExists = true;
					}
				}
				if (!subjectExists) 
				{
					notExistingSubjects.Add(suitNames[i]);
				}
			}

			for (int i = 0; i < notExistingSubjects.Num(); i++) 
			{
				//UE_LOG(LogTemp, Warning, TEXT("Removing face"));
				Client->RemoveSubject_AnyThread(FLiveLinkSubjectKey(SourceGuid, notExistingSubjects[i]));
				suitNames.RemoveSingle(notExistingSubjects[i]);
				notExistingSubjects.RemoveAt(i);
			}
		}

		//FTimer timer;
		FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
		FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
		AnimFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);

		TArray<FTransform> transforms;
		transforms.Reset(60);
		int32 transformIndex = transforms.AddUninitialized(1);
		
		transforms[transformIndex].SetLocation(FVector(0, 0, 0));
		transforms[transformIndex].SetRotation(FQuat::Identity);
		transforms[transformIndex].SetScale3D(FVector(1, 1, 1));

		
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

		AnimFrameData.Transforms.Append(transforms);

		if(Client)
			Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.GetSubjectName()), MoveTemp(FrameData1));
	}
}

void FVirtualProductionSource::HandleFace(TArray<FFace> faces) 
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
		FFace subject = faces[subjectIndex];

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



		AnimFrameData.PropertyValues.Add(subject.browDownLeft);
		AnimFrameData.PropertyValues.Add(subject.browDownRight);
		AnimFrameData.PropertyValues.Add(subject.browInnerUp);
		AnimFrameData.PropertyValues.Add(subject.browOuterUpLeft);
		AnimFrameData.PropertyValues.Add(subject.browOuterUpRight);
		AnimFrameData.PropertyValues.Add(subject.cheekPuff);
		AnimFrameData.PropertyValues.Add(subject.cheekSquintLeft);
		AnimFrameData.PropertyValues.Add(subject.cheekSquintRight);
		AnimFrameData.PropertyValues.Add(subject.eyeBlinkLeft);
		AnimFrameData.PropertyValues.Add(subject.eyeBlinkRight);
		AnimFrameData.PropertyValues.Add(subject.eyeLookDownLeft);
		AnimFrameData.PropertyValues.Add(subject.eyeLookDownRight);
		AnimFrameData.PropertyValues.Add(subject.eyeLookInLeft);
		AnimFrameData.PropertyValues.Add(subject.eyeLookInRight);
		AnimFrameData.PropertyValues.Add(subject.eyeLookOutLeft);
		AnimFrameData.PropertyValues.Add(subject.eyeLookOutRight);
		AnimFrameData.PropertyValues.Add(subject.eyeLookUpLeft);
		AnimFrameData.PropertyValues.Add(subject.eyeLookUpRight);
		AnimFrameData.PropertyValues.Add(subject.eyeSquintLeft);
		AnimFrameData.PropertyValues.Add(subject.eyeSquintRight);
		AnimFrameData.PropertyValues.Add(subject.eyeWideLeft);
		AnimFrameData.PropertyValues.Add(subject.eyeWideRight);
		AnimFrameData.PropertyValues.Add(subject.jawOpen);
		AnimFrameData.PropertyValues.Add(subject.jawForward);
		AnimFrameData.PropertyValues.Add(subject.jawLeft);
		AnimFrameData.PropertyValues.Add(subject.jawRight);
		AnimFrameData.PropertyValues.Add(subject.mouthClose);
		AnimFrameData.PropertyValues.Add(subject.mouthDimpleLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthDimpleRight);
		AnimFrameData.PropertyValues.Add(subject.mouthFrownLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthFrownRight);
		AnimFrameData.PropertyValues.Add(subject.mouthFunnel);
		AnimFrameData.PropertyValues.Add(subject.mouthLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthLowerDownLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthLowerDownRight);
		AnimFrameData.PropertyValues.Add(subject.mouthPressLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthPressRight);
		AnimFrameData.PropertyValues.Add(subject.mouthPucker);
		AnimFrameData.PropertyValues.Add(subject.mouthRight);
		AnimFrameData.PropertyValues.Add(subject.mouthRollLower);
		AnimFrameData.PropertyValues.Add(subject.mouthRollUpper);
		AnimFrameData.PropertyValues.Add(subject.mouthShrugLower);
		AnimFrameData.PropertyValues.Add(subject.mouthShrugUpper);
		AnimFrameData.PropertyValues.Add(subject.mouthSmileLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthSmileRight);
		AnimFrameData.PropertyValues.Add(subject.mouthStretchLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthStretchRight);
		AnimFrameData.PropertyValues.Add(subject.mouthUpperUpLeft);
		AnimFrameData.PropertyValues.Add(subject.mouthUpperUpRight);
		AnimFrameData.PropertyValues.Add(subject.noseSneerLeft);
		AnimFrameData.PropertyValues.Add(subject.noseSneerRight);
		AnimFrameData.PropertyValues.Add(subject.tongueOut);

		Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.GetSubjectName()), MoveTemp(FrameData));
	}
}


void FVirtualProductionSource::HandleSubjectFrame(TArray<FVirtualProductionSubject> FrameSubjects)
{
	//verify(Client != nullptr);

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
		for (int j = 0; j < subjects.Num(); j++) 
		{
			if (subjectNames[i] == subjects[j].name) 
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

	for (int subjectIndex = 0; subjectIndex < subjects.Num(); subjectIndex++) 
	{
		FVirtualProductionSubject subject = subjects[subjectIndex];
		
		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int subjectNameIndex = 0; subjectNameIndex < subjectNames.Num(); subjectNameIndex++) 
		{
			if (subject.name == subjectNames[subjectNameIndex]) 
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
		hardCodedTransform.SetTranslation(subject.position);
		hardCodedTransform.SetRotation(subject.rotation);
		hardCodedTransform.SetScale3D(FVector(1, 1, 1));

		if (subject.name.ToString().StartsWith("prop"))
		{
			FString subjectname = subject.name.ToString();
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
				
				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.name), MoveTemp(FrameData1));
			}
			else
			if (testval.StartsWith("light"))
			{
				//FTimer timer;
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkLightFrameData::StaticStruct());
				FLiveLinkLightFrameData& LightFrameData = *FrameData1.Cast<FLiveLinkLightFrameData>();
				LightFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);
				LightFrameData.Transform = hardCodedTransform;
				//CameraFrameData.LightColor = FColor::Green;

				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.name), MoveTemp(FrameData1));

			}
			else
			{
				//FTimer timer;
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
				FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
				AnimFrameData.WorldTime = FLiveLinkWorldTime(/*(double)(timer.GetCurrentTime())*/);
				AnimFrameData.Transforms.Add(hardCodedTransform);

				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.name), MoveTemp(FrameData1));
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

			Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.name), MoveTemp(FrameData1));
		}
	}
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
					//bDoesAlreadyExist = true;

					LiveLinkClient.RemoveSource(SourceGuid);

					break;
				}
			}
		}

		if (!bDoesAlreadyExist)
		{
			ILiveLinkClient* LiveLinkClient = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
			TSharedPtr<FVirtualProductionSource> Source = MakeShared<FVirtualProductionSource>(FText::FromString("Studio"), FText::FromString(""), FMessageAddress::NewAddress());
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

FName URokokoFaceMapData::GetRemappedCurveName_Implementation(FName CurveName) const
{
	if (auto RemappedName = NameMapping.Find(CurveName))
	{
		return *RemappedName;
	}
	return "";
}

void URokokoFaceMapData::Initialize()
{
	InitializeTMap();
}

//VPStreamingNetwork::VPStreamingNetwork()
//{
//}
//
//
//VPStreamingNetwork::~VPStreamingNetwork()
//{
//	// Stop the runnable
//	Stop();
//	auto livelink = FVirtualProductionSource::Get();
//	if (livelink.IsValid())
//	{
//		livelink->ClearAllSubjects();
//	}
//	//livelink->HandleClearSubject(FLiveLinkClearSubject(FString("subjectNames")));
//
//	if (Socket)
//	{
//		Socket->Close();
//	}
//
//	// And last but not least stop the main thread
//	if (Thread != NULL)
//	{
//		Thread->Kill(true);
//		delete Thread;
//	}
//}


void FVirtualProductionSource::StartRunnable(int port)
{
	streaming_port = port;
	FString ThreadName(FString::Printf(TEXT("VPStreamingNetwork%ld"), (long)(FDateTime::UtcNow().ToUnixTimestamp())));
	if (streaming_port)
	{
		UE_LOG(LogTemp, Warning, TEXT("VP port... %i"), streaming_port);
	}
	Thread = FRunnableThread::Create(this, *ThreadName, 512 * 1024, TPri_Normal);
}


bool FVirtualProductionSource::InitSocket(int port)
{
	streaming_port = port;
	if (Socket == NULL)
	{
		Socket = FUdpSocketBuilder(TEXT("VPStreamingNetwork")).BoundToAddress(FIPv4Address(0, 0, 0, 0)).BoundToPort(streaming_port).AsReusable().Build();
		//allow the socket to listen to an already bounded address.
		Socket->SetReuseAddr(true);
		Stopping = false;
	}
	return true;
}

FString BytesToStringFixed(const uint8* In, int32 Count)
{
	FString Broken = BytesToString(In, Count);
	FString Fixed;

	for (int i = 0; i < Broken.Len(); i++)
	{
		const TCHAR c = Broken[i] - 1;
		Fixed.AppendChar(c);
	}

	return Fixed;
}

void FVirtualProductionSource::SendToLiveLink(TArray<FVirtualProductionSubject> Subjects)
{
	//auto livelink = FVirtualProductionSource::Get();
	//if (livelink.IsValid())
	//{
	//	livelink->HandleSubjectFrame(Subjects);
	//}

	HandleSubjectFrame(Subjects);
}

void FVirtualProductionSource::SendFacesToLivelink(TArray<FFace> Subjects)
{
	//auto livelink = FVirtualProductionSource::Get();
	//if (livelink.IsValid())
	//{
	//	livelink->HandleFace(Subjects);
	//}

	HandleFace(Subjects);
}

void FVirtualProductionSource::SendSuitsToLiveLink(TArray<FSuitData> Smartsuits)
{
	//auto livelink = FVirtualProductionSource::Get();
	//if (livelink.IsValid())
	//{
	//	livelink->HandleSuits(Smartsuits);
	//}

	HandleSuits(Smartsuits);

}

PRAGMA_DISABLE_OPTIMIZATION
uint32 FVirtualProductionSource::Run()
{
	static LZ4F_decompressionContext_t g_dCtx;
	LZ4F_createDecompressionContext(&g_dCtx, LZ4F_VERSION);
	bool added = false;
	while (!Stopping)
	{
		auto addr_in = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		int32 bytes_read = 0;
		FString ret;

		uint8 data[32768];
		memset(data, '\0', 32768);

		FDateTime time = FDateTime::UtcNow();
		int seconds = time.ToUnixTimestamp();
		if (Socket)
		{
			if (Socket->RecvFrom(data, sizeof(data), bytes_read, *addr_in))
			{
				if (bytes_read == 0)
				{
					continue;
				}
				//FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
				//{
				if (Stopping) break;
				mtx.lock();
				FVirtualProductionFrame VPFrame;

				int32 UncompressedSize = 2097152;

				TArray<uint8> UncompressedData;
				UncompressedData.Empty(UncompressedSize);
				UncompressedData.AddUninitialized(UncompressedSize);

				//FName FormatName = NAME_LZ4;
				//if (!FCompression::UncompressMemory(FormatName, UncompressedData.GetData(), UncompressedSize, data, static_cast<int32_t>(bytes_read)))
				//{
				//	UE_LOG(LogTemp, Error, TEXT("FCompression::UncompressMemory - Failed to uncompress memory (%d/%d) from address %p using format %s, this may indicate the asset is corrupt!"), bytes_read, UncompressedSize, data, *FormatName.ToString());
				//}


				//IFileManager* FileManager = &IFileManager::Get();
				uint32 WriteFlags = 0;

				//FString path = "C:/GitHub/rokoko-studio-live-unreal-engine_JSONV3/test12345678.lz4";
				//TUniquePtr<FArchive> Ar = TUniquePtr<FArchive>(FileManager->CreateFileWriter(*path, WriteFlags));
				//if (!Ar)
				//{
				//	UE_LOG(LogTemp, Error, TEXT("Could not write file %s!"), bytes_read, UncompressedSize, data, *FormatName.ToString());
				//}
				//Ar->Serialize(const_cast<uint8*>(data), bytes_read);
				//Ar->Close();


				size_t srcSize = (size_t)bytes_read;
				size_t dstSize = (size_t)UncompressedData.Num();
				size_t decompressResult;
				verify(srcSize >= 0);
				verify(dstSize >= 0);
				decompressResult = LZ4F_decompress(g_dCtx, UncompressedData.GetData(), &dstSize, data, &srcSize, NULL);
				if (decompressResult != 0) { UE_LOG(LogTemp, Error, TEXT("Error decompressing frame : unfinished frame \n")); }
				if (srcSize != (size_t)bytes_read) { UE_LOG(LogTemp, Error, TEXT("Error decompressing frame : read size incorrect \n")); }


				//FString path2 = "C:/GitHub/rokoko-studio-live-unreal-engine_JSONV3/3actorstest.txt";
				//TUniquePtr<FArchive> Ar2 = TUniquePtr<FArchive>(FileManager->CreateFileWriter(*path2, WriteFlags));
				//if (!Ar2)
				//{
				//	UE_LOG(LogTemp, Error, TEXT("Could not write file %s!"), *path2);
				//}
				//Ar2->Serialize(UncompressedData.GetData(), dstSize);
				//Ar2->Close();




				FString result = BytesToStringFixed(UncompressedData.GetData(), static_cast<int32_t>(dstSize));
				FString test = BytesToStringFixed(data, static_cast<int32_t>(bytes_read));
				//FString result = BytesToString(UncompressedData.GetData(), static_cast<int32_t>(UncompressedData.Num()));
				//FString result = BytesToStringFixed(UncompressedData.GetData(), UncompressedData.Num());
				//FString result = BytesToStringFixed(data, static_cast<int32_t>(bytes_read));
				//UE_LOG(LogTemp, Warning, TEXT("received: %s"), *result);
				//FJsonObjectConverter::JsonObjectStringToUStruct(result, &VPFrame, 0, 0);

				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(result);
				if (FJsonSerializer::Deserialize(Reader, JsonObject))
				{

					VPFrame.version = JsonObject->GetIntegerField("version");
					//VPFrame.timestamp = JsonObject->GetNumberField("timestamp");
					//VPFrame.playbackTimestamp = JsonObject->GetNumberField("");

					//SCENE
					{
						TSharedPtr<FJsonObject> SceneObj = JsonObject->GetObjectField("scene");
						TArray<TSharedPtr<FJsonValue>> Livepropsarray = SceneObj->GetArrayField("props");

						for (auto& currentprop : Livepropsarray)
						{
							VPFrame.props.Add(FProp(true, currentprop->AsObject()));
						}

						if (SceneObj->HasField("actors"))
						{
							TArray<TSharedPtr<FJsonValue>> Livesuitsarray = SceneObj->GetArrayField("actors");
							for (auto& currentsuit : Livesuitsarray)
							{
								auto SuitData = FSuitData(true, currentsuit->AsObject());
								if (SuitData.hasFace)
								{
									auto JSONObjectface = currentsuit->AsObject()->GetObjectField("face");
									auto FaceData = FFace(JSONObjectface, SuitData.suitname);
									SuitData.faceId = FaceData.faceId;
									VPFrame.faces.Add(FaceData);
								}
								VPFrame.suits.Add(SuitData);
							}
						}
					}

					////LIVE
					//{
					//	TSharedPtr<FJsonObject> LiveObj = JsonObject->GetObjectField("live");
					//	TArray<TSharedPtr<FJsonValue>> Livepropsarray = LiveObj->GetArrayField("props");

					//	for (auto& currentprop : Livepropsarray)
					//	{
					//		VPFrame.props.Add(FProp(true, currentprop->AsObject()));
					//	}

					//	if (LiveObj->HasField("actors"))
					//	{
					//		TArray<TSharedPtr<FJsonValue>> Livesuitsarray = LiveObj->GetArrayField("actors");
					//		for (auto& currentsuit : Livesuitsarray)
					//		{
					//			auto SuitData = FSuitData(true, currentsuit->AsObject());
					//			if (SuitData.hasFace)
					//			{
					//				auto JSONObjectface = currentsuit->AsObject()->GetObjectField("face");
					//				auto FaceData = FFace(JSONObjectface, SuitData.suitname);
					//				SuitData.faceId = FaceData.faceId;
					//				VPFrame.faces.Add(FaceData);
					//			}
					//			VPFrame.suits.Add(SuitData);
					//		}
					//	}
					//}

					////PLAYBACK
					//{
					//	TSharedPtr<FJsonObject> PlaybackObj = JsonObject->GetObjectField("playback");
					//	TArray<TSharedPtr<FJsonValue>> Playbackpropsarray = PlaybackObj->GetArrayField("props");

					//	for (auto& currentprop : Playbackpropsarray)
					//	{
					//		VPFrame.props.Add(FProp(false, currentprop->AsObject()));
					//	}

					//	if (PlaybackObj->HasField("actors"))
					//	{
					//		TArray<TSharedPtr<FJsonValue>> Playbacksuitsarray = PlaybackObj->GetArrayField("actors");
					//		for (auto& currentsuit : Playbacksuitsarray)
					//		{
					//			auto SuitData = FSuitData(false, currentsuit->AsObject());
					//			if (SuitData.hasFace)
					//			{
					//				auto JSONObjectface = currentsuit->AsObject()->GetObjectField("face");
					//				auto FaceData = FFace(JSONObjectface, SuitData.suitname);
					//				SuitData.faceId = FaceData.faceId;
					//				VPFrame.faces.Add(FaceData);
					//			}
					//			VPFrame.suits.Add(SuitData);
					//		}
					//	}
					//}





				}


				//if (!GlobalVPFrame)
				//{
				//	GlobalVPFrame = new FVirtualProductionFrame();
				//}
				GlobalVPFrame.version = VPFrame.version;
				GlobalVPFrame.props.Empty();
				GlobalVPFrame.trackers.Empty();
				GlobalVPFrame.faces.Empty();
				GlobalVPFrame.suits.Empty();

				//auto livelink = FVirtualProductionSource::Get();

				//if (livelink.IsValid())
				{
					subjects.Empty();
					for (int i = 0; i < VPFrame.props.Num(); i++)
					{
						GlobalVPFrame.props.Add(VPFrame.props[i]);
						FVirtualProductionSubject subject = GlobalVPFrame.props[i].GetSubject();
						subjects.Add(subject);
					}
					for (int i = 0; i < VPFrame.trackers.Num(); i++)
					{
						GlobalVPFrame.trackers.Add(VPFrame.trackers[i]);
						FVirtualProductionSubject subject = GlobalVPFrame.trackers[i].GetSubject();
						subjects.Add(subject);
					}
					for (int i = 0; i < VPFrame.faces.Num(); i++)
					{
						GlobalVPFrame.faces.Add(VPFrame.faces[i]);
					}
					for (int i = 0; i < VPFrame.suits.Num(); i++)
					{
						GlobalVPFrame.suits.Add(VPFrame.suits[i]);
					}
					SendToLiveLink(subjects);
					SendFacesToLivelink(GlobalVPFrame.faces);
					SendSuitsToLiveLink(GlobalVPFrame.suits);
				}
				//else
				//{
				//	for (int i = 0; i < VPFrame.props.Num(); i++)
				//	{
				//		GlobalVPFrame.props.Add(VPFrame.props[i]);
				//	}
				//	for (int i = 0; i < VPFrame.trackers.Num(); i++)
				//	{
				//		GlobalVPFrame.trackers.Add(VPFrame.trackers[i]);
				//	}
				//	for (int i = 0; i < VPFrame.faces.Num(); i++)
				//	{
				//		//UE_LOG(LogTemp, Warning, TEXT("face: %d - %s - %f"), VPFrame.faces[i].version, *VPFrame.faces[i].provider, VPFrame.faces[i].jawOpen);
				//		GlobalVPFrame.faces.Add(VPFrame.faces[i]);
				//	}
				//	for (int i = 0; i < VPFrame.suits.Num(); i++)
				//	{
				//		GlobalVPFrame.suits.Add(VPFrame.suits[i]);
				//	}
				//}
				//UE_LOG(LogTemp, Warning, TEXT("Faces... %i"), GlobalVPFrame.faces.Num());
				mtx.unlock();
				//}, TStatId(), NULL, ENamedThreads::GameThread);

				// If you want to wait for the code above to complete do this:
				//FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

			}
		}
	}
	return 0;
}
PRAGMA_ENABLE_OPTIMIZATION
FProp* FVirtualProductionSource::GetPropByName(FString name, bool isLive)
{
	FProp* result = nullptr;
	mtx.lock();
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.props.Num(); i++)
		{
			if (name == GlobalVPFrame.props[i].name && GlobalVPFrame.props[i].isLive == isLive)
			{
				result = &GlobalVPFrame.props[i];
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FProp> FVirtualProductionSource::GetAllProps()
{
	//return nullptr;
	TArray<FProp> result;
	mtx.lock();
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.props.Num(); i++)
		{
			result.Add(GlobalVPFrame.props[i]);
			//result->Add
		}
	}
	mtx.unlock();
	//UE_LOG(LogTemp, Display, TEXT("Yeeee3"));
	return result;

	//return GlobalVPFrame.props;
}

FTracker* FVirtualProductionSource::GetTrackerByName(FString name, bool isLive)
{
	FTracker* result = nullptr;
	mtx.lock();
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.trackers.Num(); i++)
		{
			if (name == GlobalVPFrame.trackers[i].name && GlobalVPFrame.trackers[i].isLive == isLive)
			{
				result = &GlobalVPFrame.trackers[i];
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
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.trackers.Num(); i++)
		{
			if (name == GlobalVPFrame.trackers[i].connectionId /*&& GlobalVPFrame.trackers[i].isLive == isLive*/)
			{
				result = &GlobalVPFrame.trackers[i];
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
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.trackers.Num(); i++)
		{
			if (ConnectionId == GlobalVPFrame.trackers[i].connectionId /*&& GlobalVPFrame.trackers[i].isLive == isLive*/)
			{
				result.Add(GlobalVPFrame.trackers[i]);
			}
		}
	}
	mtx.unlock();
	return result;
}

FSuitData* FVirtualProductionSource::GetSmartsuitByName(FString suitName)
{
	if (suitName.Len() == 0 || suitName.Compare(FString("")) == 0)
	{
		return nullptr;
	}

	FSuitData* result = nullptr;
	mtx.lock();
	//if (GlobalVPFrame)
	{
		//should probably set the limit to the size of the suit array here?
		for (int i = 0; i < GlobalVPFrame.suits.Num(); i++)
		{
			FString mySuitName(GlobalVPFrame.suits[i].suitname);
			if (suitName.Compare(mySuitName) == 0 && mySuitName.Len() > 0)
			{
				result = &(GlobalVPFrame.suits[i]);
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
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.suits.Num(); i++)
		{
			if ((GlobalVPFrame.suits[i].suitname != "\0\0\0\0") /*&& GlobalVPFrame.suits[i].fps > 0*/)
			{
				result.Add(FString(GlobalVPFrame.suits[i].suitname));
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
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.suits.Num(); i++)
		{
			if ((GlobalVPFrame.suits[i].suitname != "\0\0\0\0") /*&& GlobalVPFrame.suits[i].fps > 0*/)
			{
				suits.Add(GlobalVPFrame.suits[i]);
			}
		}
	}
	mtx.unlock();
	return suits;
}

FFace FVirtualProductionSource::GetFaceByFaceID(FString faceID)
{
	FFace result;
	mtx.lock();
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.faces.Num(); i++)
		{
			if (GlobalVPFrame.faces[i].faceId == faceID)
			{
				result = GlobalVPFrame.faces[i];
				break;
			}
		}
	}
	mtx.unlock();

	return result;
}

FFace* FVirtualProductionSource::GetFaceByProfileName(const FString& profileName)
{
	FFace* result = nullptr;
	mtx.lock();
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.faces.Num(); i++)
		{
			if (GlobalVPFrame.faces[i].profileName == profileName)
			{
				result = &GlobalVPFrame.faces[i];
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
	//if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame.faces.Num(); i++)
		{
			result.Add(GlobalVPFrame.faces[i]);
			//result->Add
		}
	}
	mtx.unlock();

	return result;

	//return GlobalVPFrame.faces;
}
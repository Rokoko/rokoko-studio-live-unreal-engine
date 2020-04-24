// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionSource.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "Roles/LiveLinkCameraTypes.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkLightRole.h"
#include "Roles/LiveLinkLightTypes.h"
#include "Features/IModularFeatures.h"



TSharedPtr<FVirtualProductionSource> FVirtualProductionSource::instance = nullptr;

FVirtualProductionSource::~FVirtualProductionSource()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroying Virtual production source!!!"));
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
			FLiveLinkStaticDataStruct LightData(FLiveLinkCameraStaticData::StaticStruct());
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
	
void FVirtualProductionSource::HandleSuitData(SuitData suit) 
{
	suitNames.Add(suit.GetSubjectName());

	FLiveLinkSubjectKey Key = FLiveLinkSubjectKey(SourceGuid, suit.GetSubjectName());

	TArray<FName> boneNames;
	boneNames.Add("Base");
	boneNames.Add("Hips");
	boneNames.Add("Spine");
	boneNames.Add("Spine2");
	boneNames.Add("Neck");
	boneNames.Add("Head");

	boneNames.Add("LeftShoulder");
	boneNames.Add("LeftArm");
	boneNames.Add("LeftForeArm");
	boneNames.Add("LeftHand");
	
	boneNames.Add("RightShoulder");
	boneNames.Add("RightArm");
	boneNames.Add("RightForeArm");
	boneNames.Add("RightHand");

	boneNames.Add("LeftUpLeg");
	boneNames.Add("LeftLeg");
	boneNames.Add("LeftFoot");

	boneNames.Add("RightUpLeg");
	boneNames.Add("RightLeg");
	boneNames.Add("RightFoot");


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

	FLiveLinkStaticDataStruct StaticData(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData* SkeletonData = StaticData.Cast<FLiveLinkSkeletonStaticData>();
	SkeletonData->SetBoneNames(boneNames);
	SkeletonData->SetBoneParents(boneParents);
	Client->PushSubjectStaticData_AnyThread(Key, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticData));
}

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

void FVirtualProductionSource::HandleSuits(TArray<SuitData> suits) 
{
	//UE_LOG(LogTemp, Warning, TEXT("Handling faces %d"), faces.Num());
	existingSuits.Empty();
	notExistingSubjects.Empty();
	for (int subjectIndex = 0; subjectIndex < suits.Num(); subjectIndex++) 
	{
		SuitData subject = suits[subjectIndex];

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

		FTimer timer;
		FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
		FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
		AnimFrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));

		TArray<FTransform> transforms;
		transforms.Reset(20);
		int32 transformIndex = transforms.AddUninitialized(1);
		
		transforms[transformIndex].SetLocation(FVector(0, 0, 0));
		transforms[transformIndex].SetRotation(FQuat::Identity);
		transforms[transformIndex].SetScale3D(FVector(1, 1, 1));

		CreateJoint(transforms, 0, nullptr, subject.Hip());
		CreateJoint(transforms, -1, subject.Hip(), subject.GetSensor(SENSOR_STOMACH));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_STOMACH), subject.GetSensor(SENSOR_CHEST));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_CHEST), subject.GetSensor(SENSOR_NECK));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_NECK), subject.GetSensor(SENSOR_HEAD));

		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_CHEST), subject.GetSensor(SENSOR_LEFT_SHOULDER));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_LEFT_SHOULDER), subject.GetSensor(SENSOR_LEFT_UPPER_ARM));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_LEFT_UPPER_ARM), subject.GetSensor(SENSOR_LEFT_LOWER_ARM));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_LEFT_LOWER_ARM), subject.GetSensor(SENSOR_LEFT_HAND));

		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_CHEST), subject.GetSensor(SENSOR_RIGHT_SHOULDER));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_RIGHT_SHOULDER), subject.GetSensor(SENSOR_RIGHT_UPPER_ARM));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_RIGHT_UPPER_ARM), subject.GetSensor(SENSOR_RIGHT_LOWER_ARM));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_RIGHT_LOWER_ARM), subject.GetSensor(SENSOR_RIGHT_HAND));

		CreateJoint(transforms, -1, subject.GetSensor(SENSOR_HIP), subject.GetSensor(SENSOR_LEFT_UPPER_LEG));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_LEFT_UPPER_LEG), subject.GetSensor(SENSOR_LEFT_LOWER_LEG));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_LEFT_LOWER_LEG), subject.GetSensor(SENSOR_LEFT_FOOT));
		
		CreateJoint(transforms, -1, subject.GetSensor(SENSOR_HIP), subject.GetSensor(SENSOR_RIGHT_UPPER_LEG));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_RIGHT_UPPER_LEG), subject.GetSensor(SENSOR_RIGHT_LOWER_LEG));
		CreateJoint(transforms, 0, subject.GetSensor(SENSOR_RIGHT_LOWER_LEG), subject.GetSensor(SENSOR_RIGHT_FOOT));
		
		AnimFrameData.Transforms.Append(transforms);

		Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.GetSubjectName()), MoveTemp(FrameData1));
	}
}

void FVirtualProductionSource::HandleFace(TArray<FFace> faces) 
{
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

		FTimer timer;
		FLiveLinkFrameDataStruct FrameData(FLiveLinkAnimationFrameData::StaticStruct());
		FLiveLinkAnimationFrameData& AnimFrameData = *FrameData.Cast<FLiveLinkAnimationFrameData>();
		AnimFrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));



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


void FVirtualProductionSource::HandleSubjectFrame(TArray<FVirtualProductionSubject> subjects)
{
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
				FTimer timer;
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkCameraFrameData::StaticStruct());
				FLiveLinkCameraFrameData& CameraFrameData = *FrameData1.Cast<FLiveLinkCameraFrameData>();
				CameraFrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));
				CameraFrameData.Transform = hardCodedTransform;
				CameraFrameData.AspectRatio = 1.11f;
				CameraFrameData.FieldOfView = 130.f;
				
				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.name), MoveTemp(FrameData1));
			}
			else
			if (testval.StartsWith("light"))
			{
				FTimer timer;
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkCameraFrameData::StaticStruct());
				FLiveLinkLightFrameData& LightFrameData = *FrameData1.Cast<FLiveLinkLightFrameData>();
				LightFrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));
				LightFrameData.Transform = hardCodedTransform;
				//CameraFrameData.LightColor = FColor::Green;

				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.name), MoveTemp(FrameData1));

			}
			else
			{
				FTimer timer;
				FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
				FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
				AnimFrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));
				AnimFrameData.Transforms.Add(hardCodedTransform);

				Client->PushSubjectFrameData_AnyThread(FLiveLinkSubjectKey(SourceGuid, subject.name), MoveTemp(FrameData1));
			}
		}
		else
		//if (subject.name.ToString().StartsWith("tracker"))
		{
			FTimer timer;
			FLiveLinkFrameDataStruct FrameData1(FLiveLinkAnimationFrameData::StaticStruct());
			FLiveLinkAnimationFrameData& AnimFrameData = *FrameData1.Cast<FLiveLinkAnimationFrameData>();
			AnimFrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));
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
					UE_LOG(LogTemp, Warning, TEXT("you can't add more than one instance of FVirtualProductionSource!!"));
					bDoesAlreadyExist = true;
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
// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VirtualProductionSource.h"

FVirtualProductionSource* FVirtualProductionSource::instance = 0;

void FVirtualProductionSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
	instance = this;
}

bool FVirtualProductionSource::IsSourceStillValid()
{
	return true;
}

void FVirtualProductionSource::HandleClearSubject(const FName subjectName)
{
	Client->ClearSubject(subjectName);
}

void FVirtualProductionSource::ClearAllSubjects() {
	for (int i = 0; i < subjectNames.Num(); i++) {
		HandleClearSubject(subjectNames[i]);
	}
	for (int i = 0; i < faceNames.Num(); i++) {
		HandleClearSubject(faceNames[i]);
	}

	subjectNames.Empty();
	faceNames.Empty();
}

bool FVirtualProductionSource::RequestSourceShutdown()
{
	ClearAllSubjects();
	instance = nullptr;
	return true;
}

void FVirtualProductionSource::HandleFaceData(FFace face) {
	UE_LOG(LogTemp, Warning, TEXT("Creating a new face %s - %f"), *face.GetSubjectName().ToString(), face.jawOpen);
	faceNames.Add(face.GetSubjectName());
	FLiveLinkRefSkeleton skeletonRef;
	Client->PushSubjectSkeleton(SourceGuid, face.GetSubjectName(), skeletonRef);
}

void FVirtualProductionSource::HandleSubjectData(FVirtualProductionSubject virtualProductionObject)
{
	//UE_LOG(LogTemp, Warning, TEXT("SKELETON!! "), skeleton);
	subjectNames.Add(virtualProductionObject.name);
	FLiveLinkRefSkeleton skeletonRef;
	TArray<FName> boneNames;
	boneNames.Add("Root");
	skeletonRef.SetBoneNames(boneNames);
	TArray<int32> boneParents;
	boneParents.Add(0);
	skeletonRef.SetBoneParents(boneParents);
	Client->PushSubjectSkeleton(SourceGuid, virtualProductionObject.name, skeletonRef);
}
	

void FVirtualProductionSource::HandleFace(TArray<FFace> faces) {
	//UE_LOG(LogTemp, Warning, TEXT("Handling faces %d"), faces.Num());
	existingFaces.Empty();
	notExistingSubjects.Empty();
	for (int subjectIndex = 0; subjectIndex < faces.Num(); subjectIndex++) {
		FFace subject = faces[subjectIndex];

		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int faceNameIndex = 0; faceNameIndex < faceNames.Num(); faceNameIndex++) {
			if (subject.GetSubjectName() == faceNames[faceNameIndex]) {
				nameExists = true;
				existingFaces.Add(subject);
				break;
			}
		}

		if (!nameExists) {
			existingFaces.Add(subject);
			HandleFaceData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		if (subjectIndex == faces.Num() - 1) {
			for (int i = 0; i < faceNames.Num(); i++) {
				bool subjectExists = false;
				for (int j = 0; j < existingFaces.Num(); j++) {
					if (faceNames[i] == existingFaces[j].GetSubjectName()) {
						subjectExists = true;
					}
				}
				if (!subjectExists) {
					notExistingSubjects.Add(faceNames[i]);
				}
			}

			for (int i = 0; i < notExistingSubjects.Num(); i++) {
				//UE_LOG(LogTemp, Warning, TEXT("Removing face"));
				Client->ClearSubject(notExistingSubjects[i]);
				faceNames.RemoveSingle(notExistingSubjects[i]);
				notExistingSubjects.RemoveAt(i);
			}
		}


		FLiveLinkFrameData FrameData;
		FTimer timer;
		FrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));
		TArray<FLiveLinkCurveElement>& BlendShapes = FrameData.CurveElements;
		BlendShapes.Reset(52);
		
		int32 blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "browDownLeft";
		BlendShapes[blendIndex].CurveValue = subject.browDownLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "browDownRight";
		BlendShapes[blendIndex].CurveValue = subject.browDownRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "browInnerUp";
		BlendShapes[blendIndex].CurveValue = subject.browInnerUp;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "browOuterUpLeft";
		BlendShapes[blendIndex].CurveValue = subject.browOuterUpLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "browOuterUpRight";
		BlendShapes[blendIndex].CurveValue = subject.browOuterUpRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "cheekPuff";
		BlendShapes[blendIndex].CurveValue = subject.cheekPuff;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "cheekSquintLeft";
		BlendShapes[blendIndex].CurveValue = subject.cheekSquintLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "cheekSquintRight";
		BlendShapes[blendIndex].CurveValue = subject.cheekSquintRight;
		
		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeBlinkLeft";
		BlendShapes[blendIndex].CurveValue = subject.eyeBlinkLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeBlinkRight";
		BlendShapes[blendIndex].CurveValue = subject.eyeBlinkRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookDownLeft";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookDownLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookDownRight";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookDownRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookInLeft";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookInLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookInRight";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookInRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookOutLeft";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookOutLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookOutRight";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookOutRight;


		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookUpLeft";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookUpLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeLookUpRight";
		BlendShapes[blendIndex].CurveValue = subject.eyeLookUpRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeSquintLeft";
		BlendShapes[blendIndex].CurveValue = subject.eyeSquintLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeSquintRight";
		BlendShapes[blendIndex].CurveValue = subject.eyeSquintRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeWideLeft";
		BlendShapes[blendIndex].CurveValue = subject.eyeWideLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "eyeWideRight";
		BlendShapes[blendIndex].CurveValue = subject.eyeWideRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "jawOpen";
		BlendShapes[blendIndex].CurveValue = subject.jawOpen;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "jawForward";
		BlendShapes[blendIndex].CurveValue = subject.jawForward;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "jawLeft";
		BlendShapes[blendIndex].CurveValue = subject.jawLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "jawRight";
		BlendShapes[blendIndex].CurveValue = subject.jawRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthClose";
		BlendShapes[blendIndex].CurveValue = subject.mouthClose;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthDimpleLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthDimpleLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthDimpleRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthDimpleRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthFrownLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthFrownLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthFrownRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthFrownRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthFunnel";
		BlendShapes[blendIndex].CurveValue = subject.mouthFunnel;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthLowerDownLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthLowerDownLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthLowerDownRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthLowerDownRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthPressLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthPressLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthPressRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthPressRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthPucker";
		BlendShapes[blendIndex].CurveValue = subject.mouthPucker;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthRollLower";
		BlendShapes[blendIndex].CurveValue = subject.mouthRollLower;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthRollUpper";
		BlendShapes[blendIndex].CurveValue = subject.mouthRollUpper;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthShrugLower";
		BlendShapes[blendIndex].CurveValue = subject.mouthShrugLower;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthShrugUpper";
		BlendShapes[blendIndex].CurveValue = subject.mouthShrugUpper;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthSmileLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthSmileLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthSmileRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthSmileRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthStretchLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthStretchLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthStretchRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthStretchRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthUpperUpLeft";
		BlendShapes[blendIndex].CurveValue = subject.mouthUpperUpLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "mouthUpperUpRight";
		BlendShapes[blendIndex].CurveValue = subject.mouthUpperUpRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "noseSneerLeft";
		BlendShapes[blendIndex].CurveValue = subject.noseSneerLeft;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "noseSneerRight";
		BlendShapes[blendIndex].CurveValue = faces[subjectIndex].noseSneerRight;

		blendIndex = BlendShapes.AddUninitialized(1);
		BlendShapes[blendIndex].CurveName = "tongueOut";
		BlendShapes[blendIndex].CurveValue = faces[subjectIndex].tongueOut;

		Client->PushSubjectData(SourceGuid, subject.GetSubjectName(), FrameData);
	}
}


void FVirtualProductionSource::HandleSubjectFrame(TArray<FVirtualProductionSubject> subjects)
{
	existingSubjects.Empty();
	notExistingSubjects.Empty();

	for (int i = 0; i < subjectNames.Num(); i++) {
		bool subjectExists = false;
		for (int j = 0; j < subjects.Num(); j++) {
			if (subjectNames[i] == subjects[j].name) {
				subjectExists = true;
			}
		}
		if (!subjectExists) {
			notExistingSubjects.Add(subjectNames[i]);
		}
	}

	for (int i = 0; i < notExistingSubjects.Num(); i++) {
		Client->ClearSubject(notExistingSubjects[i]);
		subjectNames.RemoveSingle(notExistingSubjects[i]);
	}

	for (int subjectIndex = 0; subjectIndex < subjects.Num(); subjectIndex++) {
		FVirtualProductionSubject subject = subjects[subjectIndex];
		
		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int subjectNameIndex = 0; subjectNameIndex < subjectNames.Num(); subjectNameIndex++) {
			if (subject.name == subjectNames[subjectNameIndex]) {
				nameExists = true;
				existingSubjects.Add(subject);
				break;
			}
		}

		if (!nameExists) {
			existingSubjects.Add(subject);
			HandleSubjectData(subject);
			existingSubjects.Add(subject);
		}

		FTransform hardCodedTransform;
		hardCodedTransform.SetTranslation(subject.position);
		hardCodedTransform.SetRotation(subject.rotation);
		hardCodedTransform.SetScale3D(FVector(1, 1, 1));

		FLiveLinkFrameData FrameData;
		FrameData.Transforms.Add(hardCodedTransform);
		FTimer timer;

		FrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));

		Client->PushSubjectData(SourceGuid, subject.name, FrameData);

	}
}
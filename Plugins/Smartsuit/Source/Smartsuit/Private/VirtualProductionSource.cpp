// Fill out your copyright notice in the Description page of Project Settings.

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
	subjectNames.Empty();

}

bool FVirtualProductionSource::RequestSourceShutdown()
{
	ClearAllSubjects();
	instance = nullptr;
	return true;
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
	

void FVirtualProductionSource::HandleSubjectFrame(TArray<FVirtualProductionSubject> subjects)
{
	existingSubjects.Empty();
	notExistingSubjects.Empty();
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
			HandleSubjectData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		if (subjectIndex == subjects.Num() - 1) {
			for (int i = 0; i < subjectNames.Num(); i++) {
				bool subjectExists = false;
				for (int j = 0; j < existingSubjects.Num(); j++) {
					if (subjectNames[i] == existingSubjects[j].name) {
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
				notExistingSubjects.RemoveAt(i);
			}
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
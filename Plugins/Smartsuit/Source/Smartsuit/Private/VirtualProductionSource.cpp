// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtualProductionSource.h"

#include "LiveLinkMessages.h"
#include "ILiveLinkClient.h"
#include "Runtime/RenderCore/Public/RenderCore.h"
//#include "LiveLinkMessageBusHeartbeatManager.h"

#include "MessageEndpointBuilder.h"

const double LL_CONNECTION_TIMEOUT = 15.0;
const double LL_HALF_CONNECTION_TIMEOUT = LL_CONNECTION_TIMEOUT / 2.0;

FVirtualProductionSource* FVirtualProductionSource::instance = 0;

void FVirtualProductionSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
	instance = this;
	UE_LOG(LogTemp, Warning, TEXT(" - - - RECEIVE CLIENT!!!"));
	//MessageEndpoint = FMessageEndpoint::Builder(TEXT("VirtualProductionSource"))
	//	.Handling<FLiveLinkSubjectDataMessage>(this, &FVirtualProductionSource::HandleSubjectData)
	//	.Handling<FLiveLinkSubjectFrameMessage>(this, &FVirtualProductionSource::HandleSubjectFrame)
	//	.Handling<FLiveLinkHeartbeatMessage>(this, &FVirtualProductionSource::HandleHeartbeat)
	//	.Handling<FLiveLinkClearSubject>(this, &FVirtualProductionSource::HandleClearSubject);


	//MessageEndpoint->Send(new FLiveLinkConnectMessage(), ConnectionAddress);

	// Register for heartbeats
	bIsValid = true;
}

bool FVirtualProductionSource::IsSourceStillValid()
{
	return bIsValid;
}

void FVirtualProductionSource::HandleClearSubject(const FName subjectName)
{
	ConnectionLastActive = FPlatformTime::Seconds();
	Client->ClearSubject(subjectName);
}

void FVirtualProductionSource::ClearAllSubjects() {
	UE_LOG(LogTemp, Warning, TEXT("ClearAllSubjects called"));
	RequestSourceShutdown();
}

bool FVirtualProductionSource::RequestSourceShutdown()
{
	UE_LOG(LogTemp, Warning, TEXT("Shutting down"));
	//HandleClearSubject(FLiveLinkClearSubject("Studio"));
	for (int i = 0; i < subjectNames.Num(); i++) {
		HandleClearSubject(subjectNames[i]);
	}
	subjectNames.Empty();
	instance = nullptr;
	return true;
}

void FVirtualProductionSource::HandleSubjectData(FVirtualProductionSubject virtualProductionObject)
{
	ConnectionLastActive = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Warning, TEXT("Handle Subject Data!!"));
	UE_LOG(LogTemp, Warning, TEXT("SUBJECT!! %s"), &virtualProductionObject.name);
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
	for (int subjectIndex = 0; subjectIndex < subjects.Num(); subjectIndex++) {
		FVirtualProductionSubject subject = subjects[subjectIndex];
		
		//check in the known subjects list which ones don't exist anymore in subjects, and clear the ones that don't exist
		bool nameExists = false;
		for (int subjectNameIndex = 0; subjectNameIndex < subjectNames.Num(); subjectNameIndex++) {
			if (subject.name == subjectNames[subjectNameIndex]) {
				nameExists = true;
				break;
			}
		}

		if (!nameExists) {
			HandleSubjectData(subject);
		}
		//check in the subjects for the ones that don't exist in the known subjects list and create the ones that don't exist
		

		ConnectionLastActive = FPlatformTime::Seconds();
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
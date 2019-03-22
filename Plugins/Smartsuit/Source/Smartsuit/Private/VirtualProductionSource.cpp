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

bool FVirtualProductionSource::RequestSourceShutdown()
{
	UE_LOG(LogTemp, Warning, TEXT("Shutting down"));
	//HandleClearSubject(FLiveLinkClearSubject("Studio"));
	for (int i = 0; i < subjectNames.Num(); i++) {
		HandleClearSubject(subjectNames[i]);
	}
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
	
void FVirtualProductionSource::HandleSubjectFrame(FVirtualProductionSubject virtualProductionObject)
{
	ConnectionLastActive = FPlatformTime::Seconds();
	FTransform hardCodedTransform;
	hardCodedTransform.SetTranslation(virtualProductionObject.position);
	hardCodedTransform.SetRotation(virtualProductionObject.rotation);
	hardCodedTransform.SetScale3D(FVector(1, 1, 1));

	FLiveLinkFrameData FrameData;
	FrameData.Transforms.Add(hardCodedTransform);
	FTimer timer;

	FrameData.WorldTime = FLiveLinkWorldTime((double)(timer.GetCurrentTime()));
	//FLiveLinkCurveElement curve;
	//curve.CurveName = "Root.position.z";
	//curve.CurveValue = -200;
	//FrameData.CurveElements.Add(curve);
	//FLiveLinkCurveElement curve2;
	//curve2.CurveName = "Root.rotation.z";
	//curve2.CurveValue = 45;
	//FrameData.CurveElements.Add(curve2);

	Client->PushSubjectData(SourceGuid, virtualProductionObject.name, FrameData);
}
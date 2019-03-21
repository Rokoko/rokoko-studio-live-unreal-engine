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

void FVirtualProductionSource::HandleSubjectData(const FName subjectName, const FLiveLinkRefSkeleton skeleton)
{
	ConnectionLastActive = FPlatformTime::Seconds();

	//FPlatformMisc::LowLevelOutputDebugStringf(TEXT("HandleSubjectData %s\n"), *Message.SubjectName);
	/*for (const FString& Name : Message.BoneNames)
	{
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("\tName: %s\n"), *Name);
	}*/
	/*FScopeLock Lock(&GBoneDataCS);

	if (BoneID != (Message.BoneID - 1))
	{
	UE_LOG(LogTemp, Warning, TEXT("BONE ID SKIP Was On:%i Now:%i"), BoneID, Message.BoneID);
	}
	if (BoneNames.Num() == Message.BoneNames.Num() || BoneNames.Num() == 0)
	{
	BoneNames.Reset();
	for (const FString& Name : Message.BoneNames)
	{
	BoneNames.Add(FName(*Name));
	}
	//BoneTransforms.Reset();
	BoneID = Message.BoneID;
	}
	else
	{
	UE_LOG(LogTemp, Warning, TEXT("INVALID BONE NAMES RECIEVED %i != existing %i"), Message.BoneNames.Num(), BoneNames.Num());
	}*/
	UE_LOG(LogTemp, Warning, TEXT("Handle Subject Data!!"));
	UE_LOG(LogTemp, Warning, TEXT("SUBJECT!! %s"), &subjectName);
	//UE_LOG(LogTemp, Warning, TEXT("SKELETON!! "), skeleton);
	subjectNames.Add(subjectName);
	Client->PushSubjectSkeleton(SourceGuid, subjectName, skeleton);
}
	
void FVirtualProductionSource::HandleSubjectFrame(const FName subjectName, const TArray<FLiveLinkCurveElement>& Message)
{
	ConnectionLastActive = FPlatformTime::Seconds();

	//FPlatformMisc::LowLevelOutputDebugString(TEXT("HandleSubjectFrame\n"));
	/*if (BoneID != Message.BoneID)
	{
	UE_LOG(LogTemp, Warning, TEXT("BONE ID MISMATCH Exp:%i Got:%i"), BoneID, Message.BoneID);
	}*/
	/*for (const FTransform& T : Message.Transforms)
	{
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("\tTransform: %s\n"), *T.ToString());
	}*/

	FTransform hardCodedTransform;
	hardCodedTransform.TransformPosition(FVector(20, 20, 200));
	hardCodedTransform.TransformRotation(FQuat::MakeFromEuler(FVector(45, 45, 45)));

	FLiveLinkFrameData FrameData;
	FrameData.Transforms.Add(hardCodedTransform);
	

	//FrameData.Transforms = Message.Transforms;
	//FrameData.CurveElements = Message.Curves;
	//FrameData.MetaData = Message.MetaData;
	FTimer timer;
	
	FLiveLinkWorldTime((double)(timer.GetCurrentTime()));
	Client->PushSubjectData(SourceGuid, subjectName, FrameData);
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtualProductionSource.h"

#include "LiveLinkMessages.h"
#include "ILiveLinkClient.h"
//#include "LiveLinkMessageBusHeartbeatManager.h"

#include "MessageEndpointBuilder.h"

const double LL_CONNECTION_TIMEOUT = 15.0;
const double LL_HALF_CONNECTION_TIMEOUT = LL_CONNECTION_TIMEOUT / 2.0;

void FVirtualProductionSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{

	Client = InClient;
	SourceGuid = InSourceGuid;
	source = this;
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

void FVirtualProductionSource::HandleClearSubject(const FLiveLinkClearSubject& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	ConnectionLastActive = FPlatformTime::Seconds();
	Client->ClearSubject(Message.SubjectName);
}

bool FVirtualProductionSource::RequestSourceShutdown()
{
	//instance = nullptr;
	source = nullptr;
	return true;
}

void FVirtualProductionSource::HandleSubjectData(const FLiveLinkSubjectDataMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
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
	Client->PushSubjectSkeleton(SourceGuid, Message.SubjectName, Message.RefSkeleton);
	
}

void FVirtualProductionSource::HandleSubjectFrame(const FLiveLinkSubjectFrameMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
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

	FLiveLinkFrameData FrameData;
	FrameData.Transforms = Message.Transforms;
	FrameData.CurveElements = Message.Curves;
	FrameData.MetaData = Message.MetaData;
	FrameData.WorldTime = FLiveLinkWorldTime(Message.Time);
	Client->PushSubjectData(SourceGuid, Message.SubjectName, FrameData);
}
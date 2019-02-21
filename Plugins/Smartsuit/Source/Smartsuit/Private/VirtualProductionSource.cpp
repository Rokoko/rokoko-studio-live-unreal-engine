// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtualProductionSource.h"

#include "LiveLinkMessages.h"
#include "ILiveLinkClient.h"
//#include "LiveLinkMessageBusHeartbeatManager.h"
#include "VirtualProductionHeartbeatManage.h"

#include "MessageEndpointBuilder.h"

const double LL_CONNECTION_TIMEOUT = 15.0;
const double LL_HALF_CONNECTION_TIMEOUT = LL_CONNECTION_TIMEOUT / 2.0;

void FVirtualProductionSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;

	MessageEndpoint = FMessageEndpoint::Builder(TEXT("VirtualProductionSource"))
		.Handling<FLiveLinkSubjectDataMessage>(this, &FVirtualProductionSource::HandleSubjectData)
		.Handling<FLiveLinkSubjectFrameMessage>(this, &FVirtualProductionSource::HandleSubjectFrame)
		.Handling<FLiveLinkHeartbeatMessage>(this, &FVirtualProductionSource::HandleHeartbeat)
		.Handling<FLiveLinkClearSubject>(this, &FVirtualProductionSource::HandleClearSubject);


	MessageEndpoint->Send(new FLiveLinkConnectMessage(), ConnectionAddress);

	// Register for heartbeats
	bIsValid = true;
	FVirtualProductionHeartbeatManager::Get()->RegisterSource(this);
}

bool FVirtualProductionSource::SendHeartbeat()
{
	const double CurrentTime = FPlatformTime::Seconds();

	if (HeartbeatLastSent > (CurrentTime - LL_HALF_CONNECTION_TIMEOUT) &&
		ConnectionLastActive < (CurrentTime - LL_CONNECTION_TIMEOUT))
	{
		//We have recently tried to heartbeat and not received anything back
		bIsValid = false;
	}

	MessageEndpoint->Send(new FLiveLinkHeartbeatMessage(), ConnectionAddress);
	HeartbeatLastSent = CurrentTime;
	return bIsValid;
}


bool FVirtualProductionSource::IsSourceStillValid()
{
	return bIsValid;
}

void FVirtualProductionSource::HandleHeartbeat(const FLiveLinkHeartbeatMessage& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	ConnectionLastActive = FPlatformTime::Seconds();
}

void FVirtualProductionSource::HandleClearSubject(const FLiveLinkClearSubject& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	ConnectionLastActive = FPlatformTime::Seconds();
	Client->ClearSubject(Message.SubjectName);
}

bool FVirtualProductionSource::RequestSourceShutdown()
{
	FVirtualProductionHeartbeatManager* HeartbeatManager = FVirtualProductionHeartbeatManager::Get();
	if (HeartbeatManager->IsRunning())
	{
		HeartbeatManager->RemoveSource(this);
	}
	FMessageEndpoint::SafeRelease(MessageEndpoint);
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
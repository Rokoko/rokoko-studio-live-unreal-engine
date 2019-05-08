// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "ILiveLinkClient.h"
#include "ILiveLinkSource.h"
#include "Runtime/RenderCore/Public/RenderCore.h"
#include "MessageEndpoint.h"
#include "VirtualProductionFrame.h"

class ILiveLinkClient;
struct FLiveLinkPongMessage;
struct FLiveLinkSubjectDataMessage;
struct FLiveLinkSubjectFrameMessage;
struct FLiveLinkHeartbeatMessage;
struct FLiveLinkClearSubject;

/**
 * 
 */
class SMARTSUIT_API FVirtualProductionSource : public ILiveLinkSource
{
public:
	FVirtualProductionSource(const FText& InSourceType, const FText& InSourceMachineName, const FMessageAddress& InConnectionAddress)
		: SourceType(InSourceType)
		, SourceMachineName(InSourceMachineName)
		
	{}

	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid);
	virtual bool IsSourceStillValid();
	virtual bool RequestSourceShutdown();

	virtual FText GetSourceType() const { return SourceType; }
	virtual FText GetSourceMachineName() const { return SourceMachineName; }
	virtual FText GetSourceStatus() const { return SourceStatus; }
	static FVirtualProductionSource* Get() {
		return instance;
	}

	void HandleSubjectFrame(TArray<FVirtualProductionSubject> virtualProductionObject);
	void HandleFace(TArray<FFace> faces);
	void ClearAllSubjects();

private:
	void HandleClearSubject(const FName subjectName);
	void HandleSubjectData(FVirtualProductionSubject virtualProductionObject);
	void HandleFaceData(FFace face);
	TArray<FName> subjectNames;
	TArray<FName> faceNames;
	TArray<FVirtualProductionSubject> existingSubjects;
	TArray<FFace> existingFaces;
	TArray<FName> notExistingSubjects;

	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	//singleton instance
	static FVirtualProductionSource *instance;

};

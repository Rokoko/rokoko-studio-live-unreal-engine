// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "ILiveLinkClient.h"
#include "ILiveLinkSource.h"
#include "Runtime/RenderCore/Public/RenderCore.h"
#include "MessageEndpoint.h"
#include "VirtualProductionFrame.h"
#include "SmartsuitDefinitions.h"

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


	virtual ~FVirtualProductionSource();

	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid);
	virtual bool IsSourceStillValid() const override;
	virtual bool RequestSourceShutdown();

	virtual FText GetSourceType() const { return SourceType; }
	virtual FText GetSourceMachineName() const { return SourceMachineName; }
	virtual FText GetSourceStatus() const { return SourceStatus; }
	static TSharedPtr<FVirtualProductionSource> Get() { return instance; }

	void HandleSubjectFrame(TArray<FVirtualProductionSubject> virtualProductionObject);
	void HandleFace(TArray<FFace> faces);
	void HandleSuits(TArray<SuitData> suits);
	void ClearAllSubjects();

	static void SetInstance(TSharedPtr<FVirtualProductionSource> NewInstance) { instance = NewInstance; }
private:
	void HandleClearSubject(const FName subjectName);
	void HandleSubjectData(FVirtualProductionSubject virtualProductionObject);
	void HandleFaceData(FFace face);
	void HandleSuitData(SuitData suit);
	void CreateJoint(TArray<FTransform>& transforms, int32 index, Sensor* parent, Sensor* sensor);
	TArray<FName> subjectNames;
	TArray<FName> faceNames;
	TArray<FName> suitNames;

	TArray<FVirtualProductionSubject> existingSubjects;
	TArray<FFace> existingFaces;
	TArray<SuitData> existingSuits;

	TArray<FName> notExistingSubjects;

	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	//singleton instance
	//static FVirtualProductionSource *instance;
	static TSharedPtr<FVirtualProductionSource> instance;

};

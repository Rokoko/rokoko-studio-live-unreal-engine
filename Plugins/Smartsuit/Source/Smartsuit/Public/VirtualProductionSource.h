// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "ILiveLinkClient.h"
#include "ILiveLinkSource.h"
#include "Runtime/RenderCore/Public/RenderCore.h"
#include "MessageEndpoint.h"
#include "VirtualProductionFrame.h"
#include "RokokoSkeletonData.h"
#include "Engine/DataAsset.h"
#include "LiveLinkRemapAsset.h"


#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include <mutex>
#include "VirtualProductionFrame.h"
#include "VirtualProductionSourceSettings.h"

//#include "VirtualProductionSource.generated.h"


class ILiveLinkClient;
struct FLiveLinkPongMessage;
struct FLiveLinkSubjectDataMessage;
struct FLiveLinkSubjectFrameMessage;
struct FLiveLinkHeartbeatMessage;
struct FLiveLinkClearSubject;


/**
 * reading udp packets in separate thread and output extracted data into static and dynamic data of different roles
 */
class SMARTSUIT_API FVirtualProductionSource : public ILiveLinkSource, public FRunnable
{
public:
	FVirtualProductionSource(FIPv4Endpoint address, const FText& InSourceType, const FText& InSourceMachineName, const FMessageAddress& InConnectionAddress);
	
	FVirtualProductionSource()
	{
		Client = nullptr;
	}

	virtual ~FVirtualProductionSource();

	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid);
	virtual void InitializeSettings(ULiveLinkSourceSettings* Settings) override;

	virtual bool IsSourceStillValid() const override;
	virtual bool RequestSourceShutdown();

	virtual FText GetSourceType() const { return SourceType; }
	virtual FText GetSourceMachineName() const { return SourceMachineName; }
	virtual FText GetSourceStatus() const { return SourceStatus; }
	

	void HandleSubjectFrame(const TArray<FVirtualProductionSubject>& virtualProductionObject);
	void HandleFace(const TArray<FFace>& faces);
	void HandleSuits(const TArray<FSuitData>& suits);
	void HandleCharacters(const TArray<FCharacterData>& characters);
	void HandleNewtons(const TArray<FNewtonData>& newtons);
	void ClearAllSubjects();

	static void SetInstance(TSharedPtr<FVirtualProductionSource> NewInstance) { instance = NewInstance; }
	static TSharedPtr<FVirtualProductionSource> Get();
	static TSharedPtr<FVirtualProductionSource> CreateLiveLinkSource();
	static void RemoveLiveLinkSource(TSharedPtr<FVirtualProductionSource> InSource);

	virtual TSubclassOf<ULiveLinkSourceSettings> GetSettingsClass() const override { return UVirtualProductionSourceSettings::StaticClass(); }
	virtual void OnSettingsChanged(ULiveLinkSourceSettings* Settings, const FPropertyChangedEvent& PropertyChangedEvent) override;
private:
	void HandleClearSubject(const FName subjectName);
	void HandleSubjectData(const FVirtualProductionSubject& virtualProductionObject);
	void HandleFaceData(const FFace& face);
	void HandleSuitData(const FSuitData& suit);
	void HandleCharacterData(const FCharacterData& character);
	void HandleNewtonData(const FNewtonData& newton);
	void CreateJoint(TArray<FTransform>& transforms, int32 index, const FSmartsuitBone* parent, const FSmartsuitBone* sensor);

	TArray<FName> subjectNames;
	TArray<FName> faceNames;
	TArray<FName> actorNames;
	TArray<FName> characterNames;
	TArray<FName> newtonNames;

	TArray<FVirtualProductionSubject> existingSubjects;
	TArray<FFace> existingFaces;
	TArray<FSuitData> existingActors;
	TArray<FCharacterData> existingCharacters;
	TArray<FNewtonData> existingNewtons;

	TArray<FName> notExistingSubjects;

	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	//singleton instance
	static TSharedPtr<FVirtualProductionSource> instance;

	// Pointer to the settings for this source so we don't have to duplicate data
	UVirtualProductionSourceSettings* SavedSourceSettings = nullptr;

public:
	/// @private
	virtual bool InitSocket();

	/// @private
	virtual uint32 Run() override;

	/**
	* Checks whether the listener is listening for incoming connections.
	*
	* @return true if it is listening, false otherwise.
	*/
	bool IsActive() const
	{
		return (Running);
	}

	/**
	* Stop the Socket and the Thread which handles Smartsuit data streams.
	*/
	virtual void Stop() override
	{
		Running = false;
	}

	/**
	* Starts the socket and the thread which handles Smarttsuit data streams.
	*
	* @param port The port number to bind.
	*/
	void StartRunnable();

	/// @private
	virtual void Exit() override { }

	FProp* GetPropByName(FString name, bool isLive);
	TArray<FProp> GetAllProps();

	FTracker* GetTrackerByName(FString name, bool isLive);
	FTracker* GetTrackerByConnectionID(const FString& name, bool isLive);
	TArray<FTracker> GetTrackersWithMatchingId(FString name, bool isLive);

	FSuitData* GetSmartsuitByName(FString suitName);
	TArray<FString> GetAvailableSmartsuitNames();
	TArray<FSuitData> GetAllSmartsuits();

	FFace GetFaceByFaceID(FString faceID);
	FFace* GetFaceByProfileName(const FString& profileName);
	TArray<FFace> GetAllFaces();

	// TODO: GetAllCharacters

private:
	std::mutex mtx;
	
	// network receiver data
	FIPv4Endpoint			m_NetworkAddress;
	bool					m_NetworkBindAnyAddress;
	bool					m_NetworkBroadcast;

	FSocket* Socket{ nullptr };
	/** Used to tell that the thread is stopping */
	bool Running;
	
	/** Connection thread, used to not block the editor when waiting for connections */
	FRunnableThread* Thread{ nullptr };
	FVirtualProductionFrame GlobalVPFrame;
	TArray<FVirtualProductionSubject> Subjects;
	
};

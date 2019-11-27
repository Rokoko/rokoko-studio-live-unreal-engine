// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
//#include "Runtime/Core/Public/Unix/UnixCriticalSection.h"
#include <mutex>
#include "VirtualProductionFrame.h"
#include "VirtualProductionSource.h"
#define DEFAULT_ENDPOINT FIPv4Endpoint(FIPv4Address(127, 0, 0, 1), 5005)
//
//#include "ILiveLinkSource.h"
//#include "MessageEndpoint.h"
//#include "IMessageContext.h"
//#include "HAL/ThreadSafeBool.h"

class ILiveLinkClient;
struct FLiveLinkPongMessage;
struct FLiveLinkSubjectDataMessage;
struct FLiveLinkSubjectFrameMessage;
struct FLiveLinkHeartbeatMessage;
struct FLiveLinkClearSubject;


struct FLiveLinkSubjectDataMessage;
struct FLiveLinkClearSubject;

class VPStreamingNetwork : public FRunnable
{
public:
	VPStreamingNetwork();
	~VPStreamingNetwork();

	/// @private
	virtual bool Init() override;

	/// @private
	virtual uint32 Run() override;

	/**
	* Checks whether the listener is listening for incoming connections.
	*
	* @return true if it is listening, false otherwise.
	*/
	bool IsActive() const
	{
		return (!Stopping);
	}

	/**
	* Stop the Socket and the Thread which handles Smartsuit data streams.
	*/
	virtual void Stop() override
	{
		Stopping = true;
	}

	/**
	* Starts the socket and the thread which handles Smarttsuit data streams.
	*
	* @param port The port number to bind.
	*/
	void Start(int port);

	/// @private
	virtual void Exit() override { }

	FProp* GetPropByName(FString name, bool isLive);
	FTracker* GetTrackerByName(FString name, bool isLive);

	TArray<FProp> GetAllProps() {
		//return nullptr;
		TArray<FProp> result;
		mtx.lock();
		if (GlobalVPFrame) {
			for (int i = 0; i < GlobalVPFrame->props.Num(); i++) {
				result.Add(GlobalVPFrame->props[i]);
				//result->Add
			}
		}
		mtx.unlock();
		//UE_LOG(LogTemp, Display, TEXT("Yeeee3"));
		return result;
	}

private:
	std::mutex mtx;
	//FPThreadsCriticalSection FCriticalSection;
	int streaming_port;
	FSocket *Socket = NULL;
	/** Used to tell that the thread is stopping */
	bool Stopping;
	//FVirtualProductionSource* livelink = FVirtualProductionSource::Get();
	//TArray <FName> subjectNames;
	/** Connection thread, used to not block the editor when waiting for connections */
	FRunnableThread* Thread = NULL;
	FVirtualProductionFrame *GlobalVPFrame;
	TSharedPtr<FVirtualProductionSource> GetLiveLink() { return FVirtualProductionSource::Get(); }
	TArray<FVirtualProductionSubject> subjects;
	void SendToLiveLink(TArray<FVirtualProductionSubject> Subjects);
	void SendFacesToLivelink(TArray<FFace> Subjects);
};
/// @endcond


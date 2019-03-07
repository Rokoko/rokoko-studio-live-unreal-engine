// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
//#include "Runtime/Core/Public/Unix/UnixCriticalSection.h"
#include <mutex>

#include "VirtualProductionFrame.h"

#define DEFAULT_ENDPOINT FIPv4Endpoint(FIPv4Address(127, 0, 0, 1), 5005)

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
	//FVirtualProductionFrame* GetVirtualProductionFrame() {
	//	FVirtualProductionFrame* newInstance = new FVirtualProductionFrame();
	//	mtx.lock();
	//	// copy GlobalVPFrame to newInstance
	//	memcpy(&newInstance, &GlobalVPFrame, sizeof(GlobalVPFrame));
	//	mtx.unlock();
	//	return newInstance;
	//	//return GlobalVPFrame;
	//}

	TArray<FProp> GetAllProps() {
		//return nullptr;
		TArray<FProp> result;
		UE_LOG(LogTemp, Display, TEXT("Yeeee1.1")); 
		mtx.lock();
		if (GlobalVPFrame) {
			for (int i = 0; i < GlobalVPFrame->props.Num(); i++) {
				result.Add(GlobalVPFrame->props[i]);
				//result->Add
			}
		}
		UE_LOG(LogTemp, Display, TEXT("Yeeee2.1 %d"), result.Num());
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

	/** Connection thread, used to not block the editor when waiting for connections */
	FRunnableThread* Thread = NULL;
	FVirtualProductionFrame *GlobalVPFrame;
};
/// @endcond


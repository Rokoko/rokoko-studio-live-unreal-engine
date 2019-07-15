// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

//#include "Networking.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "VirtualProductionSource.h"
#include "VirtualProductionFrame.h"

#include "SmartsuitDefinitions.h"
// Default endpoint for the server
#define DEFAULT_ENDPOINT FIPv4Endpoint(FIPv4Address(127, 0, 0, 1), 5005)
class ILiveLinkClient;
struct FLiveLinkPongMessage;
struct FLiveLinkSubjectDataMessage;
struct FLiveLinkSubjectFrameMessage;
struct FLiveLinkHeartbeatMessage;
struct FLiveLinkClearSubject;


struct FLiveLinkSubjectDataMessage;
struct FLiveLinkClearSubject;



/// @cond doc_hidden
/**
* Network implementation for receiving live stream from Smartsuits.
*/
class SMARTSUIT_API SmartsuitStreamingNetwork : public FRunnable
{
public:
	SmartsuitStreamingNetwork();
	~SmartsuitStreamingNetwork();


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

	/**
	* The buffer which holds information about all active Smartsuits. The buffer currently has size for 10 Smartsuits.
	*/
	SuitData *suits;

	TSet<FString> wifiSupportedSuits;

private:
	int streaming_port;
	FSocket *Socket = NULL;

	/** Used to tell that the thread is stopping */
	bool Stopping;

	/** Connection thread, used to not block the editor when waiting for connections */
	FRunnableThread* Thread = NULL;
	void SendSuitsToLiveLink();
	int suitCount;
};
/// @endcond
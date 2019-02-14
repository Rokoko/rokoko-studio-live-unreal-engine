// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once
#include "Smartsuit.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Common/UdpSocketBuilder.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "SmartsuitDefinitions.h"
//#include "RokokoActor.h"
// Default endpoint for the server
#define DEFAULT_ENDPOINT FIPv4Endpoint(FIPv4Address(127, 0, 0, 1), 5005)



/// @cond doc_hidden
struct BodyResponse {
	Body body;
	uint32 url;
	bool valid;
};




class SMARTSUIT_API SmartsuitCommandNetwork : public FRunnable
{
public:
	SmartsuitCommandNetwork();
	~SmartsuitCommandNetwork();
	/** FRunnable init */
	virtual bool Init() override;

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

	virtual void Stop() override
	{
		Stopping = true;
	}

	void Start(int port);

	/**
	* Sends a command to the Smartsuit.
	*/
	void SendCommand(uint8 *cmd, uint32 url, int length);

	virtual void Exit() override { }
	BodyResponse *bodyResponses;
	TMap<uint32, HubInfo> hubInfoResponses;

private:
	int streaming_port;
	class FSocket *Socket = NULL;

	/** Used to tell that the thread is stopping */
	bool Stopping;

	/** Connection thread, used to not block the editor when waiting for connections */
	FRunnableThread* Thread = NULL;

	int bufferSize = 100;

};
/// @endcond
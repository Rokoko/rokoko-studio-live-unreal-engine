// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/CriticalSection.h"
#include "VirtualProductionSource.h"
#include "Containers/Set.h"

class FVirtualProductionSource;

// A class to asynchronously send heartbeats to a set of message bus sources
class FVirtualProductionHeartbeatManager : FRunnable
{
public:
	// Singleton access to the Heartbeat Manager
	static FVirtualProductionHeartbeatManager* Get();

	~FVirtualProductionHeartbeatManager();

	// Begin FRunnable interface

	virtual uint32 Run() override;

	virtual void Stop() override;

	// End FRunnable interface

	void RegisterSource(FVirtualProductionSource* const InSource);
	void RemoveSource(FVirtualProductionSource* const InSource);

	bool IsRunning() const;

private:
	FVirtualProductionHeartbeatManager();

	// Singleton instance
	static FVirtualProductionHeartbeatManager* Instance;

	// Thread safe bool for stopping the thread
	FThreadSafeBool bRunning;

	// Thread the heartbeats are sent on
	FRunnableThread* Thread;

	// Critical section for accessing the Source Set
	FCriticalSection SourcesCriticalSection;

	// Set of sources we are currently sending heartbeats for
	TSet<FVirtualProductionSource*> VPSources;
};
// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtualProductionHeartbeatManage.h"
#include "LiveLinkMessageBusSource.h"
#include "Misc/ScopeLock.h"
#include "Async/Async.h"

#define LL_HEARTBEAT_SLEEP_TIME 1.0f

FVirtualProductionHeartbeatManager* FVirtualProductionHeartbeatManager::Instance = nullptr;

FVirtualProductionHeartbeatManager::FVirtualProductionHeartbeatManager() :
	bRunning(true)
{
	Thread = FRunnableThread::Create(this, TEXT("MessageBusHeartbeatManager"));
}

FVirtualProductionHeartbeatManager::	~FVirtualProductionHeartbeatManager()
{
	Stop();
	if (Thread)
	{
		Thread->WaitForCompletion();
	}
};

FVirtualProductionHeartbeatManager* FVirtualProductionHeartbeatManager::Get()
{
	if (Instance == nullptr)
	{
		Instance = new FVirtualProductionHeartbeatManager();
	}
	return Instance;
}

uint32 FVirtualProductionHeartbeatManager::Run()
{
	while (bRunning)
	{
		{
			FScopeLock Lock(&SourcesCriticalSection);

			TArray<FVirtualProductionSource*> SourcesToRemove;

			// Loop through all sources and send heartbeat
			for (const auto& MessageBusSource : MessageBusSources)
			{
				if (MessageBusSource != nullptr)
				{
					//bool bStillValid = MessageBusSource->IsSourceStillValid();
					//if (bStillValid)
					//{
					//	bStillValid = MessageBusSource->SendHeartbeat();
					//}

					//if (!bStillValid)
					//{
					//	// Source is invalid, queue it for removal
					//	SourcesToRemove.Emplace(MessageBusSource);
					//}
				}
			}

			// Remove any dead sources
			for (const auto& SourceToRemove : SourcesToRemove)
			{
				MessageBusSources.Remove(SourceToRemove);
			}
		}
		FPlatformProcess::Sleep(LL_HEARTBEAT_SLEEP_TIME);
	}
	return 0;
};

void FVirtualProductionHeartbeatManager::Stop()
{
	bRunning = false;
}

void FVirtualProductionHeartbeatManager::RegisterSource(FVirtualProductionSource* const InSource)
{
	FScopeLock Lock(&SourcesCriticalSection);

	MessageBusSources.Add(InSource);
};

void FVirtualProductionHeartbeatManager::RemoveSource(FVirtualProductionSource* const InSource)
{
	FScopeLock Lock(&SourcesCriticalSection);

	MessageBusSources.Remove(InSource);
};

bool FVirtualProductionHeartbeatManager::IsRunning() const
{
	return bRunning;
};
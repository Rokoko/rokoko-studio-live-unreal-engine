// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VPStreamingNetwork.h"
#include "VirtualProductionFrame.h"
#include "Smartsuit.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
//#include "Engine.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"


VPStreamingNetwork::VPStreamingNetwork()
{
}


VPStreamingNetwork::~VPStreamingNetwork()
{
	// Stop the runnable
	Stop();
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid()) 
	{
		livelink->ClearAllSubjects();
	}
	//livelink->HandleClearSubject(FLiveLinkClearSubject(FString("subjectNames")));

	if (Socket) 
	{
		Socket->Close();
	}

	// And last but not least stop the main thread
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}


void VPStreamingNetwork::Start(int port) 
{
	streaming_port = port;
	FString ThreadName(FString::Printf(TEXT("VPStreamingNetwork%ld"), (long)(FDateTime::UtcNow().ToUnixTimestamp())));
	if (streaming_port) 
	{
		UE_LOG(LogTemp, Warning, TEXT("VP port... %i"), streaming_port);
	}
	Thread = FRunnableThread::Create(this, *ThreadName, 8 * 1024, TPri_Normal);
}


bool VPStreamingNetwork::Init()
{
	if (Socket == NULL)
	{
		Socket = FUdpSocketBuilder(TEXT("VPStreamingNetwork")).BoundToAddress(FIPv4Address(0, 0, 0, 0)).BoundToPort(streaming_port).AsReusable().Build();
		//allow the socket to listen to an already bounded address.
		Socket->SetReuseAddr(true);
		Stopping = false;
	}
	return true;
}

FString BytesToStringFixed(const uint8 *In, int32 Count)
{
	FString Broken = BytesToString(In, Count);
	FString Fixed;

	for (int i = 0; i < Broken.Len(); i++)
	{
		const TCHAR c = Broken[i] - 1;
		Fixed.AppendChar(c);
	}

	return Fixed;
}

void VPStreamingNetwork::SendToLiveLink(TArray<FVirtualProductionSubject> Subjects) 
{
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid()) 
	{
		livelink->HandleSubjectFrame(Subjects);
	}
}

void VPStreamingNetwork::SendFacesToLivelink(TArray<FFace> Subjects) 
{
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid()) 
	{
		livelink->HandleFace(Subjects);
	}
}

uint32 VPStreamingNetwork::Run()
{
	bool added = false;
	while (!Stopping)
	{
		auto addr_in = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		int32 bytes_read = 0;
		FString ret;

		uint8 data[8192];
		memset(data, '\0', 8192);

		FDateTime time = FDateTime::UtcNow();
		int seconds = time.ToUnixTimestamp();
		if (Socket) 
		{
			if (Socket->RecvFrom(data, sizeof(data), bytes_read, *addr_in)) 
			{
				if (bytes_read == 0) 
				{
					continue;
				}
				//FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
				//{
				if (Stopping) break;
				mtx.lock();
				FVirtualProductionFrame VPFrame;
				FString result = BytesToStringFixed(data, static_cast<int32_t>(bytes_read));
				//UE_LOG(LogTemp, Warning, TEXT("received: %s"), *result);
				//FJsonObjectConverter::JsonObjectStringToUStruct(result, &VPFrame, 0, 0);

				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(result);
				if (FJsonSerializer::Deserialize(Reader, JsonObject))
				{

					VPFrame.version = JsonObject->GetIntegerField("version");
					//VPFrame.timestamp = JsonObject->GetNumberField("timestamp");
					//VPFrame.playbackTimestamp = JsonObject->GetNumberField("");

					TArray<TSharedPtr<FJsonValue>> propsarray = JsonObject->GetArrayField("props");

					for (auto& currentprop : propsarray)
					{
						VPFrame.props.Add(FProp(currentprop->AsObject()));
					}

					TArray<TSharedPtr<FJsonValue>> trackersarray = JsonObject->GetArrayField("trackers");

					for (auto& currenttracker : trackersarray)
					{
						VPFrame.trackers.Add(FTracker(currenttracker->AsObject()));
					}

					TArray<TSharedPtr<FJsonValue>> facesarray = JsonObject->GetArrayField("faces");

					for (auto& currentface : facesarray)
					{
						VPFrame.faces.Add(FFace(currentface->AsObject()));
					}
				}

				
				if (!GlobalVPFrame) 
				{
					GlobalVPFrame = new FVirtualProductionFrame();
				}
				GlobalVPFrame->version = VPFrame.version;
				GlobalVPFrame->props.Empty();
				GlobalVPFrame->trackers.Empty();
				GlobalVPFrame->faces.Empty();

				auto livelink = FVirtualProductionSource::Get();

				if (livelink.IsValid()) 
				{
					subjects.Empty();
					for (int i = 0; i < VPFrame.props.Num(); i++) 
					{
						GlobalVPFrame->props.Add(VPFrame.props[i]);
						FVirtualProductionSubject subject = GlobalVPFrame->props[i].GetSubject();
						subjects.Add(subject);
					}
					for (int i = 0; i < VPFrame.trackers.Num(); i++) 
					{
						GlobalVPFrame->trackers.Add(VPFrame.trackers[i]);
						FVirtualProductionSubject subject = GlobalVPFrame->trackers[i].GetSubject();
						subjects.Add(subject);
					}
					for (int i = 0; i < VPFrame.faces.Num(); i++) 
					{
						GlobalVPFrame->faces.Add(VPFrame.faces[i]);
					}
					SendToLiveLink(subjects);
					SendFacesToLivelink(GlobalVPFrame->faces);
				}
				else 
				{
					for (int i = 0; i < VPFrame.props.Num(); i++) 
					{
						GlobalVPFrame->props.Add(VPFrame.props[i]);
					}
					for (int i = 0; i < VPFrame.trackers.Num(); i++) 
					{
						GlobalVPFrame->trackers.Add(VPFrame.trackers[i]);
					}
					for (int i = 0; i < VPFrame.faces.Num(); i++) 
					{
						//UE_LOG(LogTemp, Warning, TEXT("face: %d - %s - %f"), VPFrame.faces[i].version, *VPFrame.faces[i].provider, VPFrame.faces[i].jawOpen);
						GlobalVPFrame->faces.Add(VPFrame.faces[i]);
					}
				}
				//UE_LOG(LogTemp, Warning, TEXT("Faces... %i"), GlobalVPFrame->faces.Num());
				mtx.unlock();
				//}, TStatId(), NULL, ENamedThreads::GameThread);

				// If you want to wait for the code above to complete do this:
				//FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

			}
		}
	}
	return 0;
}

FProp* VPStreamingNetwork::GetPropByName(FString name, bool isLive)
{
	FProp *result = nullptr;
	mtx.lock();
	if (GlobalVPFrame) 
	{
		for (int i = 0; i < GlobalVPFrame->props.Num(); i++) 
		{
			if (name == GlobalVPFrame->props[i].name && GlobalVPFrame->props[i].isLive == isLive)
			{
				result = &GlobalVPFrame->props[i];
			}
		}
	}
	mtx.unlock();
	return result;
}

FTracker* VPStreamingNetwork::GetTrackerByName(FString name, bool isLive)
{
	FTracker *result = nullptr;
	mtx.lock();
	if (GlobalVPFrame) 
	{
		for (int i = 0; i < GlobalVPFrame->trackers.Num(); i++) 
		{
			if (name == GlobalVPFrame->trackers[i].name && GlobalVPFrame->trackers[i].isLive == isLive)
			{
				result = &GlobalVPFrame->trackers[i];
			}
		}
	}
	mtx.unlock();
	return result;
}
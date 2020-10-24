// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "VPStreamingNetwork.h"
#include "VirtualProductionFrame.h"
#include "Smartsuit.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
//#include "Engine.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Serialization/BufferArchive.h"
#include "lz4frame.h"

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
	Thread = FRunnableThread::Create(this, *ThreadName, 512 * 1024, TPri_Normal);
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

void VPStreamingNetwork::SendSuitsToLiveLink(TArray<FSuitData> Smartsuits)
{
	auto livelink = FVirtualProductionSource::Get();
	if (livelink.IsValid())
	{
		livelink->HandleSuits(Smartsuits);
	}
}

PRAGMA_DISABLE_OPTIMIZATION
uint32 VPStreamingNetwork::Run()
{
	static LZ4F_decompressionContext_t g_dCtx;
	LZ4F_createDecompressionContext(&g_dCtx, LZ4F_VERSION);
	bool added = false;
	while (!Stopping)
	{
		auto addr_in = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		int32 bytes_read = 0;
		FString ret;

		uint8 data[32768];
		memset(data, '\0', 32768);

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

				int32 UncompressedSize = 2097152;

				TArray<uint8> UncompressedData;
				UncompressedData.Empty(UncompressedSize);
				UncompressedData.AddUninitialized(UncompressedSize);

				//FName FormatName = NAME_LZ4;
				//if (!FCompression::UncompressMemory(FormatName, UncompressedData.GetData(), UncompressedSize, data, static_cast<int32_t>(bytes_read)))
				//{
				//	UE_LOG(LogTemp, Error, TEXT("FCompression::UncompressMemory - Failed to uncompress memory (%d/%d) from address %p using format %s, this may indicate the asset is corrupt!"), bytes_read, UncompressedSize, data, *FormatName.ToString());
				//}

				
				//IFileManager* FileManager = &IFileManager::Get();
				uint32 WriteFlags = 0;

				//FString path = "C:/GitHub/rokoko-studio-live-unreal-engine_JSONV3/test12345678.lz4";
				//TUniquePtr<FArchive> Ar = TUniquePtr<FArchive>(FileManager->CreateFileWriter(*path, WriteFlags));
				//if (!Ar)
				//{
				//	UE_LOG(LogTemp, Error, TEXT("Could not write file %s!"), bytes_read, UncompressedSize, data, *FormatName.ToString());
				//}
				//Ar->Serialize(const_cast<uint8*>(data), bytes_read);
				//Ar->Close();


				size_t srcSize = (size_t)bytes_read;
				size_t dstSize = (size_t)UncompressedData.Num();
				size_t decompressResult;
				verify(srcSize >= 0);
				verify(dstSize >= 0);
				decompressResult = LZ4F_decompress(g_dCtx, UncompressedData.GetData(), &dstSize, data, &srcSize, NULL);
				if (decompressResult != 0) { UE_LOG(LogTemp, Error, TEXT("Error decompressing frame : unfinished frame \n")); }
				if (srcSize != (size_t)bytes_read) { UE_LOG(LogTemp, Error, TEXT("Error decompressing frame : read size incorrect \n")); }


				//FString path2 = "C:/GitHub/rokoko-studio-live-unreal-engine_JSONV3/3actorstest.txt";
				//TUniquePtr<FArchive> Ar2 = TUniquePtr<FArchive>(FileManager->CreateFileWriter(*path2, WriteFlags));
				//if (!Ar2)
				//{
				//	UE_LOG(LogTemp, Error, TEXT("Could not write file %s!"), *path2);
				//}
				//Ar2->Serialize(UncompressedData.GetData(), dstSize);
				//Ar2->Close();




				FString result = BytesToStringFixed(UncompressedData.GetData(), static_cast<int32_t>(dstSize));
				FString test = BytesToStringFixed(data, static_cast<int32_t>(bytes_read));
				//FString result = BytesToString(UncompressedData.GetData(), static_cast<int32_t>(UncompressedData.Num()));
				//FString result = BytesToStringFixed(UncompressedData.GetData(), UncompressedData.Num());
				//FString result = BytesToStringFixed(data, static_cast<int32_t>(bytes_read));
				//UE_LOG(LogTemp, Warning, TEXT("received: %s"), *result);
				//FJsonObjectConverter::JsonObjectStringToUStruct(result, &VPFrame, 0, 0);

				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(result);
				if (FJsonSerializer::Deserialize(Reader, JsonObject))
				{

					VPFrame.version = JsonObject->GetIntegerField("version");
					//VPFrame.timestamp = JsonObject->GetNumberField("timestamp");
					//VPFrame.playbackTimestamp = JsonObject->GetNumberField("");

					//LIVE
					{
						TSharedPtr<FJsonObject> LiveObj = JsonObject->GetObjectField("live");
						TArray<TSharedPtr<FJsonValue>> Livepropsarray = LiveObj->GetArrayField("props");

						for (auto& currentprop : Livepropsarray)
						{
							VPFrame.props.Add(FProp(true, currentprop->AsObject()));
						}

						if (LiveObj->HasField("actors"))
						{
							TArray<TSharedPtr<FJsonValue>> Livesuitsarray = LiveObj->GetArrayField("actors");
							for (auto& currentsuit : Livesuitsarray)
							{
								auto SuitData = FSuitData(true, currentsuit->AsObject());
								if (SuitData.hasFace)
								{
									auto JSONObjectface = currentsuit->AsObject()->GetObjectField("face");
									auto FaceData = FFace(JSONObjectface, SuitData.suitname);
									SuitData.faceId = FaceData.faceId;
									VPFrame.faces.Add(FaceData);
								}
								VPFrame.suits.Add(SuitData);
							}
						}
					}

					//PLAYBACK
					{
						TSharedPtr<FJsonObject> PlaybackObj = JsonObject->GetObjectField("playback");
						TArray<TSharedPtr<FJsonValue>> Playbackpropsarray = PlaybackObj->GetArrayField("props");

						for (auto& currentprop : Playbackpropsarray)
						{
							VPFrame.props.Add(FProp(false, currentprop->AsObject()));
						}

						if (PlaybackObj->HasField("actors"))
						{
							TArray<TSharedPtr<FJsonValue>> Playbacksuitsarray = PlaybackObj->GetArrayField("actors");
							for (auto& currentsuit : Playbacksuitsarray)
							{
								auto SuitData = FSuitData(false, currentsuit->AsObject());
								if (SuitData.hasFace)
								{
									auto JSONObjectface = currentsuit->AsObject()->GetObjectField("face");
									auto FaceData = FFace(JSONObjectface, SuitData.suitname);
									SuitData.faceId = FaceData.faceId;
									VPFrame.faces.Add(FaceData);
								}
								VPFrame.suits.Add(SuitData);
							}
						}
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
				GlobalVPFrame->suits.Empty();

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
					for (int i = 0; i < VPFrame.suits.Num(); i++)
					{
						GlobalVPFrame->suits.Add(VPFrame.suits[i]);
					}
					SendToLiveLink(subjects);
					SendFacesToLivelink(GlobalVPFrame->faces);
					SendSuitsToLiveLink(GlobalVPFrame->suits);
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
					for (int i = 0; i < VPFrame.suits.Num(); i++)
					{
						GlobalVPFrame->suits.Add(VPFrame.suits[i]);
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
PRAGMA_ENABLE_OPTIMIZATION
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

TArray<FProp> VPStreamingNetwork::GetAllProps()
{
	//return nullptr;
	TArray<FProp> result;
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->props.Num(); i++)
		{
			result.Add(GlobalVPFrame->props[i]);
			//result->Add
		}
	}
	mtx.unlock();
	//UE_LOG(LogTemp, Display, TEXT("Yeeee3"));
	return result;

	//return GlobalVPFrame->props;
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

FTracker* VPStreamingNetwork::GetTrackerByConnectionID(const FString& name, bool isLive)
{
	FTracker* result = nullptr;
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->trackers.Num(); i++)
		{
			if (name == GlobalVPFrame->trackers[i].connectionId /*&& GlobalVPFrame->trackers[i].isLive == isLive*/)
			{
				result = &GlobalVPFrame->trackers[i];
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FTracker> VPStreamingNetwork::GetTrackersWithMatchingId(FString ConnectionId, bool isLive)
{
	TArray<FTracker> result;
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->trackers.Num(); i++)
		{
			if (ConnectionId == GlobalVPFrame->trackers[i].connectionId /*&& GlobalVPFrame->trackers[i].isLive == isLive*/)
			{
				result.Add(GlobalVPFrame->trackers[i]);
			}
		}
	}
	mtx.unlock();
	return result;
}

FSuitData* VPStreamingNetwork::GetSmartsuitByName(FString suitName)
{
	if (suitName.Len() == 0 || suitName.Compare(FString("")) == 0)
	{
		return nullptr;
	}

	FSuitData* result = nullptr;
	mtx.lock();
	if (GlobalVPFrame)
	{
		//should probably set the limit to the size of the suit array here?
		for (int i = 0; i < GlobalVPFrame->suits.Num(); i++)
		{
			FString mySuitName(GlobalVPFrame->suits[i].suitname);
			if (suitName.Compare(mySuitName) == 0 && mySuitName.Len() > 0)
			{
				result = &(GlobalVPFrame->suits[i]);
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FString> VPStreamingNetwork::GetAvailableSmartsuitNames()
{
	TArray<FString> result;
	//maybe we should set the limit to the size of the suits array
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->suits.Num(); i++)
		{
			if ((GlobalVPFrame->suits[i].suitname != "\0\0\0\0") /*&& GlobalVPFrame->suits[i].fps > 0*/)
			{
				result.Add(FString(GlobalVPFrame->suits[i].suitname));
			}
		}
	}
	mtx.unlock();
	return result;
}

TArray<FSuitData> VPStreamingNetwork::GetAllSmartsuits()
{
	TArray<FSuitData> suits;
	//maybe we should set the limit to the size of the suits array
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->suits.Num(); i++)
		{
			if ((GlobalVPFrame->suits[i].suitname != "\0\0\0\0") /*&& GlobalVPFrame->suits[i].fps > 0*/)
			{
				suits.Add(GlobalVPFrame->suits[i]);
			}
		}
	}
	mtx.unlock();
	return suits;
}

FFace VPStreamingNetwork::GetFaceByFaceID(FString faceID)
{
	FFace result;
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->faces.Num(); i++)
		{
			if (GlobalVPFrame->faces[i].faceId == faceID)
			{
				result = GlobalVPFrame->faces[i];
				break;
			}
		}
	}
	mtx.unlock();

	return result;
}

FFace* VPStreamingNetwork::GetFaceByProfileName(const FString& profileName)
{
	FFace* result = nullptr;
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->faces.Num(); i++)
		{
			if (GlobalVPFrame->faces[i].profileName == profileName)
			{
				result = &GlobalVPFrame->faces[i];
				break;
			}
		}
	}
	mtx.unlock();

	return result;
}

TArray<FFace> VPStreamingNetwork::GetAllFaces()
{
	TArray<FFace> result;
	mtx.lock();
	if (GlobalVPFrame)
	{
		for (int i = 0; i < GlobalVPFrame->faces.Num(); i++)
		{
			result.Add(GlobalVPFrame->faces[i]);
			//result->Add
		}
	}
	mtx.unlock();

	return result;

	//return GlobalVPFrame->faces;
}
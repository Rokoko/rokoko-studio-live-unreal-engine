// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#include "MocopiLiveLinkSource.h"

#include "ILiveLinkClient.h"
#include "Async/Async.h"
#include "HAL/RunnableThread.h"
#include "Common/UdpSocketBuilder.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Misc/MessageDialog.h"

#include "SMocopiLiveLinkSourceFactory.h"

#define LOCTEXT_NAMESPACE "MocopiLiveLinkModule"

const uint16 MAX_BUFFER_SIZE = 4096;
const FString DEFAULT_UDP_THREAD_NAME = "MocopiUdpThread";

FMocopiLiveLinkSource::FMocopiLiveLinkSource(uint16 inputPort, FName subjectName) :
  mUdpThreadName(""),
  mUdpThread(nullptr),
  mIsStopping(false),
  mIsThreadFinished(true),
  mSocket(nullptr),
  mSocketSubSystem(nullptr),
  mInputPort(inputPort),
  mClient(nullptr),
  mSourceGuid(),
  mSubjectName(subjectName),
  mPreviousFrameTimestamp_ms(0),
  mCurrentMocopiFPS(50),
  mNeedsToProcessSkelDef(true)
{
  SMocopiLiveLinkSourceFactory::AddSubject(mInputPort, mSubjectName);
}

FMocopiLiveLinkSource::~FMocopiLiveLinkSource()
{
  bool bIsReadyToShutdown = false;
  while (!bIsReadyToShutdown)
  {
    bIsReadyToShutdown = ShutdownThreadAndSocket();
  }

  SMocopiLiveLinkSourceFactory::RemoveSubject(mInputPort);
}

void FMocopiLiveLinkSource::OpenConnection()
{
  mSocket = FUdpSocketBuilder(TEXT("Mocopi UDP Socket"))
    .AsBlocking()
    .AsReusable()
    .BoundToAddress(FIPv4Address::Any)
    .BoundToPort(mInputPort)
    .WithReceiveBufferSize(MAX_BUFFER_SIZE);

  mRecvBuffer.SetNumUninitialized(MAX_BUFFER_SIZE);

  if ((mSocket != nullptr) && (mSocket->GetSocketType() == SOCKTYPE_Datagram))
  {
    StartUdpThread();
  }
  else
  {
    FText errorMsg = LOCTEXT("socketInitFailed", "Socket Initialization Failed");
    UE_LOG(LogMocopiLiveLink, Error, TEXT("%s"), *(errorMsg.ToString()));

    FText portInUseTitle = LOCTEXT("errorTitleUsedPort", "Error: Port in Use");
    FMessageDialog::Debugf(errorMsg, &portInUseTitle);
  }
}

void FMocopiLiveLinkSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
  mClient = InClient;
  mSourceGuid = InSourceGuid;

  OpenConnection();
}

void FMocopiLiveLinkSource::StartUdpThread()
{
  if ((mSocket == nullptr) || (mSocket->GetSocketType() != SOCKTYPE_Datagram))
  {
    FText error = LOCTEXT("errorInvalidSocket", "Trying to start UDP thread with an invalid / uninitialized socket");
    UE_LOG(LogMocopiLiveLink, Error, TEXT("%s"), *(error.ToString()));
    return;
  }

  FString udpThreadName = DEFAULT_UDP_THREAD_NAME + mInputPort;

  mSocketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
  mUdpThreadName = udpThreadName;
  mUdpThreadName.AppendInt(FAsyncThreadIndex::GetNext());

  uint32 threadStackSize = 128 * 1024; // Default taken from Unreal's JSONLiveLink Example
  mUdpThread = FRunnableThread::Create(this, *mUdpThreadName, threadStackSize, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void FMocopiLiveLinkSource::Stop()
{
  mIsStopping = true;
}

// FRunnable Interface - Runs the Udp thread
uint32 FMocopiLiveLinkSource::Run()
{
  using clock = std::chrono::steady_clock;

  TSharedRef<FInternetAddr> sender = mSocketSubSystem->CreateInternetAddr();

  mIsThreadFinished = false;

  while (!mIsStopping)
  {
    uint32 size;

    if (mSocket->HasPendingData(size))
    {
      int32 bytesRead = 0;

      if (mSocket->RecvFrom(mRecvBuffer.GetData(), mRecvBuffer.Num(), bytesRead, *sender))
      {
        if (bytesRead > 0)
        {
          TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> receivedData = MakeShareable(new TArray<uint8>());
          receivedData->SetNumUninitialized(bytesRead);
          memcpy(receivedData->GetData(), mRecvBuffer.GetData(), bytesRead);
          HandleReceivedData(receivedData);
        }

        mPreviousFrameArrivalTime = clock::now();
      }
    }
    else
    {
      std::chrono::duration<double, std::milli> duration_ms = clock::now() - mPreviousFrameArrivalTime;
      if (duration_ms.count() > TIMEOUT_MS)
      {
        // Reset variable for future data stream
        mNeedsToProcessSkelDef = true;
      }
    }
  }

  mIsThreadFinished = true;

  return 0;
}

void FMocopiLiveLinkSource::UpdateFrameData(FLiveLinkAnimationFrameData& outFrame)
{
  const size_t numBones = mDataHandler.GetNumBones();

  outFrame.Transforms.SetNumUninitialized(numBones);

  // Set Root bone translation individually
  const MocopiBoneData root = mDataHandler.GetBoneInfoByIndex(0);
  FVector boneTranslation = FVector(root.translate[0], root.translate[1], root.translate[2]);
  FQuat boneRotation = FQuat(root.rotate[0], root.rotate[1], root.rotate[2], root.rotate[3]);
  outFrame.Transforms[0] = FTransform(boneRotation, boneTranslation);

  // Then Set values for rest of the bones
  for (size_t i = 1; i < numBones; i++)
  {
    const MocopiBoneData bone = mDataHandler.GetBoneInfoByIndex(i);
    boneTranslation = FVector(bone.defaultTranslate[0], bone.defaultTranslate[1], bone.defaultTranslate[2]);
    boneRotation = FQuat(bone.rotate[0], bone.rotate[1], bone.rotate[2], bone.rotate[3]);

    outFrame.Transforms[i] = FTransform(boneRotation, boneTranslation);
  }

  // Set Timecode information on this frame
  MocopiFrameMetaData metaData = mDataHandler.GetFrameMetaData();
  FQualifiedFrameTime mocopiFrameTime = GetQualifiedFrameTime(metaData);
  outFrame.MetaData.SceneTime = mocopiFrameTime;
  outFrame.FrameId = metaData.frameId;

}

FQualifiedFrameTime FMocopiLiveLinkSource::GetQualifiedFrameTime(MocopiFrameMetaData& frameMetaData)
{
  double utcTimeDouble = frameMetaData.utcTime;

  // Convert utc time to local time
  auto timePoint = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(utcTimeDouble));
  time_t time = std::chrono::system_clock::to_time_t(timePoint);
  std::tm localTime = *std::localtime(&time);

  FTimecode mocopiTimecode;
  mocopiTimecode.Hours = localTime.tm_hour;
  mocopiTimecode.Minutes = localTime.tm_min;
  mocopiTimecode.Seconds = localTime.tm_sec;

  double ms = utcTimeDouble - (long)utcTimeDouble;

  int frameRate = mCurrentMocopiFPS;
  
  // Temporary Fix for determining mocopi stream FPS.
  // TODO:: Delete this when implemented on App Side.
  double interval_ms = ms - mPreviousFrameTimestamp_ms;
  if (interval_ms > 0)
  {
    if (interval_ms >= .033)
    {
      frameRate = 30;
    }
    else if (interval_ms >= .019)
    {
      frameRate = 50;
    }
    else if (interval_ms >= .016)
    {
      frameRate = 60;
    }

    mCurrentMocopiFPS = frameRate;
  }

  mPreviousFrameTimestamp_ms = ms;


  int frame = round(ms * frameRate); 
  mocopiTimecode.Frames = frame;

  FFrameRate mocopiFrameRate;
  mocopiFrameRate.Numerator = frameRate;
  mocopiFrameRate.Denominator = 1;

#ifdef DEBUG
  UE_LOG(LogMocopiLiveLink, Log, TEXT("timecode  %s, frameRate: %f"), *(mocopiTimecode.ToString()), mocopiFrameRate.AsDecimal());
#endif 

  return FQualifiedFrameTime(mocopiTimecode, mocopiFrameRate);
}

void FMocopiLiveLinkSource::HandleReceivedData(TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> receivedData)
{
  std::byte* dataBuffer = reinterpret_cast<std::byte*>(receivedData->GetData());
  int bufferSize = receivedData->Num();

  if (mDataHandler.IsFrameData(dataBuffer, bufferSize))
  {
    if (mNeedsToProcessSkelDef)
    {
      // Don't process data if we still haven't received a SkelDef packet
      return;
    }

    mDataHandler.ProcessFrameData(dataBuffer, bufferSize);
  }
  else if (mDataHandler.IsSkeletonDefinition(dataBuffer, bufferSize))
  {
    if (!mNeedsToProcessSkelDef)
    {
      return;
    }

    mDataHandler.ProcessSkeletonDefinitionData(dataBuffer, bufferSize);

    DefineNewMocopiSubject();

    mNeedsToProcessSkelDef = false;
  }
  else 
  {
    // Handle Unrecognized Data
    FText warningMessage = LOCTEXT("receivedUnrecognizedData", "MocopiLiveLink plugin received unrecognized data");
    UE_LOG(LogMocopiLiveLink, Warning, TEXT("%s"), *(warningMessage.ToString()));
    return;
  }

  FLiveLinkFrameDataStruct frameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
  FLiveLinkAnimationFrameData& frameData = *frameDataStruct.Cast<FLiveLinkAnimationFrameData>();

  UpdateFrameData(frameData);

  FLiveLinkSubjectKey subjectKey = { mSourceGuid, mSubjectName };
  mClient->PushSubjectFrameData_AnyThread(subjectKey, MoveTemp(frameDataStruct));
}

void FMocopiLiveLinkSource::DefineNewMocopiSubject()
{
  const int numBones = mDataHandler.GetNumBones();

  FLiveLinkStaticDataStruct staticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
  FLiveLinkSkeletonStaticData& skeletonStaticData = *staticDataStruct.Cast<FLiveLinkSkeletonStaticData>();
  skeletonStaticData.BoneNames.SetNumUninitialized(numBones);
  skeletonStaticData.BoneParents.SetNumUninitialized(numBones);

  for (int boneIndex = 0; boneIndex < numBones; boneIndex++)
  {
    const MocopiBoneData boneData = mDataHandler.GetBoneInfoByIndex(boneIndex);
    skeletonStaticData.BoneNames[boneIndex] = FName(boneData.jointName);
    skeletonStaticData.BoneParents[boneIndex] = boneData.parentIndex;
  }

  mClient->PushSubjectStaticData_AnyThread({ mSourceGuid, mSubjectName }, ULiveLinkAnimationRole::StaticClass(), MoveTemp(staticDataStruct));
}

bool FMocopiLiveLinkSource::IsSourceStillValid() const
{
  // Source is valid if we have a valid thread and socket
  bool isSourceValid = !mIsStopping && (mUdpThread != nullptr) && (mSocket != nullptr);
  return isSourceValid;
}

bool FMocopiLiveLinkSource::RequestSourceShutdown()
{
  return ShutdownThreadAndSocket();
}

bool FMocopiLiveLinkSource::ShutdownThreadAndSocket()
{
  if (!mIsStopping)
  {
    Stop();
  }

  if (!mIsThreadFinished)
  {
    return false;
  }

  if (mUdpThread != nullptr)
  {
    delete mUdpThread;
    mUdpThread = nullptr;
  }
  if (mSocket != nullptr)
  {
    mSocket->Close();
    mSocketSubSystem->DestroySocket(mSocket);
    mSocket = nullptr;
  }

  return true;
}

FText FMocopiLiveLinkSource::GetSourceType() const
{
  return FText(LOCTEXT("sourceType", "Mocopi LiveLink"));
}

FText FMocopiLiveLinkSource::GetSourceMachineName() const
{
  return FText(LOCTEXT("sourceMachineName", "Mocopi LiveLink"));
}

FText FMocopiLiveLinkSource::GetSourceStatus() const
{
  FText portString = FText::FromString(FString::FromInt(mInputPort)); // Need to do this to not have a comma in port number 
  FText sourceStatus = FText::Format(LOCTEXT("SourceStatusReceiving", "Listening to port {0}"), portString);
  return sourceStatus;
}

#undef LOCTEXT_NAMESPACE

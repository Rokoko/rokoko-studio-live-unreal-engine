// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitCommandNetwork.h"
#include "Smartsuit.h"
#include "SmartsuitDefinitions.h"
#include "../Lib/V8/Includes/SmartsuitDef.h"
#include "../Lib/V8/Includes/SmartsuitLib.h"
#include "Runtime/Core/Public/Async/TaskGraphInterfaces.h"

SmartsuitCommandNetwork::SmartsuitCommandNetwork()
{
	bodyResponses = new BodyResponse[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		bodyResponses[i].valid = false;
	}
}

SmartsuitCommandNetwork::~SmartsuitCommandNetwork()
{
	//	// Stop the runnable
	Stop();

	if (Socket) {
		Socket->Close();
	}

	//	// And last but not least stop the main thread
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}
void SmartsuitCommandNetwork::Start(int port) {
	streaming_port = port;
	FString ThreadName(FString::Printf(TEXT("SmartsuitCommandNetworkThread%ld"), (long)(FDateTime::UtcNow().ToUnixTimestamp())));

	Thread = FRunnableThread::Create(this, *ThreadName, 8 * 1024, TPri_Normal);
}

bool SmartsuitCommandNetwork::Init()
{
	if (Socket == NULL)
	{
		// Create a counter
		Socket = FUdpSocketBuilder(TEXT("SmartsuitCommandNetworkSocket")).BoundToAddress(FIPv4Address(0, 0, 0, 0)).BoundToPort(streaming_port).Build();
		Stopping = false;
	}
	return true;
}


uint32 SmartsuitCommandNetwork::Run()
{
	SmartsuitLib lib;
	UE_LOG(LogTemp, Warning, TEXT("Starting command thread... %d"), streaming_port);
	while (!Stopping)
	{

		auto addr_in = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		int32 bytes_read = 0;
		FString ret;

		uint8 data[2048];
		memset(data, '\0', 2048);

		if (Socket) {

			if (Socket->RecvFrom(data, sizeof(data), bytes_read, *addr_in)) {
				if (bytes_read == 0) {
					continue;
				}
				
				//	FString s((const char*)data);
				uint32 url;
				addr_in->GetIp(url);

				UE_LOG(LogTemp, Display, TEXT("Read %ld bytes from url: %ld. Found command type: %d"), bytes_read, url, data[2]);

				if (data[2] == SMARTSUIT_COMMAND_GET_BODY_DIMENSIONS) {
					//if setting body dimensions... add new body dimension information to buffer.

					FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
					{
						UE_LOG(LogTemp, Display, TEXT("Did receive Body dimensions response"));
						//UE_LOG(LogTemp, Warning, TEXT("Got command %d bytes from %d.%d.%d.%d: "), bytes_read, 0xff & (url >> 24), 0xff & (url >> 16), 0xff & (url >> 8), 0xff & url);
						for (int i = 0; i < bufferSize; i++) {
							if (!bodyResponses[i].valid) {
								lib.ParseBody(&(bodyResponses[i].body), data);
								bodyResponses[i].valid = true;
								bodyResponses[i].url = url;
								break;
							}
						}
					}, TStatId(), NULL, ENamedThreads::GameThread);

					// If you want to wait for the code above to complete do this:
					FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
				}
				else if (data[2] == SMARTSUIT_COMMAND_READ_HUB_INFO) {
					UE_LOG(LogTemp, Display, TEXT("Did receive HubInfo response"));
					if (bytes_read != 104) {
						UE_LOG(LogTemp, Display, TEXT("Unexpected length of hub info response. Discarding it"));
						continue;
					}
					HubInfo *hubInfo = lib.ParseInfo(data);
					
					FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
					{
						hubInfoResponses.Add(url, *hubInfo);
					}, TStatId(), NULL, ENamedThreads::GameThread);
					FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
				}
			}
			else {

			}
		}
	}
	//
	return 0;
}

void SmartsuitCommandNetwork::SendCommand(uint8 *cmd, uint32 url, int length) {
	if (Socket) {

		auto addr_out = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		addr_out->SetIp(url);
		addr_out->SetPort(4000);

		//FInternetAddr addr()
		int bytesSent = 0;
		Socket->SendTo(cmd, length, bytesSent, *addr_out);
	}

}
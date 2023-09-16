// Fill out your copyright notice in the Description page of Project Settings.


#include "RokokoRemote.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

ARokokoRemote::ARokokoRemote()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ARokokoRemote::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARokokoRemote::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

ARokokoRemote *ARokokoRemote::GetFirstAvailableActor()
{
	for (TObjectIterator<ARokokoRemote> It; It; ++It)
	{
		return *It;
	}
	return nullptr;
}


void ARokokoRemote::OnProcessRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	int32 ResponseCode = HttpResponse->GetResponseCode();
	FString ResponseString = HttpResponse->GetContentAsString();

	OnCompletedRequest.Broadcast(ResponseCode, ResponseString, bSucceeded);
}

void ARokokoRemote::OnInfoRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto& Arr = JsonObject->GetArrayField("parameters");

		if (!Arr.IsEmpty())
		{
			int Count = static_cast<int>(Arr[0]->AsNumber());

			TArray<FString> Strings1;
			TArray<FString> Strings2;

			for (int i = 0; i < Count; ++i)
			{
				Strings1.Add(Arr[i + 1]->AsString());
			}

			if (Arr.Num() > Count)
			{
				int StartIndex = Count + 1;
				// read next array
				Count = static_cast<int>(Arr[StartIndex]->AsNumber());

				for (int i = 0; i < Count; ++i)
				{
					Strings2.Add(Arr[i + StartIndex + 1]->AsString());
				}
			}

			OnInfoRequest.Broadcast(Strings1, Strings2);
		}
	}
}

void ARokokoRemote::OnTrackerRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	const double WORLD_SCALE{ 100.0 };

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto& Arr = JsonObject->GetArrayField("parameters");

		if (!Arr.IsEmpty())
		{
			auto& ObjectPosition = Arr[0]->AsObject();
			auto& ObjectRotation = Arr[1]->AsObject();

			FVector Position(WORLD_SCALE * ObjectPosition->GetNumberField("X"),
				-WORLD_SCALE * ObjectPosition->GetNumberField("Z"),
				WORLD_SCALE * ObjectPosition->GetNumberField("Y"));
			// TODO: do we need to convert rotation to UE coord system ?!
			FQuat Rotation(ObjectRotation->GetNumberField("X"), ObjectRotation->GetNumberField("Y"), ObjectRotation->GetNumberField("Z"),
				ObjectRotation->GetNumberField("W"));

			OnTrackerRequest.Broadcast(Position, Rotation);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "RokokoRemote.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "UObject/UObjectIterator.h"

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
		auto& Arr = JsonObject->GetArrayField(TEXT("parameters"));

		TArray<FString> StringsDevices;
		TArray<FString> StringsClips;
		TArray<FString> StringsActors;
		TArray<FString> StringsCharacters;

		int i = 0;
		while (!Arr.IsEmpty())
		{
			const FString ArgName = Arr[i]->AsString();
			if (ArgName == "DevicesIDs")
			{
				int Count = static_cast<int>(Arr[i+1]->AsNumber());

				for (int j = 0; j < Count; ++j)
				{
					StringsDevices.Add(Arr[i + 1 + j]->AsString());
				}
			}
			else if (ArgName == "Clips")
			{
				int Count = static_cast<int>(Arr[i + 1]->AsNumber());

				for (int j = 0; j < Count; ++j)
				{
					StringsClips.Add(Arr[i + 1 + j]->AsString());
				}
			}
			else if (ArgName == "Actors")
			{
				int Count = static_cast<int>(Arr[i + 1]->AsNumber());

				for (int j = 0; j < Count; ++j)
				{
					StringsActors.Add(Arr[i + 1 + j]->AsString());
				}
			}
			else if (ArgName == "Characters")
			{
				int Count = static_cast<int>(Arr[i + 1]->AsNumber());

				for (int j = 0; j < Count; ++j)
				{
					StringsCharacters.Add(Arr[i + 1 + j]->AsString());
				}
			}

			i += 1;
		}

		OnInfoRequest.Broadcast(StringsDevices, StringsClips, StringsActors, StringsCharacters);
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
		auto& Arr = JsonObject->GetArrayField(TEXT("parameters"));

		if (!Arr.IsEmpty())
		{
			auto& ObjectPosition = Arr[0]->AsObject();
			auto& ObjectRotation = Arr[1]->AsObject();

			FVector Position(WORLD_SCALE * ObjectPosition->GetNumberField(TEXT("X")),
				-WORLD_SCALE * ObjectPosition->GetNumberField(TEXT("Z")),
				WORLD_SCALE * ObjectPosition->GetNumberField(TEXT("Y")));
			// TODO: do we need to convert rotation to UE coord system ?!
			FQuat Rotation(ObjectRotation->GetNumberField(TEXT("X")), ObjectRotation->GetNumberField(TEXT("Y")), ObjectRotation->GetNumberField(TEXT("Z")),
				ObjectRotation->GetNumberField(TEXT("W")));

			OnTrackerRequest.Broadcast(Position, Rotation);
		}
	}
}


void ARokokoRemote::OnPlaybackRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());

	// Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		auto& Arr = JsonObject->GetArrayField(TEXT("parameters"));

		if (!Arr.IsEmpty())
		{
			const double CurrentTime = Arr[0]->AsNumber();
			const bool IsPlaying = Arr[1]->AsBool();
			const double MinTime = Arr[2]->AsNumber();
			const double MaxTime = Arr[3]->AsNumber();
			const double PlaybackSpeed = Arr[4]->AsNumber();

			OnPlaybackRequest.Broadcast(CurrentTime, IsPlaying, MinTime, MaxTime, PlaybackSpeed);
		}
	}
}

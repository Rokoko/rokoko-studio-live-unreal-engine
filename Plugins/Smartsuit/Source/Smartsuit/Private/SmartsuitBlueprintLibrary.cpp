// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#include "SmartsuitBlueprintLibrary.h"
#include "VirtualProductionSource.h"

ASmartsuitController* USmartsuitBlueprintLibrary::GetSmartsuitControllerByName(FString name)
{
	ASmartsuitController* actor = nullptr;

	for (TObjectIterator<ASmartsuitController> It; It; ++It)
	{
		if (It->IsRealLife()) 
		{
				ASmartsuitController* curActor = *It;
				FString mySuitName = curActor->suitname;
				if (name.Compare(mySuitName) == 0) 
				{
					actor = curActor;
					break;
				}
		}
	}
	return actor;
}

ASmartsuitController* USmartsuitBlueprintLibrary::GetSmartsuitController(int id)
{
	ASmartsuitController* actor = nullptr;

	for (TObjectIterator<ASmartsuitController> It; It; ++It)
	{
		if (It->IsRealLife())
		{
			ASmartsuitController* curActor = *It;

			if (id == curActor->IndexID)
			{
				actor = curActor;
				break;
			}
		}
	}
	return actor;
}

void USmartsuitBlueprintLibrary::JSONTest()
{
	FString JsonTest = "{\"version\":0,\"timestamp\":3501.165771484375,\"playbackTimestamp\":36.01746044922,\"props\":[{\"name\":\"Camera-1\",\"id\":\"764cde8d-27d9-4a3a-ac60-e53af5ddfaac\",\"position\":{\"X\":0.62,\"Y\":1.53,\"z\":-0.417694},\"rotation\":{\"x\":0.00904452,\"y\":-0.832524,\"z\":0.063621,\"w\":0.550299},\"isLive\":false,\"profile\":{\"name\":\"Camera\",\"uuid\":\"camera-1\",\"dimensions\":{\"x\":0.23999198,\"y\":0.1805738,\"z\":0.0393033},\"color\":{\"x\":0.85888933,\"y\":0.274593,\"z\":0.380505},\"trackerOffset\":{\"position\":{\"x\":-0.0599549,\"y\":0.099993,\"z\":0.0},\"rotation\":{\"x\":-90.0,\"y\":0.0,\"z\":0.0}},\"pivot\":{\"position\":{\"x\":0.0,\"y\":0.0,\"z\":0.0199165},\"rotation\":{\"x\":0.0,\"y\":0.0,\"z\":0.0}},\"grips\":[],\"propType\":0}},{\"name\":\"Paddle-1\",\"id\":\"3d39d613-db56-49e1-923a-15a43ead2c09\",\"position\":{\"x\":-0.2098624,\"y\":0.34978352,\"z\":-0.2011109},\"rotation\":{\"x\":0.1669078,\"y\":0.8222,\"z\":0.478498,\"w\":-0.303668},\"isLive\":false,\"profile\":{\"name\":\"Stick\",\"uuid\":\"stick-1\",\"dimensions\":{\"x\":0.03033,\"y\":0.699071,\"z\":0.0399033},\"color\":{\"x\":0.6629,\"y\":0.239045,\"z\":0.9814636},\"trackerOffset\":{\"position\":{\"x\":0.0,\"y\":0.156449,\"z\":0.01165},\"rotation\":{\"x\":0.0,\"y\":0.0,\"z\":0.0}},\"pivot\":{\"position\":{\"x\":0.0,\"y\":0.3499355,\"z\":0.0},\"rotation\":{\"x\":0.0,\"y\":0.0,\"z\":180.0}},\"grips\":[],\"propType\":0}}],\"trackers\":[{\"name\":\"2\",\"connectionId\":\"H23\",\"position\":{\"x\":-0.865737,\"y\":1.471664,\"z\":-1.65637984},\"rotation\":{\"x\":0.65916397,\"y\":-0.088037,\"z\":-0.7399105,\"w\":-0.11258},\"isLive\":false,\"trackingResult\":200,\"trackerType\":1,\"RenderModelName\":\"\",\"battery\":0.5931},{\"name\":\"3\",\"connectionId\":\"764cde8d-27d9-4a3a-ac60-e53af5ddfaac\",\"position\":{\"x\":0.663784485,\"y\":1.6378482,\"z\":-0.474449},\"rotation\":{\"x\":-0.3851843,\"y\":-0.6338438,\"z\":-0.551294,\"w\":0.39638153},\"isLive\":false,\"trackingResult\":200,\"trackerType\":1,\"RenderModelName\":\"\",\"battery\":0.592131},{\"name\":\"8\",\"connectionId\":\"3d39d613-db56-49e1-923a-15a43ead2c09\",\"position\":{\"x\":-0.104797,\"y\":0.464008,\"z\":-0.07374},\"rotation\":{\"x\":-0.8070222,\"y\":0.1646839,\"z\":0.3036444,\"w\":0.478498},\"isLive\":false,\"trackingResult\":101,\"trackerType\":1,\"RenderModelName\":\"\",\"battery\":0.939142}],\"faces\":[]}";

	FVirtualProductionFrame VPFrame;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonTest, &VPFrame, 0, 0);
}

FVector USmartsuitBlueprintLibrary::GetVectorField(TSharedPtr<FJsonObject> jsonObject)
{
	FVector ReturnVal;
	ReturnVal.X = jsonObject->GetNumberField("x");
	ReturnVal.Y = jsonObject->GetNumberField("y");
	ReturnVal.Z = jsonObject->GetNumberField("z");
	return ReturnVal;
}

FLinearColor USmartsuitBlueprintLibrary::GetFLinearColorField(TSharedPtr<FJsonObject> jsonObject)
{
	FLinearColor LinearColor;
	LinearColor.R = jsonObject->GetNumberField("x");
	LinearColor.G = jsonObject->GetNumberField("y");
	LinearColor.B = jsonObject->GetNumberField("z");
	
	return LinearColor;
}

FQuat USmartsuitBlueprintLibrary::GetQuaternionField(TSharedPtr<FJsonObject> jsonObject)
{
	FQuat ReturnVal;
	ReturnVal.X = jsonObject->GetNumberField("x");
	ReturnVal.Y = jsonObject->GetNumberField("y");
	ReturnVal.Z = jsonObject->GetNumberField("z");
	ReturnVal.W = jsonObject->GetNumberField("w");
	return ReturnVal;
}

void USmartsuitBlueprintLibrary::CreateVirtualProductionSource()
{
	FVirtualProductionSource::CreateLiveLinkSource();
}
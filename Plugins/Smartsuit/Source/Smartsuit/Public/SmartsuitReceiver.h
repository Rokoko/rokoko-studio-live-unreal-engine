// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "SmartsuitStreamingNetwork.h"
#include "VPStreamingNetwork.h"
#include "VirtualProductionFrame.h"
//#include "Engine.h"
#include "UObject/UObjectIterator.h"
#include "SmartsuitReceiver.generated.h"

/*! \brief This AActor class implements the network layer between unreal and the SmartsuitPro.
 *
 * It is necessary to have one instance of this ARokokoReceiver in your level in order for Unreal Engine to be able to listen to Smartsuits.
 */
UCLASS(meta = (ToolTip = "A SmartsuitReceiver implements the network interface between Smartsuit and Unreal. This is required for SmartsuitController to work properly. You can have only one SmartsuitReceiver at the same time."))
class SMARTSUIT_API ARokokoReceiver : public AActor
{
	GENERATED_BODY()
	
public:
	/// @private Sets default values for this actor's properties
	ARokokoReceiver();

	/// @private Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// @private
	virtual void BeginDestroy() override;
	/// @private Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	/// @private
	bool enabled = false;

	/**
	* Starts the SmartsuitReceiver socket so it listens for Smartsuits.
	*/
	UFUNCTION(BlueprintCallable, Category = "Rokoko")
		void StartListener();

	/**
	* Stops the SmartsuitReceiver socket so it stops listening for Smartsuits.
	*/
	UFUNCTION(BlueprintCallable, Category = "Rokoko")
		void StopListener();

	UFUNCTION(BlueprintCallable)
	FFace GetFaceByFaceID(FString faceName);

	UFUNCTION(BlueprintCallable)
	FFace GetFaceByProfileName(const FString& faceName, bool& found);

	UFUNCTION(BlueprintCallable)
	TArray<FFace> GetAllFaces();

	UFUNCTION(BlueprintCallable)
	TArray<FFace> GetFacesNotAssociatedWithActor();

	/**
	* This function returns the last frame of data received from a Smartsuit with the given name.
	* If there is no data for the given name, then nullptr will return.
	*
	* @param suitName The smartsuit name to get the last data for.
	* @return Returns the last frame received for the smartsuit with name suitName. If no frame is found, it will return nullptr.
	*/
	FSuitData* GetSmartsuit(FString suitName);

	UFUNCTION(BlueprintCallable)
	bool GetSmartsuitByName(const FString& suitName, FSuitData& SuitData);

	/**
	* Lists the names of all known Smartsuits connected to this computer.
	* It will only list the currently active Smartsuits.
	*
	* @return Returns an array with all the available in the network Smartsuit names.
	*/
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAvailableSmartsuitNames();

	UFUNCTION(BlueprintCallable)
	TArray<FSuitData> GetAllSmartsuits();

	/**
	* The port number used to listen for the streaming data of Smartsuits.
	*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smartsuit)
	//int streamingDataPort = 14041;

	/**
	* The port number used to listen for trackers and props data.
	*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
	//int VPListenPort = 14045;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
	int RokokoPortNumber;

	FProp* GetPropByNameFromVP(FString name, bool isLive);
	FTracker* GetTrackerByNameFromVP(FString name, bool isLive);

	UFUNCTION(BlueprintPure, Category = "VirtualProduction")
	FTracker GetTrackerByConnectionIDFromVP(const FString& name, bool isLive, bool& found);

	/// @cond nodoc
	//Body* CheckForBodyCommand(FString suitname);
	//HubInfo* CheckForHubInfo(FString suitname);
	void SetSupportsWiFiAPI(FString suitname);
	
	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve all props."))
	static TArray<FProp> GetAllProps(); 
	//{
	//	TArray<FProp> result;
	//	//UE_LOG(LogTemp, Display, TEXT("Yeeee1"));
	//	bool found = false;
	//	int i = 0;
	//	for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	//	{
	//		//UE_LOG(LogTemp, Display, TEXT("Looking up receiver %d"), i);
	//		i++;
	//		if (It->realLife) 
	//		{
	//			found = true;
	//			//UE_LOG(LogTemp, Display, TEXT("Real life!"));
	//			result = It->VPlistener.GetAllProps();
	//		}
	//	}
	//	if (!found)
	//	{
	//		//UE_LOG(LogTemp, Display, TEXT("not Real life..."));
	//	}
	//	//UE_LOG(LogTemp, Display, TEXT("Yeeee2 %d"), result.Num());
	//	return result;
	//}

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a prop by name."))
	bool GetProp(FString name, bool isLive, FProp& OutProp);
	//{
	//	FProp result;
	//	for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	//	{
	//		if (It->realLife) 
	//		{
	//			result = *It->GetPropByNameFromVP(name, isLive);
	//		}
	//	}
	//	return result;
	//}

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a tracker by name."))
	static FTracker GetTracker(FString name, bool isLive);
	//{
	//	FTracker result;
	//	for (TObjectIterator<ARokokoReceiver> It; It; ++It)
	//	{
	//		if (It->realLife) {
	//			result = *It->GetTrackerByNameFromVP(name, isLive);
	//		}
	//	}
	//	return result;
	//}

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Converts Quaternions into rotators."))
	static FRotator FQuatToRotator(FQuat rotation) 
	{
		FQuat result(rotation.Z, rotation.X, rotation.Y, rotation.W);
		FRotator Rotator = result.Rotator();
		return Rotator;
	}

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Converts position into Unreal position."))
	static FVector UPosition(FVector position) 
	{
		return FVector(100.0f*position.Z, 100.0f*position.X, 100.0f*position.Y);
	}
	/// @endcond

private:
	//SmartsuitStreamingNetwork listener;
	VPStreamingNetwork VPlistener;

protected:
	/*! \brief Indicates if this instance of the actor is in play mode and not an instance from the editor.*/
	bool realLife = false;
};

// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "SmartsuitStreamingNetwork.h"
#include "SmartsuitCommandNetwork.h"
#include "VPStreamingNetwork.h"
#include "VirtualProductionFrame.h"
#include "Engine.h"
#include "SmartsuitReceiver.generated.h"

/*! \brief This AActor class implements the network layer between unreal and the SmartsuitPro.
 *
 * It is necessary to have one instance of this ASmartsuitReceiver in your level in order for Unreal Engine to be able to listen to Smartsuits.
 */
UCLASS(meta = (ToolTip = "A SmartsuitReceiver implements the network interface between Smartsuit and Unreal. This is required for SmartsuitController to work properly. You can have only one SmartsuitReceiver at the same time."))
class SMARTSUIT_API ASmartsuitReceiver : public AActor
{
	GENERATED_BODY()
	
public:
	/// @private Sets default values for this actor's properties
	ASmartsuitReceiver();

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

	/**
	* This function returns the last frame of data received from a Smartsuit with the given name.
	* If there is no data for the given name, then nullptr will return.
	*
	* @param suitName The smartsuit name to get the last data for.
	* @return Returns the last frame received for the smartsuit with name suitName. If no frame is found, it will return nullptr.
	*/
	SuitData* GetSmartsuit(FString suitName);

	/**
	* Lists the names of all known Smartsuits connected to this computer.
	* It will only list the currently active Smartsuits.
	*
	* @return Returns an array with all the available in the network Smartsuit names.
	*/
	TArray<FString> GetAvailableSmartsuits();

	/**
	* The port number used to listen for the streaming data of Smartsuits.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smartsuit)
		int streamingDataPort = 14041;

	/**
	* The port number used to listen for commands from Smartsuits.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smartsuit)
		int commandListenPort = 5000;

	/**
	* The port number used to listen for trackers and props data.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
		int VPListenPort = 14045;

	/**
	* Sends a command to a Smartsuit.
	* @param suitName The name of the Smartsuit to send the command to.
	* @param cmd the Command to send. A list of all available commands is found in SmartsuitDefinitions.
	*/
	void SendCommand(FString suitName, uint8 *cmd, int length) {
		SuitData* suit = GetSmartsuit(suitName);
		if (suit) {
			//UE_LOG(LogTemp, Warning,
				//TEXT("Sending command to url %s, %s, %u"), *suitName, *FString(suit->suitname), suit->url);

			commander.SendCommand(cmd, suit->url, length);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to find suit to send command..."));
		}
	}

	FProp* GetPropByNameFromVP(FString name, bool isLive);
	FTracker* GetTrackerByNameFromVP(FString name, bool isLive);

	/// @cond nodoc
	Body* CheckForBodyCommand(FString suitname);
	HubInfo* CheckForHubInfo(FString suitname);
	void SetSupportsWiFiAPI(FString suitname);
	
	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a prop by name."))
	static FProp GetProp(FString name, bool isLive) {
		FProp result;
		for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
		{
			if (It->realLife) {
				result = *It->GetPropByNameFromVP(name, isLive);
			}
		}
		return result;
	}

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a tracker by name."))
	static FTracker GetTracker(FString name, bool isLive) {
		FTracker result;
		for (TObjectIterator<ASmartsuitReceiver> It; It; ++It)
		{
			if (It->realLife) {
				result = *It->GetTrackerByNameFromVP(name, isLive);
			}
		}
		return result;
	}

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Converts Quaternions into rotators."))
	static FRotator FQuatToRotator(FQuat rotation) {
		FQuat result(rotation.Z, rotation.X, rotation.Y, rotation.W);
		FRotator Rotator = result.Rotator();
		return Rotator;
	}

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Converts position into Unreal position."))
	static FVector UPosition(FVector position) {
		return FVector(100.0f*position.Z, 100.0f*position.X, 100.0f*position.Y);
	}
	/// @endcond

private:
	SmartsuitStreamingNetwork listener;
	SmartsuitCommandNetwork commander;
	VPStreamingNetwork VPlistener;

protected:
	/*! \brief Indicates if this instance of the actor is in play mode and not an instance from the editor.*/
	bool realLife = false;
};

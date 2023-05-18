// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "VirtualProductionFrame.h"
#include "UObject/UObjectIterator.h"
#include "RokokoReceiver.generated.h"

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

	UFUNCTION(BlueprintCallable, Category = Default)
	FFace GetFaceByFaceID(FString faceName);

	UFUNCTION(BlueprintCallable, Category = Default)
	FFace GetFaceByProfileName(const FString& faceName, bool& found);

	UFUNCTION(BlueprintCallable, Category = Default)
	TArray<FFace> GetAllFaces();

	UFUNCTION(BlueprintCallable, Category = Default)
	TArray<FFace> GetFacesNotAssociatedWithActor();

	/**
	* This function returns the last frame of data received from a Smartsuit with the given name.
	* If there is no data for the given name, then nullptr will return.
	*
	* @param suitName The smartsuit name to get the last data for.
	* @return Returns the last frame received for the smartsuit with name suitName. If no frame is found, it will return nullptr.
	*/
	FSuitData* GetSmartsuit(FString suitName);

	UFUNCTION(BlueprintCallable, Category = Default)
	bool GetSmartsuitByName(const FString& suitName, FSuitData& SuitData);

	/**
	* Lists the names of all known Smartsuits connected to this computer.
	* It will only list the currently active Smartsuits.
	*
	* @return Returns an array with all the available in the network Smartsuit names.
	*/
	UFUNCTION(BlueprintCallable, Category = Default)
	TArray<FString> GetAvailableSmartsuitNames();

	UFUNCTION(BlueprintCallable, Category = Default)
	TArray<FSuitData> GetAllSmartsuits();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
	int RokokoPortNumber;

	FProp* GetPropByNameFromVP(FString name, bool isLive);
	FTracker* GetTrackerByNameFromVP(FString name, bool isLive);

	UFUNCTION(BlueprintPure, Category = "VirtualProduction")
	FTracker GetTrackerByConnectionIDFromVP(const FString& name, bool isLive, bool& found);


	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve all props."))
	static TArray<FProp> GetAllProps(); 
	

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a prop by name."))
	bool GetProp(FString name, /*bool isLive, */FProp& OutProp);
	

	UFUNCTION(BlueprintPure, Category = "VirtualProduction", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a tracker by name."))
	static FTracker GetTracker(FString name, bool isLive);
	

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

protected:
	/*! \brief Indicates if this instance of the actor is in play mode and not an instance from the editor.*/
	bool realLife = false;
};

// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Styling/SlateBrush.h"
#include "SmartsuitController.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"

#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "ReferenceSkeleton.h"

#include "SmartsuitBlueprintLibrary.generated.h"

class FVirtualProductionSource;

/**
 *
 */
UCLASS(meta = (ScriptName = "SmartsuitLibrary"))
class USmartsuitBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Rokoko")
	static FFace GetFaceByFaceID(FString faceName);

	UFUNCTION(BlueprintCallable, Category = "Rokoko")
	static FFace GetFaceByProfileName(const FString& faceName, bool& found);

	UFUNCTION(BlueprintCallable, Category = "Rokoko")
	static TArray<FFace> GetAllFaces();

	UFUNCTION(BlueprintCallable, Category = "Rokoko")
	static TArray<FFace> GetFacesNotAssociatedWithActor();

	/**
	* This function returns the last frame of data received from a Smartsuit with the given name.
	* If there is no data for the given name, then nullptr will return.
	*
	* @param suitName The smartsuit name to get the last data for.
	* @return Returns the last frame received for the smartsuit with name suitName. If no frame is found, it will return nullptr.
	*/
	static FSuitData* GetSmartsuit(FString suitName);

	UFUNCTION(BlueprintCallable, Category = "Rokoko")
	static bool GetSmartsuitByName(const FString& suitName, FSuitData& SuitData);

	/**
	* Lists the names of all known Smartsuits connected to this computer.
	* It will only list the currently active Smartsuits.
	*
	* @return Returns an array with all the available in the network Smartsuit names.
	*/
	UFUNCTION(BlueprintCallable, Category = "Rokoko", meta = (BlueprintThreadSafe, ToolTip = "Returns actor names in the current live link source"))
	static TArray<FString> GetAvailableActorNames();

	UFUNCTION(BlueprintCallable, Category = "Rokoko")
	static TArray<FSuitData> GetAllSmartsuits();

	static FProp* GetPropByNameFromVP(FString name, bool isLive);
	FTracker* GetTrackerByNameFromVP(FString name, bool isLive);

	UFUNCTION(BlueprintPure, Category = "Rokoko")
	static FTracker GetTrackerByConnectionIDFromVP(const FString& name, bool isLive, bool& found);


	UFUNCTION(BlueprintPure, Category = "Rokoko", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve all props."))
	static TArray<FProp> GetAllProps(); 
	

	UFUNCTION(BlueprintPure, Category = "Rokoko", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a prop by name."))
	static bool GetProp(FString name, /*bool isLive, */FProp& OutProp);
	

	UFUNCTION(BlueprintPure, Category = "Rokoko", meta = (ToolTip = "Calls a function in VPStreamingNetwork to retrieve a tracker by name."))
	static FTracker GetTracker(FString name, bool isLive);
	

	UFUNCTION(BlueprintPure, Category = "Rokoko", meta = (ToolTip = "Converts Quaternions into rotators."))
	static FRotator FQuatToRotator(FQuat rotation) 
	{
		FQuat result(rotation.Z, rotation.X, rotation.Y, rotation.W);
		FRotator Rotator = result.Rotator();
		return Rotator;
	}

	UFUNCTION(BlueprintPure, Category = "Rokoko", meta = (ToolTip = "Converts position into Unreal position."))
	static FVector UPosition(FVector position) 
	{
		return FVector(100.0f*position.Z, 100.0f*position.X, 100.0f*position.Y);
	}
	/// @endcond


	/*! \brief Returns a SmartsuitController by the name.
	*
	* @param name The name of the Smartsuit to look for.
	* @return A SmartsuitController that has the specified Smartsuit name, if non found, it will return nullptr.
	*/
	UFUNCTION(BlueprintPure, Category = "Rokoko", meta = (BlueprintThreadSafe, ToolTip = "Returns the SmartsuitController given the Smartsuit name."))
	static ASmartsuitController* GetSmartsuitControllerByName(FString name);

	/*! \brief Returns a SmartsuitController from its index ID.
	*
	* @param id The index id that will look up for.
	* @return A SmartsuitController that has the specified index id, if non found, it will return nullptr.
	*/
	UFUNCTION(BlueprintPure, Category = "Rokoko", meta = (BlueprintThreadSafe, ToolTip = "Returns a SmartsuitController given it's Index ID. The Index ID is specified in the SmartsuitController details."))
	static ASmartsuitController* GetSmartsuitController(int id);

	UFUNCTION(BlueprintCallable, Category=Test)
	static void JSONTest();

	static FVector GetVectorField(TSharedPtr<FJsonObject> jsonObject);

	static FColor GetColorField(TSharedPtr<FJsonObject> jsonObject);

	static FLinearColor GetFLinearColorField(TSharedPtr<FJsonObject> jsonObject);

	static FQuat GetQuaternionField(TSharedPtr<FJsonObject> jsonObject);

	UFUNCTION(BlueprintCallable, Category = "Rokoko")
	static void CreateVirtualProductionSource();

	static FTransform GetWorldSpaceTransform(FReferenceSkeleton RefSkel, int32 BoneIdx);

	static FTransform GetRefPoseBoneTransform(USkeletalMeshComponent* SkelMesh, FName BoneName);

	static FTransform GetBoneTransform(USkeletalMeshComponent* SkelMesh, FName BoneName);

	UFUNCTION(BlueprintPure, Category = "Rokoko")
	static FVector GetPropLocation(FProp InProp);

	UFUNCTION(BlueprintPure, Category = "Rokoko")
	static FRotator GetPropRotation(FProp InProp);
};

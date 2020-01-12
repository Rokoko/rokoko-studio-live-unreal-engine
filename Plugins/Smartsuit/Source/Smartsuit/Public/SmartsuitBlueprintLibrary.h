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
	/*! \brief Returns a SmartsuitController by the name.
	*
	* @param name The name of the Smartsuit to look for.
	* @return A SmartsuitController that has the specified Smartsuit name, if non found, it will return nullptr.
	*/
	UFUNCTION(BlueprintPure, Category = "Smartsuit", meta = (BlueprintThreadSafe, ToolTip = "Returns the SmartsuitController given the Smartsuit name."))
	static ASmartsuitController* GetSmartsuitControllerByName(FString name);

	/*! \brief Returns a SmartsuitController from its index ID.
	*
	* @param id The index id that will look up for.
	* @return A SmartsuitController that has the specified index id, if non found, it will return nullptr.
	*/
	UFUNCTION(BlueprintPure, Category = "Smartsuit", meta = (BlueprintThreadSafe, ToolTip = "Returns a SmartsuitController given it's Index ID. The Index ID is specified in the SmartsuitController details."))
	static ASmartsuitController* GetSmartsuitController(int id);

	UFUNCTION(BlueprintCallable, Category=Test)
	static void JSONTest();

	static FVector GetVectorField(TSharedPtr<FJsonObject> jsonObject);

	static FLinearColor GetFLinearColorField(TSharedPtr<FJsonObject> jsonObject);

	static FQuat GetQuaternionField(TSharedPtr<FJsonObject> jsonObject);

	UFUNCTION(BlueprintCallable, Category = "Smartsuit")
	static void CreateVirtualProductionSource();
};

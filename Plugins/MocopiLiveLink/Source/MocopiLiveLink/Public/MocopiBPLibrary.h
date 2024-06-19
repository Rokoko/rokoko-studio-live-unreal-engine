// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#pragma once

#include "ILiveLinkSource.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MocopiBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MOCOPILIVELINK_API UMocopiBPLibrary : public UBlueprintFunctionLibrary
{
  GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "MocopiLiveLink", meta = (Keywords = "livelink"))
	  static void CreateMocopiSource(FLiveLinkSourceHandle& handle, const int inputPort = 12351, const FString& subjectName = "MocopiSkeleton");
};

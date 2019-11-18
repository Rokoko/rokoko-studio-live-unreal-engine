// Copyright 2019 Rokoko Electronics. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "SmartsuitReceiver.h"
#include "VirtualProductionTracker.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMARTSUIT_API UVirtualProductionTracker : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVirtualProductionTracker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* The port number used to listen for trackers and props data.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
	bool isLive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
	bool useLocalSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Virtual Production")
	float scalePosition;
		
	FVector CurrentLocation;
	AActor* parent;

private:
	ASmartsuitReceiver* GetReceiver();
};
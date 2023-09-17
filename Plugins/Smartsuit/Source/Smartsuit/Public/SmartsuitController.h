// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "RokokoSkeletonData.h"

#include "SmartsuitController.generated.h"

#define SUPPORTED_MAJOR_WIFI_API 2

/*! \brief This struct contains body dimensions information. This information is used by the Smartsuit to optimize its performance.
* 
* You can use this struct to read and write the BodyModel running in the Smartsuit. To do that you can use the ASmartsuitController methods
* GetBodyModel and SetBodyModel.
*/
USTRUCT(BlueprintType, meta = (ToolTip = "The BodyModel represents the dimensions of the player that wears the suit. Having this information correct is important for the Smartsuit to perform better."))
struct FBodyModel
{
	GENERATED_USTRUCT_BODY()

		/**A friendly name for the BodyModel.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "A friendly name for the BodyProfile."))
	FString Name;
	/**The total height of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The total height of the player wearing the Smartsuit."))
		float TotalHeight;
	/**The shoulder height of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The shoulder height of the player wearing the Smartsuit."))
		float ShoulderHeight;
	/**The hip height of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The hip height of the player wearing the Smartsuit."))
		float HipHeight;
	/**The hip width of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The hip width of the player wearing the Smartsuit."))
		float HipWidth;
	/**The shoulder width of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The shoulder width of the player wearing the Smartsuit."))
		float ShoulderWidth;
	/**The arm span of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The arm span of the player wearing the Smartsuit."))
		float ArmSpan;
	/**The ankle height of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The ankle height of the player wearing the Smartsuit."))
		float AnkleHeight;
	/**The foot length of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The foot length of the player wearing the Smartsuit."))
		float FootLength;
	/**The heel offset of the player wearing the Smartsuit.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smartsuit", meta = (ToolTip = "The heel offset of the player wearing the Smartsuit."))
		float HeelOffset;

	FBodyModel() {
		Name = "Default";
		TotalHeight = 1.9;
		ShoulderHeight = 1.6;
		HipHeight = 0.97;
		HipWidth = 0.18;
		ShoulderWidth = 0.4;
		ArmSpan = 1.9;
		AnkleHeight = 0.09;
		FootLength = 0.31;
		HeelOffset = 0.08;
	}

};

/*! \brief SmartsuitController is an UE Actor that provides methods to communicate with a Smartsuit and send commands to it.
*
* ASmartsuitController provides information about the Smartsuit like FPS, Network status if the smartsuit is broadcasting or unicasting and BodyModel information running in the Smartsuit.
* Additionally provides methods to aestart, A-pose calibrate, make the Smartsuit broadcast or unicast, get and set the BodyModel running in the Smartsuit. 
* 
* For this controller to work you need to have one reference of ARokokoReceiver in the level.
*/
UCLASS(meta = (ToolTip = "A SmartsuitController provides functionality to connect with a Smartsuit, send and receive commands. You need to define one SmartsuitController per Smartsuit you waant to support in your game. For example if you have one Smartsuit, you need one SmartsuitController that will bind to the Smartsuit. This component will work only if one SmartsuitReceiver is in the level."))
class SMARTSUIT_API ASmartsuitController : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASmartsuitController();

	/// @private Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// @private Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	/*! \brief When this is True the SmartsuitController will connect to the first available Smartsuit and the original name "suitname" will be ignored.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smartsuit, meta = (ToolTip = "If true then the suitname will be ignored. The controller will bind with the first active Smartsuit that will appear online"))
	bool AutoConnect;

	/*! \brief This is the Smartsuit name the controller will bind to. If AutoConnect is true, this variable will automatically take the name of the first Smartsuit online.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smartsuit, meta = (ToolTip = "The controller will bind with the Smartsuit with this name."))
	FString suitname;

	/*! \brief  The Index ID is a runtime defined identifier for the SmartsuitController. Is used to lookup for a specific SmartsuitController. Each SmartsuitController should have a different IndexID specified.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smartsuit, meta = (ToolTip = "This is an Internal ID that you can use to access this controller from C++ or Blueprints. You have to set this number into something unique for each SmartsuitController.", DisplayName="Index Identifier"))
	int IndexID;

	/*! \brief  This variable indicates weither the Smartsuit is broadcasting or unicasting. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SmartsuitReadOnly, meta = (ToolTip = "Read-only: Indicates if the Smartsuit is broadcasting or unicasting."))
	bool isBroadcast;

	/*! \brief This variable indicates if the Smartsuit has any BodyProfile specified.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SmartsuitReadOnly, meta = (ToolTip = "Read-only: Indicates weither the Smartsuit has a BodyProfile. If false it is using the Default profile."))
	bool hasProfile;

	/*! \brief  This variable indicates the number of frames received during the last second for the Smartsuit this controller is binded to.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SmartsuitReadOnly, meta = (ToolTip = "Read-only: Indicates the Frames per second that Unreal is receiving frames from the Smartsuit. If WiFi is stable this value should be around 100."))
	float fps;

	/*! \brief  This variable shows the WiFi API version of the Smartsuit hub once it has been retrieved.*/
	UPROPERTY(VisibleAnywhere, Category = SmartsuitReadOnly, meta = (ToolTip = "WiFi API Version of the Smartsuit hub."))
	FString wifiApiVersion;

	/// @private
	bool profileToggleState;

	/*! \brief  The current BodyModel running in the Smartsuit. This variable is updated automatically when the Smartsuit is connected, or when you apply a new BodyProfile to the Smartsuit. You can ask for the BodyModel running in Smartsuit using RefreshBodyModel().*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SmartsuitReadOnly, meta = (ToolTip = "Read-only: Indicates the current known BodyProfile for the Smartsuit connected to this SmartsuitController."))
		FBodyModel bodyModel;

	/*! \brief Various Developer Tests.*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Tests"))
		void DoTests();
		
	bool IsRealLife() { return realLife; }
private:

    uint32 GetLocalMacIP();
	uint8 GetByte(uint32 val, int i);

	bool bodyModelRequested;
	
protected:
	/*! \brief Indicates if this instance of the actor is in play mode and not an instance from the editor.*/
	bool realLife = false;


};

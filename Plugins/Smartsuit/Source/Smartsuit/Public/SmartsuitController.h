// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "SmartsuitStreamingNetwork.h"
#include "SmartsuitReceiver.h"
#include "SmartsuitDefinitions.h"

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

	/**
	* Breaks the BodyModel into individual body dimensions and returns a Body struct representing these dimensions.
	*/
	Body GetBody()
	{
		Body b;
		b.first_buffer = SMARTSUIT_COMMAND_SET_BODY_DIMENSIONS;
		b.first_buffer1 = SMARTSUIT_COMMAND_SET_BODY_DIMENSIONS;
		b.first_buffer2 = SMARTSUIT_COMMAND_SET_BODY_DIMENSIONS;
		b.first_buffer3 = SMARTSUIT_COMMAND_SET_BODY_DIMENSIONS;//put last byte of ip here.
		b._head = (fabs(TotalHeight) - fabs(ShoulderHeight)) * .7f;
		b._neck = (fabs(TotalHeight) - fabs(ShoulderHeight)) * .3f;
		b._middle_back = (fabs(ShoulderHeight) - fabs(HipHeight)) * .414f;
		b._low_back = (fabs(ShoulderHeight) - fabs(HipHeight)) * .433f;
		b._hip = (fabs(ShoulderHeight) - fabs(HipHeight)) * .153f;
		b._shoulder_blade = fabs(ShoulderWidth) * .5f;
		float arm = (fabs(ArmSpan) - fabs(ShoulderWidth)) / 2.0f;
		b._upper_arm = arm * .391f;
		b._forearm = arm * .352f;
		b._hand = arm * .257f;
		float leg = (fabs(HipHeight) - fabs(AnkleHeight));
		b._thigh = leg * .517f;
		b._leg = leg * .483f;
		b._foot_height = fabs(AnkleHeight);
		b._foot_length = fabs(FootLength);
		b._foot_heel_offset = fabs(HeelOffset);
		b._foot_width = fabs(FootLength) * .25f;
		b._hip_width = fabs(HipWidth);
		//strcpy(b.name, "unreal\0\0");
		return b;
	}

	/**
	* Calculates and sets the BodyModel dimensions from a detailed Body dimensions struct.
	*
	* @param value the Body dimensions to set to this BodyModel.
	*/
	void SetBody(Body value)
	{

		Name = "unreal";

		HipHeight = value._foot_height + value._leg + value._thigh;
		ShoulderHeight = HipHeight + value._hip + value._low_back + value._middle_back;
		TotalHeight = ShoulderHeight + value._neck + value._head;
		HipWidth = value._hip_width;
		ShoulderWidth = value._shoulder_blade * 2.0f;
		ArmSpan = (value._shoulder_blade + value._upper_arm + value._forearm + value._hand) * 2.0f;
		AnkleHeight = value._foot_height;
		FootLength = value._foot_length;
		HeelOffset = value._foot_heel_offset;
	}



};

/*! \brief SmartsuitController is an UE Actor that provides methods to communicate with a Smartsuit and send commands to it.
*
* ASmartsuitController provides information about the Smartsuit like FPS, Network status if the smartsuit is broadcasting or unicasting and BodyModel information running in the Smartsuit.
* Additionally provides methods to aestart, A-pose calibrate, make the Smartsuit broadcast or unicast, get and set the BodyModel running in the Smartsuit. 
* 
* For this controller to work you need to have one reference of ASmartsuitReceiver in the level.
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

	/* Enable this to see detected UI version based on macro added in Smartsuit.Build.cs
	UPROPERTY(VisibleAnywhere, Category = SmartsuitReadOnly, meta = (ToolTip = "Detected UE Verssion."))
	int detectedVersion;
	*/

	/// @private
	bool profileToggleState;

	/*! \brief  The current BodyModel running in the Smartsuit. This variable is updated automatically when the Smartsuit is connected, or when you apply a new BodyProfile to the Smartsuit. You can ask for the BodyModel running in Smartsuit using RefreshBodyModel().*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SmartsuitReadOnly, meta = (ToolTip = "Read-only: Indicates the current known BodyProfile for the Smartsuit connected to this SmartsuitController."))
		FBodyModel bodyModel;

	/*! \brief Restart the Smartsuit binded to this Controller.
	*
	* Restart the Smartsuit binded to this Controller.
	*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Send a restart command to the Smartsuit binded to this Controller."))
		void Restart();

	/*! \brief Performs an A-pose calibration to the Smartsuit.
	*
	* Performs an A-pose calibration to the Smartsuit.
	*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Send an A-pose calibration command to the Smartsuit binded to this Controller."))
		void Calibrate();

	/*! \brief Set the Smartsuit in broadcast mode.
	*
	* Requests the Smartsuit to broadcast its live streaming. This way more devices can listen to the same Smartsuit, as well as more applications in the same computer.
	* This setting may affect the WiFi performance.
	*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Send a request to make the Smartsuit, Broadcast it's UDP messages. This only applies for the real-time motion data. Not command responses."))
		void Broadcast();

	/*! \brief Set the Smartsuit in unicast mode.
	*
	* Requests the Smartsuit to unicast its live streaming. This way only one application in one device will have access to data from the Smartsuit.
	* This setting provides the best WiFiPerformance.
	*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Send a request to make the Smartsuit, Unicast it's UDP messages to the current device. This only applies for the real-time motion data. Not command responses."))
		void Unicast();

	/*! \brief Set the BodyModel in the Smartsuit.
	*
	* Sends a BodyModel to the Smartsuit. If the Smartsuit received the BodyModel successfully, the SmartsuitController updates its own BodyModel reference to match the one it sent.
	*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Send a command to set a BodyModel to the Smartsuit binded to this Controller."))
		void SetBodyModel(FBodyModel bodyToSet);

	/*! \brief Get the BodyModel from the Smartsuit.
	*
	* Asks the Smartsuit about the BodyModel it is running. When the Smartsuit responds, the SmartsuitController updates its BodyModel reference to match the one received from the Smartsuit.
	*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Sends a request to the Smartsuit to ask what BodyModel the Smartsuit is using. Once the Controller receives the response. It updates it's BodyModel to match the one the Smartsuit is using."))
		void GetBodyModel();

	/*! \brief Various Developer Tests.*/
	UFUNCTION(BlueprintCallable, Category = "Smartsuit", meta = (ToolTip = "Tests"))
		void DoTests();
		
	///*! \brief Returns a SmartsuitController by the name.
	//*
	//* @param name The name of the Smartsuit to look for.
	//* @return A SmartsuitController that has the specified Smartsuit name, if non found, it will return nullptr.
	//*/
	//UFUNCTION(BlueprintPure, Category = "Smartsuit", meta = (BlueprintThreadSafe, ToolTip = "Returns the SmartsuitController given the Smartsuit name."))
	//static ASmartsuitController* GetSmartsuitControllerByName(FString name){
	//	ASmartsuitController * actor = nullptr;
	//	// Find UObjects by type
	//	//UE_LOG(LogTemp, Warning, TEXT("Looking for actors...%s"), *name);

	//	for (TObjectIterator<ASmartsuitController> It; It; ++It)
	//	{
	//		if (It->realLife) {
	//			ASmartsuitController* curActor = *It;
	//			FString mySuitName = curActor->suitname;
	//			//UE_LOG(LogTemp, Warning, TEXT("Looking for actor.. %s == %s"), *mySuitName, *name);
	//			if (name.Compare(mySuitName) == 0) {
	//				//UE_LOG(LogTemp, Warning, TEXT("Actors match!!"));
	//				actor = curActor;
	//				break;
	//			}
	//		}
	//		//else {
	//		//	UE_LOG(LogTemp, Warning, TEXT("Actors no match.."));
	//		//}
	//		// ...
	//	}


	//	return actor;
	//}
	//
	///*! \brief Returns a SmartsuitController from its index ID.
	//*
	//* @param id The index id that will look up for.
	//* @return A SmartsuitController that has the specified index id, if non found, it will return nullptr.
	//*/
	//UFUNCTION(BlueprintPure, Category = "Smartsuit", meta = (BlueprintThreadSafe, ToolTip = "Returns a SmartsuitController given it's Index ID. The Index ID is specified in the SmartsuitController details."))
	//static ASmartsuitController* GetSmartsuitController(int id){
	//	ASmartsuitController * actor = nullptr;
	//	// Find UObjects by type
	//	//UE_LOG(LogTemp, Warning, TEXT("Looking for actors...%s"), *name);

	//	for (TObjectIterator<ASmartsuitController> It; It; ++It)
	//	{
	//		if (It->realLife) {
	//			ASmartsuitController* curActor = *It;
	//			//UE_LOG(LogTemp, Warning, TEXT("Looking for actor.. %s == %s"), *mySuitName, *name);
	//			if (id == curActor->IndexID) {
	//				//UE_LOG(LogTemp, Warning, TEXT("Actors match!!"));
	//				actor = curActor;
	//				break;
	//			}
	//		}
	//		//else {
	//		//	UE_LOG(LogTemp, Warning, TEXT("Actors no match.."));
	//		//}
	//		// ...
	//	}


	//	return actor;
	//}

	/// @cond no_doc
	bool SupportsWiFi();
	/// @endcond

	bool IsRealLife() { return realLife; }
private:
	ASmartsuitReceiver* GetReceiver();
	uint32 GetLocalIP();
    uint32 GetLocalMacIP();
	uint8 GetByte(uint32 val, int i);
	bool hubInfoRequested;
	bool bodyModelRequested;
	HubInfo* hubInfo;
	void GetHubInfo();
	void UpdateWiFiApiString();
	void SendCommand(unsigned char cmd, uint8 *customData, int customDataLength);

protected:
	/*! \brief Indicates if this instance of the actor is in play mode and not an instance from the editor.*/
	bool realLife = false;


};

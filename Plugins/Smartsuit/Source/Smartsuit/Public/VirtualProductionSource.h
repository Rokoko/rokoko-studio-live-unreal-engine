// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "ILiveLinkClient.h"
#include "ILiveLinkSource.h"
#include "Runtime/RenderCore/Public/RenderCore.h"
#include "MessageEndpoint.h"
#include "VirtualProductionFrame.h"
#include "SmartsuitDefinitions.h"
#include "VirtualProductionSource.generated.h"

class ILiveLinkClient;
struct FLiveLinkPongMessage;
struct FLiveLinkSubjectDataMessage;
struct FLiveLinkSubjectFrameMessage;
struct FLiveLinkHeartbeatMessage;
struct FLiveLinkClearSubject;

USTRUCT()
struct FVPMorphTargetMapping
{
	GENERATED_BODY()

	UPROPERTY()
		FName CurveName;

	UPROPERTY()
		FName CurveNameOverride;
};

UCLASS(BlueprintType)
class SMARTSUIT_API UVPFaceMorphTargetNameRemapping : public UDataAsset
{
	GENERATED_BODY()

	UVPFaceMorphTargetNameRemapping()
	{
		browDownLeft = "browDownLeft";
		browDownRight = "browDownRight";
		browInnerUp = "browInnerUp";
		browOuterUpLeft = "browOuterUpLeft";
		browOuterUpRight = "browOuterUpRight";
		cheekPuff = "cheekPuff";
		cheekSquintLeft = "cheekSquintLeft";
		cheekSquintRight = "cheekSquintRight";
		eyeBlinkLeft = "eyeBlinkLeft";
		eyeBlinkRight = "eyeBlinkRight";
		eyeLookDownLeft = "eyeLookDownLeft";
		eyeLookDownRight = "eyeLookDownRight";
		eyeLookInLeft = "eyeLookInLeft";
		eyeLookInRight = "eyeLookInRight";
		eyeLookOutLeft = "eyeLookOutLeft";
		eyeLookOutRight = "eyeLookOutRight";
		eyeLookUpLeft = "eyeLookUpLeft";
		eyeLookUpRight = "eyeLookUpRight";
		eyeSquintLeft = "eyeSquintLeft";
		eyeSquintRight = "eyeSquintRight";
		eyeWideLeft = "eyeWideLeft";
		eyeWideRight = "eyeWideRight";
		jawOpen = "jawOpen";
		jawForward = "jawForward";
		jawLeft = "jawLeft";
		jawRight = "jawRight";
		mouthClose = "mouthClose";
		mouthDimpleLeft = "mouthDimpleLeft";
		mouthDimpleRight = "mouthDimpleRight";
		mouthFrownLeft = "mouthFrownLeft";
		mouthFrownRight = "mouthFrownRight";
		mouthFunnel = "mouthFunnel";
		mouthLeft = "mouthLeft";
		mouthLowerDownLeft = "mouthLowerDownLeft";
		mouthLowerDownRight = "mouthLowerDownRight";
		mouthPressLeft = "mouthPressLeft";
		mouthPressRight = "mouthPressRight";
		mouthPucker = "mouthPucker";
		mouthRight = "mouthRight";
		mouthRollLower = "mouthRollLower";
		mouthRollUpper = "mouthRollUpper";
		mouthShrugLower = "mouthShrugLower";
		mouthShrugUpper = "mouthShrugUpper";
		mouthSmileLeft = "mouthSmileLeft";
		mouthSmileRight = "mouthSmileRight";
		mouthStretchLeft = "mouthStretchLeft";
		mouthStretchRight = "mouthStretchRight";
		mouthUpperUpLeft = "mouthUpperUpLeft";
		mouthUpperUpRight = "mouthUpperUpRight";
		noseSneerLeft = "noseSneerLeft";
		noseSneerRight = "noseSneerRight";
		tongueOut = "tongueOut";
	}
public:

	UFUNCTION(BlueprintCallable)
	void InitializeTMap()
	{
		NameMapping.Add("browDownLeft", browDownLeft);
		NameMapping.Add("browDownRight", browDownRight);
		NameMapping.Add("browInnerUp", browInnerUp);
		NameMapping.Add("browOuterUpLeft", browOuterUpLeft);
		NameMapping.Add("browOuterUpRight", browOuterUpRight);
		NameMapping.Add("cheekPuff", cheekPuff);
		NameMapping.Add("cheekSquintLeft", cheekSquintLeft);
		NameMapping.Add("cheekSquintRight", cheekSquintRight);
		NameMapping.Add("eyeBlinkLeft", eyeBlinkLeft);
		NameMapping.Add("eyeBlinkRight", eyeBlinkRight);
		NameMapping.Add("eyeLookDownLeft", eyeLookDownLeft);
		NameMapping.Add("eyeLookDownRight", eyeLookDownRight);
		NameMapping.Add("eyeLookInLeft", eyeLookInLeft);
		NameMapping.Add("eyeLookInRight", eyeLookInRight);
		NameMapping.Add("eyeLookOutLeft", eyeLookOutLeft);
		NameMapping.Add("eyeLookOutRight", eyeLookOutRight);
		NameMapping.Add("eyeLookUpLeft", eyeLookUpLeft);
		NameMapping.Add("eyeLookUpRight", eyeLookUpRight);
		NameMapping.Add("eyeSquintLeft", eyeSquintLeft);
		NameMapping.Add("eyeSquintRight", eyeSquintRight);
		NameMapping.Add("eyeWideLeft", eyeWideLeft);
		NameMapping.Add("eyeWideRight", eyeWideRight);
		NameMapping.Add("jawOpen", jawOpen);
		NameMapping.Add("jawForward", jawForward);
		NameMapping.Add("jawLeft", jawLeft);
		NameMapping.Add("jawRight", jawRight);
		NameMapping.Add("mouthClose", mouthClose);
		NameMapping.Add("mouthDimpleLeft", mouthDimpleLeft);
		NameMapping.Add("mouthDimpleRight", mouthDimpleRight);
		NameMapping.Add("mouthFrownLeft", mouthFrownLeft);
		NameMapping.Add("mouthFrownRight", mouthFrownRight);
		NameMapping.Add("mouthFunnel", mouthFunnel);
		NameMapping.Add("mouthLeft", mouthLeft);
		NameMapping.Add("mouthLowerDownLeft", mouthLowerDownLeft);
		NameMapping.Add("mouthLowerDownRight", mouthLowerDownRight);
		NameMapping.Add("mouthPressLeft", mouthPressLeft);
		NameMapping.Add("mouthPressRight", mouthPressRight);
		NameMapping.Add("mouthPucker", mouthPucker);
		NameMapping.Add("mouthRight", mouthRight);
		NameMapping.Add("mouthRollLower", mouthRollLower);
		NameMapping.Add("mouthRollUpper", mouthRollUpper);
		NameMapping.Add("mouthShrugLower", mouthShrugLower);
		NameMapping.Add("mouthShrugUpper", mouthShrugUpper);
		NameMapping.Add("mouthSmileLeft", mouthSmileLeft);
		NameMapping.Add("mouthSmileRight", mouthSmileRight);
		NameMapping.Add("mouthStretchLeft", mouthStretchLeft);
		NameMapping.Add("mouthStretchRight", mouthStretchRight);
		NameMapping.Add("mouthUpperUpLeft", mouthUpperUpLeft);
		NameMapping.Add("mouthUpperUpRight", mouthUpperUpRight);
		NameMapping.Add("noseSneerLeft", noseSneerLeft);
		NameMapping.Add("noseSneerRight", noseSneerRight);
		NameMapping.Add("tongueOut", tongueOut);
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FName> NameMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName browDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName browDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName browInnerUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName browOuterUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName browOuterUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName cheekPuff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName cheekSquintLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName cheekSquintRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeBlinkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeBlinkRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookInLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookInRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookOutLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookOutRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeLookUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeSquintLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeSquintRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeWideLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName eyeWideRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName jawOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName jawForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName jawLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName jawRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthClose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthDimpleLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthDimpleRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthFrownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthFrownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthFunnel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthLowerDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthLowerDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthPressLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthPressRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthPucker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthRollLower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthRollUpper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthShrugLower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthShrugUpper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthSmileLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthSmileRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthStretchLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthStretchRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthUpperUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName mouthUpperUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName noseSneerLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName noseSneerRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName tongueOut;

};


/**
 * 
 */
class SMARTSUIT_API FVirtualProductionSource : public ILiveLinkSource
{
public:
	FVirtualProductionSource(const FText& InSourceType, const FText& InSourceMachineName, const FMessageAddress& InConnectionAddress)
		: SourceType(InSourceType)
		, SourceMachineName(InSourceMachineName)
		
	{}

	FVirtualProductionSource(){}

	virtual ~FVirtualProductionSource();

	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid);
	virtual bool IsSourceStillValid() const override;
	virtual bool RequestSourceShutdown();

	virtual FText GetSourceType() const { return SourceType; }
	virtual FText GetSourceMachineName() const { return SourceMachineName; }
	virtual FText GetSourceStatus() const { return SourceStatus; }
	static TSharedPtr<FVirtualProductionSource> Get() { return instance; }

	void HandleSubjectFrame(TArray<FVirtualProductionSubject> virtualProductionObject);
	void HandleFace(TArray<FFace> faces);
	void HandleSuits(TArray<SuitData> suits);
	void ClearAllSubjects();

	static void SetInstance(TSharedPtr<FVirtualProductionSource> NewInstance) { instance = NewInstance; }

	static TSharedPtr<FVirtualProductionSource> CreateLiveLinkSource();
	static void RemoveLiveLinkSource(TSharedPtr<FVirtualProductionSource> InSource);
private:
	void HandleClearSubject(const FName subjectName);
	void HandleSubjectData(FVirtualProductionSubject virtualProductionObject);
	void HandleFaceData(FFace face);
	void HandleSuitData(SuitData suit);
	void CreateJoint(TArray<FTransform>& transforms, int32 index, Sensor* parent, Sensor* sensor);
	TArray<FName> subjectNames;
	TArray<FName> faceNames;
	TArray<FName> suitNames;

	TArray<FVirtualProductionSubject> existingSubjects;
	TArray<FFace> existingFaces;
	TArray<SuitData> existingSuits;

	TArray<FName> notExistingSubjects;

	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	//singleton instance
	//static FVirtualProductionSource *instance;
	static TSharedPtr<FVirtualProductionSource> instance;

};

// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "Animation/AnimNodeBase.h"

#include "CoreMinimal.h"
#include "LiveLinkClientReference.h"
#include "LiveLinkRetargetAsset.h"
#include "LiveLinkTypes.h"

#include "AnimNode_RokokoFacePose.generated.h"


class ILiveLinkClient;
class URokokoFaceMapData;


PRAGMA_DISABLE_DEPRECATION_WARNINGS

USTRUCT(BlueprintInternalUseOnly)
struct SMARTSUIT_API FAnimNode_RokokoFacePose : public FAnimNode_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FPoseLink InputPose;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SourceData, meta = (PinShownByDefault))
	//FLiveLinkSubjectName LiveLinkSubjectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SourceData, meta = (PinShownByDefault))
	FName RokokoActorName;

//#if WITH_EDITORONLY_DATA
//	UE_DEPRECATED(4.23, "FName SubjectName is deprecated. Use the SubjectName of type FLiveLinkSubjectName instead.")
//	UPROPERTY()
//	FName SubjectName_DEPRECATED;
//#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = Retarget, meta = (PinShownByDefault))
	TSubclassOf<ULiveLinkRetargetAsset> RetargetAsset;

	UPROPERTY(transient)
	ULiveLinkRetargetAsset* CurrentRetargetAsset;

public:
	FAnimNode_RokokoFacePose();

	//~ FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext & Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext & Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	virtual bool HasPreUpdate() const { return true; }
	virtual void PreUpdate(const UAnimInstance* InAnimInstance) override;
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	//~ End of FAnimNode_Base interface

	FLiveLinkSubjectName GetLiveLinkSubjectName();

	bool Serialize(FArchive& Ar);

protected:
	virtual void OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance) override;

private:

	FLiveLinkClientReference LiveLinkClient_GameThread;
	ILiveLinkClient* LiveLinkClient_AnyThread;

	// Delta time from update so that it can be passed to retargeter
	float CachedDeltaTime;
};

PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> struct TStructOpsTypeTraits<FAnimNode_RokokoFacePose> : public TStructOpsTypeTraitsBase2<FAnimNode_RokokoFacePose>
{
	enum 
	{ 
		WithSerializer = true
	};
};

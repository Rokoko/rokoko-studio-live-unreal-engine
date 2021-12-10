//// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//
//#include "AnimNode_ModifyFaceCurves.h"
//#include "AnimationRuntime.h"
//#include "Animation/AnimInstanceProxy.h"
//
//FAnimNode_ModifyFaceCurves::FAnimNode_ModifyFaceCurves()
//{
//	ApplyMode = EModifyFaceCurvesApplyMode::Blend;
//	Alpha = 1.f;
//}
//
//void FAnimNode_ModifyFaceCurves::Initialize_AnyThread(const FAnimationInitializeContext& Context)
//{
//	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
//	Super::Initialize_AnyThread(Context);
//	SourcePose.Initialize(Context);
//
//	// Init our last values array to be the right size
//	if (ApplyMode == EModifyFaceCurvesApplyMode::WeightedMovingAverage)
//	{
//		LastCurveValues.Reset(CurveValues.Num());
//		LastCurveValues.AddZeroed(CurveValues.Num());
//	}
//}
//
//void FAnimNode_ModifyFaceCurves::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
//{
//	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)
//	Super::CacheBones_AnyThread(Context);
//	SourcePose.CacheBones(Context);
//}
//
//void FAnimNode_ModifyFaceCurves::Evaluate_AnyThread(FPoseContext& Output)
//{
//	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread)
//	FPoseContext SourceData(Output);
//	SourcePose.Evaluate(SourceData);
//
//	Output = SourceData;
//
//	//	Morph target and Material parameter curves
//	USkeleton* Skeleton = Output.AnimInstanceProxy->GetSkeleton();
//
//	check(CurveNames.Num() == CurveValues.Num());
//
//	for (int32 ModIdx = 0; ModIdx < CurveNames.Num(); ModIdx++)
//	{
//		FName CurveName = CurveNames[ModIdx];
//		SmartName::UID_Type NameUID = Skeleton->GetUIDByName(USkeleton::AnimCurveMappingName, CurveName);
//		if (NameUID != SmartName::MaxUID)
//		{
//			float CurveValue = CurveValues[ModIdx];
//			float CurrentValue = Output.Curve.Get(NameUID);
//
//			// Use ApplyMode enum to decide how to apply
//			if (ApplyMode == EModifyFaceCurvesApplyMode::Add)
//			{
//				Output.Curve.Set(NameUID, CurrentValue + CurveValue);
//			}
//			else if (ApplyMode == EModifyFaceCurvesApplyMode::Scale)
//			{
//				Output.Curve.Set(NameUID, CurrentValue * CurveValue);
//			}
//			else if (ApplyMode == EModifyFaceCurvesApplyMode::WeightedMovingAverage)
//			{
//				check(LastCurveValues.Num() == CurveValues.Num());
//
//				const float LastCurveValue = LastCurveValues[ModIdx];
//				const float WAvg = FMath::WeightedMovingAverage(CurrentValue, LastCurveValue, Alpha);
//				// Update the last curve value for next run
//				LastCurveValues[ModIdx] = WAvg;
//
//				Output.Curve.Set(NameUID, WAvg);
//			}
//			else if (ApplyMode == EModifyFaceCurvesApplyMode::RemapCurve)
//			{
//				const float RemapScale = 1.f / FMath::Max(1.f - CurveValue, 0.01f);
//				const float RemappedValue = FMath::Min(FMath::Max(CurrentValue - CurveValue, 0.f) * RemapScale, 1.f);
//				Output.Curve.Set(NameUID, RemappedValue);
//			}
//			else // Blend
//			{
//				float UseAlpha = FMath::Clamp(Alpha, 0.f, 1.f);
//				Output.Curve.Set(NameUID, FMath::Lerp(CurrentValue, CurveValue, UseAlpha));
//			}
//		}
//	}
//
//
//
//	//if (Skeleton)
//	//{
//	//	ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
//
//	//	FLiveLinkSubjectFrameData SubjectData;
//	//	if (LiveLinkClient.EvaluateFrame_AnyThread(SubjectRepresentation.Subject, SubjectRepresentation.Role, SubjectData))
//	//	{
//	//		FLiveLinkSkeletonStaticData* StaticData = SubjectData.StaticData.Cast<FLiveLinkSkeletonStaticData>();
//	//		FLiveLinkAnimationFrameData* FrameData = SubjectData.FrameData.Cast<FLiveLinkCameraFrameData>();
//
//	//		if (StaticData && FrameData)
//	//		{
//	//			TransformData.ApplyTransform(CameraComponent, FrameData->Transform);
//
//	//			if (StaticData->bIsFieldOfViewSupported) { CameraComponent->SetFieldOfView(FrameData->FieldOfView); }
//	//			if (StaticData->bIsAspectRatioSupported) { CameraComponent->SetAspectRatio(FrameData->AspectRatio); }
//	//			if (StaticData->bIsProjectionModeSupported) { CameraComponent->SetProjectionMode(FrameData->ProjectionMode == ELiveLinkCameraProjectionMode::Perspective ? ECameraProjectionMode::Perspective : ECameraProjectionMode::Orthographic); }
//
//	//			if (UCineCameraComponent* CineCameraComponent = Cast<UCineCameraComponent>(CameraComponent))
//	//			{
//	//				if (StaticData->bIsFocalLengthSupported) { CineCameraComponent->CurrentFocalLength = FrameData->FocalLength; }
//	//				if (StaticData->bIsApertureSupported) { CineCameraComponent->CurrentAperture = FrameData->Aperture; }
//	//				if (StaticData->FilmBackWidth > 0.0f) { CineCameraComponent->Filmback.SensorWidth = StaticData->FilmBackWidth; }
//	//				if (StaticData->FilmBackHeight > 0.0f) { CineCameraComponent->Filmback.SensorHeight = StaticData->FilmBackHeight; }
//	//				if (StaticData->bIsFocusDistanceSupported) { CineCameraComponent->FocusSettings.ManualFocusDistance = FrameData->FocusDistance; }
//	//			}
//	//		}
//	//	}
//	//}
//}
//
//void FAnimNode_ModifyFaceCurves::Update_AnyThread(const FAnimationUpdateContext& Context)
//{
//	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread)
//	// Run update on input pose nodes
//	SourcePose.Update(Context);
//
//	// Evaluate any BP logic plugged into this node
//	GetEvaluateGraphExposedInputs().Execute(Context);
//}
//
//#if WITH_EDITOR
//
//void FAnimNode_ModifyFaceCurves::AddCurve(const FName& InName, float InValue)
//{
//	CurveValues.Add(InValue);
//	CurveNames.Add(InName);
//}
//
//void FAnimNode_ModifyFaceCurves::RemoveCurve(int32 PoseIndex)
//{
//	CurveValues.RemoveAt(PoseIndex);
//	CurveNames.RemoveAt(PoseIndex);
//}
//
//#endif // WITH_EDITOR

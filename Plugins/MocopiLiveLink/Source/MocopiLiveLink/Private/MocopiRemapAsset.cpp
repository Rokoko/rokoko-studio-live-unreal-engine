// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

// Some useful information on Retargeting here:
// https://csyonghe.github.io/AnimationRetargeting/ 

#include "MocopiRemapAsset.h"

#include "BoneIndices.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "Roles/LiveLinkAnimationTypes.h"

void UMocopiRemapAsset::BuildPoseFromAnimationData(float DeltaTime,
  const FLiveLinkSkeletonStaticData* InSkeletonData,
  const FLiveLinkAnimationFrameData* InFrameData,
  FCompactPose& OutPose)
{
  const TArray<FName>& sourceBoneNames = InSkeletonData->GetBoneNames();
  TArray<FName, TMemStackAllocator<>> targetBoneNames;
  targetBoneNames.Reserve(sourceBoneNames.Num());

  // Find remapped bone names (which bone names on the source skeleton correspond to which bone names on the target skeleton)
  for (const FName& srcBoneName : sourceBoneNames)
  {
    FName* TargetBoneName = mBoneNameMap.Find(srcBoneName);
    if (TargetBoneName == nullptr)
    {
      FName newBoneName = GetRemappedBoneName(srcBoneName);
      targetBoneNames.Add(newBoneName);
      mBoneNameMap.Add(srcBoneName, newBoneName);
    }
    else
    {
      targetBoneNames.Add(*TargetBoneName);
    }
  }

  const FBoneContainer& boneContainerRef = OutPose.GetBoneContainer();

  // Initialize World Space rotations
  if (!mHasInitialized)
  {
    // Initializing rotations from Mesh bones. (Max number of bones)
    int32 meshBoneCount = boneContainerRef.GetNumBones();
    mWorldSpaceRotations.Init(FQuat::Identity, meshBoneCount);

    const TArray<FTransform>& meshBoneRefPose = boneContainerRef.GetRefPoseArray();
    for (int32 i = 0; i < meshBoneCount; i++)
    {
      FQuat rotation = meshBoneRefPose[i].GetRotation();

      int32 ParentIndex = boneContainerRef.GetParentBoneIndex(i);
      if ((ParentIndex != INDEX_NONE) && (ParentIndex < meshBoneCount))
      {
        rotation = mWorldSpaceRotations[ParentIndex] * rotation;
      }

      mWorldSpaceRotations[i] = rotation;
    }

    // Overriding corresponding mesh bone rotations with compact pose bone rotations.
    // This takes into account the input pose that we have created in our animation blueprint.
    int32 cpBoneCount = OutPose.GetNumBones();
    TArray<FQuat> cpRefPoseRotation;
    cpRefPoseRotation.Init(FQuat::Identity, cpBoneCount);

    for (int32 i = 0; i < cpBoneCount; ++i)
    {
      FCompactPoseBoneIndex cpIndex(i);

      FQuat rotation = OutPose[cpIndex].GetRotation();

      FCompactPoseBoneIndex CPParentBoneIndex = boneContainerRef.GetParentBoneIndex(cpIndex);
      if ((CPParentBoneIndex != INDEX_NONE) && (CPParentBoneIndex < cpBoneCount))
      {
        int32 parentMeshIndex = boneContainerRef.MakeMeshPoseIndex(CPParentBoneIndex).GetInt();
        rotation = cpRefPoseRotation[CPParentBoneIndex.GetInt()] * rotation;
      }

      cpRefPoseRotation[i] = rotation;

      int32 meshIndex = boneContainerRef.MakeMeshPoseIndex(cpIndex).GetInt();

      mWorldSpaceRotations[meshIndex] = rotation;
    }

    mHasInitialized = true;
  }

  // Process incoming data
  for (int32 i = 0; i < targetBoneNames.Num(); ++i)
  {
    FName boneName = targetBoneNames[i];
    FTransform boneTransform = InFrameData->Transforms[i];

    int32 meshIndex = boneContainerRef.GetPoseBoneIndexForBoneName(boneName);
    FCompactPoseBoneIndex cpIndex = boneContainerRef.MakeCompactPoseIndex(FMeshPoseBoneIndex(meshIndex));

    if (cpIndex != INDEX_NONE)
    {
      if (boneName == TargetSkeletonHipBoneName)
      {
        FQuat rotation = ConvertBoneRotation(boneTransform.GetRotation()) * mWorldSpaceRotations[meshIndex];
        FVector position = ConvertRootPosition(boneTransform.GetLocation());
        FTransform hipsTransform(rotation, position);
        OutPose[cpIndex] = hipsTransform;
        continue;
      }

      int32 parentMeshIndex = boneContainerRef.GetParentBoneIndex(meshIndex);

      if (parentMeshIndex != INDEX_NONE)
      {
        FQuat rotationInComponentSpace = ConvertBoneRotation(boneTransform.GetRotation()) * mWorldSpaceRotations[meshIndex];
        FQuat rotationInParentBoneSpace = mWorldSpaceRotations[parentMeshIndex].Inverse() * rotationInComponentSpace;
        OutPose[cpIndex].SetRotation(rotationInParentBoneSpace);
      }
    }
  }
}

FVector UMocopiRemapAsset::ConvertRootPosition(FVector position) const
{
  return FVector(position.X, position.Z, position.Y);
}

FQuat UMocopiRemapAsset::ConvertBoneRotation(FQuat rotation) const
{
  return FQuat(rotation.X, rotation.Z, rotation.Y, -rotation.W);
}

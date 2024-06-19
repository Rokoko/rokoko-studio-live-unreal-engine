// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#pragma once

#include <vector> 
#include "SonyMotionFormat.h"

struct MocopiBoneData
{
  const char* jointName;

  int         parentIndex;

  double      translate[3];
  double      rotate[4];

  double	  defaultTranslate[3];
  double	  defaultRotate[4];
};


struct MocopiFrameMetaData
{
  int frameId;

  float timeStamp;

  double utcTime;

  int numBones;

  unsigned long long senderIp;
  int senderPort;

  std::vector<int> jointIds;
  std::vector<int> parentJointIds;
};

class MocopiDataHandler : public SonyMotionFormat {

public:

	MocopiDataHandler();

	void ProcessSkeletonDefinitionData(std::byte* dataBuffer, int bufferSize);

	void ProcessFrameData(std::byte* dataBuffer, int bufferSize);

	bool IsSkeletonDefinition(std::byte* bytes, int bufferSize);
	bool IsFrameData(std::byte* dataBuffer, int bufferSize);

	const MocopiBoneData& GetBoneInfoByIndex(int boneIndex);

	int GetNumBones();


	const std::vector<MocopiBoneData>& GetFrameBoneDataArray() const;

	const MocopiFrameMetaData GetFrameMetaData();

private:

	// Unvariable joint number at this point.
	const int SKELETON_NUM_BONES = 27;

	// For scaling translation values
	const double BONE_SCALE = 100.0;

	std::vector<MocopiBoneData>	mBoneDataArray;

	MocopiFrameMetaData mFrameMetaData;

	void InitializeSkeletonChannels();

};

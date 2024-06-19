// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#include "MocopiDataHandler.h"

MocopiDataHandler::MocopiDataHandler()
{
  InitializeSkeletonChannels();
}

void MocopiDataHandler::ProcessSkeletonDefinitionData(std::byte* dataBuffer, int bufferSize)
{
  unsigned long long senderIp = 0;
  int senderPort = 0;
  int numBones = 0;
  int* jointIds = nullptr, * parentJointIds = nullptr;

  float* rotX, * rotY, * rotZ, * rotW;
  float* posX, * posY, * posZ;
  rotX = rotY = rotZ = rotW = posX = posY = posZ = nullptr;

  bool success = ConvertBytesToSkeletonDefinition(bufferSize, dataBuffer, senderIp, senderPort,
                                                  numBones, jointIds, parentJointIds,
                                                  rotX, rotY, rotZ, rotW,
                                                  posX, posY, posZ
                                                 );

  if (success == false)
  {
    return;
  }

  mFrameMetaData.senderIp = senderIp;
  mFrameMetaData.senderPort = senderPort;
  mFrameMetaData.numBones = numBones;

  for (size_t i = 0; i < numBones; i++)
  {
    mBoneDataArray[i].defaultTranslate[0] = posX[i] * BONE_SCALE;
    mBoneDataArray[i].defaultTranslate[1] = posY[i] * BONE_SCALE;
    mBoneDataArray[i].defaultTranslate[2] = posZ[i] * BONE_SCALE;

    mBoneDataArray[i].translate[0] = posX[i] * BONE_SCALE;
    mBoneDataArray[i].translate[1] = posY[i] * BONE_SCALE;
    mBoneDataArray[i].translate[2] = posZ[i] * BONE_SCALE;

    mBoneDataArray[i].defaultRotate[0] = rotX[i];
    mBoneDataArray[i].defaultRotate[1] = rotY[i];
    mBoneDataArray[i].defaultRotate[2] = rotZ[i];
    mBoneDataArray[i].defaultRotate[3] = rotW[i];

    mBoneDataArray[i].rotate[0] = rotX[i];
    mBoneDataArray[i].rotate[1] = rotY[i];
    mBoneDataArray[i].rotate[2] = rotZ[i];
    mBoneDataArray[i].rotate[3] = rotW[i];

    mFrameMetaData.parentJointIds[i] = parentJointIds[i];
    mFrameMetaData.jointIds[i] = jointIds[i];
  } 

}

void MocopiDataHandler::ProcessFrameData(std::byte* dataBuffer, int bufferSize)
{
  unsigned long long senderIp = 0;
  int senderPort = 0;
  int frameNum = 0;
  float timeStamp = 0;
  double utcTime = 0;
  int numBones = 0;
  int* jointIds = nullptr;

  float* rotX, * rotY, * rotZ, * rotW;
  float* posX, * posY, * posZ;
  rotX = rotY = rotZ = rotW = posX = posY = posZ = nullptr;

  bool success = ConvertBytesToFrameData( bufferSize, dataBuffer, senderIp, senderPort,
                                          frameNum, timeStamp, utcTime, numBones, jointIds,
                                          rotX, rotY, rotZ, rotW,
                                          posX, posY, posZ
                                        );
    
  if (success == false)
  {
    return;
  }

  mFrameMetaData.senderIp = senderIp;
  mFrameMetaData.senderPort = senderPort;
  mFrameMetaData.frameId = frameNum;
  mFrameMetaData.timeStamp = timeStamp;
  mFrameMetaData.utcTime = utcTime;
  mFrameMetaData.numBones = numBones;

  for (size_t i = 0; i < numBones; i++)
  {
    mBoneDataArray[i].translate[0] = posX[i] * BONE_SCALE;
    mBoneDataArray[i].translate[1] = posY[i] * BONE_SCALE;
    mBoneDataArray[i].translate[2] = posZ[i] * BONE_SCALE;

    mBoneDataArray[i].rotate[0] = rotX[i];
    mBoneDataArray[i].rotate[1] = rotY[i];
    mBoneDataArray[i].rotate[2] = rotZ[i];
    mBoneDataArray[i].rotate[3] = rotW[i];

    mFrameMetaData.jointIds[i] = jointIds[i];
  }
}

const std::vector<MocopiBoneData>& MocopiDataHandler::GetFrameBoneDataArray() const
{
  return mBoneDataArray;
}

const MocopiFrameMetaData MocopiDataHandler::GetFrameMetaData()
{
  return mFrameMetaData;
}


bool MocopiDataHandler::IsSkeletonDefinition(std::byte* dataBuffer, int bufferSize)
{
  if (IsSmfBytes(bufferSize, dataBuffer))
  {
    return IsSkeletonDefinitionBytes(bufferSize, dataBuffer);
  }

  return false;
}

bool MocopiDataHandler::IsFrameData(std::byte* dataBuffer, int bufferSize)
{
  if (IsSmfBytes(bufferSize, dataBuffer))
  {
    return IsFrameDataBytes(bufferSize, dataBuffer);
  }

  return false;
}

const MocopiBoneData& MocopiDataHandler::GetBoneInfoByIndex(int boneIndex)
{
  return mBoneDataArray[boneIndex];
}

int MocopiDataHandler::GetNumBones()
{
  return SKELETON_NUM_BONES;
}

void MocopiDataHandler::InitializeSkeletonChannels()
{
  // Unvariable Number of Bones at this point.
  mFrameMetaData.parentJointIds.resize(SKELETON_NUM_BONES);
  mFrameMetaData.jointIds.resize(SKELETON_NUM_BONES);
  mBoneDataArray.resize(SKELETON_NUM_BONES);

  for (int i = 0; i < SKELETON_NUM_BONES; i++)
  {
    mBoneDataArray[i].defaultRotate[0] = 0;
    mBoneDataArray[i].defaultRotate[1] = 0;
    mBoneDataArray[i].defaultRotate[2] = 0;
    mBoneDataArray[i].defaultRotate[3] = 1;

    mBoneDataArray[i].defaultTranslate[0] = 0;
    mBoneDataArray[i].defaultTranslate[1] = 0;
    mBoneDataArray[i].defaultTranslate[2] = 0;
  }

  mBoneDataArray[0].jointName = "root";
  mBoneDataArray[0].parentIndex = -1;
  mBoneDataArray[0].translate[0] = 0;
  mBoneDataArray[0].translate[1] = 94;
  mBoneDataArray[0].translate[2] = 0;

  mBoneDataArray[1].jointName = "torso_1";
  mBoneDataArray[1].parentIndex = 0;
  mBoneDataArray[1].translate[0] = 0;
  mBoneDataArray[1].translate[1] = 5;
  mBoneDataArray[1].translate[2] = 0;

  mBoneDataArray[2].jointName = "torso_2";
  mBoneDataArray[2].parentIndex = 1;
  mBoneDataArray[2].translate[0] = 0;
  mBoneDataArray[2].translate[1] = 5;
  mBoneDataArray[2].translate[2] = 0;

  mBoneDataArray[3].jointName = "torso_3";
  mBoneDataArray[3].parentIndex = 2;
  mBoneDataArray[3].translate[0] = 0;
  mBoneDataArray[3].translate[1] = 5;
  mBoneDataArray[3].translate[2] = 0;

  mBoneDataArray[4].jointName = "torso_4";
  mBoneDataArray[4].parentIndex = 3;
  mBoneDataArray[4].translate[0] = 0;
  mBoneDataArray[4].translate[1] = 5;
  mBoneDataArray[4].translate[2] = 0;

  mBoneDataArray[5].jointName = "torso_5";
  mBoneDataArray[5].parentIndex = 4;
  mBoneDataArray[5].translate[0] = 0;
  mBoneDataArray[5].translate[1] = 5;
  mBoneDataArray[5].translate[2] = 0;

  mBoneDataArray[6].jointName = "torso_6";
  mBoneDataArray[6].parentIndex = 5;
  mBoneDataArray[6].translate[0] = 0;
  mBoneDataArray[6].translate[1] = 7;
  mBoneDataArray[6].translate[2] = 0;

  mBoneDataArray[7].jointName = "torso_7";
  mBoneDataArray[7].parentIndex = 6;
  mBoneDataArray[7].translate[0] = 0;
  mBoneDataArray[7].translate[1] = 10;
  mBoneDataArray[7].translate[2] = 0;

  mBoneDataArray[8].jointName = "neck_1";
  mBoneDataArray[8].parentIndex = 7;
  mBoneDataArray[8].translate[0] = 0;
  mBoneDataArray[8].translate[1] = 4;
  mBoneDataArray[8].translate[2] = 0;

  mBoneDataArray[9].jointName = "neck_2";
  mBoneDataArray[9].parentIndex = 8;
  mBoneDataArray[9].translate[0] = 0;
  mBoneDataArray[9].translate[1] = 4;
  mBoneDataArray[9].translate[2] = 0;

  mBoneDataArray[10].jointName = "head";
  mBoneDataArray[10].parentIndex = 9;
  mBoneDataArray[10].translate[0] = 0;
  mBoneDataArray[10].translate[1] = 4;
  mBoneDataArray[10].translate[2] = 0;

  mBoneDataArray[11].jointName = "l_shoulder";
  mBoneDataArray[11].parentIndex = 7;
  mBoneDataArray[11].translate[0] = 1;
  mBoneDataArray[11].translate[1] = -6;
  mBoneDataArray[11].translate[2] = 7;

  mBoneDataArray[12].jointName = "l_up_arm";
  mBoneDataArray[12].parentIndex = 11;
  mBoneDataArray[12].translate[0] = 12;
  mBoneDataArray[12].translate[1] = 5;
  mBoneDataArray[12].translate[2] = 0;
  mBoneDataArray[12].rotate[2] = 90;

  mBoneDataArray[13].jointName = "l_low_arm";
  mBoneDataArray[13].parentIndex = 12;
  mBoneDataArray[13].translate[0] = 0;
  mBoneDataArray[13].translate[1] = -28;
  mBoneDataArray[13].translate[2] = 0;

  mBoneDataArray[14].jointName = "l_hand";
  mBoneDataArray[14].parentIndex = 13;
  mBoneDataArray[14].translate[0] = 0;
  mBoneDataArray[14].translate[1] = -23;
  mBoneDataArray[14].translate[2] = 0;

  mBoneDataArray[15].jointName = "r_shoulder";
  mBoneDataArray[15].parentIndex = 7;
  mBoneDataArray[15].translate[0] = -1;
  mBoneDataArray[15].translate[1] = -6;
  mBoneDataArray[15].translate[2] = 7;

  mBoneDataArray[16].jointName = "r_up_arm";
  mBoneDataArray[16].parentIndex = 15;
  mBoneDataArray[16].translate[0] = -12;
  mBoneDataArray[16].translate[1] = 5;
  mBoneDataArray[16].translate[2] = 0;
  mBoneDataArray[16].rotate[2] = -90;

  mBoneDataArray[17].jointName = "r_low_arm";
  mBoneDataArray[17].parentIndex = 16;
  mBoneDataArray[17].translate[0] = 0;
  mBoneDataArray[17].translate[1] = -28;
  mBoneDataArray[17].translate[2] = 0;

  mBoneDataArray[18].jointName = "r_hand";
  mBoneDataArray[18].parentIndex = 17;
  mBoneDataArray[18].translate[0] = 0;
  mBoneDataArray[18].translate[1] = -23;
  mBoneDataArray[18].translate[2] = 0;

  mBoneDataArray[19].jointName = "l_up_leg";
  mBoneDataArray[19].parentIndex = 0;
  mBoneDataArray[19].translate[0] = 9;
  mBoneDataArray[19].translate[1] = -4;
  mBoneDataArray[19].translate[2] = 3;

  mBoneDataArray[20].jointName = "l_low_leg";
  mBoneDataArray[20].parentIndex = 19;
  mBoneDataArray[20].translate[0] = 0;
  mBoneDataArray[20].translate[1] = -40;
  mBoneDataArray[20].translate[2] = 2;

  mBoneDataArray[21].jointName = "l_foot";
  mBoneDataArray[21].parentIndex = 20;
  mBoneDataArray[21].translate[0] = 0;
  mBoneDataArray[21].translate[1] = -40;
  mBoneDataArray[21].translate[2] = -2;

  mBoneDataArray[22].jointName = "l_toes";
  mBoneDataArray[22].parentIndex = 21;
  mBoneDataArray[22].translate[0] = 0;
  mBoneDataArray[22].translate[1] = -10;
  mBoneDataArray[22].translate[2] = 13;

  mBoneDataArray[23].jointName = "r_up_leg";
  mBoneDataArray[23].parentIndex = 0;
  mBoneDataArray[23].translate[0] = -9;
  mBoneDataArray[23].translate[1] = -4;
  mBoneDataArray[23].translate[2] = 3;

  mBoneDataArray[24].jointName = "r_low_leg";
  mBoneDataArray[24].parentIndex = 23;
  mBoneDataArray[24].translate[0] = 0;
  mBoneDataArray[24].translate[1] = -40;
  mBoneDataArray[24].translate[2] = 2;

  mBoneDataArray[25].jointName = "r_foot";
  mBoneDataArray[25].parentIndex = 24;
  mBoneDataArray[25].translate[0] = 0;
  mBoneDataArray[25].translate[1] = -40;
  mBoneDataArray[25].translate[2] = -2;

  mBoneDataArray[26].jointName = "r_toes";
  mBoneDataArray[26].parentIndex = 25;
  mBoneDataArray[26].translate[0] = 0;
  mBoneDataArray[26].translate[1] = -10;
  mBoneDataArray[26].translate[2] = 13;

}

// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#pragma once

#include <cstddef>
#include <string>


typedef void* LibHandle;

typedef bool (*IsSmfBytesProc) (int bytes_size, std::byte* bytes);

typedef bool (*IsSkeletonDefinitionBytesProc) (int bytes_size, std::byte* bytes);

typedef bool (*IsFrameDataBytesProc) (int bytessize, std::byte* bytes);

typedef bool (*ConvertBytesToSkeletonDefinitionProc) (
  int bytes_size, std::byte* bytes,
  unsigned long long& sender_ip, int& sender_port,
  int& data_size, int*& joint_ids, int*& parent_joint_ids,
  float*& rotations_x, float*& rotations_y, float*& rotations_z, float*& rotations_w,
  float*& positions_x, float*& positions_y, float*& positions_z
  );

typedef bool (*ConvertBytesToFrameDataProc) (
  int bytes_size, std::byte* bytes,
  unsigned long long& sender_ip, int& sender_port,
  int& frame_number, float& timestamp, double& utc_time,
  int& data_size, int*& joint_ids,
  float*& rotations_x, float*& rotations_y, float*& rotations_z, float*& rotations_w,
  float*& positions_x, float*& positions_y, float*& positions_z
  );

class SonyMotionFormat
{
public:

  static void Initialize(const std::string& pluginPath);
  static void Shutdown();
  static bool IsValid();

protected:
  // DLL Functions
  static IsSmfBytesProc IsSmfBytes;
  static IsSkeletonDefinitionBytesProc IsSkeletonDefinitionBytes;
  static IsFrameDataBytesProc IsFrameDataBytes;
  static ConvertBytesToSkeletonDefinitionProc ConvertBytesToSkeletonDefinition;
  static ConvertBytesToFrameDataProc ConvertBytesToFrameData;

private:

  static LibHandle mDllHandle;

  static std::string mPluginPath;

  static bool LoadDll();
  static std::string GetDLLPath();

};


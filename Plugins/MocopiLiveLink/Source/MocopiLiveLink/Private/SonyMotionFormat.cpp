// 2023 Copyright Sony Corporation, Inc. All Rights Reserved

#include "SonyMotionFormat.h"

#ifdef __APPLE__
#include <dlfcn.h>
#endif

#define LOCTEXT_NAMESPACE "MocopiLiveLinkModule"

#include "HAL/PlatformProcess.h"


#define SMF_IMPORT(Func) \
  Func = (Func##Proc)FPlatformProcess::GetDllExport(mDllHandle, TEXT(#Func)); \
	if (Func == nullptr) \
	{ \
        FText error = LOCTEXT("smfImportError", "Failed to import SonyMotionFormat function"); \
		UE_LOG(LogMocopiLiveLink, Error, TEXT("%s %s"), *(error.ToString()), TEXT(#Func)); \
		Shutdown(); \
	}


#define SMF_DEFINE(Func) \
    Func##Proc SonyMotionFormat::Func = nullptr;


SMF_DEFINE(IsSmfBytes)
SMF_DEFINE(IsSkeletonDefinitionBytes)
SMF_DEFINE(IsFrameDataBytes)
SMF_DEFINE(ConvertBytesToSkeletonDefinition)
SMF_DEFINE(ConvertBytesToFrameData)


LibHandle SonyMotionFormat::mDllHandle = nullptr;
std::string SonyMotionFormat::mPluginPath;


void SonyMotionFormat::Initialize(const std::string& pluginPath)
{
  if (mDllHandle == nullptr)
  {
    mPluginPath = pluginPath;

    if (LoadDll())
    {
      SMF_IMPORT(IsSmfBytes);
      SMF_IMPORT(IsSkeletonDefinitionBytes);
      SMF_IMPORT(IsFrameDataBytes);
      SMF_IMPORT(ConvertBytesToSkeletonDefinition);
      SMF_IMPORT(ConvertBytesToFrameData);
    }
  }
}

void SonyMotionFormat::Shutdown()
{
  if (mDllHandle != nullptr)
  {
    FPlatformProcess::FreeDllHandle(mDllHandle);
    mDllHandle = nullptr;
  }
}

bool SonyMotionFormat::IsValid()
{
  return mDllHandle != nullptr;
}

bool SonyMotionFormat::LoadDll()
{
  const std::string DllPath = GetDLLPath();

  const FString fsDllPath = FString(DllPath.c_str());

  mDllHandle = FPlatformProcess::GetDllHandle(*fsDllPath);

  if (mDllHandle == nullptr)
  {
    FText error = LOCTEXT("smfLoadError", "Failed to load the required SonyMotionFormat library. Plugin will not be functional. File Path: ");
    UE_LOG(LogMocopiLiveLink, Error, TEXT("%s %s"), *(error.ToString()), *fsDllPath);
    return false;
  }

  return true;
}

std::string SonyMotionFormat::GetDLLPath()
{ 
#ifdef _WIN64
  const std::string DllName = "sony_motion_format.dll";
  const std::string Platform = "windows";
  const std::string Arch = "x64";

#elif __APPLE__
  const std::string DllName = "sony_motion_format.bundle/Contents/MacOS/sony_motion_format";
  const std::string Platform = "mac";
  const std::string Arch = "";
#endif

  const std::string SMFFolder = "Source/SonyMotionFormat";

  std::string finalPath = mPluginPath + "/" + SMFFolder + "/" + Platform + "/" + Arch + "/" + DllName;

  return finalPath;
}

#undef LOCTEXT_NAMESPACE

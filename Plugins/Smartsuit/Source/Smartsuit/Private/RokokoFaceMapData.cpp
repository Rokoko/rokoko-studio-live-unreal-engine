
#include "RokokoFaceMapData.h"


FName URokokoFaceMapData::GetRemappedCurveName_Implementation(FName CurveName) const
{
	if (auto RemappedName = NameMapping.Find(CurveName))
	{
		return *RemappedName;
	}
	return "";
}

void URokokoFaceMapData::Initialize()
{
	InitializeTMap();
}
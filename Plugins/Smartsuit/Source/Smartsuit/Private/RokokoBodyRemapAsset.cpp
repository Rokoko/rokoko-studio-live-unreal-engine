
#include "RokokoBodyRemapAsset.h"


URokokoBodyMapData::URokokoBodyMapData()
{
	hip = "hip";

	stomach = "stomach";

	chest = "chest";

	neck = "neck";

	head = "head";

	leftShoulder = "leftShoulder";

	leftArm = "leftArm";

	leftForearm = "leftForearm";

	leftHand = "leftHand";

	rightShoulder = "rightShoulder";

	rightArm = "rightArm";

	rightForearm = "rightForearm";

	rightHand = "rightHand";

	leftUpleg = "leftUpleg";

	leftLeg = "leftLeg";

	leftFoot = "leftFoot";

	leftToe = "leftToe";

	rightUpleg = "rightUpleg";

	rightLeg = "rightLeg";

	rightFoot = "rightFoot";

	rightToe = "rightToe";

	leftThumbProximal = "leftThumbProximal";

	leftThumbMedial = "leftThumbMedial";

	leftThumbDistal = "leftThumbDistal";

	leftThumbTip = "leftThumbTip";

	leftIndexProximal = "leftIndexProximal";

	leftIndexMedial = "leftIndexMedial";

	leftIndexDistal = "leftIndexDistal";

	leftIndexTip = "leftIndexTip";

	leftMiddleProximal = "leftMiddleProximal";

	leftMiddleMedial = "leftMiddleMedial";

	leftMiddleDistal = "leftMiddleDistal";

	leftMiddleTip = "leftMiddleTip";

	leftRingProximal = "leftRingProximal";

	leftRingMedial = "leftRingMedial";

	leftRingDistal = "leftRingDistal";

	leftRingTip = "leftRingTip";

	leftLittleProximal = "leftLittleProximal";

	leftLittleMedial = "leftLittleMedial";

	leftLittleDistal = "leftLittleDistal";

	leftLittleTip = "leftLittleTip";

	rightThumbProximal = "rightThumbProximal";

	rightThumbMedial = "rightThumbMedial";

	rightThumbDistal = "rightThumbDistal";

	rightThumbTip = "rightThumbTip";

	rightIndexProximal = "rightIndexProximal";

	rightIndexMedial = "rightIndexMedial";

	rightIndexDistal = "rightIndexDistal";

	rightIndexTip = "rightIndexTip";

	rightMiddleProximal = "rightMiddleProximal";

	rightMiddleMedial = "rightMiddleMedial";

	rightMiddleDistal = "rightMiddleDistal";

	rightMiddleTip = "rightMiddleTip";

	rightRingProximal = "rightRingProximal";

	rightRingMedial = "rightRingMedial";

	rightRingDistal = "rightRingDistal";

	rightRingTip = "rightRingTip";

	rightLittleProximal = "rightLittleProximal";

	rightLittleMedial = "rightLittleMedial";

	rightLittleDistal = "rightLittleDistal";

	rightLittleTip = "rightLittleTip";


}


void URokokoBodyMapData::InitializeTMap()
{
	NameMapping.Empty();

	NameMapping.Add("hip", hip);

	NameMapping.Add("stomach", stomach);

	NameMapping.Add("chest", chest);

	NameMapping.Add("neck", neck);

	NameMapping.Add("head", head);

	NameMapping.Add("leftShoulder", leftShoulder);

	NameMapping.Add("leftArm", leftArm);

	NameMapping.Add("leftForearm", leftForearm);

	NameMapping.Add("leftHand", leftHand);

	NameMapping.Add("rightShoulder", rightShoulder);

	NameMapping.Add("rightArm", rightArm);

	NameMapping.Add("rightForearm", rightForearm);

	NameMapping.Add("rightHand", rightHand);

	NameMapping.Add("leftUpleg", leftUpleg);

	NameMapping.Add("leftLeg", leftLeg);

	NameMapping.Add("leftFoot", leftFoot);

	NameMapping.Add("leftToe", leftToe);

	NameMapping.Add("rightUpleg", rightUpleg);

	NameMapping.Add("rightLeg", rightLeg);

	NameMapping.Add("rightFoot", rightFoot);

	NameMapping.Add("rightToe", rightToe);

	NameMapping.Add("leftThumbProximal", leftThumbProximal);

	NameMapping.Add("leftThumbMedial", leftThumbMedial);

	NameMapping.Add("leftThumbDistal", leftThumbDistal);

	NameMapping.Add("leftThumbTip", leftThumbTip);

	NameMapping.Add("leftIndexProximal", leftIndexProximal);

	NameMapping.Add("leftIndexMedial", leftIndexMedial);

	NameMapping.Add("leftIndexDistal", leftIndexDistal);

	NameMapping.Add("leftIndexTip", leftIndexTip);

	NameMapping.Add("leftMiddleProximal", leftMiddleProximal);

	NameMapping.Add("leftMiddleMedial", leftMiddleMedial);

	NameMapping.Add("leftMiddleDistal", leftMiddleDistal);

	NameMapping.Add("leftMiddleTip", leftMiddleTip);

	NameMapping.Add("leftRingProximal", leftRingProximal);

	NameMapping.Add("leftRingMedial", leftRingMedial);

	NameMapping.Add("leftRingDistal", leftRingDistal);

	NameMapping.Add("leftRingTip", leftRingTip);

	NameMapping.Add("leftLittleProximal", leftLittleProximal);

	NameMapping.Add("leftLittleMedial", leftLittleMedial);

	NameMapping.Add("leftLittleDistal", leftLittleDistal);

	NameMapping.Add("leftLittleTip", leftLittleTip);

	NameMapping.Add("rightThumbProximal", rightThumbProximal);

	NameMapping.Add("rightThumbMedial", rightThumbMedial);

	NameMapping.Add("rightThumbDistal", rightThumbDistal);

	NameMapping.Add("rightThumbTip", rightThumbTip);

	NameMapping.Add("rightIndexProximal", rightIndexProximal);

	NameMapping.Add("rightIndexMedial", rightIndexMedial);

	NameMapping.Add("rightIndexDistal", rightIndexDistal);

	NameMapping.Add("rightIndexTip", rightIndexTip);

	NameMapping.Add("rightMiddleProximal", rightMiddleProximal);

	NameMapping.Add("rightMiddleMedial", rightMiddleMedial);

	NameMapping.Add("rightMiddleDistal", rightMiddleDistal);

	NameMapping.Add("rightMiddleTip", rightMiddleTip);

	NameMapping.Add("rightRingProximal", rightRingProximal);

	NameMapping.Add("rightRingMedial", rightRingMedial);

	NameMapping.Add("rightRingDistal", rightRingDistal);

	NameMapping.Add("rightRingTip", rightRingTip);

	NameMapping.Add("rightLittleProximal", rightLittleProximal);

	NameMapping.Add("rightLittleMedial", rightLittleMedial);

	NameMapping.Add("rightLittleDistal", rightLittleDistal);

	NameMapping.Add("rightLittleTip", rightLittleTip);
}

//FName URokokoBodyMapData::GetRemappedBoneName_Implementation(FName CurveName) const
//{
//	if (auto RemappedName = NameMapping.Find(CurveName))
//	{
//		return *RemappedName;
//	}
//	return "";
//}
//
//void URokokoBodyMapData::Initialize()
//{
//	InitializeTMap();
//}
//PRAGMA_DISABLE_OPTIMIZATION
FName URokokoBodyMapData::GetRemappedBoneName_Implementation(FName CurveName) const
{
	if (auto RemappedName = NameMapping.Find(CurveName))
	{
		return *RemappedName;
	}

	return "";
}
//PRAGMA_ENABLE_OPTIMIZATION


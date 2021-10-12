// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once
/// @cond doc_hidden
/**Holds information a Smartsuit character pose.*/
struct TPose {
	FTransform hip;
	FTransform stomach;
	FTransform chest;
	FTransform neck;
	FTransform head;
	FTransform leftShoulder;
	FTransform leftArm;
	FTransform leftForearm;
	FTransform leftHand;
	FTransform rightShoulder;
	FTransform rightArm;
	FTransform rightForearm;
	FTransform rightHand;
	FTransform leftUpLeg;
	FTransform leftLeg;
	FTransform leftFoot;
	FTransform leftToe;
	FTransform rightUpLeg;
	FTransform rightLeg;
	FTransform rightFoot;
	FTransform rightToe;
	FTransform leftThumbProximal;
	FTransform leftThumbMedial;
	FTransform leftThumbDistal;
	FTransform leftThumbTip;
	FTransform leftIndexProximal;
	FTransform leftIndexMedial;
	FTransform leftIndexDistal;
	FTransform leftIndexTip;
	FTransform leftMiddleProximal;
	FTransform leftMiddleMedial;
	FTransform leftMiddleDistal;
	FTransform leftMiddleTip;
	FTransform leftRingProximal;
	FTransform leftRingMedial;
	FTransform leftRingDistal;
	FTransform leftRingTip;
	FTransform leftLittleProximal;
	FTransform leftLittleMedial;
	FTransform leftLittleDistal;
	FTransform leftLittleTip;
	FTransform rightThumbProximal;
	FTransform rightThumbMedial;
	FTransform rightThumbDistal;
	FTransform rightThumbTip;
	FTransform rightIndexProximal;
	FTransform rightIndexMedial;
	FTransform rightIndexDistal;
	FTransform rightIndexTip;
	FTransform rightMiddleProximal;
	FTransform rightMiddleMedial;
	FTransform rightMiddleDistal;
	FTransform rightMiddleTip;
	FTransform rightRingProximal;
	FTransform rightRingMedial;
	FTransform rightRingDistal;
	FTransform rightRingTip;
	FTransform rightLittleProximal;
	FTransform rightLittleMedial;
	FTransform rightLittleDistal;
	FTransform rightLittleTip;
};

/**
 * Helper function that stores the T Pose for a Smartsuit character.
 */
class SMARTSUIT_API SmartsuitTPose
{
public:
	SmartsuitTPose();
	~SmartsuitTPose();
	/**The T-pose transforms.*/
	TPose Pose;
	/**The start position of the character, used to offset the character if it is set.*/
	FVector startPos;
	/**Indicates if the pose has been stored.*/
	bool StoredTPose;
};

/// @endcond
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
	FTransform rightUpLeg;
	FTransform rightLeg;
	FTransform rightFoot;
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
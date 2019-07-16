// Copyright 2019 Rokoko Electronics. All Rights Reserved.

#pragma once

#include "../Lib/V8/Includes/SmartsuitDef.h"
/*! \file SmartsuitDefinitions.h
\brief This file includes all definitions used by Smartsuit plugin.
*
* The main structs, command definitions, sensor mapping and rotation mappings are defined in this class.
*/


/*! \brief Expected rotation of the hip in T-Pose. */
#define SMARTSUIT_TPOSE_HIP					FQuat::MakeFromEuler(FVector(89.999954, 0.000055, -0.000005))
/*! \brief Expected rotation of the stomach in T-Pose. */
#define SMARTSUIT_TPOSE_STOMACH				FQuat::MakeFromEuler(FVector(-89.999954, -0.000171, 180.000000)) 
/*! \brief Expected rotation of the chest in T-Pose. */
#define SMARTSUIT_TPOSE_CHEST				FQuat::MakeFromEuler(FVector(-89.999985, -0.000171, 180.000000)) 
/*! \brief Expected rotation of the neck in T-Pose. */
#define SMARTSUIT_TPOSE_NECK				FQuat::MakeFromEuler(FVector(-90.000053, -0.000191, 180.000000))
/*! \brief Expected rotation of the head in T-Pose. */
#define SMARTSUIT_TPOSE_HEAD				FQuat::MakeFromEuler(FVector(-89.999893,-0.000137,180.000000))
/*! \brief Expected rotation of the left shoulder in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_SHOULDER		FQuat::MakeFromEuler(FVector(-75.371887, 90.000000,-170.371826))
/*! \brief Expected rotation of the left arm in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_ARM			FQuat::MakeFromEuler(FVector(179.999969, 0.000014,89.999954))
/*! \brief Expected rotation of the left forearm in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_FOREARM		FQuat::MakeFromEuler(FVector(179.999969, 0.000014,89.999908))
/*! \brief Expected rotation of the left hand in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_HAND			FQuat::MakeFromEuler(FVector(179.999969, 0.000055,90.000099))
/*! \brief Expected rotation of the right shoulder in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_SHOULDER		FQuat::MakeFromEuler(FVector(87.917419, -90.000000,2.082565))
/*! \brief Expected rotation of the right arm in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_ARM			FQuat::MakeFromEuler(FVector(179.999817, -0.000075,-90.000023))
/*! \brief Expected rotation of the right forearm in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_FOREARM		FQuat::MakeFromEuler(FVector(179.999847,-0.000075,-90.000053))
/*! \brief Expected rotation of the right hand in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_HAND			FQuat::MakeFromEuler(FVector(-179.999969,-0.000075,-90.000046))
/*! \brief Expected rotation of the left up leg in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_UP_LEG			FQuat::MakeFromEuler(FVector(90.000145,0.000041,89.999908))
/*! \brief Expected rotation of the left leg in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_LEG			FQuat::MakeFromEuler(FVector(90.000107,0.000041,89.999878))
/*! \brief Expected rotation of the left foot in T-Pose. */
#define SMARTSUIT_TPOSE_LEFT_FOOT			FQuat::MakeFromEuler(FVector(179.999863,-0.000109,179.999802))
/*! \brief Expected rotation of the right up leg in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_UP_LEG		FQuat::MakeFromEuler(FVector(89.999855,-0.000041,-89.999947))
/*! \brief Expected rotation of the right leg in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_LEG			FQuat::MakeFromEuler(FVector(89.999825,-0.000041,-89.999962))
/*! \brief Expected rotation of the right foot in T-Pose. */
#define SMARTSUIT_TPOSE_RIGHT_FOOT			FQuat::MakeFromEuler(FVector(-179.999893,-0.000171,-179.999939))

/*! \brief Information about specific sensor, including its status, rotation, position.*/
struct Sensor {

	/**
	* The address of the Sensor in the Smartsuit. Represents its position.
	* A list of all supported addresses can be found in ASmartsuitCommands
	*/
	uint16_t addr;

	/**Indicates weither there is another sensor connected after this, or this is the last sensor in a branch.*/
	char isAnotherSensorConnected;

	char behavior;
	/**Indicates if the sensor detects metal or not.*/
	char command;
	/**Holds information about the acceleration detected in the sensor during the last frame.*/
	FVector acceleration;

	/**
	* The rotation information for this sensor. This quaternion is represented in Smartsuits coordinate system.
	* To get a quaternion in Unreal coordinate system use UQuaternion() instead.
	*/
	FQuat quaternion;

	/** Gyroscope information for this sensor.*/
	FVector gyro;

	/**
	* Position information for this sensor. This position is represented in Smartsuits coordinate system.
	* To get the position in Unreal coordinate system use UPosition() instead.
	*/
	FVector position;

	/** Timestamp information received from the sensor. This is the sensors internal clock.*/
	uint32_t microseconds;

	/// @private
	FQuat NED2Unreal(FQuat rotation) {
		/*FVector forward = rotation.GetForwardVector();
		forward.Z = -forward.Z;
		FQuat result = */
		FQuat result(rotation.X, rotation.Y, rotation.Z, rotation.W);
		//result.X = -result.X;
		result.Z = -result.Z;
		result.Y = -result.Y;
		FQuat modifier = FQuat::MakeFromEuler(FVector(180, 0, 90));
		FQuat postModifier = FQuat::MakeFromEuler(FVector(0, 0, 180));
		//result.Z = -result.Z;
		FQuat finalResult = modifier * result * postModifier;
		
		return FQuat(finalResult.X, finalResult.Y, finalResult.Z, finalResult.W);
	}



	/**
	* Get sensor rotation in Unreal coordinate system.
	*
	* @return Sensor rotation.
	*/
	FQuat Uquaternion() {
		
		if (FGenericPlatformMath::IsNaN(quaternion.X) || FGenericPlatformMath::IsNaN(quaternion.Y) || FGenericPlatformMath::IsNaN(quaternion.Z) || FGenericPlatformMath::IsNaN(quaternion.W)) {
			return FQuat::Identity;
		}
		return NED2Unreal(quaternion);

	}

	/**
	* Get sensor position in Unreal coordinate system.
	*
	* @return Sensor position.
	*/
	FVector UPosition() {
		if (FGenericPlatformMath::IsNaN(position.X) || FGenericPlatformMath::IsNaN(position.Y) || FGenericPlatformMath::IsNaN(position.Z)) {
			return FVector::ZeroVector;
		}
		return FVector(100.0f*position.Y, -100.0f*position.X, -100.0f*position.Z);
	}
};


/*! \brief Contains data that represent the last frame received from the Smartsuit.
*
* This struct represents a Smartsuit data frame as received from the Smartsuit.
* It also includes meta variables used to manage the state of the Smartsuit in Unreal.
*/
struct SuitData {

	/** The name of the Smartsuit. */
	char suitname[5];

	/** Information about the sensors connected to this Smartsuit, like position, rotation, etc.*/
	Sensor sensors[19];

	/** The time to live indicator for this Smartsuit. This indica*/
	float ttl;

	/** The ip address of the Smartsuit.*/
	uint32 url;

	/** Indicator if the Smartsuit is broadcasting.*/
	bool isBroadcasting;

	/** Indicator if the Smartsuit has profile.*/
	bool hasProfile;

	/// @private.
	bool profileToggle;

	/** The number of frames received from this Smartsuit during the last second.*/
	float fps;

	/// @private
	int currFPSCount;

	/// @private
	int lastFPSSecond;

	/**
	* Shortcut function to get the Hip sensor information for this Smartsuit.
	*
	* @return The Sensor information that corresponds to hip, if no sensor is found, it returns nullptr.
	*/
	Sensor* Hip() {
		return GetSensor(SENSOR_HIP);
	}

	Sensor* GetSensor(uint16_t address) {
		for (int i = 0; i < 19; i++) {
			if (sensors[i].addr == address) {
				return &(sensors[i]);
			}
		}
		return nullptr;
	}

	FName GetSubjectName() {
		return FName(*FString(ANSI_TO_TCHAR(suitname)));
	}
};




/// @cond doc_hidden
/**
*
*/
class SMARTSUIT_API SmartsuitDefinitions
{
public:
	SmartsuitDefinitions();
	~SmartsuitDefinitions();
};
/// @endcond
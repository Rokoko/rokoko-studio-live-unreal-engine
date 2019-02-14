#pragma once
#include<cstdint>

#define SMARTSUIT_COMMAND_GET_BODY_DIMENSIONS		0x59
#define SMARTSUIT_COMMAND_READ_HUB_INFO				0x63
#define SMARTSUIT_COMMAND_RESET_KALMAN_FILTER		0x68
#define SMARTSUIT_COMMAND_PERFORM_APOSE				0x60
#define SMARTSUIT_COMMAND_BROADCAST_ADDR			0xAE
#define SMARTSUIT_COMMAND_USE_SPECIFIC_ADDR			0x56
#define SMARTSUIT_COMMAND_USE_BROADCAST_ADDR		0x57
#define SMARTSUIT_COMMAND_SET_BODY_DIMENSIONS		0x58


/*! \brief Address of the hip sensor as received from the Smartsuit. */
#define SENSOR_HIP				0xA0
/*! \brief Address of the stomach position as received from the Smartsuit. */
#define SENSOR_STOMACH			0xA1
/*! \brief Address of the chest position as received from the Smartsuit. */
#define SENSOR_CHEST			0xA2
/*! \brief Address of the neck position as received from the Smartsuit. */
#define SENSOR_NECK				0xA3
/*! \brief Address of the head sensor as received from the Smartsuit. */
#define SENSOR_HEAD				0x40

/*! \brief Address of the left hip sensor as received from the Smartsuit. */
#define SENSOR_LEFT_HIP			0x00
/*! \brief Address of the left upper leg sensor as received from the Smartsuit. */
#define SENSOR_LEFT_UPPER_LEG	0x01
/*! \brief Address of the left lower leg sensor as received from the Smartsuit. */
#define SENSOR_LEFT_LOWER_LEG	0x02
/*! \brief Address of the left foot sensor as received from the Smartsuit. */
#define SENSOR_LEFT_FOOT		0x03

/*! \brief Address of the left shoulder sensor as received from the Smartsuit. */
#define SENSOR_LEFT_SHOULDER	0x21
/*! \brief Address of the left upper arm sensor as received from the Smartsuit. */
#define SENSOR_LEFT_UPPER_ARM	0x22
/*! \brief Address of the lower arm sensor as received from the Smartsuit. */
#define SENSOR_LEFT_LOWER_ARM	0x23
/*! \brief Address of the left hand sensor as received from the Smartsuit. */
#define SENSOR_LEFT_HAND		0x24

/*! \brief Address of the right shoulder sensor as received from the Smartsuit. */
#define SENSOR_RIGHT_SHOULDER	0x61
/*! \brief Address of the right upper arm sensor as received from the Smartsuit. */
#define SENSOR_RIGHT_UPPER_ARM	0x62
/*! \brief Address of the right lower arm sensor as received from the Smartsuit. */
#define SENSOR_RIGHT_LOWER_ARM	0x63
/*! \brief Address of the right hand sensor as received from the Smartsuit. */
#define SENSOR_RIGHT_HAND		0x64

/*! \brief Address of the right upper leg sensor as received from the Smartsuit. */
#define SENSOR_RIGHT_UPPER_LEG	0x81
/*! \brief Address of the right lower leg sensor as received from the Smartsuit. */
#define SENSOR_RIGHT_LOWER_LEG	0x82
/*! \brief Address of the right foot sensor as received from the Smartsuit. */
#define SENSOR_RIGHT_FOOT		0x83


/*! \brief Detailed body dimensions.
*
* This struct describes the dimensions of the body in more detail by defining the individual bone lengths, as opposed to FBodyModel which contains more abstract lengths.
*/
struct Body {
	/// @private
	char first_buffer;
	/// @private
	char first_buffer1;
	/// @private
	char first_buffer2;
	/// @private
	char first_buffer3;

	float _head;
	float _neck;
	float _middle_back;
	float _shoulder_blade;
	float _upper_arm;
	float _forearm;
	float _hand;
	float _low_back;
	float _hip;
	float _hip_width;
	float _thigh;
	float _leg;
	float _foot_length;
	float _foot_height;
	float _foot_width;
	float _foot_heel_offset;
	char name[32];
};


/*! \brief Information about specific sensor, including its status, rotation, position.*/
struct SensorDef {

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
	float acceleration[3];

	/**
	* The rotation information for this sensor. This quaternion is represented in Smartsuits coordinate system.
	* To get a quaternion in Unreal coordinate system use UQuaternion() instead.
	*/
	float quaternion[4];

	/** Gyroscope information for this sensor.*/
	float gyro[3];

	/**
	* Position information for this sensor. This position is represented in Smartsuits coordinate system.
	* To get the position in Unreal coordinate system use UPosition() instead.
	*/
	float position[3];

	/** Timestamp information received from the sensor. This is the sensors internal clock.*/
	uint32_t microseconds;
};

struct HubInfo {

	uint32_t uniqueId[4];
	uint32_t firmware;
	uint32_t bootloader;
	uint32_t usbApiVersion;
	uint32_t wifiApiVersion;
	uint32_t btApiVersion;
	unsigned char serialNumber[12];
	float latitude;
	float longitude;
	float earthMagFieldX;
	float earthMagFieldY;
	float gravity;
};


/*! \brief Contains data that represent the last frame received from the Smartsuit.
*
* This struct represents a Smartsuit data frame as received from the Smartsuit.
* It also includes meta variables used to manage the state of the Smartsuit in Unreal.
*/
struct SuitDataDef {

	/** The name of the Smartsuit. */
	char suitname[5];

	/** Information about the sensors connected to this Smartsuit, like position, rotation, etc.*/
	SensorDef sensors[19];
	int sensorCount;

	/** The time to live indicator for this Smartsuit. This indica*/
	float ttl;

	
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
};

class SmartsuitDef
{
public:
	SmartsuitDef();
	~SmartsuitDef();
	int SayOne();
	
};
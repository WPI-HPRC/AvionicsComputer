/*
 * PayloadRobot.h
 * Created on: Dec 26, 2020
 * Author: Peter Dentch
 */

#ifndef SRC_PAYLOADROBOT_H_
#define SRC_PAYLOADROBOT_H_

#include "Arduino.h"
//#include "Wire.h"

//#include "../Constants.h"
#include "loops/Looper.h"
#include "loops/loop.h"

#include "subsystems/DriveTrain.h"
//#include "peripheral/GyroAccel.h"
//#include "peripheral/LoRaCustom.h"


/*
 * Robot has TODO primary states of autonomous operation throughout its mission which begins when the system is powered on
 *
 * STATE DETAILS
 */
enum RobotState {
//	TESTING, OnPad, Ascent, Coast, Descent,
//	Landed, SelfRighting, Stabilizing, SendPhoto
	TESTING,
	Waiting,
	Driving
};

/*
 * The Payload Robot System object, instantiates all robot subsystems and registers their loops
 *
 * DETAILS
 *
 */
class PayloadRobot {

private:

	Looper * runningLooper;					// looper instance so robot can register its subsystem loops on startup

	RobotState robotState;					// initial state is Landed


//	DriveTrain driveTrain = new DriveTrain();
	GyroAccel * imu = new GyroAccel();		// sensor
	uint8_t imuData[12];
	LoRaComms * myRadio = new LoRaComms();



	//	PIDMotor leftMotor;			 	// motor driving left side robot tread
	//	PIDMotor rightMotor;			// motor driving right side robot tread


	//	PIDMotor selfRightingMotor1; 	// motor driving petal (3D print color?)
	//	PIDMotor selfRightingMotor2; 	// motor driving petal (3D print color?)
	//	PIDMotor selfRightingMotor3; 	// motor driving petal (3D print color?)



	// timeout variable(s) for loop transition


public:

	PayloadRobot(Looper * looper);
	//	PayloadRobot(){}


	void systemInit();

	void zeroAllSensors();

	void beginStateMachine();
	void updateStateMachine();
	void endStateMachine();

};




#endif /* SRC_PAYLOADROBOT_H_ */

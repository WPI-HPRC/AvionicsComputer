/*
 * PayloadRobot.h
 * Created on: Dec 26, 2020
 * Author: Peter Dentch
 */

#ifndef SRC_PAYLOADROBOT_H_
#define SRC_PAYLOADROBOT_H_

#include "Arduino.h"

//#include "../Constants.h"
#include "loops/Looper.h"
#include "loops/loop.h"

#include "subsystems/DriveTrain.h"
#include "subsystems/SelfRighting.h"

//#include "peripheral/GyroAccel.h"
//#include "peripheral/LoRaCustom.h"


/*
 * Robot has TODO primary states of autonomous operation throughout its mission which begins when the system is powered on
 *
 * STATE DETAILS
 */
enum RobotState {
//	TESTING, OnPad, Launch, Coast, Descent,
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


	MPU6050 * robotIMU = new MPU6050();

//	LoRaComms * robotRadio = new LoRaComms();


//	DriveTrain * driveTrain = new DriveTrain(robotIMU);

	SelfRighting * selfRighting = new SelfRighting(robotIMU);


	//	PIDMotor * selfRightingMotor1; 		// motor driving petal (3D print color?)
	//	PIDMotor * selfRightingMotor2; 		// motor driving petal (3D print color?)
	//	PIDMotor * selfRightingMotor3; 		// motor driving petal (3D print color?)




	// timeout variable(s) for loop transition ?


public:

	PayloadRobot(Looper * looper);
	//	PayloadRobot(){}

	/* Robot loop functionality */
	class RobotLoop : public Loop {
		PayloadRobot * robot_;

	public:
		RobotLoop(PayloadRobot * instance){
			robot_ = instance;
		};

		void onStart(uint32_t timestamp){
			robot_->beginStateMachine();
		}
		void onLoop(uint32_t timestamp){
			robot_->updateStateMachine();

		}
		void onStop(uint32_t timestamp){
			robot_->endStateMachine();
		}
	} * robotLoop = new RobotLoop(this);		// instantiate the main system loop and pass it the system instance



	void systemInit();

	void zeroAllSensors();

	void beginStateMachine();
	void updateStateMachine();
	void endStateMachine();

};




#endif /* SRC_PAYLOADROBOT_H_ */

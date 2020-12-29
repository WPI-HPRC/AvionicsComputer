/*
 * PayloadRobot.cpp
 * Created on: Dec 26, 2020
 * Author: Peter Dentch
 */

#include "PayloadRobot.h"

/*
* Constructor for the Payload Robot System object, should only be one instance, one robot per processor
* @param looper is the running looper of the system manager
*/
PayloadRobot::PayloadRobot(Looper * looper){
	runningLooper = looper;
	robotState = Waiting;
};


/*
 * Init function for the system, should be run after instantiation
 */
void PayloadRobot::systemInit(){

//TODO enable all sensors and register all subsystem loops

	runningLooper->registerLoop(robotLoop);		// robot system loop is registered first in order to be run first

	imu->gyroStart();
	myRadio->init();

//	drivetrain->registerEnabledLoops(runningLooper);
//	drivetrain->subsystemInit();

//	etc.


}


void PayloadRobot::zeroAllSensors(){

	//robotStateEstimator->reset(millis());
	//rocketStateEstimator->reset(millis());

	//driveTrain->zeroSensors();
}


/*
 * Configuring robot subsystems for start of mission states sequence
 */
void PayloadRobot::beginStateMachine(){

	Serial.println(F("START ROBOT LOOP"));

	//zeroAllSensors();
}


void PayloadRobot::updateStateMachine(){




	imu->pull(imuData, 12);
	int16_t accelX = imuData[0] <<8| imuData[1];
//	runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
//	Serial.println(accelX);

	if(myRadio->getMode() == RadioState::RHModeIdle){
				myRadio->send(imuData, 12);

				runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
				Serial.println(accelX);
	}






}


void PayloadRobot::endStateMachine(){

}


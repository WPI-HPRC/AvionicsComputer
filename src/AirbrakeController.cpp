/*
 * AirbrakeController.cpp
 *
 *  Created on: Feb 27, 2021
 *      Author: Jackson Neu
 */

#include "AirbrakeController.h"



/*
* Constructor for the payload robot system object, should only be one instance, one robot per processor
* @param looper is the running looper of the system manager
*/
AirbrakeController::AirbrakeController(Looper * looper){
	runningLooper = looper;
	robotState = Waiting;
};


/*
 * Init function for the system, should be run after instantiation
 */
void AirbrakeController::systemInit(){

	runningLooper->registerLoop(robotLoop);		// robot system loop is registered first in order to be run first

	driveTrain->registerEnabledLoops(runningLooper);


//	robotRadio->init();

	driveTrain->subsystemInit();

//	etc.


}


void AirbrakeController::zeroAllSensors(){

	//robotStateEstimator->reset(millis());
	//rocketStateEstimator->reset(millis());

	driveTrain->zeroSensors();

}


/*
 * Configuring robot subsystems for start of mission states sequence
 */
void AirbrakeController::beginStateMachine(){

	Serial.println(F("START ROBOT LOOP"));
	//zeroAllSensors();

}


void AirbrakeController::updateStateMachine(){


	runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
	Serial.println(driveTrain->getHeading());






}


void AirbrakeController::endStateMachine(){

}



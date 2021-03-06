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
	flightState = STANDBY;
};


/*
 * Init function for the system, should be run after instantiation
 */
void AirbrakeController::systemInit(){

	runningLooper->registerLoop(airbrakeLoop);		// robot system loop is registered first in order to be run first

//	driveTrain->registerEnabledLoops(runningLooper);


//	robotRadio->init();

//	driveTrain->subsystemInit();

//	etc.


}


void AirbrakeController::zeroAllSensors(){

	//robotStateEstimator->reset(millis());
	//rocketStateEstimator->reset(millis());

//	driveTrain->zeroSensors();

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
//	Serial.println(driveTrain->getHeading());

	switch(flightState){
		case STANDBY:

			float avgAccel;// average from last 0.1sec
			float G_Threshold = 2.0;
			if(avgAccel > 9.8 * G_Threshold){
				flightState = POWERED_FLIGHT;
			}

			break;
		case POWERED_FLIGHT:

			float avgAccel;// average from last 0.1sec
			float G_Threshold = 2.0;
			if(avgAccel < 9.8 * G_Threshold){
				flightState = UNPOWERED_FLIGHT;
			}

			break;
		case UNPOWERED_FLIGHT:

			//alt = getAltData(seconds)
			float index;
			//float index = index(alt, maxAlt)/alt.length
			if(index < 0.25)
			{
				flightState = DESCENT;
			}

			break;
		case DESCENT:
			float alt[];// = getAltData(seconds)
			float landedDiffThreshold = 1;
			//if(max(alt) - min(alt) < landedDiffThreshold){
			//	flightState = LANDED;
			//}
			break;
		case LANDED:
			break;
		case ABORT:
			break;


	}





}


void AirbrakeController::endStateMachine(){

}



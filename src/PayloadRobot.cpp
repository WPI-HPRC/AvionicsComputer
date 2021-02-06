/*
 * PayloadRobot.cpp
 * Created on: Dec 26, 2020
 * Author: Peter Dentch
 */

#include "PayloadRobot.h"

/*
* Constructor for the payload robot system object, should only be one instance, one robot per processor
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

	runningLooper->registerLoop(robotLoop);		// robot system loop is registered first in order to be run first

	driveTrain->registerEnabledLoops(runningLooper);


//	robotRadio->init();

	robotGPS->enable();

	driveTrain->subsystemInit();

//	etc.


}


void PayloadRobot::zeroAllSensors(){

	//robotStateEstimator->reset(millis());
	//rocketStateEstimator->reset(millis());

	driveTrain->zeroSensors();

}


/*
 * Configuring robot subsystems for start of mission states sequence
 */
void PayloadRobot::beginStateMachine(){

	Serial.println(F("START ROBOT LOOP"));
	//zeroAllSensors();

}


void PayloadRobot::updateStateMachine(){


//	runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
//	Serial.println("Hello");

	long latitude = robotGPS->getLatitude();
	    Serial.print(F(" Lat: "));
	    Serial.print(latitude);
	    Serial.println(F(" (degrees * 10^-7)"));

	  long longitude = robotGPS->getLongitude();
	    Serial.print(F(" Long: "));
	    Serial.print(longitude);
	    Serial.println(F(" (degrees * 10^-7)"));

	  long altitude = robotGPS->getAltitude();
	    Serial.print(F(" Alt: "));
	    Serial.print(altitude);
	    Serial.println(F(" (mm)"));

	  delay(500);


}


void PayloadRobot::endStateMachine(){

}


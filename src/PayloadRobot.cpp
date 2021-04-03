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
	if(!baro->initBaro()){
		Serial.println("baro not found");
	}
	baro->setZeroAltitude();
	enabledLight->enable();


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

	enabledLight->set(HIGH);

}


void PayloadRobot::updateStateMachine(){


	runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
	//Serial.println(driveTrain->getHeading());

	baro->update();
	Serial.print("pressure: ");
	Serial.println(baro->getPressure() );
	Serial.print("Altitude: ");
	Serial.println(baro->getAltitude() );
	//Serial.print("launched?: ");
	//Serial.println(baro->hasLaunched );
	//Serial.print("apogee?: ");
	//Serial.println(baro->reachedApogee );
	//runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
	//Serial.println(driveTrain->getHeading());
	//Serial.println("hello");


	enabledLight->update();



}


void PayloadRobot::endStateMachine(){

}


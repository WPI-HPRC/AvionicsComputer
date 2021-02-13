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

	if (myGPS.begin(Wire, 0x42) == false)
	  {
	    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
	    while (1){
	    }
	  }

}


void PayloadRobot::updateStateMachine(){

//	runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
//	Serial.println(driveTrain->getHeading());

	myGPS.checkUblox(); //See if new data is available. Process bytes as they come in.

	  Serial.println("");

	  long latitude = myGPS.getLatitude();
	    Serial.print(F(" Lat: "));
	    Serial.print(latitude);
	    Serial.println(F(" (degrees * 10^-7)"));

	  long longitude = myGPS.getLongitude();
	    Serial.print(F(" Long: "));
	    Serial.print(longitude);
	    Serial.println(F(" (degrees * 10^-7)"));

	  long altitude = myGPS.getAltitude();
	    Serial.print(F(" Alt: "));
	    Serial.print(altitude);
	    Serial.println(F(" (mm)"));

	  byte Hour = myGPS.getHour();
	    Serial.print(F(" "));
	    if (Hour < 10)
	    {
	      Serial.print(F("0"));
	    }
	    Serial.print(Hour);
	    Serial.print(F(":"));

	  byte Minute = myGPS.getMinute();
	    if (Minute < 10)
	    {
	      Serial.print(F("0"));
	    }
	    Serial.print(Minute);
	    Serial.print(F(":"));

	  byte Second = myGPS.getSecond();
	    if (Second < 10)
	    {
	      Serial.print(F("0"));
	    }
	    Serial.print(Second);

	    Serial.println("");

}


void PayloadRobot::endStateMachine(){

}


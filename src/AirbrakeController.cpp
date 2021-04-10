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
	flightState = ON_PAD;
	for(int i = 0 ; i < ACCEL_BUFFER_SIZE;i++){
		accelBuffer[i] = 0;
	}
};


/*
 * Init function for the system, should be run after instantiation
 */
void AirbrakeController::systemInit(){

	runningLooper->registerLoop(airbrakeLoop);		// robot system loop is registered first in order to be run first

//	driveTrain->registerEnabledLoops(runningLooper);


//	robotRadio->init();
	barometer->enable();
	barometer->setZeroAltitude();
	accelerometer->enable();


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

/*
 * Function for transitioning between states and executing routines run in each state. Part of main loop
 *
 */

void AirbrakeController::updateStateMachine(){

	Serial.println("UpdateStateMachine");
	Serial.println(flightState);
	//runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
//	Serial.println(driveTrain->getHeading());

	switch(flightState){//conditions for switching state
		case ON_PAD:
		{
			float avgAccel = avgAccelArray();// average from last 0.1sec
			float G_Threshold = .90;

			if(avgAccel > G_Threshold){
				flightState = POWERED_FLIGHT;
			}
		}
			break;
		case POWERED_FLIGHT:
		{

			float G_Threshold = 0.7;
			if(avgAccelArray() <  G_Threshold){
				flightState = UNPOWERED_FLIGHT;
			}
		}
			break;
		case UNPOWERED_FLIGHT:
		{

			//alt = getAltData(seconds)
			float index;
			//float index = index(alt, maxAlt)/alt.length
			if(index < 0.25)
			{
				//flightState = DESCENT;
			}
		}
			break;
		case DESCENT:
		{
			float alt[BARO_BUFFER_SIZE];// = getAltData(seconds)
			float landedDiffThreshold = 1;
			//if(max(alt) - min(alt) < landedDiffThreshold){
			//	flightState = LANDED;
			//}
		}
			break;
		case LANDED:
			break;
		case ABORT:
			break;
	}



	switch(flightState){//To run while in state
		case ON_PAD:
		{
			takeBaroReading();
			float testAvg = avgBaroArray();
			Serial.print("AvgAlt: ");
			Serial.println(testAvg);

			takeAccelReading();
			float testAvgAcc = avgAccelArray();
			Serial.print("Accel: ");Serial.println(accelerometer->getAccZg());
			Serial.print("AvgAccel: ");
			Serial.println(testAvgAcc);

		}
				break;
		case POWERED_FLIGHT:
			takeAccelReading();
			//takeBaroReading()
			//log data
				break;
		case UNPOWERED_FLIGHT:
			//log data
			//airbrake control
				break;
		case DESCENT:
			//log data
			//close airbrake
				break;
		case LANDED:
				break;
		case ABORT:
				break;
	}




}


void AirbrakeController::endStateMachine(){

}


/*
 * Average the altitude values in the Barometer Buffer array and return the result
 * @return float containing the average of the buffer data readings
 */
float AirbrakeController::avgBaroArray(){
	float total = 0;
	for(int i = 0 ; i < BARO_BUFFER_SIZE;i++){
		total += baroBuffer[i];
	}
	return total/BARO_BUFFER_SIZE;
}

/*
 * Average the acceleration values in the Accelerometer Buffer array and return the result
 * @return float containing the average of the buffer data readings
 */
float AirbrakeController::avgAccelArray(){
	float total = 0;
	for(int i = 0 ; i < ACCEL_BUFFER_SIZE;i++){
		total += accelBuffer[i];
	}
	return total/ACCEL_BUFFER_SIZE;
}

/*
 * Updates the barometer and gets an altitude reading, adds reading to the buffer array
 * Increments buffer index
 */
void AirbrakeController::takeBaroReading(){
	barometer->update();
	float reading = barometer->getPressure();
	baroBuffer[baroBufIndex] = reading;
	baroBufIndex = (baroBufIndex + 1)%BARO_BUFFER_SIZE;
}

/*
 * Updates the accelerometer and gets an accelerationZ reading, adds reading to the buffer array
 * Increments buffer index
 */
void AirbrakeController::takeAccelReading(){
	accelerometer->update();
	float reading = accelerometer->getAccZg();
	accelBuffer[accelBufIndex] = reading;
	accelBufIndex = (accelBufIndex + 1)%ACCEL_BUFFER_SIZE;
}


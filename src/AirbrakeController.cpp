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

	//Serial.println("UpdateStateMachine");
	debugData();
	//runningLooper->printOutput();//TODO Only for debug, func should be private, lazy
//	Serial.println(driveTrain->getHeading());

	switch(flightState){//conditions for switching state
		case ON_PAD:
		{
			float avgAccel = avgAccelArray();// average from last 0.1sec
			float G_Threshold = .75;

			if(avgAccel > G_Threshold){//might be better to just use raw accel here?
				flightState = POWERED_FLIGHT;
			}
		}
			break;
		case POWERED_FLIGHT:
		{

			float G_Threshold = 0.73;
			if(avgAccelArray() <  G_Threshold){
				flightState = UNPOWERED_FLIGHT;
			}
		}
			break;
		case UNPOWERED_FLIGHT:
		{

			if(avgAltitude < maxAvgAltitude - 1)
			{
				flightState = DESCENT;
			}
		}
			break;
		case DESCENT:
		{
			//find min and max altitudes in the buffer
			float maxAltInBuffer = baroBuffer[0];
			float minAltInBuffer = baroBuffer[0];
			for(int i = 0;i< BARO_BUFFER_SIZE;i++){
				if(baroBuffer[i] < minAltInBuffer){
					minAltInBuffer = baroBuffer[i];
				}

				if(baroBuffer[i] > maxAltInBuffer){
					maxAltInBuffer = baroBuffer[i];
				}

			}

			//Serial.println("AltitudeDiff");
			//Serial.print(maxAltInBuffer - minAltInBuffer);
			if(maxAltInBuffer - minAltInBuffer < 7){// if small difference between min and max then landed
			  flightState = LANDED;
			}

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
			takeAccelReading();
		}
				break;
		case POWERED_FLIGHT:
			takeAccelReading();
			takeBaroReading();
			//log data
				break;
		case UNPOWERED_FLIGHT:
			takeAccelReading();
			takeBaroReading();
			//log data
			//airbrake control
				break;
		case DESCENT:
			takeAccelReading();
			takeBaroReading();
			//log data
			//close airbrake
				break;
		case LANDED:
			takeAccelReading();
			takeBaroReading();
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
 * @return void
 */
void AirbrakeController::takeBaroReading(){
	barometer->update();
	float reading = barometer->getAltitude();
	baroBuffer[baroBufIndex] = reading;
	baroBufIndex = (baroBufIndex + 1)%BARO_BUFFER_SIZE;
	avgAltitude = avgBaroArray();
	if(avgAltitude > maxAvgAltitude){
		maxAvgAltitude = avgAltitude;
	}
}

/*
 * Updates the accelerometer and gets an accelerationZ reading, adds reading to the buffer array
 * Increments buffer index
 * @return void
 */
void AirbrakeController::takeAccelReading(){
	accelerometer->update();
	float reading = accelerometer->getAccZg();
	accelBuffer[accelBufIndex] = reading;
	accelBufIndex = (accelBufIndex + 1)%ACCEL_BUFFER_SIZE;
	avgAccel = avgAccelArray();
}

/*
 * Function for printing sensor readings and other data
 * @return void
 */

void AirbrakeController::debugData(){
	Serial.print("FlightState:");Serial.println(flightState);
	Serial.print("AvgAlt: ");
	Serial.println(avgAltitude);
	Serial.print("Current Alt: ");
	Serial.println(barometer->getAltitude());
	Serial.print("Accel: ");Serial.println(accelerometer->getAccZg());
	Serial.print("AvgAccel: ");
	Serial.println(avgAccel);
}


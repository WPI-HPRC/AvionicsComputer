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
};


/*
 * Init function for the system, should be run after instantiation
 */
void AirbrakeController::systemInit(){

	runningLooper->registerLoop(airbrakeLoop);		// robot system loop is registered first in order to be run first

//	driveTrain->registerEnabledLoops(runningLooper);


//	robotRadio->init();
	barometer->enable();
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


void AirbrakeController::updateStateMachine(){

	Serial.println("UpdateStateMachine");
	//runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
//	Serial.println(driveTrain->getHeading());

	switch(flightState){//conditions for switching state
		case ON_PAD:
		{
			/*float avgAccel = avgAccelArray();// average from last 0.1sec
			float G_Threshold = 2.0;
			if(avgAccel > 9.8 * G_Threshold){
				flightState = POWERED_FLIGHT;
			}*/
		}
			break;
		case POWERED_FLIGHT:
		{
			float avgAccel;// average from last 0.1sec
			float G_Threshold = 2.0;
			if(avgAccel < 9.8 * G_Threshold){
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
				flightState = DESCENT;
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
			barometer->update();
			float testAlt = barometer->getPressure();
			//Serial.println(testAlt);
			baroBuffer[baroBufIndex] = testAlt;
			baroBufIndex = (baroBufIndex + 1)%BARO_BUFFER_SIZE;
			float testAvg = avgBaroArray();
			//Serial.print("Avg: ");
			//Serial.println(testAvg);

			accelerometer->update();
			float testAccel = accelerometer->getAccZg();
			Serial.println(testAccel);
			accelBuffer[accelBufIndex] = testAccel;
			accelBufIndex = (accelBufIndex + 1)%ACCEL_BUFFER_SIZE;
			float testAvgAcc = avgAccelArray();
			Serial.print("Avg: ");
			Serial.println(testAvgAcc);

		}
				break;
		case POWERED_FLIGHT:
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



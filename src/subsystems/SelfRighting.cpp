/*
 * SelfRighting.cpp
 *
 *  Created on: May 8, 2021
 *      Author: Abby Hyde
 */

#include "SelfRighting.h"

SelfRighting::SelfRighting(MPU6050 * imuSensor) {
	// TODO Auto-generated constructor stub
	imu = imuSensor;

}

/*
 * Init function for the system, should be run after instantiation
 */
void SelfRighting::subsystemInit(){

	imu->enable();
	motor1->enable();
//	rightMotor->enable();

}

/*
 * Zero self righting
 */
void SelfRighting::zeroSensors(){

}

/*
 * Register self righting loop
 */
void SelfRighting::registerEnabledLoops(Looper * enabledLooper){

	enabledLooper->registerLoop(srLoop);
}

/*
 * Print output of self righting
 */
void SelfRighting::printOutput(){
	Serial.println(imu->getAcc_z());
}

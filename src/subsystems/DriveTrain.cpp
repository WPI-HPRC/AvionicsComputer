/*
 * DriveTrain.cpp
 * Created on: Dec 28, 2020
 * Author: Peter Dentch
 */

#include "DriveTrain.h"

/*
* Constructor for the drive train subsystem object, should only be one instance, robot only has one drive train
* @param IMU is the robot's IMU sensor peripheral
*/
DriveTrain::DriveTrain(MPU6050 * imuSensor){

	driveControlState = Idle;

	imu = imuSensor;

};


/*
 * Init function for the system, should be run after instantiation
 */
void DriveTrain::subsystemInit(){

	imu->enable();
	leftMotor->enable();
	rightMotor->enable();

	//TODO REMOVE LATER For channel 3 (Throttle)
	pinMode(21, INPUT);

	//TODO REMOVE LATER For channel 2 (Pitch)
	pinMode(22, INPUT);

}


/*
 * Get the latest heading value of the drive train chassis
 * @return the latest heading in deg //TODO
 */
int16_t DriveTrain::getHeading(){

	return imu->getAcc_z();

}

/*
 * Takes float parameters leftMotorSpeed and rightMotorSpeed to command each motor to be driven at commanded speeds.
 * Parameters should be between -1 and 1.
 * If above or below -1 to 1, setSpeed will truncate inputs such that the absolute value of the input is no greater than 1.
 */
void DriveTrain::driveTank(float leftMotorSpeed, float rightMotorSpeed) {

	leftMotor->setSpeed(leftMotorSpeed);
	rightMotor->setSpeed(rightMotorSpeed);

}

/*
 * Zero drive train
 */
void DriveTrain::zeroSensors(){

}

/*
 * Register drive train loop
 */
void DriveTrain::registerEnabledLoops(Looper * enabledLooper){

	enabledLooper->registerLoop(driveLoop);
}

/*
 * Print output of drive train
 */
void DriveTrain::printOutput(){
	Serial.println(imu->getAcc_z());
}

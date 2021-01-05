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


}


/*
 * Get the latest heading value of the drive train chassis
 * @return the latest heading in deg //TODO
 */
int16_t DriveTrain::getHeading(){

	return imu->getAcc_z();

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

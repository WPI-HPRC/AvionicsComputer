/*
 * MPU6050.cpp
 *
 *  Created on: Dec 22, 2020
 *      Author: Giovanni Giacalone
 */

#include "MPU6050.h"

/*
 * Constructor for this class MPU6050, should only be one instance from only one MPU6050 board per Arduino
 */
MPU6050::MPU6050(){}

/*
 *
 */
void MPU6050::enable(){

	Wire.beginTransmission(MPU6050_ADDRESS);  		//Starts the transmission with the MPU6050.
	Wire.write(MPU6050_PWR_MGMT_1);             	//register for power management
	Wire.write(SLEEP_MODE);                 		//disable sleep mode, 'wake up' the device
	Wire.endTransmission(true);    					//Ends the transmition with the MPU6050.

	Wire.beginTransmission(MPU6050_ADDRESS);  		//Starts the transmission with the MPU6050.
	Wire.write(GYRO_CONFIG);              			//Configures the gyro to the register GYRO_CONFIG.
	Wire.write(FS_SEL);         					//Sets the gyro range to FS_SEL.
	Wire.endTransmission(true);						//Ends the transmition with the MPU6050.

	Wire.beginTransmission(MPU6050_ADDRESS);  		//Starts the transmission with the MPU6050.
	Wire.write(ACCEL_CONFIG);              			//Configures the Accelerometer to the register ACCEL_CONFIG.
	Wire.write(AFS_SEL);         					//Set the accelerometer range to AFS_SEL.
	Wire.endTransmission(true);						//Ends the transmition with the MPU6050.

}

void MPU6050::disable(){
	//IM NOT SURE WHAT THIS IS SUPPOSED TO DO.
}

void MPU6050::update(){
	Wire.beginTransmission(MPU6050_ADDRESS);       	//Starts the transmission with the MPU6050.
	Wire.write(ACCEL_XOUT_H);                   	//Begins with the ACCEL_XOUT_H register, the rest of the data registers begin there.
	Wire.endTransmission(false);        			//Leaves the transmition with the MPU6050 open.
	Wire.requestFrom(MPU6050_ADDRESS, 14, true);   	//Requests 14 bytes of data starting from the indicated register. 6 for accelerometer, 2 for temp, 6 for gyro

	this->acc_x = Wire.read() <<8| Wire.read();     //the data for each axis of the accelerometer or gyro is 2 bytes, one high byte and one low
	this->acc_y = Wire.read() <<8| Wire.read();     //the two must be added together to get the raw sensor value
	this->acc_z = Wire.read() <<8| Wire.read();
	this->temperature = Wire.read() <<8| Wire.read();
	this->gyro_x = Wire.read() <<8| Wire.read();
	this->gyro_y = Wire.read() <<8| Wire.read();
	this->gyro_z = Wire.read() <<8| Wire.read();
}
void MPU6050::updateIMU(){
	gyro_x -= gyro_x_cal;							//Subtract the offset calibration value from the raw gyro_x value
	gyro_y -= gyro_y_cal;							//Subtract the offset calibration value from the raw gyro_y value
	gyro_z -= gyro_z_cal;							//Subtract the offset calibration value from the raw gyro_z value

	this->roll += gyro_x / LSB * dt;				//Integrates the angular rate of X axis over dt to return absolute position of X axis.
	this->pitch += gyro_y / LSB * dt;				//Integrates the angular rate of Y axis over dt to return absolute position of Y axis.
	this->yaw += gyro_z / LSB * dt;					//Integrates the angular rate of Z axis over dt to return absolute position of Z axis.
}

/*
 * Calibrates Gyro to remove biases from X, Y, and Z axes.
 */
void MPU6050::gyroCalibrate() {

  Serial.println(F("Gyro calibrating, don't move it!!!"));

  for(int cal_int = 0; cal_int < calibrationSamples; cal_int++){
	  if(cal_int % 125 == 0)
		  Serial.print(".");
	  update();
	  gyro_x_cal += gyro_x;
	  gyro_y_cal += gyro_y;
	  gyro_z_cal += gyro_z;
	  delay(3);
  }

  gyro_x_cal /= calibrationSamples;
  gyro_y_cal /= calibrationSamples;
  gyro_z_cal /= calibrationSamples;

  Serial.println(gyro_z_cal); 				//Check to see if calibration was successfull. Value should be very close to 0.
  delay(6000); 								//Delay to allow for previous println to be read.
  Serial.println(F("Done Calibrating!"));	//Alerts that the Gyro has completed it's calibration routine.

}

/*
 * Returns the current Roll which is the absolute position of the X axis.
 */
//float MPU6050::getRoll(){
//	return this->roll;
//}

/*
 * Returns the current Pitch which is the absolute position of the Y axis.
 */
//float MPU6050::getPitch(){
//	return this->pitch;
//}

/*
 * Returns the current Yaw which is the absolute position of the Z axis.
 */
float MPU6050::getYaw(){
	return this->yaw;
}

/*
 * Returns the current Acceleration value in the x direction in Gs.
 */
int16_t MPU6050::getRawAcc_x(){
	return this->acc_x;
}

/*
 * Returns the current Acceleration value in the y direction in Gs.
 */
int16_t MPU6050::getRawAcc_y(){
	return this->acc_y;
}

/*
 * Returns the current Acceleration value in the z direction in Gs.
 */
int16_t MPU6050::getRawAcc_z(){
	return this->acc_z;
}

/*
 * Returns the current temperature value in deg C.
 */
int16_t MPU6050::getRawTemperature(){
	return this->temperature;
}

/*
 * Returns the current Gyro orientation in the x direction in deg/s.
 */
int16_t MPU6050::getRawGyro_x(){
	return this->gyro_x;
}

/*
 * Returns the current Gyro orientation in the y direction in deg/s.
 */
int16_t MPU6050::getRawGyro_y(){
	return this->gyro_y;
}

/*
 * Returns the current Gyro orientation in the z direction in deg/s.
 */
int16_t MPU6050::getRawGyro_z(){
	return this->gyro_z;
}


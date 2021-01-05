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

int16_t MPU6050::getAcc_x(){		//Returns the current Acceleration value in the x direction in Gs.
	return this->acc_x;
}

int16_t MPU6050::getAcc_y(){		//Returns the current Acceleration value in the y direction in Gs.
	return this->acc_y;
}

int16_t MPU6050::getAcc_z(){		//Returns the current Acceleration value in the z direction in Gs.
	return this->acc_z;
}

int16_t MPU6050::getTemperature(){	//Returns the current temperature value in deg C.
	return this->temperature;
}

int16_t MPU6050::getGyro_x(){		//Returns the current Gyro orientation in the x direction in deg/s.
	return this->gyro_x;
}

int16_t MPU6050::getGyro_y(){		//Returns the current Gyro orientation in the y direction in deg/s.
	return this->gyro_y;
}

int16_t MPU6050::getGyro_z(){		//Returns the current Gyro orientation in the z direction in deg/s.
	return this->gyro_z;
}


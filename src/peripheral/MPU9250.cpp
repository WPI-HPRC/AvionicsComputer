/*
 * MPU9050.cpp
 *
 *  Created on: Mar 24, 2021
 *      Author: Giovanni Giacalone
 */

#include "MPU9250.h"

/*
 * Constructor for this class, MPU9250. Should only be one instance from only one MPU6050 board per Arduino.
 */
MPU9250::MPU9250(){}

/*
 *
 */
void MPU9250::enable(){
	MPU6050::enable();
	//Magnetometer functionality here
}

/*
 *
 */
void MPU9250::updateSensorValues(){
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

/*
 *
 */
void MPU9250::update(){
	MPU6050::update();
	updateSensorValues();
	//Magnetometer functionality here
	complementaryFilter();
}

void MPU9250::complementaryFilter(){
	//Magnetometer functionality here
}

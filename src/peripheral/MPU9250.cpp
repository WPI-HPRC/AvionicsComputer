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

	MPU6050::enable(); 							//Powers on sensor, configures gyro and accelerometer

	Wire.beginTransmission(MPU6050_ADDRESS);  	//Starts the transmission with the MPU6050.
	Wire.write(USER_CTRL);                    	//Register for User Control.
	Wire.write(0x00);                         	//Disables I2C Master Interface.
	Wire.endTransmission(true);               	//Ends the transmition with the MPU6050.

	Wire.beginTransmission(MPU6050_ADDRESS);  	//Starts the transmission with the MPU6050.
	Wire.write(INT_BYPASS_CONFIG);            	//Register for Bypass Enable Configuration.
	Wire.write(0x02);                         	//Turns on the bypass multiplexer.
	Wire.endTransmission(true);               	//Ends the transmition with the MPU6050.

	Wire.beginTransmission(MAG_AD);  			//Starts the transmission with the MPU6050.
	Wire.write(CNTL1_AD);                     	//Control 1 register of the magnetometer.
	Wire.write(0x16);                         	//Sets magnetometer to continuous read mode at 100 Hz and 16 bit output.
	Wire.endTransmission(true);               	//Ends the transmition with the MPU6050.
}

/*
 *
 */
void MPU9250::updateSensorValues(){
	Wire.beginTransmission(MAG_AD);
	Wire.write(HXL);
	Wire.endTransmission(false);
	Wire.requestFrom(MAG_AD, 7, true); 			//Must request 7 bytes to read overflow or sensor won't update

	magneX = Wire.read() | (Wire.read() << 8); 	//Must read low byte first
	magneY = Wire.read() | (Wire.read() << 8);
	magneZ = Wire.read() | (Wire.read() << 8);

	status_2 = Wire.read();

	Serial.print("magneX: ");
	Serial.print(magneX);
	Serial.print(", magneY: ");
	Serial.print(magneY);
	Serial.print(", magneZ: ");
	Serial.println(magneZ);

	// TODO Add conversion

}

/*
 *
 */
void MPU9250::update(){
	MPU6050::update();
	updateSensorValues();
	//Magnetometer functionality here
	//complementaryFilter();
}

void MPU9250::complementaryFilter(){
	//Magnetometer functionality here
}
